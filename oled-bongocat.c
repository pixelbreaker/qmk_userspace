/* Copyright (C) 2021 @filterpaper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Graphical bongocat animation, driven by key press timer or WPM.
   It has left and right aligned cats optimized for both OLEDs.
   This code uses run-length encoded frames that saves space by
   storing frames of consecutively repeated bytes encoded in
   "byte,count" format.

   Inspired by @j-inc's bongocat animation code
   (keyboards/kyria/keymaps/j-inc)

   Cat images courtesy of @plandevida

   RLE code is modified from @vectorstorm's Bongocat:
   (https://github.com/vectorstorm/qmk_firmware/tree/new_vectorstorm_bongo/keyboards/crkbd/keymaps/vectorstorm)


   Usage guide
   1 Place this file next to keymap.c or in userspace.
   2 Add the following lines into rules.mk:
        OLED_ENABLE = yes
        SRC += oled-bongocat.c
   3 Left and right aligned Bongocat is default. To save space:
      * Add 'OPT_DEFS += -DLEFTCAT' into rules.mk
      * Or 'OPT_DEFS += -DRIGHTCAT' into rules.mk
   4 To animate with WPM, add 'WPM_ENABLE = yes' into rules.mk.
     Otherwise add the following integer variable and 'if'
     statement inside 'process_record_user()' in keymap.c:
        uint32_t tap_timer = 0;
        bool process_record_user(uint16_t keycode, keyrecord_t *record) {
            if (record->event.pressed) { tap_timer = timer_read32(); }
        }
   5 The 'oled_task_user()' calls 'render_mod_status()' for secondary OLED.
     It can be replaced with your own function, or delete the 'else' line.
 */

#include QMK_KEYBOARD_H

#define IDLE_FRAMES 5
#define TAP_FRAMES  2
#define FRAME_DURATION 200 // Milliseconds per frame


// Run-length encoded animation frames
#ifndef LEFTCAT // Right aligned Bongocat
static unsigned char const idle0[] PROGMEM = {175,
	0x00,0x33,0x80,0x02,0x00,0x05,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x04,0x08,0x01,
	0x04,0x01,0x02,0x01,0x01,0x02,0x02,0x01,0x0c,0x01,0x30,0x01,0x40,0x01,0x80,0x01,
	0x00,0x2e,0x80,0x03,0x00,0x31,0x1e,0x01,0xe1,0x01,0x00,0x02,0x01,0x02,0x02,0x02,
	0x01,0x01,0x00,0x04,0x80,0x02,0x00,0x09,0x80,0x01,0x00,0x01,0x30,0x02,0x00,0x02,
	0x01,0x02,0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x01,0x20,0x01,0x40,0x01,0x80,0x01,
	0x00,0x03,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x04,0x08,0x05,0x04,0x05,0x02,0x04,
	0x01,0x04,0x00,0x33,0x80,0x01,0x70,0x01,0x0c,0x01,0x03,0x01,0x00,0x0a,0x01,0x02,
	0x00,0x01,0x40,0x01,0xa0,0x01,0x21,0x01,0x22,0x01,0x12,0x01,0x11,0x03,0x09,0x01,
	0x08,0x04,0x04,0x02,0x08,0x02,0x10,0x05,0x11,0x01,0x0f,0x01,0x01,0x02,0x00,0x36,
	0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x05,0x02,0x01,0x03,0x01,
	0x02,0x02,0x01,0x06,0x02,0x02,0x04,0x02,0x08,0x05,0x07,0x01,0x00,0x3d};
static unsigned char const idle1[] PROGMEM = {161,
	0x00,0x3b,0x80,0x02,0x40,0x04,0x20,0x04,0x10,0x01,0x08,0x01,0x04,0x01,0x02,0x02,
	0x04,0x01,0x18,0x01,0x60,0x01,0x80,0x01,0x00,0x2f,0x80,0x03,0x00,0x31,0x3c,0x01,
	0xc2,0x01,0x01,0x02,0x02,0x02,0x04,0x02,0x02,0x01,0x01,0x01,0x00,0x10,0x60,0x02,
	0x00,0x01,0x01,0x01,0x02,0x02,0x04,0x01,0x08,0x01,0x10,0x01,0x20,0x01,0x40,0x01,
	0x80,0x01,0x00,0x04,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x04,0x08,0x05,0x04,0x05,
	0x02,0x04,0x01,0x04,0x00,0x33,0x80,0x01,0x70,0x01,0x19,0x01,0x06,0x01,0x00,0x0a,
	0x03,0x02,0x00,0x01,0x40,0x01,0xa0,0x01,0x22,0x01,0x24,0x01,0x14,0x01,0x12,0x03,
	0x0b,0x01,0x08,0x04,0x04,0x02,0x08,0x02,0x10,0x05,0x11,0x01,0x0f,0x01,0x01,0x02,
	0x00,0x36,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x05,0x02,0x01,
	0x03,0x01,0x02,0x02,0x01,0x06,0x02,0x02,0x04,0x02,0x08,0x05,0x07,0x01,0x00,0x3d};
