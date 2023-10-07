// Copyright 2023 @filterpaper
// Copyright 2023 @pixelbreaker
// SPDX-License-Identifier: GPL-2.0+

#include "pixelbreaker.h"

#ifdef CONSOLE_ENABLE
#  include "print.h"
#endif

#if POINTING_DEVICE_ENABLE
#  include "features/scrollspam.h"
#endif

#ifndef SCROLL_BUFFER_SIZE
#  define SCROLL_BUFFER_SIZE 50
#endif

#define TYPING_TERM (TAPPING_TERM * 2)
// #define IS_TYPING() (timer_elapsed_fast(tap_timer) < TYPING_TERM)
// #define IS_HOMEROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
// #define IS_MT_SHIFT(k) (QK_MOD_TAP_GET_MODS(k) & MOD_MASK_SHIFT)

#if defined(PERMISSIVE_HOLD_PER_KEY) || defined(HOLD_ON_OTHER_KEY_PRESS_PER_KEY)
static uint16_t    next_keycode;
static keyrecord_t next_record;

#  define TAP_INTERVAL_MS 100

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
  // static uint16_t prev_keycode;
  if (record->event.pressed) {
    // Store the previous keycode for instant tap decision
    // prev_keycode = next_keycode;
    // Cache the next input for mod-tap decisions
    next_keycode = keycode;
    next_record  = *record;
  }
  // Match home row mod-tap keys when it is not preceded by a Layer key
  // if (IS_HOMEROW(record) && IS_QK_MOD_TAP(keycode) && !IS_QK_LAYER_TAP(prev_keycode)) {
  //   // Tap the mod-tap key instantly when it follows a short interval
  //   if (record->event.pressed && last_input_activity_elapsed() < TAP_INTERVAL_MS) {
  //     record->keycode = keycode & 0xff;
  //     action_tapping_process(*record);
  //     return false;
  //   } else { // Send the base keycode key up event
  //     keyrecord_t base_record   = *record;
  //     base_record.keycode       = keycode & 0xff;
  //     base_record.event.pressed = false;
  //     action_tapping_process(base_record);
  //   }
  // }
  return true;
}
#endif

#ifdef TAPPING_TERM_PER_KEY
static fast_timer_t tap_timer = 0;

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case THM_1:
      return TAPPING_TERM;
    default:
      // Increase tapping term for the non-Shift home row mod-tap while typing
      return IS_HOMEROW(record) && !IS_MOD_TAP_SHIFT(keycode) && IS_TYPING() ? TAPPING_TERM * 2 : TAPPING_TERM;
  }
}
#endif

#ifdef PERMISSIVE_HOLD_PER_KEY
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  // Hold Control and Shift with a nested key tap on the opposite hand
  return IS_BILATERAL_TAP(record, next_record) && IS_MOD_TAP_CS(keycode);
}
#endif

#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  // Replace the mod-tap key with its base keycode
  // when tapped with another key on the same hand
  if (IS_UNILATERAL_TAP(record, next_record)) {
    // Mask the base keycode and send the tap event
    record->keycode = keycode & 0xff;
    process_record(record);
    // Send the base keycode key up event
    record->event.pressed = false;
    process_record(record);
    // Return true to end action tapping process
    return true;
  }
  return false;
}
#endif

// Send custom hold keycode
static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record) {
  if (record->tap.count) return true;
  tap_code16(keycode);
  return false;
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

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
#ifdef TAPPING_TERM_PER_KEY
  tap_timer = timer_read_fast();
#endif

  // TAP holds
  if (record->event.pressed) {
    if (keycode == TH_X)
      return process_tap_hold(Z_CUT, record);
    else if (keycode == TH_C)
      return process_tap_hold(Z_CPY, record);
    else if (keycode == TH_D || keycode == TH_V)
      return process_tap_hold(Z_PST, record);
    else if (keycode == TH_QUOT)
      return process_tap_hold(S(KC_QUOT), record);
    else if (keycode == TH_SLSH || keycode == MSE(TH_SLSH))
      return process_tap_hold(KC_BSLS, record);
    else if (keycode == TH_W)
      return process_tap_hold(KC_AT, record);
    else if (keycode == TH_F || keycode == TH_E)
      return process_tap_hold(Z_HASH, record);
    else if (keycode == TH_G)
      return process_tap_hold(KC_MINS, record);
    else if (keycode == TH_B)
      return process_tap_hold(KC_UNDS, record);

    // return process_tap_hold(OSM(MOD_HYPR), record);
  }

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
    case THM_1:
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
      return true;
    case THM_4:
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
      return true;

    case THM_2:
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
      return true;

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
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      encoder_mode = HUE;
#endif
      return false;
    case E_SAT:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      encoder_mode = SAT;
#endif
      return false;
    case E_VAL:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      encoder_mode = VAL;
#endif
      return false;
    case E_SPD:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      encoder_mode = SPD;
#endif
      return false;
    case E_MOD:
#if defined(RGB_MATRIX_ENABLE) && defined(ENCODER_ENABLE)
      encoder_mode = MOD;
#endif
      return false;

    default:
      return true;
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
  if (IS_HOMEROW(record) && IS_QK_MOD_TAP(keycode)) {
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

void keyboard_post_init_user(void) {
// Customise these values to desired behaviour
#ifdef CONSOLE_ENABLE
  debug_enable = true;
  //   debug_matrix = true;
  // debug_mouse = true;
#endif

#ifdef RGBLIGHT_ENABLE
  // rgblight_set_effect_range(0, 1);
  rgblight_sethsv_at(HSV_WHITE, 0);
#endif
}

#ifdef RGBLIGHT_ENABLE
layer_state_t default_layer_state_set_user(layer_state_t state) {
  rgblight_sethsv_at(HSV_WHITE, 0);
  return state;
}

layer_state_t layer_state_set_user(layer_state_t state) {
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

#if defined(KEYBOARD_tenome) || defined(KEYBOARD_buteo)
void pointing_device_init_kb() {
#  ifdef TRACKBALL_ENABLE
  pointing_device_set_cpi(900);
#  else
  pointing_device_set_cpi(500);
#  endif
}
#endif
