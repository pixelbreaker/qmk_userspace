// Copyright 2023 @filterpaper
// Copyright 2023 @pixelbreaker
// SPDX-License-Identifier: GPL-2.0+

#include "pixelbreaker.h"
#include "layout.h"
#include "quantum_keycodes.h"

#ifdef RGB_MATRIX_ENABLE
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
uint8_t track_mode   = CURSOR;

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
// disable the shift key when holding down shift and moving the caret
#  ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
    // if (IS_LAYER_OFF(get_auto_mouse_layer()) && (abs(mouse_report.x) > 2 || abs(mouse_report.y) > 2)) {
    //   unregister_mods(MOD_MASK_SHIFT);
    // }
    tap_tb(KC_RIGHT, KC_LEFT, KC_UP, KC_DOWN);
#  endif
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

/*
 Main Processing
 */
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  uprintf("Col: %d Row: %d\n", record->event.key.col, record->event.key.row);

// set_single_persistent_default_layer(BSE);
#ifdef TAPPING_TERM_PER_KEY
  tap_timer = timer_read_fast();
#endif

  // TAP holds
  if (record->event.pressed) {
    prev_event = record->event.type;

    switch (keycode) {
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

      case TH_CT_UN:
        if (record->tap.count) {
          tap_code16(Z_UND);
        } else {
          tap_code16(Z_CUT);
        }
        return false;

      case TH_CP_SV:
        if (record->tap.count) {
          tap_code16(Z_SAVE);
        } else {
          tap_code16(Z_CPY);
        }
        return false;

      case TH_PS_RE:
        if (record->tap.count) {
          tap_code16(Z_RDO);
        } else {
          tap_code16(Z_PST);
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
      if (IS_LAYER_ON(get_auto_mouse_layer())) {
        layer_off(get_auto_mouse_layer());
      }
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
  hsv_t hsv        = {0, 255, 255};
  uint  curr_layer = get_highest_layer(layer_state | default_layer_state);

  if (curr_layer == 0) return false;

  // generate a hue for the current layer 0-255
  uint8_t hue = (((float)curr_layer + 1) / 6) * 255;
  hsv.h       = hue;
  hsv.v       = rgb_matrix_get_val();
  rgb_t rgb   = hsv_to_rgb(hsv);

  // set the rgb of all underglow and modifier flagged RGB LEDs
  for (uint8_t i = led_min; i < led_max; i++) {
    if (HAS_ANY_FLAGS(g_led_config.flags[i], (0x01 | 0x02))) { // Encoder and Thumbs
      rgb_matrix_set_color(i, rgb.r, rgb.g, rgb.b);
    }
  }
  return false;
}
#endif

/*
 MISC
 */
 #ifdef COMBO_ENABLE
 #  ifdef COMBO_SHOULD_TRIGGER
 bool combo_should_trigger(uint16_t index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
   bool below_base = get_highest_layer(layer_state) <= BSE;

   switch (index) {
     case thmb_r:
     case key_ent:
     // case copy:
     case tab:
 #    ifndef POINTING_DEVICE_AUTO_MOUSE_ENABLE
     case mouse_layer:
 #    endif
       return below_base && !IS_TYPING();
   }

   return below_base;
 }
 #  endif
 #endif

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
    case THM_1:
      return 0;
    case KC_BSPC:
      return QUICK_TAP_TERM * 3;
    default:
      return QUICK_TAP_TERM;
  }
}

/*
 Keyboard init
 */
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