static unsigned char const idle2[] PROGMEM = {163,
	0x00,0x3a,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x05,0x08,0x02,0x04,0x03,0x08,0x01,
	0x30,0x01,0x40,0x01,0x80,0x01,0x00,0x2e,0x80,0x03,0x00,0x31,0x18,0x01,0x64,0x01,
	0x82,0x01,0x02,0x05,0x01,0x01,0x00,0x04,0x80,0x02,0x00,0x09,0x80,0x01,0x00,0x01,
	0x30,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x01,0x20,0x01,
	0x40,0x01,0x80,0x01,0x00,0x03,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x04,0x08,0x05,
	0x04,0x05,0x02,0x04,0x01,0x04,0x00,0x33,0xc0,0x01,0x38,0x01,0x04,0x01,0x03,0x01,
	0x00,0x0a,0x01,0x02,0x00,0x01,0x40,0x01,0xa0,0x01,0x21,0x01,0x22,0x01,0x12,0x01,
	0x11,0x03,0x09,0x01,0x08,0x04,0x04,0x02,0x08,0x02,0x10,0x05,0x11,0x01,0x0f,0x01,
	0x01,0x02,0x00,0x36,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x05,
	0x02,0x01,0x03,0x01,0x02,0x02,0x01,0x06,0x02,0x02,0x04,0x02,0x08,0x05,0x07,0x01,
	0x00,0x3d};
static unsigned char const idle3[] PROGMEM = {165,
	0x00,0x3a,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x04,0x08,0x02,0x04,0x01,0x02,0x03,
	0x04,0x01,0x38,0x01,0x40,0x01,0x80,0x01,0x00,0x2e,0x80,0x03,0x00,0x31,0x1c,0x01,
	0xe2,0x01,0x01,0x02,0x02,0x04,0x01,0x01,0x00,0x04,0x80,0x02,0x00,0x09,0x80,0x01,
	0x00,0x01,0x30,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x01,
	0x20,0x01,0x40,0x01,0x80,0x01,0x00,0x03,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x04,
	0x08,0x05,0x04,0x05,0x02,0x04,0x01,0x04,0x00,0x33,0x80,0x01,0x70,0x01,0x0c,0x01,
	0x03,0x01,0x00,0x0a,0x01,0x02,0x00,0x01,0x40,0x01,0xa0,0x01,0x21,0x01,0x22,0x01,
	0x12,0x01,0x11,0x03,0x09,0x01,0x08,0x04,0x04,0x02,0x08,0x02,0x10,0x05,0x11,0x01,
	0x0f,0x01,0x01,0x02,0x00,0x36,0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,
	0x04,0x05,0x02,0x01,0x03,0x01,0x02,0x02,0x01,0x06,0x02,0x02,0x04,0x02,0x08,0x05,
	0x07,0x01,0x00,0x3d};
static unsigned char const *idle_anim[IDLE_FRAMES] = {
	idle0, idle0, idle1, idle2, idle3 };

