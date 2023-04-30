// Copyright 2023 @filterpaper
// Copyright 2023 @pixelbreaker
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

#if defined(POINTING_DEVICE_ENABLE)
#  include "features/scrollspam.h"
#endif
#ifdef ACHORDION_ENABLE
#  include "features/achordion.h"
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
      return TAPPING_TERM + 80;
    default:
      return TAPPING_TERM;
  }
  //   return TAPPING_TERM; // IS_HOME_ROW(record) && !IS_MT_SHIFT(keycode) && IS_TYPING() ? TYPING_TERM : TAPPING_TERM;
}

// Select Shift hold immediately with a nested key
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
  return IS_QK_MOD_TAP(keycode) && IS_MT_SHIFT(keycode) && !IS_TYPING();
}

// Select layer hold immediately with another key
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
  if (keycode == SPC_NAV) {
    return false;
  }
  return IS_QK_LAYER_TAP(keycode) && QK_LAYER_TAP_GET_LAYER(keycode) > 0;
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
  CARRET,
  MEDIA,
};
uint8_t track_mode = CURSOR;

bool     mouse_is_down    = false;
uint16_t last_mouse_press = 0; // for click tracking pause

// carret layer delay timers
static deferred_token activate_track_mode_token = INVALID_DEFERRED_TOKEN;

uint32_t activate_media_mode(uint32_t trigger_time, void *cb_arg) {
  track_mode = MEDIA;
  return 0;
}

uint32_t activate_carret_mode(uint32_t trigger_time, void *cb_arg) {
  track_mode = CARRET;
  return 0;
}

report_mouse_t pointing_device_task_user(report_mouse_t mouse_report) {
  // Pause mouse report updates for short time after clicking to make it easier
  // to double click with small movement of trackball
  bool mouse_pause = mouse_is_down && timer_elapsed(last_mouse_press) < 150;

  if (track_mode != CURSOR || appswitch_active || tabswitch_active || mouse_pause) {
    // Nerf mouse_report as we're doing something else
    tap_report(mouse_report);
    mouse_report.x = 0;
    mouse_report.y = 0;
  }
  if (track_mode == MEDIA) {
    tap_media();
  } else if (track_mode == CARRET) {
    tap_tb(KC_RIGHT, KC_LEFT, KC_UP, KC_DOWN);
  } else if (appswitch_active || tabswitch_active) {
    tap_switcher();
  }
  return mouse_report;
}
#endif // POINTING_DEVICE_ENABLE

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
    // set trackball modes...
    case SPC_NAV:
#ifdef KEYBOARD_charybdis
      charybdis_set_pointer_dragscroll_enabled(record->event.pressed);
#endif
      return return_or_achordion(true, keycode, record);
    case ESC_MED:
      if (record->event.pressed) {
        if (!extend_deferred_exec(activate_track_mode_token, MEDIA_TIMEOUT_MS)) {
          activate_track_mode_token = defer_exec(MEDIA_TIMEOUT_MS, activate_media_mode, NULL);
        }
      } else {
        cancel_deferred_exec(activate_track_mode_token);
        track_mode = CURSOR;
      }
      return return_or_achordion(true, keycode, record);
    case TAB_SYM:
      if (record->event.pressed) {
        if (!extend_deferred_exec(activate_track_mode_token, CARRET_TIMEOUT_MS)) {
          activate_track_mode_token = defer_exec(CARRET_TIMEOUT_MS, activate_carret_mode, NULL);
        }
      } else {
        cancel_deferred_exec(activate_track_mode_token);
        track_mode = CURSOR;
      }
      return return_or_achordion(true, keycode, record);

    // Pause mouse report updates for short time after clicking to make it easier
    // to double click with small movement of trackball
    case KC_BTN1 ... KC_BTN3:
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
      // case HRM_A:
      //     if (other_keycode == KC_C || other_keycode == KC_X) {
      //         return true;
      //     }
      //     break;

    case HM_D: // S + Tab.
      if (other_keycode == HM_S || other_keycode == TAB_SYM) {
        return true;
      }
      break;

    case HM_S: // Tab.
    case HM_F: // Tab.
      if (other_keycode == TAB_SYM) {
        return true;
      }
      break;
  }

  // Otherwise, follow the opposite hands rule.
  return achordion_opposite_hands(tap_hold_record, other_record);
}

uint16_t achordion_timeout(uint16_t tap_hold_keycode) {
  switch (tap_hold_keycode) {
    case TAB_SYM:
    case ENT_FUN:
    case BSP_NUM:
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
    case MOD_HYPR:
    case MOD_MEH:
      return true;

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

// reset CPI after wake
#ifdef KEYBOARD_charybdis
void suspend_wakeup_init_user(void) {
  keyboard_post_init_kb();
}
#endif
