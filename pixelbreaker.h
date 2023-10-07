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
/*
#define LAYOUT_23332x2( \
         K00, K01, K02, K03, K04,    K05, K06, K07, K08, K09, \
         K10, K11, K12, K13, K14,    K15, K16, K17, K18, K19, \
              K20, K21, K22,              K27, K28, K29,      \
                        K23, K24,    K25, K26                 \
    )                                        \
    {                                        \
        { K00,   K02,   K04,         K06,   K08 }, \
        { K01,   K03,         K05,   K07,   K09 }, \
        { K10,   K12,   K14,         K16,   K18 }, \
        { K11,   K13,         K15,   K17,   K19 }, \
        { K20,   K22,   K24,         K28,   K26 }, \
        { K21,   K23,         K27,   K29,   K25 } \
    }


#define LAYOUT_split_3x5_3( \
    L00, L01, L02, L03, L04,     R00, R01, R02, R03, R04, \
    L10, L11, L12, L13, L14,     R10, R11, R12, R13, R14, \
    L20, L21, L22, L23, L24,     R20, R21, R22, R23, R24, \
              L32, L33, L34,     R30, R31, R32 \
) { \
    { L04, L03, L02, L01, L00 }, \
    { L14, L13, L12, L11, L10 }, \
    { L24, L23, L22, L21, L20 }, \
    { L34, L33, L32, XXX, XXX }, \
    { R00, R01, R02, R03, R04 }, \
    { R10, R11, R12, R13, R14 }, \  
    { R20, R21, R22, R23, R24 }, \
    { R30, R31, R32, XXX, XXX } \
}

*/
#define IS_TYPING()          (timer_elapsed_fast(tap_timer) < TAPPING_TERM * 2)
#define IS_MOD_TAP_SHIFT(kc) (QK_MOD_TAP_GET_MODS(kc) & MOD_LSFT)
#define IS_MOD_TAP_CS(kc)    (QK_MOD_TAP_GET_MODS(kc) & (MOD_LCTL | MOD_LSFT))
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
    #define IS_UNILATERAL_TAP(r, n) ((r->event.key.col != n.event.key.col) && (    \
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

// clang-format on
#ifdef COMBO_ENABLE
#  include "combos.h"
#endif
