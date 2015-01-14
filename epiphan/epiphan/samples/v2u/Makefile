# -*- Mode: makefile -*-
#
# $Id: Makefile 22817 2013-06-26 18:07:15Z monich $
#
# Copyright (C) 2003-2013 Epiphan Systems Inc. All rights reserved.
#
# Unix makefile for the v2u sample
#

.PHONY: all clean 

#
# Name of this makefile
#

MAKEFILE = Makefile

#
# Include master config file
#

OPENSOURCE_DIR = ../../../opensource
CONFIG_DIR = $(OPENSOURCE_DIR)/config
EXPAT_DIR = $(OPENSOURCE_DIR)/expat
include $(CONFIG_DIR)/Config.mak

#
# Include libv2u config file
#

V2ULIB_REL_DIR = ../v2u_lib
V2ULIB_DIR = ${shell (if [ -d "$(V2ULIB_REL_DIR)" ] ; then cd "$(V2ULIB_REL_DIR)"; pwd; else echo "V2ULIB_DIR"; fi)}
include $(V2ULIB_DIR)/Config.mak

#
# Include frmgrab config file
#

FRMGRAB_REL_DIR = ../../frmgrab
FRMGRAB_DIR = ${shell (if [ -d "$(FRMGRAB_REL_DIR)" ] ; then cd "$(FRMGRAB_REL_DIR)"; pwd; else echo "FRMGRAB_DIR"; fi)}
include $(FRMGRAB_DIR)/Config.mak

#
# Location of avi_writer lib
#

AVI_WRITER_DIR = ../../codec/avi_writer

#
# Source files
#

SRC_DIR = .
SRC_C = v2u.c

SRC_DIR1 = $(AVI_WRITER_DIR)
SRC_C1 = avi_writer.c

#
# Compiler settings
#

INCLUDES = \
 -I../../include \
 -I$(AVI_WRITER_DIR) \
 $(FRMGRAB_INCLUDES) \
 $(V2ULIB_INCLUDES)

#
# Targets
#

DEBUG_APP_FILE = v2ud
RELEASE_APP_FILE = v2u
PROFILE_APP_FILE = v2up

DEBUG_APP = $(DEBUG_BUILD_DIR)/$(DEBUG_APP_FILE)
RELEASE_APP = $(RELEASE_BUILD_DIR)/$(RELEASE_APP_FILE)
PROFILE_APP = $(PROFILE_BUILD_DIR)/$(PROFILE_APP_FILE)
RESULTS = $(DEBUG_APP) $(RELEASE_APP) $(PROFILE_APP) 

#
# Dependencies
#

DEBUG_DEPENDENCIES = \
 $(SLIB_DEBUG_TARGET) \
 $(JPEG_DEBUG_TARGET) \
 $(PNG_DEBUG_TARGET)

PROFILE_DEPENDENCIES = \
 $(SLIB_PROFILE_TARGET) \
 $(JPEG_PROFILE_TARGET) \
 $(PNG_PROFILE_TARGET)

RELEASE_DEPENDENCIES = \
 $(SLIB_RELEASE_TARGET) \
 $(JPEG_RELEASE_TARGET) \
 $(PNG_RELEASE_TARGET)

DEBUG_LIBS = \
 $(FRMGRAB_SDK_LIB) \
 $(SLIB_DEBUG_LIB) \
 $(JPEG_DEBUG_LIB) \
 $(PNG_DEBUG_LIB)

PROFILE_LIBS = \
 $(FRMGRAB_SDK_LIB) \
 $(SLIB_PROFILE_LIB) \
 $(JPEG_PROFILE_LIB) \
 $(PNG_PROFILE_LIB)

RELEASE_LIBS = \
 $(FRMGRAB_SDK_LIB) \
 $(SLIB_RELEASE_LIB) \
 $(JPEG_RELEASE_LIB) \
 $(PNG_RELEASE_LIB)

ifeq ($(OS),darwin)
DEBUG_DEPENDENCIES += $(EXPAT_DEBUG_TARGET)
PROFILE_DEPENDENCIES += $(EXPAT_PROFILE_TARGET)
RELEASE_DEPENDENCIES += $(EXPAT_RELEASE_TARGET)
DEBUG_LIBS += $(EXPAT_DEBUG_LIB)
PROFILE_LIBS += $(EXPAT_PROFILE_LIB)
RELEASE_LIBS += $(EXPAT_RELEASE_LIB)
endif

#
# Default rule
#

all: debug release

#
# Clean rule
#

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
