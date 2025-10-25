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
REPEAT_KEY_ENABLE = no
RGBLIGHT_ENABLE = no
RGB_MATRIX_ENABLE = no

VPATH += $(USER_PATH)/features
INTROSPECTION_KEYMAP_C = pixelbreaker.c

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), bastardkb/charybdis/3x5/elitec))
  OPT_DEFS += -DKEYBOARD_charybdis
  OPT_DEFS += -DTRACKBALL_ENABLE
	SRC += scrollspam.c
	DEFERRED_EXEC_ENABLE = yes
  CONVERT_TO = rp2040_ce
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/cnano/stock pixelbreaker/cnano/encoder))
  OPT_DEFS += -DKEYBOARD_charybdis
  OPT_DEFS += -DKEYBOARD_cnano
  OPT_DEFS += -DTRACKBALL_ENABLE
	SRC += scrollspam.c
	DEFERRED_EXEC_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/cnano/encoder))
	RGB_MATRIX_ENABLE = yes
  OPT_DEFS += -DENCODER_ENABLE
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/tenome/trackball pixelbreaker/tenome/trackpad))
  OPT_DEFS += -DKEYBOARD_tenome
  OPT_DEFS += -DENCODER_ENABLE
	SRC += scrollspam.c
	RGB_MATRIX_ENABLE = yes
	DEFERRED_EXEC_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/tenome/trackball))
  OPT_DEFS += -DTRACKBALL_ENABLE
endif


ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/sphynx))
  OPT_DEFS += -DKEYBOARD_sphynx
  OPT_DEFS += -DENCODER_ENABLE
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/hummingbird))
  OPT_DEFS += -DKEYBOARD_hummingbird
  OPT_DEFS += -DLAYOUT_23332
  RGBLIGHT_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), ferris/sweep))
  OPT_DEFS += -DKEYBOARD_ferris
  AUTOCORRECT_ENABLE = no
  # fix flashing for elite-c
  BOOTLOADER = atmel-dfu
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/buteo/cirque pixelbreaker/buteo/talon))
  OPT_DEFS += -DKEYBOARD_buteo
  OPT_DEFS += -DLAYOUT_23332
  OPT_DEFS += -DENCODER_ENABLE
  SRC += scrollspam.c
  DEFERRED_EXEC_ENABLE = yes
endif

ifeq ($(strip $(KEYBOARD)), $(filter $(KEYBOARD), pixelbreaker/buteo/talon))
  OPT_DEFS += -DKEYBOARD_buteo_talon
endif
