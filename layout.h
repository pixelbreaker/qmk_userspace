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
#define Z_SLEEP G(A(KC_PWR))
#define Z_SHUT C(A(G(KC_PWR)))
#define Z_SSHT G(S(KC_4))
#define Z_SRCD G(S(KC_5))
#define Z_VSML A(KC_LSFT)
#define Z_HASH A(KC_3)
#define Z_EUR A(KC_2)
#define Z_GBP S(KC_3)

#define Z_AT S(KC_2)

#define DELLINE MEH(KC_DEL)

#define ___N___ KC_NO

#define SNIPE P_SNIPE

// Tap hold macros
#define TH_QU LT(0, KC_Q)

#define TH_SCR LT(0, KC_PWR)
#define TH_DEL LT(0, KC_DEL)
#define TH_DLR LT(0, KC_DLR)
#define TH_EQL LT(0, KC_EQL)

// clang-format off
// #define __________HOME_ROW_CAGS_L_________ KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT
// #define __________HOME_ROW_CAGS_R_________ KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL

#define _________HOME_ROW_OS_CAGS_________ OS_LCTL, OS_LALT, OS_LGUI, OS_LSFT
#define _________HOME_ROW_OS_SGAC_________ OS_RSFT, OS_RGUI, OS_LALT, OS_RCTL
#define ___________________________________________ ___N___, ___N___, ___N___, ___N___, ___N___
#define __________________APP_NAV__________________ C(S(KC_TAB)), C(KC_TAB), C(G(S(KC_TAB))), C(G(KC_TAB)), ___N___
#define _________________FILE_NAV__________________ ___N___, Z_UND,   Z_SAVE,  Z_RDO,   ___N___
// clang-format on

// Layers
enum layers { BSE, SYM, NAV, NUM, FNC, MOU, EXT };

// #define ESC_MED LT(MED, KC_ESC)
#define THM_0 LT(FNC, KC_ESC)

#define THM_1 LT(NAV, KC_SPC)
#define THM_2 LT(SYM, KC_ENT)

#define THM_3 LT(NUM, KC_BSPC)
#define THM_4 KC_E

// clang-format off

// Aptmak
#define _BASE \
	KC_V,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,    \
	KC_R,    KC_S,    KC_T,    KC_H,    KC_K,        KC_X,    KC_N,    KC_A,    KC_I,    KC_O,       \
	KC_Z,    KC_C,    KC_G,    KC_D,    KC_Q,        KC_GRV,  KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    \
	                           THM_1,   THM_2,       THM_3,   THM_4

#define _NAV \
  KC_TAB,  A(KC_TAB),APP_L,  APP_R,   TH_SCR,      KC_PGUP, QK_REP,  KC_UP,   AS_TOGG, KC_GRV,     \
	_________HOME_ROW_OS_CAGS_________, TH_DEL,      KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_SCLN,    \
	_________________FILE_NAV__________________,     ___N___, OS_HYPR, DELLINE, OS_MEH,  ___N___,    \
	                           _______, _______,     KC_BSPC, KC_DEL

#define _SYMB \
	KC_ESC,  KC_AT,   KC_LCBR, KC_RCBR, KC_PLUS,     KC_PERC, KC_AMPR, KC_EXLM, KC_PIPE, KC_SCLN,   \
	KC_TILDE,KC_CIRC, KC_LPRN, KC_RPRN, KC_MINS,     TH_DLR,  _________HOME_ROW_OS_SGAC_________,   \
	___N___, KC_UNDS, KC_LBRC, KC_RBRC, ___N___,     ___N___, KC_BSLS, TH_EQL,  KC_SLSH, ___N___,   \
	                           _______, _______,     KC_UNDS, KC_MINS

#define _NUMB \
	KC_ASTR, KC_7,    KC_8,    KC_9,    KC_PLUS,     KC_PERC, Z_GBP,   Z_HASH,   TH_DLR, Z_EUR,     \
	KC_SLSH, KC_4,    KC_5,    KC_6,    KC_MINS,     KC_EQL,  _________HOME_ROW_OS_SGAC_________,   \
	KC_0,    KC_1,    KC_2,    KC_3,    ___N___,     _______, KC_D,    KC_E,     KC_F,   _______,   \
	                           KC_0,    KC_DOT,      _______, _______

#define _FUNC \
  Z_SLEEP, KC_F7,   KC_F8,   KC_F9,   KC_F12,      CW_TOGG, KC_MPLY, KC_VOLU, _______, QK_BOOT,   \
  KC_F10,  KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_CAPS, KC_MPRV, KC_VOLD, KC_MNXT, RM_TOGG,   \
  ___N___, KC_F1,   KC_F2,   KC_F3,   ___N___,     E_HUE,   E_SAT,   E_VAL,   E_SPD,   E_MOD,     \
	                				   _______, _______,     _______, _______

// For keyboards that don't have a physical pointing device
#define _MOUSE \
	_______, _______, _______, _______, _______,     MS_WHLU, MS_WHLL,  MS_UP,   MS_WHLR,_______,   \
  _________HOME_ROW_OS_CAGS_________, _______,     MS_WHLD, MS_LEFT,  MS_DOWN, MS_RGHT,_______,   \
  _______, _______, _______, MS_BTN3, _______,     _______, _______, _______, _______, _______,   \
                      		   MS_BTN1, MS_BTN2,     _______, _______

// For keyboards that have a pointing device (and possibly auto mouse layer)
#define _MOUSE_DEVICE \
	_______, _______, _______, _______, _______,     _______, _______, _______, _______, _______,   \
  _________HOME_ROW_OS_CAGS_________, _______,     _______, _______, _______, _______, _______,   \
  MO(MOU), SNIPE,   _______, MS_BTN3, _______,     _______, _______, _______, _______, _______,   \
                     		     MS_BTN1, MS_BTN2,     _______, _______

#define _EXTRA \
	___________________________________________,     ___________________________________________,   \
  ___N___, KC_Z,    ___N___, KC_V,    ___N___,     ___N___, KC_K,    ___N___, KC_SLSH, KC_BSLS,   \
  ___________________________________________,     ___________________________________________,   \
                      		   _______, _______,     _______, _______

// Layout aliases for json keymap
#define LAYOUT_w(...) LAYOUT(__VA_ARGS__)

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
