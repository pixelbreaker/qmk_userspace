// Copyright 2023 @filterpaper
// Copyright 2023 @pixelbreaker
// SPDX-License-Identifier: GPL-2.0+

#include "pixelbreaker.h"
#include <stdbool.h>
#include "action.h"
#include "action_tapping.h"
#include "keycodes.h"
#include "layout.h"
#include "quantum.h"
#include "quantum_keycodes.h"

#ifdef RGB_MATRIX_ENABLE
#  include "rgb_matrix_types.h"
#  include "rgb_matrix.h"
#endif

#ifdef CONSOLE_ENABLE
#  include "print.h"
#endif

#if POINTING_DEVICE_ENABLE
#  include "features/scrollspam.h"
#endif

#ifndef SCROLL_BUFFER_SIZE
#  define SCROLL_BUFFER_SIZE 50
#endif

// Track typing state
static keyevent_type_t prev_event;
static fast_timer_t    tap_timer = 0;

/*
  APP/TAB switchers
*/
#if defined(POINTING_DEVICE_ENABLE) || defined(ENCODER_ENABLE)
bool appswitch_active = false;
bool tabswitch_active = false;
#endif

bool appkeys_active = false;

/*
POINTING DEVICE
*/
#ifdef POINTING_DEVICE_ENABLE
enum trackball_modes {
  CURSOR = 0,
  SCROLL,
  CARRET,
  MEDIA,
};
uint8_t track_mode = CURSOR;

bool     sniping          = false;
bool     mouse_is_down    = false;
uint16_t last_mouse_press = 0; // for click tracking pause

#  define SCROLL_DIVISOR 24.0

float scroll_accumulated_h = 0;
float scroll_accumulated_v = 0;

#  if defined(KEYBOARD_tenome) || defined(KEYBOARD_buteo) || defined(KEYBOARD_buteo_talon) || defined(KEYBOARD_cnano)
void pointing_device_init_kb() {
  pointing_device_set_cpi(DPI_POINTER);
}
#  endif

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  // two finger scrolling on Azoteq
#  ifdef KEYBOARD_buteo_talon
  scroll_accumulated_h += (float)mouse_report.h / SCROLL_DIVISOR;
  scroll_accumulated_v -= (float)mouse_report.v / SCROLL_DIVISOR;

  // Assign integer parts of accumulated scroll values to the mouse report
  mouse_report.h = (int8_t)scroll_accumulated_h;
  mouse_report.v = (int8_t)scroll_accumulated_v;

  // Update accumulated scroll values by subtracting the integer parts
  scroll_accumulated_h -= mouse_report.h;
  scroll_accumulated_v -= mouse_report.v;
#  endif
  // Pause mouse report updates for short time after clicking to make it easier
  // to double click with small movement of trackball
  bool mouse_pause = mouse_is_down && timer_elapsed(last_mouse_press) < 150;

#  ifdef TRACKBALL_ENABLE
  pointing_device_set_cpi(track_mode == SCROLL && !appkeys_active ? DPI_SCROLL
                          : sniping                               ? DPI_POINTER_SNIPE
                                                                  : DPI_POINTER);
#  else
  pointing_device_set_cpi(track_mode == SCROLL && !appkeys_active ? DPI_SCROLL : DPI_POINTER);
#  endif
  // #  endif

  if (track_mode == SCROLL && !appkeys_active) {
    scroll_accumulated_h += (float)mouse_report.x / SCROLL_DIVISOR;
    scroll_accumulated_v -= (float)mouse_report.y / SCROLL_DIVISOR;

    // Assign integer parts of accumulated scroll values to the mouse report
    mouse_report.h = (int8_t)scroll_accumulated_h;
    mouse_report.v = (int8_t)scroll_accumulated_v;

    // Update accumulated scroll values by subtracting the integer parts
    scroll_accumulated_h -= mouse_report.h;
    scroll_accumulated_v -= mouse_report.v;

    // Clear the X and Y values of the mouse report
    mouse_report.x = 0;
    mouse_report.y = 0;
  } else if (track_mode == MEDIA) {
    tap_media();
  } else if (track_mode == CARRET) {
    tap_tb(KC_RIGHT, KC_LEFT, KC_UP, KC_DOWN);
  }

  if ((track_mode != CURSOR && (track_mode != SCROLL && !appkeys_active)) ||
      mouse_pause) { // appswitch_active || tabswitch_active ||
    // Nerf mouse_report as we're doing something else
    tap_report(mouse_report);
    mouse_report.x = 0;
    mouse_report.y = 0;
  }

  return mouse_report;
}
#endif

