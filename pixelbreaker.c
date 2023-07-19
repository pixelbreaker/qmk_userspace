// Copyright 2023 @filterpaper
// Copyright 2023 @pixelbreaker
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

#ifdef CONSOLE_ENABLE
#  include "print.h"
#endif

#if defined(POINTING_DEVICE_ENABLE)
#  include "features/scrollspam.h"
#endif
#ifdef ACHORDION_ENABLE
#  include "features/achordion.h"
#endif

#ifndef SCROLL_BUFFER_SIZE
#  define SCROLL_BUFFER_SIZE 50
#endif

static fast_timer_t tap_timer = 0;
#define TYPING_TERM (TAPPING_TERM * 2)
#define IS_TYPING() (timer_elapsed_fast(tap_timer) < TYPING_TERM)
#define IS_HOME_ROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_MT_SHIFT(k) (QK_MOD_TAP_GET_MODS(k) & MOD_MASK_SHIFT)

// Increase tapping term while typing
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case SPC_NAV:
      return TAPPING_TERM;
    default:
      return IS_HOME_ROW(record) && !IS_MT_SHIFT(keycode) && IS_TYPING() ? TYPING_TERM : TAPPING_TERM;
  }
}

// Select Shift hold immediately with a nested key
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  return IS_QK_MOD_TAP(keycode) && IS_MT_SHIFT(keycode) && !IS_TYPING();
}

// Select layer hold immediately with another key
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  // Hold space to toggle layer immediately when not currently typing else wait for tapping term
  switch (keycode) {
    case SPC_NAV:
    case BSP_NUM:
      return !IS_TYPING();

    default:
      return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) > 0;
  }
}

// Send custom hold keycode for mod tap
static inline bool process_tap_hold(uint16_t hold_keycode, keyrecord_t *record) {
  if (record->tap.count == 0) {
    tap_code16(hold_keycode);
    return false;
  }
  return true;
}

/*
        APP/TAB switchers
*/
#if defined(POINTING_DEVICE_ENABLE) || defined(ENCODER_ENABLE)
bool appswitch_active = false;
bool tabswitch_active = false;
#endif

/*
        POINTING device related
*/
#ifdef POINTING_DEVICE_ENABLE
enum trackball_modes {
  CURSOR = 0,
  SCROLL,
  CARRET,
  MEDIA,
};
enum encoder_modes {
  HUE = 0,
  SAT,
  VAL,
  SPD,
  MOD,
};
uint8_t track_mode   = CURSOR;
uint8_t encoder_mode = HUE;

bool     mouse_is_down    = false;
uint16_t last_mouse_press = 0; // for click tracking pause

// carret layer delay timers
static deferred_token activate_track_mode_token = INVALID_DEFERRED_TOKEN;

uint32_t activate_media_mode(uint32_t trigger_time, void *cb_arg) {
  tap_reset();
  track_mode = MEDIA;
  return 0;
}

uint32_t activate_carret_mode(uint32_t trigger_time, void *cb_arg) {
  tap_reset();
  track_mode = CARRET;
  return 0;
}

uint32_t activate_scroll_mode(uint32_t trigger_time, void *cb_arg) {
  tap_reset();
  track_mode = SCROLL;
  return 0;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  static int16_t scroll_buffer_x = 0;
  static int16_t scroll_buffer_y = 0;
  // Pause mouse report updates for short time after clicking to make it easier
  // to double click with small movement of trackball
  bool mouse_pause = mouse_is_down && timer_elapsed(last_mouse_press) < 150;

  if ((track_mode != CURSOR && track_mode != SCROLL) || appswitch_active || tabswitch_active || mouse_pause) {
    // Nerf mouse_report as we're doing something else
    tap_report(mouse_report);
    mouse_report.x = 0;
    mouse_report.y = 0;
  }
  if (track_mode == SCROLL) {
    scroll_buffer_x -= mouse_report.x;
    scroll_buffer_y -= mouse_report.y;
    mouse_report.x = 0;
    mouse_report.y = 0;
    if (abs(scroll_buffer_x) > SCROLL_BUFFER_SIZE) {
      mouse_report.h  = scroll_buffer_x > 0 ? -1 : 1;
      scroll_buffer_x = 0;
    }
    if (abs(scroll_buffer_y) > SCROLL_BUFFER_SIZE) {
      mouse_report.v  = scroll_buffer_y > 0 ? 1 : -1;
      scroll_buffer_y = 0;
    }
  } else if (track_mode == MEDIA) {
    tap_media();
  } else if (track_mode == CARRET) {
    tap_tb(KC_RIGHT, KC_LEFT, KC_UP, KC_DOWN);
  } else if (appswitch_active || tabswitch_active) {
    tap_switcher();
  }
  return mouse_report;
}
#endif // POINTING_DEVICE_ENABLE

