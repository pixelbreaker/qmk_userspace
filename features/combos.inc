// clang-format off

// Layout key reference
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_QUOT
// KC_Z    KC_X    KC_C    KC_V    KC_B        TH_N    TH_M    TH_COMM TH_DOT  TH_SLSH
//                      SPC_SYM TAB_MED        ENT_FUN BSP_NUM

// general
COMB(sft_capsword, CW_TOGG, HM_F, HM_J)

// switchers
COMB(appswitch, APPSWITCH, KC_C, KC_V)
COMB(tabswitch, TABSWITCH, KC_X, KC_C)

// thumbs
COMB(thmb_l,  OSM_HYPR, SPC_SYM, TAB_MED)
COMB(thmb_r,  KC_DEL, ENT_FUN, BSP_NUM)

// macros
SUBS(c_arr, "->", KC_H, KC_N)
SUBS(js_arrfn, "=>", HM_J, KC_M)
