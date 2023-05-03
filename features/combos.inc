// clang-format off

// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// MS_Z    KC_X    KC_C    KC_V    KC_B        TH_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//                      SPC_NAV TAB_SYM        ENT_FUN BSP_NUM

// general
COMB(sft_capsword, CW_TOGG, 0, HM_F, HM_J)
COMB(hypr, OSM_HYPR, 0, KC_C, KC_V)
COMB(meh, OSM_MEH, 0, KC_M, KC_COMM)

// switchers
HOLD(appswitch, APPSWITCH, 0, KC_E, KC_R)
HOLD(tabswitch, TABSWITCH, 0, MS_Z, KC_X)

// characters
COMB(char_tilde, KC_TILDE, 50, KC_O, KC_P)
COMB(char_scln, KC_SCLN, 0, HM_L, HM_QUOT)
COMB(char_bsls, KC_BSLS, 0, KC_DOT, KC_SLSH)
COMB(char_unds, KC_UNDS, 0, KC_U, KC_I)  

// brackets
COMB(char_lbrc, KC_LBRC, 0, KC_W, HM_S)
COMB(char_lcbr, KC_LCBR, 0, KC_E, HM_D)
COMB(char_lprn, KC_LPRN, 0, KC_R, HM_F)
COMB(char_lt, KC_LT, 0, KC_T, KC_G)
COMB(char_gt, KC_GT, 0, KC_Y, KC_H)
COMB(char_rprn, KC_RPRN, 0, KC_U, HM_J)
COMB(char_rcbr, KC_RCBR, 0, KC_I, HM_K)
COMB(char_rbrc, KC_RBRC, 0, KC_O, HM_L)

// thumbs
COMB(thmb_l, ESC_MED, 0, SPC_NAV, TAB_SYM)
COMB(thmb_r, KC_DEL, 0, ENT_FUN, BSP_NUM)

// macros
SUBS(c_arr, "->", 0, KC_H, KC_N)
SUBS(js_arr, "=>", 0, HM_J, KC_M)
SUBS(js_arrfn, "() => {}" SS_TAP(X_LEFT) "\n", 0, HM_K, KC_COMM)
