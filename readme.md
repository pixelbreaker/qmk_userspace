# Summary

This is my personal _userspace_ for [QMK Firmware](https://github.com/qmk/qmk_firmware). It is set up as a self-contained repository that avoids `keymap.c` files within keyboard sub-folders. It can by build by placing this repository within QMK's [userspace](https://docs.qmk.fm/#/feature_userspace) folder and compiling with the JSON files. [Actions](https://docs.github.com/en/actions) can also be leveraged to do likewise on a GitHub container with [build.yml](.github/workflows/build.yml) workflow.

![corneplanck](https://github.com/filterpaper/filterpaper.github.io/raw/main/images/corneplanck.png)

# Features

-   Shared [layout](layout.h) wrapper macros
-   [Combos](#combo-helper-macros) simplified with preprocessors
-   [Tap-hold](#tap-hold-macros) clipboard shortcuts
-   [Word](features/) processing features
    -   Autocorrection for typos
    -   Caps Unlock that follows a word
-   [RGB](rgb/) matrix lighting and effects
    -   Custom "candy" matrix effect
    -   [Layer indicators](#light-configured-layers-keys) of active keys

# Corne (CRKBD) OLED display

Corne keyboard can be build with few OLED display options using `-e OLED=` environment variable to select pet animation on primary display:

## Bongocat or Luna and Felix

Bongocat animation is the default pet. Use the following option to select Luna or Felix:

```
qmk compile -e OLED=LUNA corne.json
qmk compile -e OLED=FELIX corne.json
```

## Logo file

Icons used to render keyboard state is stored in `glcdfont.c`. Images in that file can be viewed and edited with:

-   [Helix Font Editor](https://helixfonteditor.netlify.app/)
-   [QMK Logo Editor](https://joric.github.io/qle/)
-   [image2cpp](https://javl.github.io/image2cpp/)

# Code Snippets

## Tap hold macros

```c
#define TH_W LT(0, KC_W)

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
    case TH_W:
        if (record->event.pressed && record->tap.count == 0) {
            // Send macro string on hold
            SEND_STRING(":wq");
            return false;
        }
        break;
    }
    return true; // continue with unmatched keycodes
}
```

Tap hold shortcuts with layer tap (`LT()`) uses less firmware space than [tap dance](https://docs.qmk.fm/#/feature_tap_dance) (~35 bytes per shortcut). Macro `W_TH` replaces `KC_W` on the key map, and the code will intercept hold function of `LT()` to send the macro string. See QMK's [Intercepting Mod Taps](https://docs.qmk.fm/#/mod_tap?id=intercepting-mod-taps) for details.

## Combo helper macros

The [QMK combo](https://docs.qmk.fm/#/feature_combo?id=combos) code file `combos.c` is modified from [Jane Bernhardt's helper macros](http://combos.gboards.ca/) to simplify management. Combos are simple one-liners in `combos.inc` and preprocessor macros will generate source codes at compile time.

## Pro Micro RX/TX LEDs

Data LEDs on Pro Micro can be used as indicators with code. They are pins `B0` (RX) and `D5` (TX) on Atmega32u4. To use them with QMK's [LED Indicators](https://docs.qmk.fm/#/feature_led_indicators), flag the pin in `config.h`:

```c
#define LED_CAPS_LOCK_PIN B0
#define LED_PIN_ON_STATE 0
```

For advance usage, setup the following macros to call both pins with GPIO functions:

```c
// Pro Micro data LED pins
#define RXLED B0
#define TXLED D5
// GPIO control macros
#define RXLED_INIT setPinOutput(RXLED)
#define TXLED_INIT setPinOutput(TXLED)
#define RXLED_ON   writePinLow(RXLED)
#define TXLED_ON   writePinLow(TXLED)
#define RXLED_OFF  writePinHigh(RXLED)
#define TXLED_OFF  writePinHigh(TXLED)
```

Initialise LEDs with the `*_INIT` macro on startup inside `matrix_init_user(void)`. Subsequently, LEDs can be used as indicators with the `*_ON` and `*_OFF` macros that follows.

# Tap Hold Mod Tweaks

These are some custom mod tap settings to avoid false positives with home row mods.

## Tap timer

Setup a tap timer to detect key presses with `process_record_user`:

```c
static fast_timer_t tap_timer = 0;

bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        tap_timer = timer_read_fast();
    }
    return true;
}
```

## Increase tapping term while typing

Use the previous tap timer to detect typing interval that are shorter than `TAPPING_TERM * 1.5` with the following macros:

```c
#define TYPING_TERM (TAPPING_TERM * 2)
#define IS_TYPING() (timer_elapsed_fast(tap_timer) < TYPING_TERM)
```

Use `get_tapping_term()` to return higher value on short typing interval to avoid modifier activation:

```c
#define TAPPING_TERM_PER_KEY

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    return IS_QK_MOD_TAP(keycode) && IS_TYPING() ? TYPING_TERM : TAPPING_TERM;
}
```

## Permissive hold for thumb shift

Activate Shift mod tap with another nested key press when not within typing interval.

```c
#define PERMISSIVE_HOLD_PER_KEY

bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
    if (IS_QK_MOD_TAP(keycode) && QK_MODS_GET_MODS(keycode) & MOD_MASK_SHIFT && !IS_TYPING()) {
        return true;
    }
    return false;
}
```

## Hold on layer tap

Trigger layer taps immediately with another key press.

```c
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY

bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return IS_QK_LAYER_TAP(keycode) ? true : false;
}
```

# Layout wrapper macros

## Basic setup

A single key map layout can be shared with multiple keyboards using C preprocessors macros. They are referenced in the keyboard JSON files and the build process will expand them into a transient `keymap.c` file during compile time.

The `split_3x5_2` layout is used as the base layout and defined in `layout.h` file, e.g.:

```c
#define _BASE \
    KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,        KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,    \
    KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_QUOT, \
    KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, \
                  LT(SYM,KC_TAB), LCA_T(KC_ENT),     RSFT_T(KC_SPC), LT(NUM,KC_BSPC)
```

Next, a wrapper alias to the layout used by the keyboard is also defined in `layout.h` file, e.g. for Cradio:

```c
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
```

Both macros are referenced in the keyboard's JSON file with the following format:

```c
{
    "keyboard": "cradio",
    "keymap": "filterpaper",
    "layout": "LAYOUT_34key_w",
    "layers": [
        [ "_BASE" ],
        [ "_NUMB" ],
        [ "_SYMB" ],
        [ "_FUNC" ]
    ]
}
```

Append `#include layout.h` to `config.h`. The build process will construct a transient `keymap.c` using the C preprocessor macros referenced by JSON. Wrapper aliases are necessary because `LAYOUT_34key_w(_BASE)` contains a single variable and both macros are replaced (expanded) in the build process concurrently.

## Wrapping home row modifiers

[Home row mods](https://precondition.github.io/home-row-mods) can be wrapped over the layout macros. Order of home row modifiers are defined with these two macros:

```c
#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRMR(k1,k2,k3,k4) RGUI_T(k1),RCTL_T(k2),RALT_T(k3),RSFT_T(k4)
```

Both are then placed within the `HRM` macro for the `split_3x5_2` base:

```c
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
      l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, \
      l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, \
      l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, \
                     l16, l17,    r16, r17                 \
) \
      l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05,       \
HRML(l06, l07, l08, l09), l10,    r06, HRMR(r07, r08, r09, r10), \
      l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15,       \
                     l16, l17,    r16, r17
```

They come together in the JSON file, by wrapping `HRM()` on layers that require them, e.g.:

```c
"layers": [
    [ "HRM(_BASE)" ],
    [ "HRM(_COLE)" ],
    [ "_NUMB" ],
    [ "_SYMB" ],
    [ "_FUNC" ]
],
```

## Adapting layouts

The base layout is shared and adapted for other split keyboards by expanding them with macros. The following example expands Cradio's 34-key to Corne's 42-key 3x6_3 (6-column, 3-thumb) using the following wrapper macro to add additional keys to outer columns:

```c
#define LAYOUT_corne_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
// 3x5_2 to 42-key conversion
#define C_42(k) CONV_42(k)
#define CONV_42( \
         l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, \
         l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, \
         l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, \
                        l16, l17,    r16, r17                 \
) \
KC_TAB,  l01, l02, l03, l04, l05,    r01, r02, r03, r04, r05, KC_BSPC, \
QK_GESC, l06, l07, l08, l09, l10,    r06, r07, r08, r09, r10, KC_SCLN, \
KC_LSFT, l11, l12, l13, l14, l15,    r11, r12, r13, r14, r15, KC_ENT,  \
         RSA_T(KC_ESC), l16, l17,    r16, r17, RAG_T(KC_DEL)
```

The JSON file for 42-key Corne uses the `C_42()` macro in the following format:

```c
{
    "keyboard": "crkbd/rev1",
    "keymap": "filterpaper",
    "layout": "LAYOUT_corne_w",
    "layers": [
        [ "C_42(HRM(_BASE))" ],
        [ "C_42(_NUMB)" ],
        [ "C_42(_SYMB)" ],
        [ "C_42(_FUNC)" ]
    ]
}
```

# KB2040 Neopixel

The neopixel LEDs can be enabled for RGB Matrix with the following:

```c
rules.mk:
RGB_MATRIX_ENABLE = yes
RGB_MATRIX_DRIVER = WS2812

config.h:
#define RGBW
#define WS2812_DI_PIN 17U
#define WS2812_PIO_USE_PIO1
```

Additional directives for a pair on split:

```c
config.h
#define RGB_MATRIX_LED_COUNT 2
#define RGB_MATRIX_SPLIT {1, 1}
#define SPLIT_TRANSPORT_MIRROR

keymap.c (or userspace source file):
// Example for 3x5_2
led_config_t g_led_config = { {
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
    { 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
    {109, 48}, {115, 48}
}, {
    0x0f, 0xf0
} };
```

# STeMcell notes

STM32F411 replacement [controller](https://github.com/megamind4089/STeMCell) with Pro micro footprint, [v1.0.1](https://github.com/megamind4089/STeMCell/releases/tag/v1.0.1). Runs on [tinyuf2 bootloader](https://megamind4089.github.io/STeMCell/software/).

-   Reset new STMC to `stm-dfu`:
    -   Connect USB while holding button
    -   Short `RST` and `GND` while holding button
-   Reset STMC with tinyuf2:
    -   Double-short `RST` and `GND`
    -   `QK_BOOT` keycode
    -   Bootmagic lite

## Bootloaders

To install the STeMcell tinyuf2 bootloader

```
dfu-util -a 0 -i 0 -s 0x08000000:leave -D tinyuf2-stemcell.bin
```

To wipe the entire STeMcell flash (wait up to 30s):

```
dfu-util -a 0 -i 0 -s 0x08000000:mass-erase:force
```

# Flashing Notes

## ISP Flashing

## Autocorrect dictionary creation

`qmk generate-autocorrect-data ./users/pixelbreaker/features/dictionaries/dictionary_huge.txt -o ./users/pixelbreaker/features/autocorrect_data.h`
