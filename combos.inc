// clang-format off

// Layout key reference
// KC_V,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    TH_QUOT,
// KC_R,    KC_S,    KC_T,    KC_H,    KC_K,        KC_X,    KC_N,    KC_A,    KC_I,    TH_O,
// KC_Z,    KC_C,    KC_G,    KC_D,    KC_V,        KC_Q,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,
//                            THM_1,   THM_2,       THM_3,   THM_4

// general
// COMB(capsword, CW_TOGG, KC_D, KC_M)
COMB(esc, KC_ESC, KC_V, KC_W)

// mouse layer
#ifndef POINTING_DEVICE_AUTO_MOUSE_ENABLE
HOLD(mouse_layer, MO(MOU), KC_S, KC_T)
#endif

// OSM on thumbs
HOLD(thmb_l, MO(FNC), THM_1, THM_4)
COMB(capsword, CW_TOGG, KC_D, KC_M)

// left hand
COMB(tab, KC_TAB, KC_R, KC_S)

// COMB(key_q, KC_Q, KC_W, KC_F)
COMB(key_qu, TH_QU, KC_W, KC_F)
COMB(key_z, KC_Z, KC_C, KC_G)

// cut copy paste
COMB(cut, Z_CUT, KC_F, KC_P)
COMB(copy, Z_CPY, KC_T, KC_H)
COMB(paste, Z_PST, KC_G, KC_D)

// right hand
COMB(key_mins, KC_MINS, KC_U, KC_Y)
COMB(key_unds, KC_UNDS, KC_L, KC_U)
COMB(key_slsh, KC_SLSH, KC_COMM, KC_DOT)
