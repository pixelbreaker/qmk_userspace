#pragma once
#include QMK_KEYBOARD_H

// #include "quantum/keycodes.h"

extern void autocorrect_toggle(void);
extern bool process_autocorrect(uint16_t keycode, keyrecord_t *record);

// Return mod-tap modifiers in 8bit for MOD_MASK_* logical operations
#define GET_MT_MOD_BITS(kc) ((kc & 0x1000) ? ((kc >> 8) & 0x0f) << 4 : (kc >> 8) & 0x0f)

// Tap-hold decision helper macros
#define IS_TYPING() (timer_elapsed_fast(tap_timer) < FLOW_TAP_TERM * 2)

#ifdef TRACKBALL_ENABLE
  #define DPI_POINTER 800
  #define DPI_POINTER_SNIPE 400
  #define DPI_SCROLL 350
#else
  #define DPI_POINTER 400
  #define DPI_SCROLL 300
#endif

// clang-format on
#ifdef COMBO_ENABLE
#  include "combos.h"
#endif
