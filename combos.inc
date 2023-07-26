// clang-format off

// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// MS_Z    KC_X    KC_C    KC_V    KC_B        KC_N    KC_M    KC_COMM KC_DOT  MS_SLSH
//                      SPC_NAV TAB_SYM        ENT_FUN BSP_NUM

// general
COMB(sft_capsword, CW_TOGG, HM_F, HM_J)
// COMB(hypr, OSM_HYPR, KC_C, KC_V)
// COMB(meh, OSM_MEH, KC_M, KC_COMM)
// COMB(rgb_layer, TG(LED), KC_M, KC_COMM, KC_DOT)
COMB(meh, OSM_MEH, HM_J, HM_K, HM_L)
COMB(hypr, OSM_HYPR, HM_S, HM_D, HM_F)
// COMB(meh, OSM_HYPR, KC_X, KC_V)
// COMB(hypr, OSM_MEH, KC_M, KC_DOT)
COMB(esc, KC_ESC, KC_Q, KC_W)

// switchers
// HOLD(tabswitch, TABSWITCH, MS_Z, KC_X)

// shortcuts
COMB(save, G(KC_S), MS_Z, KC_X)

// characters
COMB(char_scln, KC_SCLN, HM_L, HM_QUOT)
COMB(key_at, KC_AT, KC_W, KC_E)
COMB(key_hash, Z_HASH, KC_I, KC_O)

// thumbs
// COMB(thmb_l, OSM_HYPR, SPC_NAV, TAB_SYM)
COMB(thmb_r, OSM_MEH, ENT_FUN, BSP_NUM)

// macros
SUBS(c_arr, "->", KC_H, KC_N)
SUBS(js_arr, "=>", HM_J, KC_M)
SUBS(js_arrfn, "() =>", HM_K, KC_COMM)
SUBS(md_link, "[]()" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), HM_L, KC_DOT)
SUBS(dir_up, "../", KC_DOT, MS_SLSH)

// 23332 combos (colemak only)
// #ifdef KEYBOARD_hummingbird
COMB(osl_extra, OSL(EXT), SPC_NAV, BSP_NUM)
COMB(key_z, KC_Z, KC_X, KC_C)
COMB(key_v, KC_V, KC_C, KC_V)
COMB(key_k, KC_K, KC_COMM, KC_M)
COMB(key_slsh, KC_SLSH, KC_COMM, KC_DOT)

// num
COMB(key_0, KC_0, KC_1, KC_2)
COMB(key_eql, KC_EQL, KC_2, KC_3)

// sym

// #else
// HOLD(appswitch, APPSWITCH, KC_X, KC_C)
// #endif

