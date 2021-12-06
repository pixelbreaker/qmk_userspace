// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "filterpaper.h"


// Tap hold customization
#ifdef TAPPING_FORCE_HOLD_PER_KEY // Enable for right thumb keys
bool get_tapping_force_hold(uint16_t keycode, keyrecord_t *record) {
	return keycode == RSFT_T(KC_SPC) || keycode == LT(NUM,KC_BSPC) ? true : false;
}
#endif

#ifdef PERMISSIVE_HOLD_PER_KEY // Disable for alphanumeric tap hold
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return (keycode & 0xF000) == LMT_BITS || (keycode & 0xFF00) == LT0_BITS ? false : true;
}
#endif


// Send custom keycode on hold for mod tap
static bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
	if (!record->tap.count && record->event.pressed) {
	//	tap_code16(keymap_config.swap_lctl_lgui ? C(hold_keycode) : hold_keycode);
		tap_code16(hold_keycode);
		return false;
	}
	return true;
}


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
	if (record->event.pressed) {
#if defined(OLED_ENABLE) && !defined(WPM_ENABLE)
		extern uint32_t tap_timer;
		tap_timer = timer_read32();  // Reset OLED animation tap timer
#endif
#ifdef WORD_FEATURES
		extern bool process_autocorrection(uint16_t keycode, keyrecord_t* record);
		extern bool process_caps_word(uint16_t keycode, keyrecord_t *record);
		if (!process_autocorrection(keycode, record)) { return false; }
		if (!process_caps_word(keycode, record)) { return false; }
#endif
	}

	switch (keycode) {
		// Undo cut copy paste using LT(0,kc)
		case SLSH_TH: return process_tap_hold(Z_UND, record);
		case DOT_TH:  return process_tap_hold(Z_CUT, record);
		case COMM_TH: return process_tap_hold(Z_CPY, record);
		case M_TH:    return process_tap_hold(Z_PST, record);
#ifdef RGB_MATRIX_ENABLE
		case RESET:
			if (record->event.pressed) {
				rgb_matrix_set_color_all(RGB_RED);
				rgb_matrix_driver.flush();
			}
			break;
#endif
	}

	return true; // Continue with unmatched keycodes
}


#ifdef ENCODER_ENABLE
bool encoder_update_user(uint8_t index, bool clockwise) {
	if (index == 0) {
		clockwise ? tap_code(KC_VOLU) : tap_code(KC_VOLD);
	} else if (index == 1) {
		clockwise ? tap_code(KC_MNXT) : tap_code(KC_MPRV);
	}
	return false;
}
#endif


#ifdef KEYBOARD_a_dux
// Pro Micro data LED pins
#define RXLED B0
#define TXLED D5
// GPIO control macros
#define RXLED_INIT setPinOutput(RXLED)
#define TXLED_INIT setPinOutput(TXLED)
#define RXLED_ON   writePinLow(RXLED)
#define TXLED_ON   writePinLow(TXLED)
#define RXLED_OFF  writePinHigh(RXLED)
#define TXLED_OFF  writePinHigh(TXLED)

void matrix_init_user(void) { RXLED_INIT; TXLED_INIT; }

layer_state_t layer_state_set_user(layer_state_t const state) {
	switch (get_highest_layer(state)) {
		case CMK:
		case FNC: RXLED_ON;  TXLED_ON;  break;
		case SYM: RXLED_OFF; TXLED_ON;  break;
		case NUM: RXLED_ON;  TXLED_OFF; break;
		default:  RXLED_OFF; TXLED_OFF;
	}
	return state;
}
#endif // KEYBOARD_a_dux
