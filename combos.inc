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
HOLD(mouse_layer, MO(MOU), KC_S, KC_T)

// OSM on thumbs
COMB(thmb_l, OSL(FNC), THM_1, THM_4)
COMB(capsword, CW_TOGG, THM_2, THM_3)

// left hand
// COMB(sft_l, OSM_LSFT, KC_T, KC_H)
COMB(tab, KC_TAB, KC_R, KC_S)
// COMB(tab_mse, KC_TAB, MSE(KC_R), KC_S)

// COMB(key_q, KC_Q, KC_W, KC_F)
COMB(key_qu, TH_QU, KC_W, KC_F)
COMB(key_z, KC_Z, KC_C, KC_G)

// cut copy paste
COMB(cut, Z_CUT, KC_F, KC_P)
COMB(copy, Z_CPY, KC_T, KC_H)
COMB(paste, Z_PST, KC_G, KC_D)

COMB(bckspc, KC_BSPC, KC_M, KC_COMM)
COMB(key_slsh, KC_SLSH, KC_COMM, KC_DOT)

// 23332 combos (colemak only)
#ifdef LAYOUT_23332
// COMB(osl_extra, OSL(EXT), THM_3, THM_4)
#endif
