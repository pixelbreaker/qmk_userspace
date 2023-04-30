// clang-format off

// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// MS_Z    KC_X    KC_C    KC_V    KC_B        TH_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//                      SPC_NAV TAB_SYM        ENT_FUN BSP_NUM

// general
COMB(sft_capsword, CW_TOGG, HM_F, HM_J)
COMB(hypr, OSM_HYPR, KC_C, KC_V)
COMB(meh, OSM_MEH, KC_M, KC_COMM)

// switchers
HOLD(appswitch, APPSWITCH, KC_E, KC_R)
HOLD(tabswitch, TABSWITCH, MS_Z, KC_X)

// characters
COMB(char_tilde, KC_TILDE, KC_O, KC_P)

// thumbs
COMB(thmb_l,  ESC_MED, SPC_NAV, TAB_SYM)
COMB(thmb_r,  KC_DEL, ENT_FUN, BSP_NUM)

// macros
SUBS(c_arr, "->", KC_H, KC_N)
SUBS(js_arr, "=>", HM_J, KC_M)
SUBS(js_arrfn, "() => {}" SS_TAP(X_LEFT) "\n", HM_K, KC_COMM)