static unsigned char const prep[] PROGMEM = {181,
	0x00,0x33,0x80,0x02,0x00,0x05,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x04,0x08,0x01,
	0x04,0x01,0x02,0x01,0x01,0x02,0x02,0x01,0x0c,0x01,0x30,0x01,0x40,0x01,0x80,0x01,
	0x00,0x2e,0x80,0x03,0x00,0x31,0x1e,0x01,0xe1,0x01,0x00,0x02,0x01,0x02,0x02,0x02,
	0x81,0x01,0x80,0x02,0x00,0x02,0x80,0x02,0x00,0x09,0x80,0x01,0x00,0x01,0x30,0x02,
	0x00,0x02,0x01,0x01,0xe1,0x01,0x1a,0x01,0x06,0x01,0x09,0x01,0x31,0x01,0x35,0x01,
	0x01,0x01,0x8a,0x01,0x7c,0x01,0x00,0x02,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x04,
	0x08,0x05,0x04,0x05,0x02,0x04,0x01,0x04,0x00,0x33,0x80,0x01,0x70,0x01,0x0c,0x01,
	0x03,0x01,0x00,0x02,0x18,0x01,0x06,0x01,0x05,0x01,0x98,0x01,0x99,0x01,0x84,0x01,
	0xc3,0x01,0x7c,0x01,0x41,0x02,0x40,0x02,0x20,0x01,0x21,0x01,0x22,0x01,0x12,0x01,
	0x11,0x03,0x09,0x01,0x08,0x04,0x04,0x06,0x02,0x03,0x01,0x04,0x00,0x36,0x80,0x05,
	0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x05,0x02,0x01,0x03,0x01,0x02,0x02,
	0x01,0x06,0x00,0x47};

static unsigned char const tap0[] PROGMEM = {221,
	0x00,0x33,0x80,0x02,0x00,0x05,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x04,0x08,0x01,
	0x04,0x01,0x02,0x01,0x01,0x02,0x02,0x01,0x0c,0x01,0x30,0x01,0x40,0x01,0x80,0x01,
	0x00,0x07,0xf8,0x04,0x00,0x05,0x80,0x02,0x00,0x1c,0x80,0x03,0x00,0x31,0x1e,0x01,
	0xe1,0x01,0x00,0x02,0x01,0x02,0x02,0x02,0x81,0x01,0x80,0x02,0x00,0x02,0x80,0x02,
	0x00,0x09,0x80,0x01,0x00,0x01,0x30,0x02,0x00,0x02,0x01,0x02,0x02,0x01,0x04,0x01,
	0x08,0x01,0x10,0x01,0x20,0x01,0x43,0x01,0x87,0x01,0x07,0x01,0x01,0x01,0x00,0x01,
	0xb8,0x01,0xbc,0x01,0xbe,0x01,0x9f,0x01,0x5f,0x02,0x4f,0x01,0x4c,0x01,0x20,0x04,
	0x10,0x04,0x08,0x05,0x04,0x05,0x02,0x04,0x01,0x04,0x00,0x33,0x80,0x01,0x70,0x01,
	0x0c,0x01,0x03,0x01,0x00,0x02,0x18,0x01,0x06,0x01,0x05,0x01,0x98,0x01,0x99,0x01,
	0x84,0x01,0x43,0x01,0x7c,0x01,0x41,0x02,0x40,0x02,0x20,0x01,0x21,0x01,0x22,0x01,
	0x12,0x01,0x11,0x03,0x09,0x01,0x08,0x04,0x04,0x02,0x08,0x02,0x10,0x05,0x11,0x01,
	0x0f,0x01,0x01,0x01,0x3d,0x01,0x7c,0x01,0xfc,0x05,0x3c,0x01,0x0c,0x01,0x00,0x2e,
	0x80,0x05,0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x05,0x02,0x01,0x03,0x01,
	0x02,0x02,0x01,0x06,0x00,0x24,0x01,0x01,0x03,0x02,0x00,0x20};
static unsigned char const tap1[] PROGMEM = {181,
	0x00,0x33,0x80,0x02,0x00,0x05,0x80,0x01,0x40,0x02,0x20,0x04,0x10,0x04,0x08,0x01,
	0x04,0x01,0x02,0x01,0x01,0x02,0x02,0x01,0x0c,0x01,0x30,0x01,0x40,0x01,0x80,0x01,
	0x00,0x2e,0x80,0x03,0x00,0x31,0x1e,0x01,0xe1,0x01,0x00,0x02,0x01,0x02,0x02,0x02,
	0x01,0x01,0x00,0x04,0x80,0x02,0x00,0x09,0x80,0x01,0x00,0x01,0x30,0x02,0x00,0x02,
	0x01,0x01,0xe1,0x01,0x1a,0x01,0x06,0x01,0x09,0x01,0x31,0x01,0x35,0x01,0x01,0x01,
	0x8a,0x01,0x7c,0x01,0x00,0x02,0x80,0x04,0x40,0x04,0x20,0x04,0x10,0x04,0x08,0x05,
	0x04,0x05,0x02,0x04,0x01,0x04,0x00,0x33,0x80,0x01,0x70,0x01,0x0c,0x01,0x03,0x01,
	0x00,0x0a,0x01,0x02,0x00,0x01,0x40,0x01,0xa0,0x01,0x21,0x01,0x22,0x01,0x12,0x01,
	0x11,0x03,0x09,0x01,0x08,0x04,0x04,0x06,0x02,0x03,0x01,0x04,0x00,0x36,0x80,0x05,
	0x40,0x05,0x20,0x05,0x10,0x05,0x08,0x05,0x04,0x05,0x02,0x01,0x03,0x01,0x7a,0x02,
	0x79,0x04,0x39,0x01,0x31,0x01,0x02,0x02,0x04,0x02,0x08,0x03,0x88,0x02,0x87,0x01,
	0x80,0x01,0x00,0x3c};
