// clang-format off

// Layout key reference
// KC_Q    TH_W    TH_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    TH_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// MS_Z    TH_X    TH_C    TH_V    KC_B        KC_N    KC_M    KC_COMM KC_DOT  MS_SLSH
//                      THM_1 THM_2        THM_3 THM_4

// general
COMB(sft_capsword, CW_TOGG, HM_F, HM_J)
// COMB(hypr, OSM_HYPR, KC_C, KC_V)
// COMB(meh, OSM_MEH, KC_M, KC_COMM)
// COMB(rgb_layer, TG(LED), KC_M, KC_COMM, KC_DOT)
// COMB(meh, OSM_MEH, HM_J, HM_K, HM_L)
// COMB(hypr, OSM_HYPR, HM_S, HM_D, HM_F)
// COMB(meh, OSM_HYPR, KC_X, KC_V)
// COMB(hypr, OSM_MEH, KC_M, KC_DOT)
COMB(esc, KC_ESC, KC_Q, TH_W)
COMB(esc_2, KC_ESC, MSE(KC_Q), TH_W)

// switchers
// HOLD(tabswitch, TABSWITCH, MS_Z, KC_X)

// characters
// COMB(char_scln, KC_SCLN, HM_L, HM_QUOT)
// COMB(key_at, KC_AT, KC_W, TH_E)
// COMB(key_hash, Z_HASH, KC_I, KC_O)

// thumbs
COMB(thmb_l, OSM_HYPR, THM_1, THM_2)
// COMB(hypr, OSM_HYPR, TH_C, TH_V)
COMB(thmb_r, OSM_MEH, THM_3, THM_4)

// macros
// SUBS(c_arr, "->", KC_H, KC_N)
// SUBS(js_arr, "=>", HM_J, KC_M)
// SUBS(js_arrfn, "() =>", HM_K, KC_COMM)
// SUBS(md_link, "[]()" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), HM_L, KC_DOT)
SUBS(dir_up, "../", KC_DOT, MS_SLSH)

COMB(char_lbrc, KC_LBRC, TH_W, HM_S)
COMB(char_lcbr, KC_LCBR, TH_E, HM_D)
COMB(char_lprn, KC_LPRN, KC_R, HM_F)
COMB(char_lt, KC_LT, KC_T, TH_G)
COMB(char_gt, KC_GT, KC_Y, KC_H)
COMB(char_rprn, KC_RPRN, KC_U, HM_J)
COMB(char_rcbr, KC_RCBR, KC_I, HM_K)
COMB(char_rbrc, KC_RBRC, KC_O, HM_L)


// 23332 combos (colemak only)
#if defined(KEYBOARD_hummingbird) || defined(KEYBOARD_buteo)
COMB(osl_extra, OSL(EXT), THM_1, THM_4)

COMB(key_z, KC_Z, TH_X, TH_C)
COMB(key_v, KC_V, TH_C, TH_V)
COMB(key_k, KC_K, KC_COMM, KC_M)
COMB(key_slsh, KC_SLSH, KC_COMM, KC_DOT)
// COMB(key_z, KC_Z, HM_S, TH_X)
// COMB(key_v, KC_V, HM_F, TH_V)
// COMB(key_k, KC_K, HM_J, KC_M)
// COMB(key_slsh, KC_SLSH, HM_L, KC_DOT)

// sym

#else
HOLD(appswitch, APPSWITCH, TH_X, TH_C)
#endif

