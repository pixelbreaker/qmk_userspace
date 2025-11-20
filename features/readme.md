# Word Processing Features

## Autocorrection


Dictionary is encoded in a trie data structure and the code will send corrections for typos detected in a buffer as they are being processed by QMK. Typos and corrections can be added into `dictionary.txt` in the format:
```c
typo   -> correction
```
Run `qmk generate-autocorrect-data users/pixelbreaker/features/dictionaries/dictionary_large.txt -o users/pixelbreaker/features/autocorrect_data.h` to generate trie dictionary array in `autocorrection_data.h`. The script will read from `dictionary.txt` as default without an input file argument.

## Caps Unlock
Automatically disable caps lock at a word boundary.

## QMK Integration
Include both source files in `rules.mk` to build them:
```c
SRC += autocorrection.c caps_unlock.c
```
Add the following into `process_record_user` to process every key press:
```c
bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        extern bool process_autocorrect(uint16_t keycode, keyrecord_t* record);
        if (!process_autocorrect(keycode, record)) {
            return false;
        }
        extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
        if (!process_caps_unlock(keycode, record)) {
            return false;
        }
    }
    return true;
}
```
