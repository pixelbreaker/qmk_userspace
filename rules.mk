# Disable unused features
MAGIC_ENABLE = no
UNICODE_ENABLE = no
SPACE_CADET_ENABLE = no
SWAP_HANDS_ENABLE = no
CONSOLE_ENABLE = yes

# Enable common features
COMBO_ENABLE = yes
CAPS_WORD_ENABLE = yes
MOUSEKEY_ENABLE = yes
AUTOCORRECT_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = yes
BOOTMAGIC_ENABLE = yes
LEADER_ENABLE = yes

VPATH += $(USER_PATH)/features
INTROSPECTION_KEYMAP_C = pixelbreaker.c
SRC += achordion.c

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

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/tenome/trackball pixelbreaker/tenome/trackpad))
	SRC += scrollspam.c
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = yes
	DEFERRED_EXEC_ENABLE = yes
    OPT_DEFS += -DKEYBOARD_tenome
    OPT_DEFS += -DENCODER_ENABLE
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/tenome/trackball))
    OPT_DEFS += -DTRACKBALL_ENABLE
endif


ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/sphynx))
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = no
    OPT_DEFS += -DKEYBOARD_sphynx
    OPT_DEFS += -DENCODER_ENABLE
endif
