// clang-format off

// Layout key reference
// KC_Q    TH_W    TH_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    TH_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// MS_Z    TH_X    TH_C    TH_V    KC_B        KC_N    KC_M    KC_COMM KC_DOT  MS_SLSH
//                      THM_1 THM_2        THM_3 THM_4

// general
COMB(capsword, CW_TOGG, KC_P, KC_L)
COMB(capslock, KC_CAPS, TH_F, KC_U)
COMB(esc, KC_ESC, QK_REP, TH_W)

// layers on thumbs
// COMB(lyr_fn, MO(FNC), THM_1, THM_2)
COMB(lyr_num, MO(FNC), THM_3, THM_4)

// OSM on thumbs
COMB(thmb_l, OSM_HYPR, THM_1, THM_4)
COMB(thmb_r, OSM_MEH, THM_2, THM_3)

// left hand 
COMB(sft_l, OSM_LSFT, KC_S, KC_T)
COMB(tab, KC_TAB, KC_R, KC_S)
COMB(tab_mse, KC_TAB, MSE(KC_R), KC_S)

COMB(key_q, KC_Q, TH_W, TH_F)
COMB(key_z, KC_V, TH_G, TH_D)
COMB(key_v, KC_Z, TH_C, TH_G)

// right hand
COMB(sft_r, OSM_LSFT, KC_A, KC_I)

COMB(bckspc, KC_BSPC, KC_M, KC_COMM)
COMB(bckspc_word, A(KC_BSPC), KC_M, TH_DOT)
COMB(del, KC_DEL, KC_Y, TH_QUOT)
COMB(ent, KC_ENT, KC_I, TH_O)
COMB(hyphen, KC_MINS, KC_U, KC_Y)

COMB(key_slsh, KC_SLSH, KC_COMM, TH_DOT)

// switchers
// HOLD(tabswitch, TABSWITCH, MS_Z, KC_X)
HOLD(appswitch, APPSWITCH, TH_C, TH_D)

// characters
// COMB(char_scln, KC_SCLN, HM_L, HM_QUOT)
// COMB(key_at, KC_AT, KC_W, TH_E)
// COMB(key_hash, Z_HASH, KC_I, KC_O)

// thumbs
// COMB(hypr, OSM_HYPR, TH_C, TH_V)

// SUBS(c_arr, "->", KC_H, KC_N)
// SUBS(js_arr, "=>", HM_J, KC_M)
// SUBS(js_arrfn, "() =>", HM_K, KC_COMM)
// SUBS(md_link, "[]()" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), HM_L, KC_DOT)
// SUBS(dir_up, "../", KC_DOT, MS_SLSH)

COMB(char_lbrc, KC_LBRC, TH_W, KC_S) // [
COMB(char_lcbr, KC_LCBR, TH_F, KC_T) // {
COMB(char_lprn, KC_LPRN, KC_P, KC_H) // (
COMB(char_lt, KC_LT, KC_B, KC_K) // <
COMB(char_gt, KC_GT, KC_J, KC_X) // >
COMB(char_rprn, KC_RPRN, KC_L, KC_N) // )
COMB(char_rcbr, KC_RCBR, KC_U, KC_A) // }
COMB(char_rbrc, KC_RBRC, KC_Y, KC_I) // ]

// 23332 combos (colemak only)
#ifdef LAYOUT_23332
// COMB(osl_extra, OSL(EXT), THM_3, THM_4)
#endif
