// clang-format off

// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// MS_Z    KC_X    KC_C    KC_V    KC_B        KC_N    KC_M    KC_COMM KC_DOT  MS_SLSH
//                      SPC_NAV TAB_SYM        ENT_FUN BSP_NUM

// general
COMB(sft_capsword, CW_TOGG, HM_F, HM_J)
COMB(hypr, OSM_HYPR, KC_C, KC_V)
COMB(meh, OSM_MEH, KC_M, KC_COMM)
// COMB(rgb_layer, TG(LED), KC_M, KC_COMM, KC_DOT)
COMB(esc, KC_ESC, KC_Q, KC_W)

// switchers
HOLD(appswitch, APPSWITCH, KC_X, KC_C)
// HOLD(tabswitch, TABSWITCH, MS_Z, KC_X)
 
// shortcuts
COMB(save, G(KC_S), MS_Z, KC_X)

// characters
//COMB(char_tilde, KC_TILDE, KC_Q, KC_W)
COMB(char_scln, KC_SCLN, HM_L, HM_QUOT)
//COMB(char_btn1, KC_BTN1, KC_DOT, MS_SLSH)
//COMB(char_unds, KC_UNDS, KC_U, KC_I)  

// brackets
// COMB(char_lbrc, KC_LBRC, KC_W, HM_S)
// COMB(char_lcbr, KC_LCBR, KC_E, HM_D)
// COMB(char_lprn, KC_LPRN, KC_R, HM_F)
// COMB(char_lt, KC_LT, KC_T, KC_G)
// COMB(char_gt, KC_GT, KC_Y, KC_H)
// COMB(char_rprn, KC_RPRN, KC_U, HM_J)
// COMB(char_rcbr, KC_RCBR, KC_I, HM_K)
// COMB(char_rbrc, KC_RBRC, KC_O, HM_L)

// thumbs
COMB(thmb_l, KC_ESC, SPC_NAV, TAB_SYM) // hold space for panning in graphics apps
COMB(thmb_r, QK_LEAD, ENT_FUN, BSP_NUM)

// macros
SUBS(c_arr, "->", KC_H, KC_N)
SUBS(js_arr, "=>", HM_J, KC_M)
SUBS(js_arrfn, "() =>", HM_K, KC_COMM)
SUBS(md_link, "[]()" SS_TAP(X_LEFT) SS_TAP(X_LEFT) SS_TAP(X_LEFT), HM_L, KC_DOT)
SUBS(dir_up, "../", KC_DOT, MS_SLSH)