static unsigned char const *tap_anim[TAP_FRAMES] = {
	tap0, tap1 };
#endif // #ifndef LEFTCAT

#ifndef RIGHTCAT // Left aligned Bongocat
static unsigned char const left_idle0[] PROGMEM = {173,
	0x80,0x03,0x00,0x2e,0x80,0x01,0x40,0x01,0x30,0x01,0x0c,0x01,0x02,0x01,0x01,0x02,
	0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x04,0x20,0x04,0x40,0x02,0x80,0x01,0x00,0x05,
	0x80,0x02,0x00,0x36,0x01,0x04,0x02,0x04,0x04,0x05,0x08,0x05,0x10,0x04,0x20,0x04,
	0x40,0x04,0x80,0x04,0x00,0x03,0x80,0x01,0x40,0x01,0x20,0x01,0x10,0x01,0x08,0x01,
	0x04,0x01,0x02,0x01,0x01,0x02,0x00,0x02,0x30,0x02,0x00,0x01,0x80,0x01,0x00,0x09,
	0x80,0x02,0x00,0x04,0x01,0x01,0x02,0x02,0x01,0x02,0x00,0x02,0xe1,0x01,0x1e,0x01,
	0x00,0x56,0x01,0x02,0x0f,0x01,0x11,0x01,0x10,0x05,0x08,0x02,0x04,0x02,0x08,0x04,
	0x09,0x01,0x11,0x03,0x12,0x01,0x22,0x01,0x21,0x01,0xa0,0x01,0x40,0x01,0x00,0x01,
	0x01,0x02,0x00,0x0a,0x03,0x01,0x0c,0x01,0x70,0x01,0x80,0x01,0x00,0x6d,0x07,0x01,
	0x08,0x05,0x04,0x02,0x02,0x02,0x01,0x06,0x02,0x02,0x03,0x01,0x02,0x01,0x04,0x05,
	0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,0x00,0x11};
static unsigned char const left_idle1[] PROGMEM = {159,
	0x80,0x03,0x00,0x2f,0x80,0x01,0x60,0x01,0x18,0x01,0x04,0x01,0x02,0x02,0x04,0x01,
	0x08,0x01,0x10,0x01,0x20,0x04,0x40,0x04,0x80,0x02,0x00,0x3e,0x01,0x04,0x02,0x04,
	0x04,0x05,0x08,0x05,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x04,0x00,0x04,0x80,0x01,
	0x40,0x01,0x20,0x01,0x10,0x01,0x08,0x01,0x04,0x01,0x02,0x02,0x01,0x01,0x00,0x01,
	0x60,0x02,0x00,0x10,0x01,0x01,0x02,0x01,0x04,0x02,0x02,0x02,0x01,0x02,0xc2,0x01,
	0x3c,0x01,0x00,0x56,0x01,0x02,0x0f,0x01,0x11,0x01,0x10,0x05,0x08,0x02,0x04,0x02,
	0x08,0x04,0x0b,0x01,0x12,0x03,0x14,0x01,0x24,0x01,0x22,0x01,0xa0,0x01,0x40,0x01,
	0x00,0x01,0x03,0x02,0x00,0x0a,0x06,0x01,0x19,0x01,0x70,0x01,0x80,0x01,0x00,0x6d,
	0x07,0x01,0x08,0x05,0x04,0x02,0x02,0x02,0x01,0x06,0x02,0x02,0x03,0x01,0x02,0x01,
	0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,0x00,0x11};
