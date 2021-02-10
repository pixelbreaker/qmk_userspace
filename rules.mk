# Disable defaults for space
VIA_ENABLE = no
WPM_ENABLE = no
NKRO_ENABLE = no
AUDIO_ENABLE = no
STENO_ENABLE = no
LEADER_ENABLE = no
UNICODE_ENABLE = no
CONSOLE_ENABLE = no
COMMAND_ENABLE = no
MOUSEKEY_ENABLE = no
TERMINAL_ENABLE = no
KEY_LOCK_ENABLE = no
BOOTMAGIC_ENABLE = no
SLEEP_LED_ENABLE = no
TAP_DANCE_ENABLE = no
VELOCIKEY_ENABLE = no
SWAP_HANDS_ENABLE = no
SPACE_CADET_ENABLE = no
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no

# Common features
EXTRAKEY_ENABLE = yes

# Platform and keyboard specific features
ifneq ($(PLATFORM),CHIBIOS)
	LTO_ENABLE = yes
endif
ifeq ($(KEYBOARD),$(filter $(KEYBOARD), bm40hsrgb planck/rev6 boardsource/the_mark))
	BOOTMAGIC_ENABLE = lite
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
endif
ifeq ($(strip $(KEYBOARD)), crkbd/rev1/common)
	BOOTLOADER = atmel-dfu
	WPM_ENABLE = yes
	MOUSEKEY_ENABLE = yes
	OLED_DRIVER_ENABLE = yes
endif

# Main shared source file
SRC += filterpaper.c
