// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include "quantum/keycodes.h"
#include "pixelbreaker.h"

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
#define Z_LOCK C(G(KC_Q))
#define Z_SLEEP S(C(KC_EJCT))
#define Z_SHUT C(A(G(KC_PWR)))
#define Z_SSHT G(S(KC_4))
#define Z_SRCD G(S(KC_5))
#define Z_VSML A(KC_LSFT)
#define Z_HASH A(KC_3)
#define Z_AT S(KC_2)

#define MS_Z MSE(KC_Z)
#define MS_SLSH MSE(KC_SLSH)

#define OSM_HYPR OSM(MOD_HYPR)
#define OSM_MEH OSM(MOD_MEH)

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

// clang-format off
#define ___________________________________________ ___N___, ___N___, ___N___, ___N___, ___N___
#define ______________HOME_ROW_CAGS_L______________ KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, ___N___
#define ______________HOME_ROW_CAGS_R______________ ___N___, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL
#define __________________APP_NAV__________________ C(S(KC_TAB)), C(KC_TAB), C(G(S(KC_TAB))), C(G(KC_TAB)), ___N___
#define ________________COPY_PASTA_________________ Z_UND,   Z_CUT,   Z_CPY,   Z_PST,   Z_RDO
// clang-format on

// Layers
enum layers { BSE, CMK, NAV, NUM, SYM, FNC, MOU };

// #define ESC_MED LT(MED, KC_ESC)
#define SPC_NAV LT(NAV, KC_SPC)
#define TAB_SYM LT(SYM, KC_TAB)
#define ENT_FUN LT(FNC, KC_ENT)
#define BSP_NUM LT(NUM, KC_BSPC)

// Default 3x5_2 split layout
// clang-format off
#define _BASE \
	KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,       \
	KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT,    \
	KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    \
	                           SPC_NAV, TAB_SYM,     ENT_FUN, BSP_NUM

// Colemak standard
/* #define _COLE \
	KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,    \
	KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,       \
	KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    \
	                           _______, _______,     _______, _______*/

// Colemak DH
#define _COLE \
	KC_Q,    KC_W,    KC_F,    KC_P,    KC_B,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,    \
	KC_A,    KC_R,    KC_S,    KC_T,    KC_G,        KC_M,    KC_N,    KC_E,    KC_I,    KC_O,       \
	KC_Z,    KC_X,    KC_C,    KC_D,    KC_V,        KC_K,    KC_H,    KC_COMM, KC_DOT,  KC_SLSH,    \
	                           _______, _______,     _______, _______

#define _NAV \
	__________________APP_NAV__________________,     ___N___, DELLINE, KC_UP,    ___N___, KC_COLN,   \
	______________HOME_ROW_CAGS_L______________,     KC_EQL,  KC_LEFT, KC_DOWN,  KC_RGHT, KC_SCLN,   \
	________________COPY_PASTA_________________,     KC_MINS, KC_HOME, KC_PGDN,  KC_PGUP, KC_END,    \
	                           _______, _______,     KC_ENT,  KC_BSPC

#define _NUMB \
	KC_ASTR, KC_7,    KC_8,    KC_9,    KC_PLUS,     __________________APP_NAV__________________,    \
	KC_SLSH, KC_4,    KC_5,    KC_6,    KC_MINS,     ______________HOME_ROW_CAGS_R______________,    \
	KC_0,    KC_1,    KC_2,    KC_3,    KC_EQL,      ________________COPY_PASTA_________________,    \
	                           KC_SPC,  KC_DOT,      _______, _______

/*
#define _SYMB \
	_______, _______, KC_LBRC, KC_RBRC, _______,     KC_PIPE, KC_PLUS, KC_ASTR,  KC_DLR,  KC_TILDE,  \
	_______, KC_LT,   KC_LPRN, KC_RPRN, KC_GT,       KC_EXLM, KC_MINS, KC_SLSH,  KC_EQL,  KC_GRV,    \
	_______, _______, KC_LCBR, KC_RCBR, _______,     KC_AMPR, KC_UNDS, Z_HASH,   Z_AT,    KC_BSLS,   \
	                           _______, _______,     _______, KC_DEL
*/

#define _SYMB \
	___N___, ___N___, KC_LBRC, KC_RBRC, ___N___,     KC_ASTR, KC_PLUS, KC_PIPE,  KC_DLR,  KC_TILDE,  \
	___N___, KC_LT,   KC_LPRN, KC_RPRN, KC_GT,       Z_HASH,  KC_MINS, KC_EXLM,  KC_EQL,  KC_GRV,    \
	___N___, ___N___, KC_LCBR, KC_RCBR, ___N___,     Z_AT,    KC_UNDS, KC_AMPR,  KC_PERC, KC_BSLS,   \
	                           _______, _______,     KC_SPC,  KC_DEL

#define _FUNC \
    QK_BOOT, KC_F7,   KC_F8,   KC_F9,   KC_F12,      Z_SSHT,  Z_SRCD,  KC_VOLU, Z_VSML,  TGL_BASE,   \
    ___N___, KC_F4,   KC_F5,   KC_F6,   KC_F11,      KC_CAPS, KC_MPRV, KC_VOLD, KC_MNXT, RGB_TOG,    \
    Z_SLEEP, KC_F1,   KC_2,    KC_F3,   KC_F10,      E_HUE,   E_SAT,   E_VAL,   E_SPD,   E_MOD,      \
							   _______, _______,     KC_MSTP, KC_MPLY

#define _MOUSE \
	DPI_MOD, S_D_MOD, ___N___, ___N___, ___N___,     ___N___, ___N___, KC_MS_U, ___N___, ___N___,    \
    ______________HOME_ROW_CAGS_L______________,     KC_WH_U, KC_MS_L, KC_MS_D, KC_MS_R, ___N___,    \
    ___N___, SNIPE,   ___N___, KC_BTN3, ___N___,     KC_WH_D, ___N___, ___N___, ___N___, ___N___,    \
                      		   KC_BTN1, KC_BTN2,     _______, _______

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


// Layout aliases for json keymap
#define LAYOUT_sphynx_w(...) LAYOUT_split_3x5_4(__VA_ARGS__)
#define LAYOUT_tenome_w(...) LAYOUT_split_3x5_3(__VA_ARGS__)
#define LAYOUT_charybdis_w(...) LAYOUT(__VA_ARGS__)


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
		       KC_ESC, l16, l17,   r16, r17

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
		KC_NO, ESC_MED,l16, l17,   r16, r17, KC_DEL, ENC_DOWN

// Convert 3x5_2 to Tenome 3x5_3 (with encoders optional)
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

// clang-format on
