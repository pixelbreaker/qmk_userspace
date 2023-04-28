#include "tracktap.h"

// trackball mode
int16_t cum_x         = 0;
int16_t cum_y         = 0;
int16_t tap_factor    = 150;
int16_t switch_factor = 250;

uint16_t last_media_nav = 0; // debounce media next/previous tracks

// utils
int max(int num1, int num2) {
    return (num1 > num2) ? num1 : num2;
}
int min(int num1, int num2) {
    return (num1 > num2) ? num2 : num1;
}

int8_t sign(int x) {
    return (x > 0) - (x < 0);
}

void tap_code_fast(uint8_t code) {
    register_code(code);
    unregister_code(code);
}

// update cumulative mouse travel
void tap_report(report_mouse_t mouse_report) {
    cum_x += mouse_report.x;
    cum_y -= mouse_report.y;
}

void tap_tb(uint8_t keycode0, uint8_t keycode1, uint8_t keycode2, uint8_t keycode3) {
    if (abs(cum_x) + abs(cum_y) >= tap_factor) {
        if (abs(cum_x) * 0.4 > abs(cum_y)) {
            for (int8_t i = 0; i <= (abs(cum_x) + abs(cum_y)) / tap_factor; i++) {
                if (cum_x > 0) {
                    tap_code_fast(keycode0);
                    cum_x = max(cum_x - tap_factor, 0);
                } else {
                    tap_code_fast(keycode1);
                    cum_x = min(cum_x + tap_factor, 0);
                }
            }
            cum_y = 0;
        } else {
            for (int8_t i = 0; i <= (abs(cum_x) + abs(cum_y)) / tap_factor; i++) {
                if (cum_y > 0) {
                    tap_code_fast(keycode2);
                    cum_y = max(cum_y - tap_factor, 0);
                } else {
                    tap_code_fast(keycode3);
                    cum_y = min(cum_y + tap_factor, 0);
                }
            }
            cum_x = 0;
        }
    }
}

void tap_media(void) {
    if (abs(cum_x) + abs(cum_y) >= tap_factor) {
        if (abs(cum_x) * 0.4 > abs(cum_y)) {
            for (int8_t i = 0; i <= (abs(cum_x) + abs(cum_y)) / tap_factor; i++) {
                if (cum_x > 0) {
                    if (cum_x > MEDIA_TAP_THRESHOLD && timer_elapsed(last_media_nav) > MEDIA_TAP_TERM) {
                        tap_code_fast(KC_MNXT);
                        last_media_nav = timer_read();
                    }
                    cum_x = max(cum_x - tap_factor, 0);
                } else {
                    if (cum_x < -MEDIA_TAP_THRESHOLD && timer_elapsed(last_media_nav) > MEDIA_TAP_TERM) {
                        tap_code_fast(KC_MPRV);
                        last_media_nav = timer_read();
                    }
                    cum_x = max(cum_x + tap_factor, 0);
                }
            }
            cum_y = 0;
        } else {
            for (int8_t i = 0; i <= (abs(cum_x) + abs(cum_y)) / tap_factor; i++) {
                if (cum_y > 0) {
                    tap_code_fast(KC_VOLU);
                    cum_y = max(cum_y - tap_factor, 0);
                } else {
                    tap_code_fast(KC_VOLD);
                    cum_y = min(cum_y + tap_factor, 0);
                }
            }
            cum_x = 0;
        }
    }
}

void tap_switcher(void) {
    if (abs(cum_x) + abs(cum_y) >= switch_factor) {
        if (cum_x > 0) {
            for (int8_t i = 0; i <= (abs(cum_x) + abs(cum_y)) / switch_factor; i++) {
                tap_code_fast(KC_TAB);
                cum_x = max(cum_x - switch_factor, 0);
            }
            cum_y = 0;
        } else {
            for (int8_t i = 0; i <= (abs(cum_x) + abs(cum_y)) / switch_factor; i++) {
                register_code(KC_LSFT);
                tap_code_fast(KC_TAB);
                unregister_code(KC_LSFT);
                cum_x = min(cum_x + switch_factor, 0);
            }
            cum_y = 0;
        }
    }
}
