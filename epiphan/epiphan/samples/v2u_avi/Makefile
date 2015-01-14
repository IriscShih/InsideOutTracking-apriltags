# -*- Mode: makefile -*-
#
# $Id: Makefile 22817 2013-06-26 18:07:15Z monich $
#
# Copyright (C) 2007-2013 Epiphan Systems Inc. All rights reserved.
#
# Unix makefile for the v2u_avi sample
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
# Output directories
#

DEBUG_DEPENDENCIES = v2u_debug
PROFILE_DEPENDENCIES = v2u_profile
RELEASE_DEPENDENCIES = v2u_release

DEBUG_LIBS = $(V2U_DEBUG_LIB)
PROFILE_LIBS = $(V2U_PROFILE_LIB)
RELEASE_LIBS = $(V2U_RELEASE_LIB)

#
# Targets
#

APP_NAME = v2u_avi
DEBUG_APP = $(DEBUG_BUILD_DIR)/$(APP_NAME)
PROFILE_APP = $(PROFILE_BUILD_DIR)/$(APP_NAME)
RELEASE_APP = $(RELEASE_BUILD_DIR)/$(APP_NAME)
RESULTS = $(DEBUG_APP) $(RELEASE_APP) $(PROFILE_APP) 

#
# Rules
#

all: debug release

clean:
	-rm -fr core core.* *~ $(SRC_DIR)/*~ $(BUILD_DIR)

#
# Source files
#

SRC_DIR = .
SRC_C = v2u_avi.c

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
# Dependencies
#

DEBUG_DEPENDENCIES = $(SLIB_DEBUG_TARGET)
PROFILE_DEPENDENCIES = $(SLIB_PROFILE_TARGET)
RELEASE_DEPENDENCIES = $(SLIB_RELEASE_TARGET)

DEBUG_LIBS = $(FRMGRAB_SDK_LIB) $(SLIB_DEBUG_LIB)
PROFILE_LIBS = $(FRMGRAB_SDK_LIB) $(SLIB_PROFILE_LIB)
RELEASE_LIBS = $(FRMGRAB_SDK_LIB) $(SLIB_RELEASE_LIB)

ifeq ($(OS),darwin)
DEBUG_DEPENDENCIES += $(EXPAT_DEBUG_TARGET)
PROFILE_DEPENDENCIES += $(EXPAT_PROFILE_TARGET)
RELEASE_DEPENDENCIES += $(EXPAT_RELEASE_TARGET)
DEBUG_LIBS += $(EXPAT_DEBUG_LIB)
PROFILE_LIBS += $(EXPAT_PROFILE_LIB)
RELEASE_LIBS += $(EXPAT_RELEASE_LIB)
endif

#
# Common rules
#

include $(V2ULIB_DIR)/Rules.mak
include $(CONFIG_DIR)/Rules.mak

#
# Dependencies
#

include $(DEPENDS)