static unsigned char const left_idle2[] PROGMEM = {161,
	0x80,0x03,0x00,0x2e,0x80,0x01,0x40,0x01,0x30,0x01,0x08,0x01,0x04,0x03,0x08,0x02,
	0x10,0x05,0x20,0x04,0x40,0x02,0x80,0x01,0x00,0x3d,0x01,0x04,0x02,0x04,0x04,0x05,
	0x08,0x05,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x04,0x00,0x03,0x80,0x01,0x40,0x01,
	0x20,0x01,0x10,0x01,0x08,0x01,0x04,0x01,0x02,0x01,0x01,0x02,0x00,0x02,0x30,0x02,
	0x00,0x01,0x80,0x01,0x00,0x09,0x80,0x02,0x00,0x04,0x01,0x01,0x02,0x05,0x82,0x01,
	0x64,0x01,0x18,0x01,0x00,0x56,0x01,0x02,0x0f,0x01,0x11,0x01,0x10,0x05,0x08,0x02,
	0x04,0x02,0x08,0x04,0x09,0x01,0x11,0x03,0x12,0x01,0x22,0x01,0x21,0x01,0xa0,0x01,
	0x40,0x01,0x00,0x01,0x01,0x02,0x00,0x0a,0x03,0x01,0x04,0x01,0x38,0x01,0xc0,0x01,
	0x00,0x6d,0x07,0x01,0x08,0x05,0x04,0x02,0x02,0x02,0x01,0x06,0x02,0x02,0x03,0x01,
	0x02,0x01,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,0x00,0x11};
static unsigned char const left_idle3[] PROGMEM = {163,
	0x80,0x03,0x00,0x2e,0x80,0x01,0x40,0x01,0x38,0x01,0x04,0x01,0x02,0x03,0x04,0x01,
	0x08,0x02,0x10,0x04,0x20,0x04,0x40,0x02,0x80,0x01,0x00,0x3d,0x01,0x04,0x02,0x04,
	0x04,0x05,0x08,0x05,0x10,0x04,0x20,0x04,0x40,0x04,0x80,0x04,0x00,0x03,0x80,0x01,
	0x40,0x01,0x20,0x01,0x10,0x01,0x08,0x01,0x04,0x01,0x02,0x01,0x01,0x02,0x00,0x02,
	0x30,0x02,0x00,0x01,0x80,0x01,0x00,0x09,0x80,0x02,0x00,0x04,0x01,0x01,0x02,0x04,
	0x01,0x02,0xe2,0x01,0x1c,0x01,0x00,0x56,0x01,0x02,0x0f,0x01,0x11,0x01,0x10,0x05,
	0x08,0x02,0x04,0x02,0x08,0x04,0x09,0x01,0x11,0x03,0x12,0x01,0x22,0x01,0x21,0x01,
	0xa0,0x01,0x40,0x01,0x00,0x01,0x01,0x02,0x00,0x0a,0x03,0x01,0x0c,0x01,0x70,0x01,
	0x80,0x01,0x00,0x6d,0x07,0x01,0x08,0x05,0x04,0x02,0x02,0x02,0x01,0x06,0x02,0x02,
	0x03,0x01,0x02,0x01,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,
	0x00,0x11};
static unsigned char const *left_idle_anim[IDLE_FRAMES] = {
	left_idle0, left_idle0, left_idle1, left_idle2, left_idle3 };

static unsigned char const left_prep[] PROGMEM = {179,
	0x80,0x03,0x00,0x2e,0x80,0x01,0x40,0x01,0x30,0x01,0x0c,0x01,0x02,0x01,0x01,0x02,
	0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x04,0x20,0x04,0x40,0x02,0x80,0x01,0x00,0x05,
	0x80,0x02,0x00,0x36,0x01,0x04,0x02,0x04,0x04,0x05,0x08,0x05,0x10,0x04,0x20,0x04,
	0x40,0x04,0x80,0x04,0x00,0x02,0x7c,0x01,0x8a,0x01,0x01,0x01,0x35,0x01,0x31,0x01,
	0x09,0x01,0x06,0x01,0x1a,0x01,0xe1,0x01,0x01,0x01,0x00,0x02,0x30,0x02,0x00,0x01,
	0x80,0x01,0x00,0x09,0x80,0x02,0x00,0x02,0x80,0x02,0x81,0x01,0x02,0x02,0x01,0x02,
	0x00,0x02,0xe1,0x01,0x1e,0x01,0x00,0x56,0x01,0x04,0x02,0x03,0x04,0x06,0x08,0x04,
	0x09,0x01,0x11,0x03,0x12,0x01,0x22,0x01,0x21,0x01,0x20,0x01,0x40,0x02,0x41,0x02,
	0x7c,0x01,0xc3,0x01,0x84,0x01,0x99,0x01,0x98,0x01,0x05,0x01,0x06,0x01,0x18,0x01,
	0x00,0x02,0x03,0x01,0x0c,0x01,0x70,0x01,0x80,0x01,0x00,0x77,0x01,0x06,0x02,0x02,
	0x03,0x01,0x02,0x01,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,
	0x00,0x11};

