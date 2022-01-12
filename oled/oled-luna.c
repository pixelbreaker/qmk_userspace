// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/* Graphical Luna / Felix animation, driven by key press timer or WPM.
   Frames are 4x3 on OLED and oriented for OLED_ROTATION_270.

   Modified from @HellSingCoder's Luna dog
   (https://github.com/HellSingCoder/qmk_firmware/tree/master/keyboards/sofle/keymaps/helltm)

   Includes white Felix dog frames from @ItsWaffIe
   (https://github.com/ItsWaffIe/waffle_corne/blob/proton-c/firmware/oled.c)


   Usage guide
   1 Place this file next to keymap.c or in userspace.
   2 Add the following lines into rules.mk:
        OLED_ENABLE = yes
        SRC += oled-luna.c
   3 Animation defaults to Luna, an outlined dog. Add
     'OPT_DEFS += -DFELIX' into rules.mk for "filled" version.
   4 To animate with WPM, add 'WPM_ENABLE = yes' into rules.mk.
     Otherwise add the following 'process_record_user()' code block into
     keymap.c to trigger animation tap timer with key presses:
        bool process_record_user(uint16_t keycode, keyrecord_t *record) {
            if (record->event.pressed) {
                extern uint32_t tap_timer;
                tap_timer = timer_read32();
            }
            return true;
        }
   5 The 'oled_task_user()' calls 'render_mod_status()' for secondary OLED.
     It can be replaced with your own function, or deleted.
*/

#include QMK_KEYBOARD_H

#ifndef CMK
#	define CMK 1
#endif

#define LUNA_SIZE 96
#define LUNA_FRAME_DURATION 200 // milliseconds
#define RUN_INTERVAL  LUNA_FRAME_DURATION*2
#define WALK_INTERVAL LUNA_FRAME_DURATION*8

uint32_t tap_timer = 0;

#ifndef FELIX // Outlined Luna frames
static char const sit[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
	0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x68,0x10,0x08,0x04,0x03,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x02,0x06,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
	0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0x1c,
	0x02,0x05,0x02,0x24,0x04,0x04,0x02,0xa9,0x1e,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xe0,0x90,0x08,0x18,0x60,0x10,0x08,0x04,0x03,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x02,0x0e,0x82,0x7c,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x04,0x0c,0x10,0x10,0x20,0x20,0x20,0x28,
	0x3e,0x1c,0x20,0x20,0x3e,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const walk[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x10,0x90,0x90,0x90,0xa0,0xc0,0x80,0x80,
	0x80,0x70,0x08,0x14,0x08,0x90,0x10,0x10,0x08,0xa4,0x78,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x07,0x08,0xfc,0x01,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
	0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x08,0x18,0xea,0x10,0x0f,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x03,
	0x06,0x18,0x20,0x20,0x3c,0x0c,0x12,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x20,0x20,0x20,0x40,0x80,0x00,0x00,0x00,
	0x00,0xe0,0x10,0x28,0x10,0x20,0x20,0x20,0x10,0x48,0xf0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x1f,0x20,0xf8,0x02,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x03,0x00,0x00,0x00,0x00,0x01,0x00,0x10,0x30,0xd5,0x20,0x1f,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,
	0x02,0x1c,0x14,0x08,0x10,0x20,0x2c,0x32,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const run[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0xe0,0x10,0x08,0x08,0xc8,0xb0,0x80,0x80,0x80,0x80,0x80,0x80,
	0x80,0x40,0x40,0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x02,0xc4,0xa4,0xfc,0x00,0x00,0x00,0x00,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc8,0x58,0x28,0x2a,0x10,0x0f,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x0e,0x09,0x04,0x04,0x04,0x04,0x02,0x03,0x02,0x01,0x01,
	0x02,0x02,0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
	0x80,0x80,0x78,0x28,0x08,0x10,0x20,0x30,0x08,0x10,0x20,0x40,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x10,0xb0,0x50,0x55,0x20,0x1f,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,
	0x02,0x1e,0x20,0x20,0x18,0x0c,0x14,0x1e,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const bark[][LUNA_SIZE] PROGMEM = { {
	0x00,0xc0,0x20,0x10,0xd0,0x30,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
	0x3c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x80,0xc8,0x48,0x28,0x2a,0x10,0x0f,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
	0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0xe0,0x10,0x10,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0x40,
	0x40,0x2c,0x14,0x04,0x08,0x90,0x18,0x04,0x08,0xb0,0x40,0x80,0x00,0x00,0x00,0x00,
	0x00,0x03,0x04,0x08,0x10,0x11,0xf9,0x01,0x01,0x01,0x01,0x01,0x01,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0x48,0x28,0x2a,0x10,0x0f,0x20,0x4a,0x09,0x10,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x02,0x0c,0x10,0x20,0x28,0x37,0x02,0x02,
	0x04,0x08,0x10,0x26,0x2b,0x32,0x04,0x05,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const sneak[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1e,0x21,0xf0,0x04,0x02,0x02,0x02,0x02,0x03,0x02,0x02,0x04,
	0x04,0x04,0x03,0x01,0x00,0x00,0x09,0x01,0x80,0x80,0xab,0x04,0xf8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1c,0x20,0x20,0x3c,0x0f,0x11,0x1f,0x02,0x06,
	0x18,0x20,0x20,0x38,0x08,0x10,0x18,0x04,0x04,0x02,0x02,0x01,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xe0,0xa0,0x20,0x40,0x80,0xc0,0x20,0x40,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3e,0x41,0xf0,0x04,0x02,0x02,0x02,0x03,0x02,0x02,0x02,0x04,
	0x04,0x02,0x01,0x00,0x00,0x00,0x04,0x00,0x40,0x40,0x55,0x82,0x7c,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x20,0x30,0x0c,0x02,0x05,0x09,0x12,0x1e,0x04,
	0x18,0x10,0x08,0x10,0x20,0x28,0x34,0x06,0x02,0x01,0x01,0x00,0x00,0x00,0x00,0x00
} };
#else // Filled Felix frames
static char const sit[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xfc,
	0xfe,0xfd,0xfe,0xdc,0xfc,0xfc,0xfe,0x5d,0xfe,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0x1c,0xf0,0xf8,0xfc,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xfd,0xf9,0xfd,0x7f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x37,
	0x21,0x13,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xe0,0xfe,
	0xfd,0xfe,0xfc,0xdc,0xfc,0xfe,0xfd,0x5e,0xfc,0xe0,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0xfc,0x80,0xf0,0xf8,0xfc,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xfd,0xf1,0xfd,0x7f,0x03,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x07,0x0f,0x1f,0x1f,0x3f,0x3f,0x3f,0x37,
	0x21,0x13,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const walk[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xf0,0xf0,0x70,0x30,0x20,0x00,0x80,0xc0,
	0xc0,0xf0,0xf8,0xf4,0xf8,0x70,0xf0,0xf0,0xf8,0x74,0xf8,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x07,0x0f,0xff,0xfd,0xfe,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0xe7,0xf5,0x1f,0x0f,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1f,0x3f,0x3f,0x3f,0x0f,0x11,0x1f,0x1f,
	0x03,0x07,0x1f,0x3f,0x3f,0x0f,0x13,0x1f,0x1f,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xe0,0xe0,0xe0,0xc0,0x80,0x00,0x00,0x00,
	0x00,0xe0,0xf0,0xe8,0xf0,0xe0,0xe0,0xe0,0xf0,0xe8,0xf0,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x1f,0x3f,0xfb,0xfd,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,0xfe,
	0xff,0xff,0xff,0xff,0xff,0xfe,0xff,0xef,0xcf,0xea,0x3f,0x1f,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x0f,0x03,0x07,0x0f,0x1f,0x03,
	0x1f,0x1f,0x17,0x0f,0x1f,0x3f,0x3f,0x33,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const run[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0xe0,0xf0,0xf8,0xf8,0xf8,0xb0,0xa0,0x80,0x80,0x80,0x80,0x80,
	0x80,0xc0,0xc0,0xfc,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,
	0x00,0x00,0x00,0x00,0x01,0x63,0xe7,0xe7,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xf7,0x67,0x37,0x35,0x1f,0x0f,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x0e,0x0f,0x07,0x07,0x07,0x07,0x03,0x03,0x03,0x01,0x01,
	0x03,0x03,0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0xe0,0xf0,0xf0,0xf8,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,
	0x80,0x80,0xf8,0xe8,0xc8,0xf0,0xe0,0xf0,0xc8,0xf0,0xe0,0xc0,0x80,0x00,0x00,0x00,
	0x00,0x00,0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xfe,0xef,0xcf,0x6f,0x6a,0x3f,0x1f,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,
	0x03,0x1f,0x3f,0x3f,0x1f,0x0f,0x1f,0x3f,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const bark[][LUNA_SIZE] PROGMEM = { {
	0x00,0xc0,0xe0,0xf0,0xf0,0x30,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xc0,
	0xfc,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xf7,0xf7,0x37,0x35,0x1f,0x0f,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,0x03,0x03,
	0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
}, {
	0x00,0xe0,0xf0,0xf0,0xf0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x80,0xc0,
	0xc0,0xec,0xf4,0xe4,0xf8,0x70,0xf8,0xe4,0xf8,0x70,0xc0,0x80,0x00,0x00,0x00,0x00,
	0x00,0x03,0x07,0x0f,0x1f,0x1f,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,0xff,
	0xff,0xff,0xff,0xff,0xff,0xff,0xff,0x77,0x37,0x35,0x1f,0x0f,0x20,0x4a,0x09,0x10,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x03,0x0f,0x1f,0x3f,0x3f,0x37,0x03,0x03,
	0x07,0x0f,0x1f,0x3f,0x3b,0x33,0x07,0x07,0x06,0x00,0x00,0x00,0x00,0x00,0x00,0x00
} };
static char const sneak[][LUNA_SIZE] PROGMEM = { {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,
	0x00,0x00,0xc0,0x40,0x40,0x80,0x00,0x80,0x40,0x80,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x1e,0x3f,0xff,0xfb,0xfd,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,
	0xfc,0xfe,0xff,0xff,0xfe,0xff,0xf7,0xff,0x7e,0x7f,0x57,0xfc,0xf8,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x1f,0x3f,0x3f,0x3f,0x0f,0x1f,0x1f,0x03,0x07,
	0x1f,0x3f,0x3f,0x3f,0x0f,0x1f,0x1f,0x07,0x07,0x03,0x03,0x01,0x00,0x00,0x00,0x00
}, {
	0x00,0x00,0x00,0x00,0x00,0x00,0x80,0xc0,0xc0,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0xe0,0xa0,0x20,0xc0,0x80,0xc0,0x20,0xc0,0x80,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x3e,0x7f,0xf7,0xfb,0xfd,0xfd,0xfd,0xfd,0xfc,0xfc,0xfc,0xfc,
	0xfc,0xfe,0xff,0xff,0xff,0xff,0xfb,0xff,0xbf,0xbf,0xab,0xfe,0x7c,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x3f,0x3f,0x3f,0x0f,0x03,0x07,0x0f,0x1f,0x1f,0x07,
	0x1f,0x1f,0x0f,0x1f,0x3f,0x3f,0x37,0x07,0x03,0x01,0x01,0x00,0x00,0x00,0x00,0x00
} };
#endif


static void render_logo(void) {
	static char const corne_logo[] PROGMEM = {
		0x80, 0x81, 0x82, 0x83, 0x84,
		0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
	static char const katakana[] PROGMEM = {
		0x20, 0xd1, 0xd2, 0xd3, 0x20, 0};

	oled_write_P(corne_logo, false);
	oled_write_P(layer_state_is(CMK) ? PSTR("corne") : katakana, false);
}


static void luna_action(char const action[][LUNA_SIZE]) {
	static uint8_t current_frame = 0;
	current_frame = (current_frame + 1) & 1;
	oled_write_raw_P(action[current_frame], LUNA_SIZE);
}


static void render_luna_status(void) {
	// Animation timer
	static uint16_t anim_timer = 0;

#ifdef WPM_ENABLE
	static uint8_t prev_wpm = 0;
	// Update tap_timer with sustained WPM
	if (get_current_wpm() > prev_wpm || get_mods()) {
		tap_timer = timer_read32();
	}
	prev_wpm = get_current_wpm();
#endif

	void animate_luna(void) {
		render_logo();
		oled_set_cursor(0,8);
		if (get_mods() & MOD_MASK_SHIFT || host_keyboard_led_state().caps_lock) {
			luna_action(bark);
		} else if (get_mods() & MOD_MASK_CAG) {
			luna_action(sneak);
		} else if (timer_elapsed32(tap_timer) < RUN_INTERVAL) {
			luna_action(run);
		} else if (timer_elapsed32(tap_timer) < WALK_INTERVAL) {
			luna_action(walk);
		} else {
			luna_action(sit);
		}
	}

	if (timer_elapsed32(tap_timer) > OLED_TIMEOUT) {
		oled_off();
	} else if (timer_elapsed(anim_timer) > LUNA_FRAME_DURATION) {
		anim_timer = timer_read();
		animate_luna();
	}
}


// Init and rendering calls
oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
	return OLED_ROTATION_270;
}

bool oled_task_user(void) {
	extern void render_mod_status(void);
	is_keyboard_master() ? render_luna_status() : render_mod_status();
	return false;
}
