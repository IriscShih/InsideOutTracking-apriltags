# -*- Mode: makefile -*-
#
# $Id: Makefile,v 1.1 2005-04-09 22:58:59 monich Exp $
#
# Copyright (C) 2001-2005 Epiphan Consulting, Inc.  All rights reserved.
#
# Makefile for libpng
#

.PHONY: all clean 

#
# Include master config file
#

CONFIG_DIR = ../config
include $(CONFIG_DIR)/Config.mak

#
# Directories
#

SRC_DIR = .

#
# Targets
#

DEBUG_LIB = $(PNG_DEBUG_LIB)
RELEASE_LIB = $(PNG_RELEASE_LIB)
PROFILE_LIB = $(PNG_PROFILE_LIB)
RESULTS = $(DEBUG_LIB) $(RELEASE_LIB) $(PROFILE_LIB) 

#
# Source files
#

SRC_C = png.c pngerror.c pnggccrd.c pngget.c pngmem.c pngpread.c pngread.c \
  pngrio.c pngrtran.c pngrutil.c pngset.c pngtest.c pngtrans.c pngvcrd.c \
  pngwio.c pngwrite.c pngwtran.c pngwutil.c

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
