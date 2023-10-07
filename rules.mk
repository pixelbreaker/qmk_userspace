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
REPEAT_KEY_ENABLE = yes

VPATH += $(USER_PATH)/features
INTROSPECTION_KEYMAP_C = pixelbreaker.c

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), bastardkb/charybdis/3x5/v2/splinky_3))
  OPT_DEFS += -DKEYBOARD_charybdis
  OPT_DEFS += -DTRACKBALL_ENABLE
	SRC += scrollspam.c
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = no
	DEFERRED_EXEC_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/tenome/trackball pixelbreaker/tenome/trackpad))
  OPT_DEFS += -DKEYBOARD_tenome
  OPT_DEFS += -DENCODER_ENABLE
	SRC += scrollspam.c
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = yes
	DEFERRED_EXEC_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/tenome/trackball))
  OPT_DEFS += -DTRACKBALL_ENABLE
endif


ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/sphynx))
  OPT_DEFS += -DKEYBOARD_sphynx
  OPT_DEFS += -DENCODER_ENABLE
	RGBLIGHT_SUPPORTED = no
	RGB_MATRIX_ENABLE = no
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/hummingbird))
  OPT_DEFS += -DKEYBOARD_hummingbird
  RGBLIGHT_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/buteo))
  OPT_DEFS += -DKEYBOARD_buteo
  OPT_DEFS += -DENCODER_ENABLE
  SRC += scrollspam.c
  RGBLIGHT_ENABLE = no
  RGB_MATRIX_ENABLE = no
  DEFERRED_EXEC_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/sweep))
  OPT_DEFS += -DKEYBOARD_sweep
  # AUTOCORRECT_ENABLE = no
  # CONVERT_TO = elite_pi
endif
