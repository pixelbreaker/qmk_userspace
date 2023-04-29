#pragma once

#include QMK_KEYBOARD_H

void tap_report(report_mouse_t mouse_report);

void tap_code_fast(uint8_t code);

void tap_tb(uint8_t keycode0, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3);

void tap_media(void);

void tap_switcher(void);
