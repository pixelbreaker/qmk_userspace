// Copyright @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once

// Common QMK settings
#define TAP_CODE_DELAY 5
#define TAP_HOLD_CAPS_DELAY 35
#define USB_SUSPEND_WAKEUP_DELAY 500
#define RP2040_BOOTLOADER_DOUBLE_TAP_RESET_TIMEOUT 1000U

// Space saving options
// #define LAYER_STATE_8BIT

//  Tap-hold settings
#define TAPPING_TERM 180
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 100
#define QUICK_TAP_TERM_PER_KEY
#define PERMISSIVE_HOLD
#define PERMISSIVE_HOLD_PER_KEY
// #define IGNORE_MOD_TAP_INTERRUPT
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define LEADER_TIMEOUT 1000

// #define ACHORDION_ENABLE

#ifdef POINTING_DEVICE_ENABLE
#  define MEDIA_TAP_THRESHOLD 85
#  define MEDIA_TAP_TERM 600
#  define CARRET_TIMEOUT_MS 850
// #    define POINTING_DEVICE_AUTO_MOUSE_ENABLE
#  ifdef POINTING_DEVICE_AUTO_MOUSE_ENABLE
#    define AUTO_MOUSE_DEFAULT_LAYER 3
#    define AUTO_MOUSE_TIME 700
#    define AUTO_MOUSE_DEBOUNCE 10
#    define AUTO_MOUSE_DELAY 500
#  endif
// #  ifndef CARRET_TIMEOUT_MS
// #    define CARRET_TIMEOUT_MS 350
// #  endif
#  ifndef MEDIA_TIMEOUT_MS
#    define MEDIA_TIMEOUT_MS 150
#  endif
#endif

#ifdef KEYBOARD_charybdis
#  define CHARYBDIS_DRAGSCROLL_REVERSE_Y
#  define CHARYBDIS_DRAGSCROLL_BUFFER_SIZE 8

#  ifdef CHARYBDIS_DRAGSCROLL_DPI
#    undef CHARYBDIS_DRAGSCROLL_DPI
#  endif
#  define CHARYBDIS_DRAGSCROLL_DPI 40

/* Trackball angle adjustment. */
#  ifdef ROTATIONAL_TRANSFORM_ANGLE
#    undef ROTATIONAL_TRANSFORM_ANGLE
#  endif

#  define ROTATIONAL_TRANSFORM_ANGLE -28
#endif

#ifdef KEYBOARD_tenome
#  define TENOME_ENCODER_LEDS

#  ifdef TRACKBALL_ENABLE
#    ifdef ROTATIONAL_TRANSFORM_ANGLE
#      undef ROTATIONAL_TRANSFORM_ANGLE
#    endif

#    define ROTATIONAL_TRANSFORM_ANGLE -28
#  else
// #    define POINTING_DEVICE_GESTURES_SCROLL_ENABLE // Circular scroll.
#    define CIRQUE_PINNACLE_TAP_ENABLE
// absolute mode
// #  undef CIRQUE_PINNACLE_POSITION_MODE
// #  define CIRQUE_PINNACLE_POSITION_MODE CIRQUE_PINNACLE_RELATIVE_MODE
// #  ifdef CIRQUE_PINNACLE_ABSOLUTE_MODE
#    define POINTING_DEVICE_GESTURES_CURSOR_GLIDE_ENABLE
#    define CIRQUE_PINNACLE_TAPPING_TERM 150
#    define CIRQUE_PINNACLE_TOUCH_DEBOUNCE 200
#    define CIRQUE_PINNACLE_ATTENUATION EXTREG__TRACK_ADCCONFIG__ADC_ATTENUATE_2X
#  endif
#endif

#ifdef COMBO_ENABLE
#  ifndef COMBO_TERM
#    define COMBO_TERM 80
#  endif
#  define COMBO_MUST_HOLD_PER_COMBO
#  define COMBO_SHOULD_TRIGGER
#  define COMBO_ONLY_FROM_LAYER 0
#endif

#ifdef MOUSEKEY_ENABLE
#  define MOUSEKEY_DELAY 0      // Delay between pressing a key and cursor movement
#  define MOUSEKEY_INTERVAL 15  // Time between cursor movements in milliseconds
#  define MOUSEKEY_MOVE_DELTA 8 // Step size for acceleration
#  define MOUSEKEY_MAX_SPEED 3
#  define MOUSEKEY_TIME_TO_MAX 80
#  define MOUSEKEY_WHEEL_DELAY 16
#  define MOUSEKEY_WHEEL_INTERVAL 30
#  define MOUSEKEY_WHEEL_MAX_SPEED 3
#  define MOUSEKEY_WHEEL_TIME_TO_MAX 95
#endif

// Layout macros
#ifndef __ASSEMBLER__
#  include "layout.h"
#endif
