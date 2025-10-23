// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include "quantum/keycodes.h"

enum custom_keycode { APPSWITCH = QK_USER, TABSWITCH, APP_L, APP_R, ENC_DOWN, TGL_BASE, E_HUE, E_SAT, E_VAL, E_SPD, E_MOD, P_SNIPE };

// #ifdef POINTING_DEVICE_ENABLE
#define MSE(k) LT(MOU, k)
// #else
// #  define MSE(k) k
// #endif

// macOS shortcuts
#define Z_UND G(KC_Z)
#define Z_CUT G(KC_X)
#define Z_CPY G(KC_C)
#define Z_PST G(KC_V)
#define Z_RDO S(G(KC_Z))
#define Z_SAVE G(KC_S)
#define Z_LOCK C(G(KC_Q))
#define Z_SLEEP S(C(KC_EJCT))
#define Z_SHUT C(A(G(KC_PWR)))
#define Z_SSHT G(S(KC_4))
#define Z_SRCD G(S(KC_5))
#define Z_VSML A(KC_LSFT)
#define Z_HASH A(KC_3)
#define Z_EUR A(KC_2)
#define Z_GBP S(KC_3)

#define Z_AT S(KC_2)

#define MS_Z MSE(KC_Z)
#define MS_SLSH MSE(KC_SLSH)

#define OSM_HYPR OSM(MOD_HYPR)
#define OSM_MEH OSM(MOD_MEH)
#define OSM_LCTL OSM(MOD_LCTL)
#define OSM_LALT OSM(MOD_LALT)
#define OSM_LGUI OSM(MOD_LGUI)
#define OSM_LSFT OSM(MOD_LSFT)

#define DELLINE MEH(KC_DEL)

#define ___N___ KC_NO

// BastardKB Charybdis specific keycodes
// #if defined(KEYBOARD_charybdis) && !defined(KEYBOARD_cnano)
// #  define SNIPE SNIPING_MODE
#  define SNIPE P_SNIPE
// #else
// #  define SNIPE P_SNIPE
// #  define DPI_MOD _______
// #  define S_D_MOD _______
// #endif

// Mod tap macros
#define HM_R LCTL_T(KC_R)
#define HM_S LALT_T(KC_S)
#define HM_T LGUI_T(KC_T)
#define HM_H LSFT_T(KC_H)
#define HM_N RSFT_T(KC_N)
#define HM_A RGUI_T(KC_A)
#define HM_I LALT_T(KC_I)
#define HM_O RCTL_T(KC_O)
#define HRML(k1, k2, k3, k4) LCTL_T(k1), LALT_T(k2), LGUI_T(k3), LSFT_T(k4)
#define HRMR(k1, k2, k3, k4) RSFT_T(k1), RGUI_T(k2), LALT_T(k3), RCTL_T(k4)

// Tap hold macros

#define TH_C LT(0, KC_C)
#define TH_G LT(0, KC_G)
#define TH_D LT(0, KC_D)

#define TH_W LT(0, KC_W)
#define TH_F LT(0, KC_F)
#define TH_QU LT(0, KC_Q)

#define TH_QUOT LT(0, KC_QUOT)
#define TH_O LT(0, KC_O)
#define TH_DOT LT(0, KC_DOT)
#define TH_SLSH LT(0, KC_SLSH)

#define TH_DLR LT(0, KC_DLR)
#define TH_EQL LT(0, KC_EQL)
#define TH_LBRC LT(0, KC_LBRC)
#define TH_LPRN LT(0, S(KC_9))
#define TH_LCBR LT(0, KC_SPC)
#define TH_LT LT(0, KC_LT)
#define TH_DEL LT(0, KC_DEL)
#define TH_SCR LT(0, KC_PWR)

#define V_Q KC_V