/*
 ENCODER
 */
enum encoder_modes {
  NONE = 0,
  HUE,
  SAT,
  VAL,
  SPD,
  MOD,
};
uint8_t encoder_mode = NONE;

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
    } else if (IS_LAYER_ON(NUM)) {
      tap_code16(clockwise ? MS_WHLU : MS_WHLD);
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

// Send custom hold keycode
static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
  if (record->tap.count) return true;
  tap_code16(keycode);
  return false;
}

/*
 Main Processing
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  // uprintf("Col: %d Row: %d\n", record->event.key.col, record->event.key.row);

  tap_timer = timer_read_fast();

  // TAP holds
  if (record->event.pressed) {
    prev_event = record->event.type;

    switch (keycode) {
      case TH_C: // cut, copy, paste
        return process_tap_hold(Z_CUT, record);
      case TH_G:
        return process_tap_hold(Z_CPY, record);
      case TH_D:
        return process_tap_hold(Z_PST, record);
      case TH_ESC:
        return process_tap_hold(G(A(KC_ESC)), record);
      case TH_QU:
        if (is_caps_word_on()) {
          register_mods(MOD_MASK_SHIFT);
        }
        if (record->tap.count) {
          tap_code(KC_Q);
          bool shifted = !is_caps_word_on() && get_mods() & MOD_MASK_SHIFT;
          if (shifted) {
            unregister_mods(MOD_MASK_SHIFT);
          }
          tap_code(KC_U);
        } else {
          tap_code16(KC_Q);
        }
        if (is_caps_word_on()) {
          unregister_mods(MOD_MASK_SHIFT);
        }
        return false;

      case TH_DLR:
        if (record->tap.count) {
          tap_code16(S(KC_4));
        } else {
          SEND_STRING("${}" SS_TAP(X_LEFT));
        }
        return false;

      case TH_EQL:
        if (record->tap.count) return true;
        SEND_STRING("=>");
        return false;

      case TH_LCBR:
        if (record->tap.count) {
          tap_code16(KC_LCBR);
        } else {
          SEND_STRING("() => {}" SS_TAP(X_LEFT));
        };
        return false;

      case TH_LBRC:
        if (record->tap.count) {
          tap_code16(KC_LBRC);
        } else {
          SEND_STRING("[]()" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
        };
        return false;

      case TH_RCBR:
        if (record->tap.count) {
          tap_code16(KC_RCBR);
        } else {
          SEND_STRING("() => {}, []" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
        };
        return false;

      case TH_LT:
        if (record->tap.count) {
          tap_code16(KC_LT);
        } else {
          SEND_STRING("<></>" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT));
        };
        return false;

      case TH_DEL:
        if (record->tap.count) {
          tap_code16(KC_DEL);
        } else {
          tap_code16(G(KC_BSPC));
        }
        return false;

      case TH_SCR:
        if (record->tap.count) {
          tap_code16(Z_SSHT);
        } else {
          tap_code16(Z_SRCD);
        }
        return false;
    }
  }

  // Other keycodes
  switch (keycode) {
    // set trackball modes...
    case THM_0:
    case THM_3:
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        tap_reset();
        track_mode = MEDIA;
      } else {
        track_mode = CURSOR;
      }
#endif
      return true;

    case THM_1:
      if (appkeys_active) {
        unregister_code(KC_LGUI);
        appkeys_active = false;
      }
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        tap_reset();
        track_mode = SCROLL;
      } else {
        track_mode = CURSOR;
      }
#endif
      return true;

    case THM_1_2:
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        tap_reset();
        track_mode = CARRET;
      } else {
        track_mode = CURSOR;
      }
#endif
      return true;

    // Pause mouse report updates for short time after clicking to make it easier
    // to double click with small movement of trackball
    case MS_BTN1 ... MS_BTN3:
#ifdef POINTING_DEVICE_ENABLE
      if (record->event.pressed) {
        mouse_is_down    = true;
        last_mouse_press = timer_read();
      } else {
        mouse_is_down = false;
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

    case APP_R:
      if (record->event.pressed) {
        if (!appkeys_active) {
          register_code(KC_LGUI);
          appkeys_active = true;
        }
        register_code(KC_TAB);
        unregister_code(KC_TAB);
      }
      return false;

    case APP_L:
      if (record->event.pressed) {
        if (!appkeys_active) {
          register_code(KC_LGUI);
          appkeys_active = true;
        }
        register_code16(S(KC_TAB));
        unregister_code16(S(KC_TAB));
      }
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

    case P_SNIPE:
#ifdef TRACKBALL_ENABLE
      sniping = record->event.pressed;
#endif
      return false;

    case E_HUE:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        encoder_mode = HUE;
      } else {
        encoder_mode = NONE;
      }
#endif
      return false;
    case E_SAT:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        encoder_mode = SAT;
      } else {
        encoder_mode = NONE;
      }
#endif
      return false;
    case E_VAL:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        encoder_mode = VAL;
      } else {
        encoder_mode = NONE;
      }
#endif
      return false;
    case E_SPD:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        encoder_mode = SPD;
      } else {
        encoder_mode = NONE;
      }
#endif
      return false;
    case E_MOD:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      if (record->event.pressed) {
        encoder_mode = MOD;
      } else {
        encoder_mode = NONE;
      }
#endif
      return false;

    // if on mouse layer an hitting blocking keys, turn off the mouse layer
    case KC_NO:
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
      if (IS_LAYER_ON(get_auto_mouse_layer())) {
        layer_off(get_auto_mouse_layer());
      }
#endif
      return true;

    default:
      return true;
  }
}

/*
 RGB Matrix handling
 */