#ifdef ENCODER_ENABLE
bool encoder_down  = false;
bool encoder_moved = false;

bool encoder_update_user(uint8_t index, bool clockwise) {
  if (index == 0) { /* First encoder */
    bool fnc_on;
#  ifdef RGB_MATRIX_ENABLE
    fnc_on = IS_LAYER_ON(FNC);
#  else
    fnc_on = false;
#  endif
    if (fnc_on) {
#  ifdef RGB_MATRIX_ENABLE
      switch (encoder_mode) {
        case HUE:
          clockwise ? rgb_matrix_increase_hue() : rgb_matrix_decrease_hue();
          return false;
        case SAT:
          clockwise ? rgb_matrix_increase_sat() : rgb_matrix_decrease_sat();
          return false;
        case VAL:
          clockwise ? rgb_matrix_increase_val() : rgb_matrix_decrease_val();
          return false;
        case SPD:
          clockwise ? rgb_matrix_increase_speed() : rgb_matrix_decrease_speed();
          return false;
        case MOD:
          clockwise ? rgb_matrix_step() : rgb_matrix_step_reverse();
          return false;
      }
#  endif
    } else if (appswitch_active || tabswitch_active) {
      tap_code16(clockwise ? KC_TAB : S(KC_TAB));
    } else {
      if (encoder_down) {
        tap_code_delay(clockwise ? KC_MNXT : KC_MPRV, 10);
      } else {
        tap_code_delay(clockwise ? KC_VOLU : KC_VOLD, 10);
      }
    }
    encoder_moved = true;
    return false;
  } else {
    return true;
  }
}
#endif

bool return_or_achordion(bool default_return, uint16_t keycode, keyrecord_t *record) {
#if !defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE) && defined(ACHORDION_ENABLE)
  return process_achordion(keycode, record);
#else
  return default_return;
#endif
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  tap_timer = timer_read_fast();

  // custom keycodes
  switch (keycode) {
    case TGL_BASE:
      if (record->event.pressed) {
        if (IS_LAYER_ON(CMK)) {
          set_single_persistent_default_layer(BSE);
          layer_off(CMK);
        } else {
          set_single_persistent_default_layer(CMK);
          layer_on(CMK);
        }
        return false;
      }
    // set trackball modes...
    case SPC_NAV:
#ifdef POINTING_DEVICE_ENABLE
#  ifdef KEYBOARD_charybdis
      charybdis_set_pointer_dragscroll_enabled(record->event.pressed);

#  else
      if (record->event.pressed) {
        if (!extend_deferred_exec(activate_track_mode_token, MEDIA_TIMEOUT_MS)) {
          activate_track_mode_token = defer_exec(MEDIA_TIMEOUT_MS, activate_scroll_mode, NULL);
        }
      } else {
        cancel_deferred_exec(activate_track_mode_token);
        track_mode = CURSOR;
      }
#  endif

#endif
      return return_or_achordion(true, keycode, record);
    case BSP_NUM:
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        if (!extend_deferred_exec(activate_track_mode_token, MEDIA_TIMEOUT_MS)) {
          activate_track_mode_token = defer_exec(MEDIA_TIMEOUT_MS, activate_media_mode, NULL);
        }
      } else {
        cancel_deferred_exec(activate_track_mode_token);
        track_mode = CURSOR;
      }
#endif
      return return_or_achordion(true, keycode, record);
    case TAB_SYM:
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        if (!extend_deferred_exec(activate_track_mode_token, CARRET_TIMEOUT_MS)) {
          activate_track_mode_token = defer_exec(CARRET_TIMEOUT_MS, activate_carret_mode, NULL);
        }
      } else {
        cancel_deferred_exec(activate_track_mode_token);
        track_mode = CURSOR;
      }
