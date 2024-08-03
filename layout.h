// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include "quantum/keycodes.h"

enum custom_keycode { APPSWITCH = QK_USER, TABSWITCH, APP_L, APP_R, ENC_DOWN, TGL_BASE, E_HUE, E_SAT, E_VAL, E_SPD, E_MOD };

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
#ifdef KEYBOARD_charybdis
#  define SNIPE SNIPING_MODE
#else
#  define SNIPE _______
#  define DPI_MOD _______
#  define S_D_MOD _______
#endif

// Mod tap macros
#define HM_A LCTL_T(KC_A)
#define HM_R LALT_T(KC_R)
#define HM_S LALT_T(KC_S)
#define HM_S2 LGUI_T(KC_S)
#define HM_T LSFT_T(KC_T)
#define HM_D LGUI_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J RSFT_T(KC_J)
#define HM_N RSFT_T(KC_N)
#define HM_E RGUI_T(KC_E)
#define HM_I LALT_T(KC_I)
#define HM_O RCTL_T(KC_O)
#define HM_K RGUI_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_QUOT RCTL_T(KC_QUOT)
#define HRML(k1, k2, k3, k4) LCTL_T(k1), LALT_T(k2), LGUI_T(k3), LSFT_T(k4)
#define HRMR(k1, k2, k3, k4) RSFT_T(k1), RGUI_T(k2), LALT_T(k3), RCTL_T(k4)

// Tap hold macros

#define TH_C LT(0, KC_C)
#define TH_G LT(0, KC_G)
#define TH_D LT(0, KC_D)

#define TH_W LT(0, KC_W)
#define TH_F LT(0, KC_F)

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

// clang-format off
#define ___________________________________________ ___N___, ___N___, ___N___, ___N___, ___N___
#define ______________HOME_ROW_CAGS_L______________ OSM_LCTL, OSM_LALT, OSM_LGUI, OSM_LSFT, KC_DEL
#define ______________HOME_ROW_CAGS_R______________ ___N___, OSM_LSFT, OSM_LGUI, OSM_LALT, OSM_LCTL
#define __________________APP_NAV__________________ C(S(KC_TAB)), C(KC_TAB), C(G(S(KC_TAB))), C(G(KC_TAB)), ___N___
#define _________________FILE_NAV__________________ ___N___, Z_UND,   Z_SAVE,  Z_RDO,   ___N___
// clang-format on

// Layers
enum layers { BSE, SYM, NAV, NUM, FNC, MOU, EXT };

// #define ESC_MED LT(MED, KC_ESC)
#define THM_0 LT(FNC, KC_ESC)

#define THM_1 LT(NAV, KC_SPC)
#define THM_2 MO(SYM)

#define THM_3 LT(NUM, KC_ENT)
#define THM_4 KC_E

// Default 3x5_2 split layout
// clang-format off

// Aptmak
#define _BASE \
	QK_REP,  TH_W,    TH_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    TH_QUOT,    \
	KC_R,    KC_S,    KC_T,    KC_H,    KC_K,        KC_X,    KC_N,    KC_A,    KC_I,    TH_O,       \
	KC_Z,    TH_C,    TH_G,    TH_D,    KC_V,        KC_Q,    KC_M,    KC_COMM, TH_DOT,  TH_SLSH,    \
	                           THM_1,   THM_2,       THM_3,   THM_4

#define _NAV \
  KC_TAB,  APP_L,   _______, APP_R,   G(KC_BSPC),  KC_PGUP, KC_HOME, KC_UP,   KC_END,  KC_COLN,    \
	______________HOME_ROW_CAGS_L______________,     KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_SCLN,    \
	_________________FILE_NAV__________________,     ___N___, KC_BSPC, DELLINE, KC_QUES, ___N___,    \
	                           _______, _______,     KC_ENT,  KC_DEL

#define _SYMB \
	Z_SSHT,  Z_EUR,   Z_GBP,   TH_LCBR, TH_LBRC,     KC_ASTR, KC_PLUS, KC_PIPE,  KC_AMPR, KC_GRV,    \
	Z_SRCD,  KC_AT,   Z_HASH,  TH_LPRN, TH_LT,       TH_DLR,  KC_MINS, KC_EXLM,  TH_EQL,  KC_TILDE,  \
	___N___, KC_BSLS, KC_PERC, KC_CIRC, ___N___,     ___N___, KC_UNDS, _______,  KC_SLSH, KC_SLSH,   \
	                           _______, _______,     _______, _______

#define _FUNC \
  QK_BOOT, KC_F7,   KC_F8,   KC_F9,   KC_F12,      KC_MSTP, KC_MPLY, KC_VOLU, Z_VSML,  TGL_BASE,   \
  KC_F10,  KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_CAPS, KC_MPRV, KC_VOLD, KC_MNXT, RGB_TOG,    \
  ___N___, KC_F1,   KC_F2,   KC_F3,   ___N___,     E_HUE,   E_SAT,   E_VAL,   E_SPD,   E_MOD,      \
	                				   _______, _______,     _______, _______

#define _NUMB \
	KC_ASTR, KC_7,    KC_8,    KC_9,    KC_EQL,      ___________________________________________,    \
	KC_SLSH, KC_4,    KC_5,    KC_6,    KC_MINS,     ______________HOME_ROW_CAGS_R______________,    \
	KC_0,    KC_1,    KC_2,    KC_3,    ___N___,     ___________________________________________,    \
	                           KC_0,    KC_DOT,      _______, _______

#define _MOUSE \
	DPI_MOD, S_D_MOD, ___N___, G(KC_BSPC), KC_DEL,   KC_WH_U, ___N___, KC_MS_U, ___N___, ___N___,    \
  ______________HOME_ROW_CAGS_L______________,     KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, ___N___,    \
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
	  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05,       \
 MSE(l06), l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
 l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15,  \
	                 l16, l17,   r16, r17


// Layout aliases for json keymap
#define LAYOUT_ferris(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_sphynx_w(...) LAYOUT_split_3x5_4(__VA_ARGS__)
#define LAYOUT_tenome_w(...) LAYOUT_split_3x5_3(__VA_ARGS__)
#define LAYOUT_charybdis_w(...) LAYOUT(__VA_ARGS__)
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