#ifdef RGB_MATRIX_ENABLE
bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
  uint curr_layer = get_highest_layer(layer_state | default_layer_state);

  if (curr_layer == 0) return false;

  hsv_t hsv = {0, 255, rgb_matrix_get_val()};
  // generate a hue for the current layer 0-255
  uint8_t hue = (((float)curr_layer + 1) / 6) * 255;
  hsv.h       = hue;
  rgb_t rgb   = hsv_to_rgb(hsv);

  // set the rgb of all underglow and modifier flagged RGB LEDs
  for (uint8_t i = led_min; i < led_max; i++) {
    if (HAS_ANY_FLAGS(g_led_config.flags[i], (LED_FLAG_MODIFIER | LED_FLAG_UNDERGLOW))) { // Encoder and Thumbs
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
  return false;
}
#endif

/*
 MISC
 */
#ifdef COMBO_SHOULD_TRIGGER
bool combo_should_trigger(uint16_t index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
  switch (index) {
    case key_z:
    case key_q:
    case key_quot:
      return true;
  }
  return !within_flow_tap_term(keycode, record);
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

// Tap hold decisions
#ifdef QUICK_TAP_TERM_PER_KEY
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // case THM_1:
    //   return 0;
    case KC_BSPC:
      return QUICK_TAP_TERM * 3;
    default:
      return QUICK_TAP_TERM;
  }
}
#endif

// Tap hold decisions in order of precedence
/*
- Flow Tap (fast typing → force tap)
- Other-key pressed → hold
- Chord behaviour → hold
- Tapping term exceeded → hold
- Otherwise → tap
*/
#ifdef FLOW_TAP_TERM
bool is_flow_tap_key(uint16_t keycode) {
  if ((get_mods() & (MOD_MASK_CG | MOD_BIT_LALT)) != 0) {
    return false; // Disable Flow Tap on hotkeys.
  }
  switch (get_tap_keycode(keycode)) {
    // case KC_SPC: // space was causing issues as it's a layer tap
    case KC_A ... KC_Z:
    case KC_DOT:
    case KC_COMM:
    case KC_GRV:
    case KC_SCLN:
    case KC_SLSH:
      return true;
  }
  return false;
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
  if (is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode)) {
    switch (keycode) {
      // reduce accidental home row mod (non-shift) triggers
      case HM_R:
      case HM_S:
      case HM_T:
      case HM_K:
      case HM_X:
      case HM_A:
      case HM_I:
      case HM_O:
        return FLOW_TAP_TERM + 20;

      default:
        return FLOW_TAP_TERM;
    }
  }
  return 0;
}
#endif