#endif
      return return_or_achordion(true, keycode, record);

    // Pause mouse report updates for short time after clicking to make it easier
    // to double click with small movement of trackball
    case KC_BTN1 ... KC_BTN3:
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        mouse_is_down    = true;
        last_mouse_press = timer_read();

#  ifdef KEYBOARD_charybdis
        charybdis_set_pointer_sniping_enabled(true);
#  endif
      } else {
        mouse_is_down = false;
#  ifdef KEYBOARD_charybdis
        charybdis_set_pointer_dragscroll_enabled(false);
        charybdis_set_pointer_sniping_enabled(false);
#  endif
      }
#endif
      return true;

    case APPSWITCH:
#if defined(POINTING_DEVICE_ENABLE) || defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        if (!appswitch_active) {
          register_code(KC_LGUI);
          register_code(KC_TAB);
          unregister_code(KC_TAB);
        }
      } else {
        if (appswitch_active) {
          unregister_code(KC_LGUI);
        }
      }
      appswitch_active = record->event.pressed;
#endif
      return false;

    case TABSWITCH:
#if defined(POINTING_DEVICE_ENABLE) || defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        if (!tabswitch_active) {
          register_code(KC_LCTL);
        }
      } else {
        if (tabswitch_active) {
          unregister_code(KC_LCTL);
        }
      }
      tabswitch_active = record->event.pressed;
#endif
      return false;

    case ENC_DOWN:
#ifdef ENCODER_ENABLE
      encoder_down = record->event.pressed;
      if (encoder_moved && record->event.pressed) {
        encoder_moved = false;
      }
      if (!record->event.pressed && !encoder_moved) {
        tap_code16(KC_MPLY);
      }
#endif
      return false;

    case E_HUE:
      encoder_mode = HUE;
      return false;
    case E_SAT:
      encoder_mode = SAT;
      return false;
    case E_VAL:
      encoder_mode = VAL;
      return false;
    case E_SPD:
      encoder_mode = SPD;
      return false;
    case E_MOD:
      encoder_mode = MOD;
      return false;

    default:
#if !defined(POINTING_DEVICE_AUTO_MOUSE_ENABLE) && defined(ACHORDION_ENABLE)
      return process_achordion(keycode, record);
#else
      return true;
#endif
  }
}

void housekeeping_task_user(void) {
  // Restore state after 3 minutes
  if (last_input_activity_elapsed() > TAPPING_TERM * 1000U) {
    if (host_keyboard_led_state().caps_lock) {
      tap_code(KC_CAPS);
    }
    if (get_highest_layer(layer_state | default_layer_state) > 0) {
      layer_off(get_highest_layer(layer_state | default_layer_state));
    }
  }
}

// Handle keyrecord before quantum processing
bool pre_process_record_quantum_user(keyrecord_t *record) {
  uint16_t keycode = get_record_keycode(record, true);

  // Implement instant-tap of mod-tap keys
  if (IS_HOME_ROW(record) && IS_QK_MOD_TAP(keycode)) {
    keyrecord_t quick_tap_record;
    quick_tap_record.keycode = keycode & 0xff;

    // When a mod-tap key is pressed within QUICK_TAP_TERM of a previous key,
    // send its masked base keycode through process_record and skip processing
    if (record->event.pressed && (timer_elapsed_fast(tap_timer) < QUICK_TAP_TERM)) {
      quick_tap_record.event.pressed = true;
      process_record(&quick_tap_record);
#if TAP_CODE_DELAY > 0
      wait_ms(TAP_CODE_DELAY);
#endif
      return false; // Skip processing
    } else {
      // Handle key up record event
      quick_tap_record.event.pressed = false;
      process_record(&quick_tap_record);
    }
  }

  return true; // Continue processing record
}

