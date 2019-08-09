# Project Code : CZ8A
#
# Make Settings for Ruks CZ8A Project
#
# Copyright (C) 2007 SPJ Systems Pune,India 
# All rights reserved.
#
# -------
# History
# ----------------
# Date       |Release|Author| Changes
# DD-MM-YYYY |       |      |
# ----------------------------------------------------------------------------
# 09.02.2007 |  0.1  |  BM  | Initial Version
#=============================================================================
RELEASE  := LPC2148
DEST  := build
GNU_LIB_PATH := C:\SCARM\GNUARM\arm-elf\lib
GCC_LIB_PATH := C:\SCARM\GNUARM\lib\gcc\arm-elf\4.4.1
# info
TRACE=@
MAKE_TARGET      := echo Compiling...    ;
MAKE_TARGET_AS   := echo Assembling....  ;
MAKE_TARGET_LINK := echo Linking and Generating    $(RELEASE).elf;
MAKE_GUI_LIB     := echo Building Micrium GUI Library;

##############################################################################
# paths for application files
##############################################################################

OUTPUT_PATH         := $(WORK_DIR)/$(DEST)
BIN_PATH            := $(OUTPUT_PATH)/bin
OBJ_PATH            := $(OUTPUT_PATH)/obj
LIST_FILES_PATH     := $(OUTPUT_PATH)/lst/



APP_PATH            := $(WORK_DIR)\src\appl
CONFIG_PATH         := $(WORK_DIR)\src\config
OS_PATH             := $(WORK_DIR)\src\os
SYSTEM_PATH         := $(WORK_DIR)\src\system
BSP_PATH            := $(WORK_DIR)\src\bsp
HAL_PATH            := $(WORK_DIR)\src\hal


WS_OUT_CODE_PATH    := $(WORK_DIR)\$(DEST)\bin


##############################################################################
# paths for build tools  #            
MCU = arm7tdmi
SUBMDL = LPC2148
#THUMB    = -mthumb
THUMB_IW = 
#THUMB_IW = -mthumb-interwork
# Place -D or -U options for C here
## Create ROM-Image (final)
RUN_MODE=ROM_RUN
## Create RAM-Image (debugging)
#RUN_MODE=RAM_RUN

CDEFS =  -D$(RUN_MODE)
ADEFS =  -D$(RUN_MODE)
DEBUG = 2

# Place -I options here
CINCS =

CCINCLUDES   := $(APP_PATH) $(CONFIG_PATH) $(OS_PATH) $(SYSTEM_PATH)\
                $(BSP_PATH) $(HAL_PATH)

CFLAGS = -g$(DEBUG)
CFLAGS += $(CDEFS) $(CINCS)
#CFLAGS += -O$(OPT)
CFLAGS += -Wall -Wcast-align -Wcast-qual -Wimplicit 
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
#CFLAGS += -Wa,-adhlns=$(LIST_FILES_PATH)/$(subst $(suffix $<),.lst,$<) 
CFLAGS += -Wa,-adhlns=$(LIST_FILES_PATH)/$(notdir $*.lst)
CFLAGS += $(patsubst %,-I%,$(CCINCLUDES))

ASFLAGS = $(ADEFS) -Wa,-adhlns=$(LIST_FILES_PATH)/$(notdir $*.lst)

GENDEPFLAGS = -MD -MP -MF .dep/$(@F).d

# Compiler flag to set the C Standard level.
# c89   - "ANSI" C
# gnu89 - c89 plus GCC extensions
# c99   - ISO C99 standard (not yet fully implemented)
# gnu99 - c99 plus GCC extensions
CSTANDARD = -std=gnu99

# flags only for C
CONLYFLAGS  = -Wstrict-prototypes -Wmissing-declarations
CONLYFLAGS += -Wmissing-prototypes -Wnested-externs 
CONLYFLAGS += $(CSTANDARD)


GUI_LIB = -lmicrium-gui

#Support for newlibc-lpc (file: libnewlibc-lpc.a)
NEWLIBLPC =

MATH_LIB = -lm

CPLUSPLUS_LIB = -lstdc++

# Linker flags.
#  -Wl,...:     tell GCC to pass this to linker.
#    -Map:      create map file
#    --cref:    add cross reference to  map file
#LDFLAGS = -nostartfiles -Map=$(RELEASE).map --cref
LDFLAGS =  -nostartfiles -Wl,-Map=$(BIN_PATH)/$(RELEASE).map,--cref
LDFLAGS += $(NEWLIBLPC) $(MATH_LIB) 
LDFLAGS += -lc
LDFLAGS += -lc -lgcc -Lbuild/lib
LDFLAGS += $(CPLUSPLUS_LIB)

# Set Linker-Script Depending On Selected Memory
ifeq ($(RUN_MODE),RAM_RUN)
LDFLAGS +=-T$(CONFIG_PATH)/$(SUBMDL)-RAM.ld
else 
LDFLAGS +=-T$(CONFIG_PATH)/$(SUBMDL)-ROM.ld
endif
##############################################################################
CC          = arm-elf-gcc
ARC         = arm-elf-ar
OBJCOPY     = arm-elf-objcopy
LINK        = arm-elf-ld

CCINFILE    = $<
CCOUTFILE   = $@

CCFLAGS     = 
#ALL_CFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I $(CFLAGS) $(GENDEPFLAGS) 
ALL_CFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I $(CFLAGS) -o$(CCOUTFILE)
ALL_ASFLAGS = -mcpu=$(MCU) $(THUMB_IW) -I. -x assembler-with-cpp $(ASFLAGS)

#$(THUMB_IW) $(CFLAGS) $(GENDEPFLAGS)


LIB_FLAGS   = 
# linker

##############################################################################
# settings for build tools
##############################################################################
OBJECT_SUFFIX  := .o
LIBRARY_SUFFIX := .a
LIST_SUFFIX    := .src


