// clang-format off

// Layout key reference
// QK_REP,  TH_W,    TH_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    TH_QUOT,
// KC_R,    KC_S,    KC_T,    KC_H,    KC_K,        KC_X,    KC_N,    KC_A,    KC_I,    TH_O,   
// KC_Z,    TH_C,    TH_G,    TH_D,    KC_V,        KC_Q,    KC_M,    KC_COMM, TH_DOT,  TH_SLSH,
//                            THM_1,   THM_2,       THM_3,   THM_4

// general
COMB(capsword, CW_TOGG, TH_D, KC_M)
// COMB(capsword, CW_TOGG, KC_H, KC_N)
COMB(capslock, KC_CAPS, KC_B, KC_J)
// COMB(esc, KC_ESC, QK_REP, TH_W)
// COMB(esc, KC_ESC, KC_V, TH_W)
COMB(esc, KC_ESC, V_Q, TH_W)
// COMB(esc_mse, KC_ESC, MSE(QK_REP), TH_W)
COMB(esc_mse, KC_ESC, MSE(KC_V), TH_W)

// layers on thumbs
// COMB(lyr_num, MO(FNC), THM_1, THM_2)
// COMB(lyr_fn, MO(FNC), THM_3, THM_4)

// OSM on thumbs
COMB(thmb_l, OSM_HYPR, THM_1, THM_4)
COMB(thmb_r, OSM_MEH, THM_2, THM_3)

// left hand 
// COMB(sft_l, OSM_LSFT, KC_T, KC_H)
COMB(tab, KC_TAB, HM_R, HM_S)
// COMB(tab_mse, KC_TAB, MSE(KC_R), KC_S)

// COMB(key_q, TH_QU, TH_W, TH_F)
COMB(key_q, KC_Q, TH_W, TH_F)
// COMB(key_v, KC_V, TH_G, TH_D)
// SUBS(key_qu, "qu", TH_G, TH_D)
COMB(key_qu, TH_QU, TH_G, TH_D)
COMB(key_z, KC_Z, TH_C, TH_G)

// right hand
// COMB(sft_r, OSM_LSFT, KC_N, KC_A)

// COMB(bckspc, KC_BSPC, KC_M, KC_COMM)
COMB(bckspc_word, A(KC_BSPC), KC_M, TH_DOT)
// COMB(del, KC_DEL, KC_Y, TH_QUOT)
// COMB(ent, KC_ENT, THM_3, THM_4)
COMB(hyphen, KC_MINS, KC_U, KC_Y)

COMB(key_slsh, KC_SLSH, KC_COMM, TH_DOT)

// switchers
// HOLD(tabswitch, TABSWITCH, MS_Z, KC_X)
HOLD(appswitch, APPSWITCH, TH_C, TH_D)

// chars
// COMB(char_lbrc, KC_LBRC, TH_W, HM_S) // [
// COMB(char_lcbr, KC_LCBR, TH_F, HM_T) // {
// COMB(char_lprn, KC_LPRN, KC_P, HM_H) // (
// COMB(char_lt, KC_LT, KC_B, KC_K) // <
// COMB(char_lt_mse, KC_LT, KC_B, MSE(KC_K)) // <
// COMB(char_gt, KC_GT, KC_J, KC_X) // >
// COMB(char_rprn, KC_RPRN, KC_L, HM_N) // )
// COMB(char_rcbr, KC_RCBR, KC_U, HM_A) // }
// COMB(char_rbrc, KC_RBRC, KC_Y, HM_I) // ]

// 23332 combos (colemak only)
#ifdef LAYOUT_23332
// COMB(osl_extra, OSL(EXT), THM_3, THM_4)
#endif
