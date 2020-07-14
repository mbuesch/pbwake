# Build configuration:

# Target device. May be one of:
#	t13	=> ATTiny13
#	t85	=> ATTiny85
#	m328p	=> ATMega328P
DEV			:= t85

# Load pulse configuration (all values in milliseconds):

# Minimum length of high-phase:
CONF_HIGH_MIN_MS	:= 4
# Maximum length of high-phase:
CONF_HIGH_MAX_MS	:= 6
# Minimum length of low-phase:
CONF_LOW_MIN_MS		:= 40000
# Maximum length of low-phase:
CONF_LOW_MAX_MS		:= 55000


##############################################################################
##############################################################################
##############################################################################

$(info	DEV=$(DEV) \
	CONF_HIGH_MIN_MS=$(CONF_HIGH_MIN_MS) \
	CONF_HIGH_MAX_MS=$(CONF_HIGH_MAX_MS) \
	CONF_LOW_MIN_MS=$(CONF_LOW_MIN_MS) \
	CONF_LOW_MAX_MS=$(CONF_LOW_MAX_MS))

# Project name
NAME			:= pbwake

# Project source files
SRCS			:= \
			load.c \
			main.c \
			shr3.c \
			watchdog.c

GEN_SRCS		:=

# Bootloader source files
BOOT_SRCS		:=
BOOT_GEN_SRCS		:=
BOOT_OFFSET		:=

ifeq ($(DEV),t13)
# ATTiny 13
# 128 kHz, SUT 64 ms, BOD 2.7 V, WDT off, SPIEN
F_CPU			:= 128000UL
LFUSE			:= 0x7B
HFUSE			:= 0xFB
EFUSE			:=
endif

ifeq ($(DEV),t85)
# ATTiny 85
# 128 kHz, SUT 64 ms, BOD 2.7 V, WDT off, SPIEN
F_CPU			:= 128000UL
LFUSE			:= 0xE4
HFUSE			:= 0xDD
EFUSE			:= 0xFF
endif

ifeq ($(DEV),m328p)
# ATMega 328P
# 128 kHz, SUT 65 ms, BOD 2.7 V, WDT off, SPIEN
F_CPU			:= 128000UL
LFUSE			:= 0xE3
HFUSE			:= 0xD9
EFUSE			:= 0xFD
endif

# Architecture configuration
GCC_ARCH		:= $(subst m,atmega,$(subst t,attiny,$(DEV)))
AVRDUDE_ARCH		:= $(DEV)
FUNC_STACK_LIMIT	:=

# Programmer selection.
# Values can be:  avrisp2, mysmartusb
PROGRAMMER		:= avrisp2
AVRDUDE_SPEED		:= 50
AVRDUDE_SLOW_SPEED	:= 200

# Instrumentation
INSTRUMENT_FUNC		:=
BOOT_INSTRUMENT_FUNC	:=

# Additional compiler flags
CFLAGS			:= -DCONF_HIGH_MIN_MS="$(CONF_HIGH_MIN_MS)" \
			   -DCONF_HIGH_MAX_MS="$(CONF_HIGH_MAX_MS)" \
			   -DCONF_LOW_MIN_MS="$(CONF_LOW_MIN_MS)" \
			   -DCONF_LOW_MAX_MS="$(CONF_LOW_MAX_MS)"
BOOT_CFLAGS		:= $(CFLAGS)
LDFLAGS			:=

# Additional "clean" and "distclean" target files
CLEAN_FILES		:=
DISTCLEAN_FILES		:=


streq = $(and $(filter 1,$(words $2)),$(filter $1,$(firstword $2)))
deveq = $(call streq,$1,$(DEV))

ifeq ($(strip $(call deveq,t13)\
	      $(call deveq,t85)\
	      $(call deveq,m328p)),)
$(error "DEV=$(DEV) is not supported.")
endif

include avrmakelib.mk
