// clang-format off

// Layout key reference
// KC_V,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    TH_QUOT,
// KC_R,    KC_S,    KC_T,    KC_H,    KC_K,        KC_X,    KC_N,    KC_A,    KC_I,    TH_O,
// KC_Z,    KC_C,    KC_G,    KC_D,    KC_V,        KC_Q,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
//                            THM_1,   THM_2,       THM_3,   THM_4

// general
// COMB(capsword, CW_TOGG, KC_D, KC_M)
#include "keycodes.h"
COMB(esc, TH_ESC, KC_V, KC_W)
COMB(esc_mse, TH_ESC, MSE(KC_V), KC_W)

// mouse layer
#ifndef POINTING_DEVICE_AUTO_MOUSE_ENABLE
HOLD(mouse_layer, MO(MOU), KC_S, KC_T)
#endif

// both hands
HOLD(caret_nav, THM_1_2, THM_1, THM_2)
// HOLD(thmb_r, MO(FNC), THM_3, THM_4)

COMB(capsword, CW_TOGG, TH_D, KC_M)

// left hand
// COMB(tab, KC_TAB, KC_R, KC_S)
COMB(tab, KC_TAB, HM_R, HM_S)
COMB(key_q, KC_Q, TH_G, TH_D)
COMB(key_z, KC_Z, TH_C, TH_G)
COMB(key_quot, S(KC_QUOT), KC_W, KC_F)

// right hand
COMB(key_unds, KC_UNDS, KC_M, KC_COMM)
COMB(key_mins, KC_MINS, KC_L, KC_U)
COMB(key_grv, KC_GRV, KC_U, KC_Y)
COMB(key_slsh, KC_SLSH, KC_COMM, KC_DOT)