// clang-format off
#define ___________________________________________ ___N___, ___N___, ___N___, ___N___, ___N___
// #define ______________HOME_ROW_CAGS_L______________ OSM_LCTL, OSM_LALT, OSM_LGUI, OSM_LSFT, KC_DEL
// #define ______________HOME_ROW_CAGS_R______________ ___N___, OSM_LSFT, OSM_LGUI, OSM_LALT, OSM_LCTL
// #define ______________HOME_ROW_CAGS_L______________ KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, KC_DEL
#define __________HOME_ROW_CAGS_L__________ KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT
#define __________HOME_ROW_CAGS_R__________ KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL
#define __________________APP_NAV__________________ C(S(KC_TAB)), C(KC_TAB), C(G(S(KC_TAB))), C(G(KC_TAB)), ___N___
#define _________________FILE_NAV__________________ ___N___, Z_UND,   Z_SAVE,  Z_RDO,   ___N___
// clang-format on

// Layers
enum layers { BSE, SYM, NAV, NUM, FNC, MOU, EXT };

// #define ESC_MED LT(MED, KC_ESC)
#define THM_0 LT(FNC, KC_ESC)

#define THM_1 LT(NAV, KC_SPC)
#define THM_2 LT(SYM, KC_ENT) // MO(SYM)

#define THM_3 LT(FNC, KC_BSPC)
#define THM_4 LT(NUM, KC_E)

// Default 3x5_2 split layout
// clang-format off

// Aptmak
#define _BASE \
	V_Q,     TH_W,    TH_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    TH_QUOT,    \
	KC_R,    KC_S,    KC_T,    KC_H,    KC_K,        KC_X,    KC_N,    KC_A,    KC_I,    TH_O,       \
	KC_Z,    TH_C,    TH_G,    TH_D,    KC_Q,        KC_GRV,  KC_M,    KC_COMM, TH_DOT,  TH_SLSH,    \
	                           THM_1,   THM_2,       THM_3,   THM_4

#define _NAV \
  KC_TAB,  APP_L,  A(KC_TAB),APP_R,    TH_SCR,     KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_GRV,     \
	__________HOME_ROW_CAGS_L__________, TH_DEL,     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_COLN,    \
	_________________FILE_NAV__________________,     ___N___, KC_ESC,  DELLINE, KC_SLSH, ___N___,    \
	                           _______, _______,     KC_BSPC, KC_DEL

/*
#define _SYMB \
	Z_GBP,   KC_AT,   Z_HASH,  KC_PERC, KC_CIRC,     TH_DLR,  KC_LCBR, KC_QUES,  KC_RCBR, KC_GRV,    \
	Z_EUR,   KC_LT,   KC_AMPR, KC_GT,   KC_ASTR,     KC_PLUS, KC_LPRN, KC_EXLM,  KC_RPRN, KC_TILDE,  \
	___N___, KC_BSLS, KC_PIPE, KC_SLSH, ___N___,     ___N___, KC_LBRC, TH_EQL,   KC_RBRC, ___N___,   \
	                           _______, _______,     KC_UNDS, KC_MINS
*/

#define _SYMB \
	___N___, KC_AT,   KC_LCBR, KC_RCBR, KC_CIRC,     TH_DLR,  KC_PERC, Z_HASH,  KC_ASTR, KC_SCLN,   \
	KC_TILDE,KC_LT,   KC_LPRN, KC_RPRN, KC_GT,       KC_PLUS, KC_AMPR, KC_EXLM, KC_PIPE, KC_COLN,   \
	___N___, KC_SPC,  KC_LBRC, KC_RBRC, ___N___,     ___N___, KC_BSLS, TH_EQL,  KC_SLSH, ___N___,   \
	                           _______, _______,     KC_UNDS, KC_MINS

#define _FUNC \
  QK_BOOT, KC_F7,   KC_F8,   KC_F9,   KC_F12,      KC_MSTP, KC_MPLY, KC_VOLU, Z_VSML,  TGL_BASE,   \
  KC_F10,  KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_CAPS, KC_MPRV, KC_VOLD, KC_MNXT, RGB_TOG,    \
  ___N___, KC_F1,   KC_F2,   KC_F3,   ___N___,     E_HUE,   E_SAT,   E_VAL,   E_SPD,   E_MOD,      \
	                				   _______, _______,     _______, _______

#define _NUMB \
	KC_ASTR, KC_7,    KC_8,    KC_9,    KC_PLUS,     _______, Z_GBP,   Z_EUR,   TH_DLR,  _______,    \
	KC_SLSH, KC_4,    KC_5,    KC_6,    KC_MINS,     KC_EQL, __________HOME_ROW_CAGS_R__________,    \
	KC_0,    KC_1,    KC_2,    KC_3,    ___N___,     ___________________________________________,    \
	                           KC_0,    KC_DOT,      _______, _______

