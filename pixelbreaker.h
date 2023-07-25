#pragma once
#include QMK_KEYBOARD_H

// #include "quantum/keycodes.h"

// RGB_HUD, RGB_SAD, RGB_VAD, RGB_SPD, RGB_RMOD
// enum custom_keycode { APPSWITCH = QK_USER, TABSWITCH, ENC_DOWN, TGL_BASE, E_HUE, E_SAT, E_VAL, E_SPD, E_MOD };

extern void autocorrect_toggle(void);
extern bool process_autocorrect(uint16_t keycode, keyrecord_t *record);

__attribute__((always_inline)) static inline bool process_tap_hold(uint16_t keycode, keyrecord_t *record);

// Return mod-tap modifiers in 8bit for MOD_MASK_* logical operations
#define GET_MT_MOD_BITS(kc) ((kc & 0x1000) ? ((kc >> 8) & 0x0f) << 4 : (kc >> 8) & 0x0f)

// Tap-hold decision helper macros
// clang-format off
#define IS_TYPING()          (timer_elapsed_fast(tap_timer) < TAPPING_TERM * 2)
#define IS_HOMEROW(r)        (r->event.key.row == 1 || r->event.key.row == 5)
#define IS_MOD_TAP_SHIFT(kc) (QK_MOD_TAP_GET_MODS(kc) & MOD_LSFT)
#define IS_MOD_TAP_CS(kc)    (QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LSFT))

#define IS_UNILATERAL_TAP(r, n) ((r->event.key.col != n.event.key.col) && (    \
    (r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
    (r->event.key.row == 5 && 4 <= n.event.key.row && n.event.key.row <= 6) ))

#define IS_BILATERAL_TAP(r, n) ( \
    (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 6) || \
    (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 2) )

// clang-format on
#ifdef COMBO_ENABLE
#  include "combos.h"
#endif
