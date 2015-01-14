# -*- Mode: makefile -*-
#
# $Id: Makefile 8545 2010-01-27 07:59:08Z monich $
#
# Copyright (C) 2003-2010 Epiphan Systems Inc. All rights reserved.
#
# Makefile for libv2u
#

.PHONY: all clean 

#
# Include master config file
#

CONFIG_DIR = ../../../opensource/config
include $(CONFIG_DIR)/Config.mak

#
# Include v2ulib config file
#

V2ULIB_DIR = .
include $(V2ULIB_DIR)/Config.mak

#
# Directories
#

SRC_DIR = src
INCLUDES = -I../../include $(V2ULIB_INCLUDES) $(PNG_INCLUDES) $(JPEG_INCLUDES)

#
# Targets
#

DEBUG_LIB = $(V2U_DEBUG_LIB)
RELEASE_LIB = $(V2U_RELEASE_LIB)
PROFILE_LIB = $(V2U_PROFILE_LIB)
RESULTS = $(DEBUG_LIB) $(RELEASE_LIB) $(PROFILE_LIB) 

#
# Source files
#

SRC_C = v2u_bmp.c v2u_jpeg.c v2u_png.c v2u_sys.c v2u_util.c v2u_pal.c

#
# Rules
#

all: debug release

clean:
	-rm -fr core core.* *~ $(SRC_DIR)/*~ $(RESULTS) $(BUILD_DIR)

#
# Common rules
#

include $(CONFIG_DIR)/Rules.mak

#
# Dependencies
#

include $(DEPENDS)