static unsigned char const left_tap0[] PROGMEM = {219,
	0x80,0x03,0x00,0x1c,0x80,0x02,0x00,0x05,0xf8,0x04,0x00,0x07,0x80,0x01,0x40,0x01,
	0x30,0x01,0x0c,0x01,0x02,0x01,0x01,0x02,0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x04,
	0x20,0x04,0x40,0x02,0x80,0x01,0x00,0x05,0x80,0x02,0x00,0x36,0x01,0x04,0x02,0x04,
	0x04,0x05,0x08,0x05,0x10,0x04,0x20,0x04,0x4c,0x01,0x4f,0x01,0x5f,0x02,0x9f,0x01,
	0xbe,0x01,0xbc,0x01,0xb8,0x01,0x00,0x01,0x01,0x01,0x07,0x01,0x87,0x01,0x43,0x01,
	0x20,0x01,0x10,0x01,0x08,0x01,0x04,0x01,0x02,0x01,0x01,0x02,0x00,0x02,0x30,0x02,
	0x00,0x01,0x80,0x01,0x00,0x09,0x80,0x02,0x00,0x02,0x80,0x02,0x81,0x01,0x02,0x02,
	0x01,0x02,0x00,0x02,0xe1,0x01,0x1e,0x01,0x00,0x4e,0x0c,0x01,0x3c,0x01,0xfc,0x05,
	0x7c,0x01,0x3d,0x01,0x01,0x01,0x0f,0x01,0x11,0x01,0x10,0x05,0x08,0x02,0x04,0x02,
	0x08,0x04,0x09,0x01,0x11,0x03,0x12,0x01,0x22,0x01,0x21,0x01,0x20,0x01,0x40,0x02,
	0x41,0x02,0x7c,0x01,0x43,0x01,0x84,0x01,0x99,0x01,0x98,0x01,0x05,0x01,0x06,0x01,
	0x18,0x01,0x00,0x02,0x03,0x01,0x0c,0x01,0x70,0x01,0x80,0x01,0x00,0x50,0x03,0x02,
	0x01,0x01,0x00,0x24,0x01,0x06,0x02,0x02,0x03,0x01,0x02,0x01,0x04,0x05,0x08,0x05,
	0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,0x00,0x11};
static unsigned char const left_tap1[] PROGMEM = {179,
	0x80,0x03,0x00,0x2e,0x80,0x01,0x40,0x01,0x30,0x01,0x0c,0x01,0x02,0x01,0x01,0x02,
	0x02,0x01,0x04,0x01,0x08,0x01,0x10,0x04,0x20,0x04,0x40,0x02,0x80,0x01,0x00,0x05,
	0x80,0x02,0x00,0x36,0x01,0x04,0x02,0x04,0x04,0x05,0x08,0x05,0x10,0x04,0x20,0x04,
	0x40,0x04,0x80,0x04,0x00,0x02,0x7c,0x01,0x8a,0x01,0x01,0x01,0x35,0x01,0x31,0x01,
	0x09,0x01,0x06,0x01,0x1a,0x01,0xe1,0x01,0x01,0x01,0x00,0x02,0x30,0x02,0x00,0x01,
	0x80,0x01,0x00,0x09,0x80,0x02,0x00,0x04,0x01,0x01,0x02,0x02,0x01,0x02,0x00,0x02,
	0xe1,0x01,0x1e,0x01,0x00,0x56,0x01,0x04,0x02,0x03,0x04,0x06,0x08,0x04,0x09,0x01,
	0x11,0x03,0x12,0x01,0x22,0x01,0x21,0x01,0xa0,0x01,0x40,0x01,0x00,0x01,0x01,0x02,
	0x00,0x0a,0x03,0x01,0x0c,0x01,0x70,0x01,0x80,0x01,0x00,0x6c,0x80,0x01,0x87,0x01,
	0x88,0x02,0x08,0x03,0x04,0x02,0x02,0x02,0x31,0x01,0x39,0x01,0x79,0x04,0x7a,0x02,
	0x03,0x01,0x02,0x01,0x04,0x05,0x08,0x05,0x10,0x05,0x20,0x05,0x40,0x05,0x80,0x05,
	0x00,0x11};
