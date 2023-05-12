# Disable unused features
MAGIC_ENABLE = no
UNICODE_ENABLE = no
SPACE_CADET_ENABLE = no
SWAP_HANDS_ENABLE = no
CONSOLE_ENABLE = no

# Enable common features
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes
MOUSEKEY_ENABLE = yes
AUTOCORRECT_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes

VPATH += $(USER_PATH)/features
SRC += pixelbreaker.c combos.c achordion.c

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), bastardkb/charybdis/3x5/v2/splinky_3))
	SRC += scrollspam.c
	VIA_ENABLE = no
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = no
	DEFERRED_EXEC_ENABLE = yes
    OPT_DEFS += -DKEYBOARD_charybdis
    OPT_DEFS += -DTRACKBALL_ENABLE
	OPT_DEFS += -DCOMBO_TERM=90 
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), handwired/sphynx))
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = no
    OPT_DEFS += -DKEYBOARD_sphynx
    OPT_DEFS += -DENCODER_ENABLE
endif
