# -*- Mode: makefile -*-
#
# $Id: Makefile 18364 2012-09-10 15:46:02Z pzeldin $
#
# Copyright (C) 2007-2010 Epiphan Systems Inc. All rights reserved.
#
# Unix makefile for the v2u_dec sample
#

.PHONY: all clean 

#
# Name of this makefile
#

MAKEFILE = Makefile

#
# Include master config file
#

CONFIG_DIR = ../../../opensource/config
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

APP_NAME = v2u_dec
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
SRC_C = v2u_dec.c

#
# Compiler settings
#

INCLUDES = \
 -I../../include \
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

#
# Common rules
#

include $(V2ULIB_DIR)/Rules.mak
include $(CONFIG_DIR)/Rules.mak

#
# Dependencies
#

include $(DEPENDS)