#if defined(HOLD_ON_OTHER_KEY_PRESS) || defined(PERMISSIVE_HOLD)
#  ifdef HOLD_ON_OTHER_KEY_PRESS
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
#  else
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
#  endif
  switch (keycode) {
    // case THM_1:
    case THM_2:
    case THM_3:
      return true;

    // Immediately select the hold action when another key is pressed if not typing
    case THM_4:
      return !IS_TYPING();
  }
  return false;
}
#endif

// Hummingbird has a crazy matrix, so handedness it defined in its keyboard.json
#ifndef KEYBOARD_hummingbird
char chordal_hold_handedness(keypos_t key) {
  // char hand = key.row == 3 || key.row == 7 ? '*' : key.row < MATRIX_ROWS / 2 ? 'L' : 'R';
  char hand = key.row < MATRIX_ROWS / 2 ? 'L' : 'R';
  return hand;
}
#endif

bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode,
                      keyrecord_t *other_record) {
  // Exceptionally allow some one-handed chords for hotkeys.
  switch (tap_hold_keycode) {
    case HM_T:
      if (other_keycode == KC_W || other_keycode == KC_Q || other_keycode == KC_R) {
        return true;
      }
      break;

    case THM_1:
    case THM_4:
      return !IS_TYPING();

    case THM_2:
    case THM_3:
      return true;
  }

  return get_chordal_hold_default(tap_hold_record, other_record);
}

#ifdef TAPPING_TERM_PER_KEY
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  uint8_t mod = mod_config(QK_MOD_TAP_GET_MODS(keycode));
  if (mod & (MOD_LSFT != 0) || mod & (MOD_RSFT != 0)) {
    return TAPPING_TERM - 50;
  }

  switch (keycode) {
    case HM_R:
    case HM_S:
    case HM_T:
    case HM_A:
    case HM_I:
    case HM_O:
      return TAPPING_TERM + 100;

    case THM_2:
    case THM_3:
      // case THM_4:
      return TAPPING_TERM - 80;
    default:
      return TAPPING_TERM;
  }
}
#endif

/*
 Keyboard lifecycle
 */
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

void keyboard_post_init_user(void) {
  set_single_persistent_default_layer(BSE);
// Customise these values to desired behaviour
#ifdef CONSOLE_ENABLE
  debug_enable = true;
  // debug_matrix = true;
  // debug_mouse  = true;
#endif

#ifdef RGBLIGHT_ENABLE
  rgblight_sethsv_at(HSV_WHITE, 0);
#endif

#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
  set_auto_mouse_enable(true);
#endif
}

/*
 Layer state
 */
layer_state_t layer_state_set_user(layer_state_t state) {
#ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
  switch (get_highest_layer(remove_auto_mouse_layer(state, true))) {
    case SYM ... FNC:
      // remove_auto_mouse_target must be called to adjust state *before* setting enable
      state = remove_auto_mouse_layer(state, false);
      set_auto_mouse_enable(false);
      break;
    default:
      set_auto_mouse_enable(true);
      break;
  }
#endif

#ifdef RGBLIGHT_ENABLE
  switch (get_highest_layer(state)) {
    case NAV:
      rgblight_sethsv_at(HSV_SPRINGGREEN, 0);
      break;
    case SYM:
      rgblight_sethsv_at(HSV_ORANGE, 0);
      break;
    case NUM:
      rgblight_sethsv_at(HSV_CYAN, 0);
      break;
    case FNC:
      rgblight_sethsv_at(HSV_PINK, 0);
      break;
    default:
      rgblight_sethsv_at(HSV_WHITE, 0);
      break;
  }
#endif
  return state;
}

#ifdef RGBLIGHT_ENABLE
layer_state_t default_layer_state_set_user(layer_state_t state) {
  rgblight_sethsv_at(HSV_WHITE, 0);
  return state;
}
#endif

// reset CPI after wake
#ifdef KEYBOARD_charybdis
void suspend_wakeup_init_user(void) {
  keyboard_post_init_kb();
  keyboard_post_init_user();
}
#endif