static unsigned char const *left_tap_anim[TAP_FRAMES] = {
	left_tap0, left_tap1 };
#endif // #ifndef RIGHTCAT


// RLE decoding loop that reads byte,count pairs from frame array
// to render repeated bytes consecutively based on count
static void decode_frame(unsigned char const *frame) {
	uint16_t cursor = 0;
	uint8_t size    = pgm_read_byte(frame);

	oled_set_cursor(0,0);
	for (uint8_t i = 1; i < size-1; i+=2 ) {
		// Get byte and consecutive count
		char byte     = pgm_read_byte(frame + i);
		uint8_t count = pgm_read_byte(frame + i + 1);

		for (uint8_t j = 0; j < count; ++j) {
			// Render bytes consecutively
			oled_write_raw_byte(byte, cursor);
			cursor++;
		}
	}
}


static void render_cat_idle(void) {
	static uint8_t current_frame = 0;
	current_frame = current_frame < IDLE_FRAMES - 1 ? current_frame + 1 : 0;

#if defined(LEFTCAT)
	decode_frame(left_idle_anim[current_frame]);
#elif defined(RIGHTCAT)
	decode_frame(idle_anim[current_frame]);
#else
	is_keyboard_left() ? decode_frame(left_idle_anim[current_frame]) : decode_frame(idle_anim[current_frame]);
#endif
}


static void render_cat_prep(void) {
#if defined(LEFTCAT)
	decode_frame(left_prep);
#elif defined(RIGHTCAT)
	decode_frame(prep);
#else
	is_keyboard_left() ? decode_frame(left_prep) : decode_frame(prep);
#endif
}


static void render_cat_tap(void) {
	static uint8_t current_frame = 0;
	current_frame = (current_frame + 1) & 1;

#if defined(LEFTCAT)
	decode_frame(left_tap_anim[current_frame]);
#elif defined(RIGHTCAT)
	decode_frame(tap_anim[current_frame]);
#else
	is_keyboard_left() ? decode_frame(left_tap_anim[current_frame]) : decode_frame(tap_anim[current_frame]);
#endif
}


void render_bongocat(void) {
	// Animation timer
	static uint16_t anim_timer = 0;

#ifdef WPM_ENABLE
	static uint8_t prev_wpm   = 0;
	static uint32_t tap_timer = 0;
	// tap_timer updated by sustained WPM
	if (get_current_wpm() > prev_wpm) { tap_timer = timer_read32(); }
	prev_wpm = get_current_wpm();
#else
	// tap_timer updated by key presses in process_record_user()
	extern uint32_t tap_timer;
#endif

	// Time gap between tap_timer updates
	uint32_t keystroke = timer_elapsed32(tap_timer);

	void animate_cat(void) {
		if (keystroke < FRAME_DURATION*2) { render_cat_tap(); }
		else if (keystroke < FRAME_DURATION*8) { render_cat_prep(); }
		else { render_cat_idle(); }
	}

	if (keystroke > OLED_TIMEOUT) { oled_off(); }
	else if (timer_elapsed(anim_timer) > FRAME_DURATION) {
		anim_timer = timer_read();
		animate_cat();
	}
}


// Init and rendering calls
oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
	if (is_keyboard_master()) {
#if defined(LEFTCAT)
		return rotation;
#elif defined(RIGHTCAT)
		return OLED_ROTATION_180;
#else
		return is_keyboard_left() ? rotation : OLED_ROTATION_180;
#endif
	}
	else { return OLED_ROTATION_270; }
}

void oled_task_user(void) {
	extern void render_mod_status(void);
	if (is_keyboard_master()) { render_bongocat(); }
	else { render_mod_status(); }
}