#define _MOUSE \
	___N___, SNIPE,   ___N___, G(KC_BSPC),KC_DEL,    KC_WH_U, KC_WH_L, KC_MS_U, KC_WH_R, ___N___,    \
  __________HOME_ROW_CAGS_L__________, ___N___,    KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, ___N___,    \
  ___N___, SNIPE,   ___N___, KC_BTN3, ___N___,     ___N___, ___N___, ___N___, ___N___, ___N___,    \
                      		   KC_BTN1, KC_BTN2,     _______, _______

#define _EXTRA \
	___________________________________________,     ___________________________________________,    \
  ___N___, KC_Z,    ___N___, KC_V,    ___N___,     ___N___, KC_K,    ___N___, KC_SLSH, KC_BSLS,    \
  ___________________________________________,     ___________________________________________,    \
                      		   _______, _______,     _______, _______

// Mod-tap wrapper
#define HRM(k) HR_MODTAP(k)

#define HR_MODTAP( \
	  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	  l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	  l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                 l16, l17,   r16, r17                 \
) \
	  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,   r06, HRMR(r07, r08, r09, r10), \
 MSE(l11), l12, l13, l14, l15,   r11, r12, r13, r14, MSE(r15),  \
	                 l16, l17,   r16, r17


#define HRM2332(k) HR2332_MODTAP(k)

#define HR2332_MODTAP( \
	  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	  l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	  l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                 l16, l17,   r16, r17                 \
) \
	  MSE(l01), l02, l03, l04, l05,   r01, r02, r03, r04, r05,       \
 HRML(l06, l07, l08, l09), l10,   r06, HRMR(r07, r08, r09, r10), \
 l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15,  \
	                 l16, l17,   r16, r17


// Layout aliases for json keymap
#define LAYOUT_ferris(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_sphynx_w(...) LAYOUT_split_3x5_4(__VA_ARGS__)
#define LAYOUT_tenome_w(...) LAYOUT_split_3x5_3(__VA_ARGS__)
#define LAYOUT_charybdis_w(...) LAYOUT_charybdis_3x5(__VA_ARGS__)
#define LAYOUT_hummingbird_w(...) LAYOUT_23332x2(__VA_ARGS__)
#define LAYOUT_buteo_w(...) LAYOUT_23332x2_enc(__VA_ARGS__)


// Convert 3x5_2 to Charybdis Nano 3x5_3+2
#define CHARYBDIS(k) CONV_CHARYBDIS(k)
#define CONV_CHARYBDIS( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
		l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
		l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
		l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
		       THM_0, l16, l17,   r16, r17

#define CHARYBDIS_ENC(k) CONV_CHARYBDIS_ENC(k)
#define CONV_CHARYBDIS_ENC( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
		l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
		l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
		l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
		     ENC_DOWN, l16, l17,   r16, r17

// Convert 3x5_2 to Sphynx dacman 3x5_4 (with encoders optional)
#define SPHYNX(k) CONV_SPHYNX(k)
#define CONV_SPHYNX( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
		l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
		l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
		l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
		KC_NO, THM_0,l16, l17,   r16, r17, KC_DEL, ENC_DOWN

// Convert 3x5_2 to Tenome 3x5_2 (with encoders optional)
#define TENOME(k) CONV_TENOME(k)
#define CONV_TENOME( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
		l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
		l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
		l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
		      ENC_DOWN,l16, l17,   r16, r17, KC_NO

// Convert 3x5_2 to to 23332_2 split
#define HUMMINGBIRD(k) CONV_HUMMINGBIRD(k)
#define CONV_HUMMINGBIRD( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
		l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
		l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
		     l12, l13, l14,             r12, r13, r14,      \
		          l16, l17,             r16, r17

// Convert 3x5_2 to to 23332_2 split with encoder
#define BUTEO(k) CONV_BUTEO(k)
#define CONV_BUTEO( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
		l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
		l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
		     l12, l13, l14,             r12, r13, r14,      \
              l16, l17,  ENC_DOWN,  r16, r17

// clang-format on
