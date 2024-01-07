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
#define IS_MOD_TAP_SHIFT(kc) (QK_MOD_TAP_GET_MODS(kc) & MOD_LSFT)
#define IS_MOD_TAP_CS(kc)    (QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LSFT))
#define IS_MOD_TAP_CAG(kc)   (QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LALT | MOD_LGUI))
#define IS_MOD_TAP_G(kc)   (QK_MOD_TAP_GET_MODS(kc) & (MOD_LGUI))
#define IS_LAYER_TAP(kc)     (IS_QK_LAYER_TAP(kc) &&  QK_LAYER_TAP_GET_LAYER(kc))

#ifdef KEYBOARD_hummingbird
    // specifically for hummingbird's odd matrix
    #define IS_LEFT(e) (e.key.col < 2 || (e.key.col == 2 && e.key.row % 2 == 1))
    #define IS_HOMEROW(r) (r->event.key.row == 2 || r->event.key.row == 3)
    #define IS_UNILATERAL_TAP(r, n) (r->event.key.col != n.event.key.col) && IS_HOMEROW(r) && (IS_LEFT(r->event) == IS_LEFT(n.event))
    #define IS_BILATERAL_TAP(r, n) IS_HOMEROW(r) && IS_LEFT(r->event) != IS_LEFT(n.event)
#elif KEYBOARD_buteo
    // specifically for buteo's matrix
    #define IS_HOMEROW(r) (r->event.key.row == 1 || r->event.key.row == 4)
    
    // Mod-tap and the key that follows are not the same keys
    // and they are on the same side of the keyboard
    #define IS_UNILATERAL_TAP(r, n) (IS_HOMEROW(r) && (r->event.key.col != n.event.key.col) && (    \
        (r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
        (r->event.key.row == 4 && 3 <= n.event.key.row && n.event.key.row <= 5) ))

    // Mod-tap and the key that follows are on opposite sides of the keyboard
    #define IS_BILATERAL_TAP(r, n) ( \
        (r->event.key.row == 1 && 3 <= n.event.key.row && n.event.key.row <= 5) || \
        (r->event.key.row == 4 && 0 <= n.event.key.row && n.event.key.row <= 2) )
#else
    #define IS_HOMEROW(r) (r->event.key.row == 1 || r->event.key.row == 5)
    
    // Mod-tap and the key that follows are not the same keys
    // and they are on the same side of the keyboard
    #define IS_UNILATERAL_TAP(r, n) ((r->event.key.col != n.event.key.col) && (    \
        (r->event.key.row == 1 && 0 <= n.event.key.row && n.event.key.row <= 2) || \
        (r->event.key.row == 5 && 4 <= n.event.key.row && n.event.key.row <= 6) ))

    // Mod-tap and the key that follows are on opposite sides of the keyboard
    #define IS_BILATERAL_TAP(r, n) ( \
        (r->event.key.row == 1 && 4 <= n.event.key.row && n.event.key.row <= 6) || \
        (r->event.key.row == 5 && 0 <= n.event.key.row && n.event.key.row <= 2) )
#endif

#ifdef TRACKBALL_ENABLE 
  #define DPI_POINTER 900
  #define DPI_SCROLL 600
#else 
  #define DPI_POINTER 500
  #define DPI_SCROLL 400
#endif

// clang-format on
#ifdef COMBO_ENABLE
#  include "combos.h"
#endif