/*
   ACHORDION config
*/
#ifdef ACHORDION_ENABLE

void matrix_scan_user(void) {
  achordion_task();
}

bool achordion_chord(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
  // Exceptionally consider the following chords as holds, even though they
  // are on the same hand.
  switch (tap_hold_keycode) {
    case HM_A:
      if (other_keycode == KC_C || other_keycode == KC_X) {
        return true;
      }
      break;

    case HM_D: // GUI.
      if (other_keycode == HM_S || other_keycode == KC_W || other_keycode == KC_Q || other_keycode == KC_R || other_keycode == KC_T || other_keycode == HM_T) {
        return true;
      }
      break;

    case HM_S2:                                                                                               // GUI Colemak.
      if (other_keycode == HM_S || other_keycode == KC_W || other_keycode == KC_Q || other_keycode == KC_R) { // || other_keycode == HM_T) {
        return true;
      }
      break;
  }

  // Also allow same-hand holds when the other key is in the rows below the
  // alphas. I need the `% (MATRIX_ROWS / 2)` because my keyboard is split.
  if (other_record->event.key.row % (MATRIX_ROWS / 2) >= 3) {
    return true;
  }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case TAB_SYM:
    case ENT_FUN:
    case BSP_NUM:
    // case ESC_MED:
    case SPC_NAV:
      return 0;

    case MSE(KC_Z):
      return 100;

    default:
      return 800;
  }
}

bool achordion_eager_mod(uint8_t mod) {
  switch (mod) {
    case MOD_LSFT:
    case MOD_RSFT:
    case MOD_LGUI:
    case MOD_RGUI:
      // case MOD_HYPR:
      // case MOD_MEH:
      return true; //! IS_TYPING();

    default:
      return false;
  }
}

#endif

bool caps_word_press_user(uint16_t keycode) {
  switch (keycode) {
    // Keycodes that continue Caps Word, with shift applied.
    case KC_A ... KC_Z:
      add_weak_mods(MOD_BIT(KC_LSFT)); // Apply shift to next key.
      return true;

    // Keycodes that continue Caps Word, without shifting.
    case KC_1 ... KC_0:
    case KC_MINS:
    case KC_UNDS:
    case KC_BSPC:
    case KC_DEL:
      return true;

    default:
      return false; // Deactivate Caps Word.
  }
}

uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case KC_SPC:
      return 0;
    case KC_BSPC:
      return QUICK_TAP_TERM * 2;
    default:
      return QUICK_TAP_TERM;
  }
}

void leader_start_user(void) {
#ifdef RGB_MATRIX_ENABLE
  rgb_matrix_sethsv_noeeprom(255, 255, 127);
#endif
}

void leader_end_user(void) {
  if (leader_sequence_one_key(KC_F)) {
    SEND_STRING("function ");
  } else if (leader_sequence_one_key(KC_R)) {
    SEND_STRING("return ");
  } else if (leader_sequence_two_keys(KC_E, KC_C)) {
    SEND_STRING("export const  = " SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
  } else if (leader_sequence_three_keys(KC_E, KC_D, KC_F)) {
    SEND_STRING("export default function () { " SS_TAP(X_LEFT) "\n" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
  } else if (leader_sequence_three_keys(KC_I, KC_M, KC_P)) {
    SEND_STRING("import  from \"\"" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
  }

#ifdef RGB_MATRIX_ENABLE
  rgb_matrix_reload_from_eeprom();
#endif
}

void keyboard_post_init_user(void) {
// Customise these values to desired behaviour
#ifdef CONSOLE_ENABLE
  debug_enable = true;
//   debug_matrix = true;
#endif
}

// reset CPI after wake
#ifdef KEYBOARD_charybdis
void suspend_wakeup_init_user(void) {
  keyboard_post_init_kb();
}
#endif

#ifdef KEYBOARD_tenome
void pointing_device_init_kb() {
#  ifdef TRACKBALL_ENABLE
  pointing_device_set_cpi(900);
#  else
  pointing_device_set_cpi(500);
#  endif
}
#endif
