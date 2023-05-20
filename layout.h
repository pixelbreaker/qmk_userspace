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
#define HM_S LALT_T(KC_S)
#define HM_D LGUI_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J RSFT_T(KC_J)
#define HM_K RGUI_T(KC_K)
#define HM_L LALT_T(KC_L)
#define HM_QUOT RCTL_T(KC_QUOT)
#define HRML(k1, k2, k3, k4) LCTL_T(k1), LALT_T(k2), LGUI_T(k3), LSFT_T(k4)
#define HRMR(k1, k2, k3, k4) RSFT_T(k1), RGUI_T(k2), LALT_T(k3), RCTL_T(k4)

#define ___________________________________________ _______, _______, _______, _______, _______
#define ______________HOME_ROW_CAGS_L______________ KC_LCTL, KC_LALT, KC_LGUI, KC_LSFT, _______
#define ______________HOME_ROW_CAGS_R______________ _______, KC_LSFT, KC_LGUI, KC_LALT, KC_LCTL
#define ________________COPY_PASTA_________________ Z_UND, Z_CUT, Z_CPY, Z_PST, Z_RDO

// Layers
enum layers { BSE, CMK, NAV, NUM, SYM, FNC, MED, MOU };

#define ESC_MED LT(MED, KC_ESC)
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

#define _COLE \
	KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,    KC_Y,    KC_QUOT,    \
	KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,       \
	KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH,    \
	                           _______, _______,     _______, _______

#define _NAV \
	___________________________________________,     _______, DELLINE, KC_UP,    _______, KC_COLN,   \
	______________HOME_ROW_CAGS_L______________,     KC_EQL,  KC_LEFT, KC_DOWN,  KC_RGHT, KC_SCLN,   \
	________________COPY_PASTA_________________,     KC_MINS, KC_HOME, KC_PGDN,  KC_PGUP, KC_END,    \
	                           _______, _______,     KC_ENT,  KC_BSPC

#define _NUMB \
	KC_ASTR, KC_7,    KC_8,    KC_9,    KC_PLUS,     ___________________________________________,    \
	KC_SLSH, KC_4,    KC_5,    KC_6,    KC_MINS,     ______________HOME_ROW_CAGS_R______________,    \
	KC_0,    KC_1,    KC_2,    KC_3,    KC_EQL,      ________________COPY_PASTA_________________,    \
	                           KC_SPC,  KC_DOT,      _______, _______

#define _SYMB \
	KC_GT,   KC_RBRC, KC_RCBR, KC_RPRN, _______,     KC_EXLM, KC_PLUS, KC_ASTR,  KC_DLR,  KC_TILDE,  \
	KC_LT,   KC_LBRC, KC_LCBR, KC_LPRN, _______,     KC_PIPE, KC_MINS, KC_SLSH,  KC_EQL,  KC_GRV,    \
	KC_GT,   KC_RBRC, KC_RCBR, KC_RPRN, _______,     KC_AMPR, KC_UNDS, Z_HASH,   Z_AT,    KC_BSLS,   \
	                           _______, _______,     _______, KC_DEL

#define _FUNC \
	_______, KC_F7,   KC_F8,   KC_F9,   KC_F12,      _______, _______, _______, _______, TG(CMK),    \
	_______, KC_F4,   KC_F5,   KC_F6,   KC_F11,      ______________HOME_ROW_CAGS_R______________,    \
	_______, KC_F1,   KC_F2,   KC_F3,   KC_F10,      ________________COPY_PASTA_________________,    \
	                           _______, _______,     _______, _______

#define _MEDIA \
    QK_BOOT, _______, Z_SSHT,  Z_SRCD,  _______,     _______, _______, KC_VOLU, _______, _______,    \
    _______, _______, _______, Z_VSML,  _______,     KC_CAPS, KC_MPRV, KC_VOLD, KC_MNXT, _______,    \
    Z_SLEEP, _______, _______, _______, _______,     _______, _______, _______, _______, _______,    \
							   _______, _______,     KC_MSTP, KC_MPLY

#define _MOUSE \
	DPI_MOD, S_D_MOD, _______, _______, _______,     _______, _______, KC_MS_U, _______, _______,    \
    ______________HOME_ROW_CAGS_L______________,     KC_WH_U, KC_MS_L, KC_MS_D, KC_MS_R, _______,    \
    _______, SNIPE,   _______, KC_BTN3, _______,     KC_WH_D, _______, _______, _______, _______,    \
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
		       ESC_MED, l16, l17,   r16, r17

// Convert 3x5_2 to Sphynx dacman 3x5_3 (with encoders optional)
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

// clang-format on
