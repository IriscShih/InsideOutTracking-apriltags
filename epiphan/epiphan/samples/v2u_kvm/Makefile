# -*- Mode: makefile -*-
#
# $Id: Makefile 8545 2010-01-27 07:59:08Z monich $
#
# Copyright (C) 2003-2010 Epiphan Systems Inc. All rights reserved.
#
# Unix makefile for the v2u_kvm sample
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

DEBUG_APP = $(DEBUG_BUILD_DIR)/v2u_kvm
PROFILE_APP = $(PROFILE_BUILD_DIR)/v2u_kvm
RELEASE_APP = $(RELEASE_BUILD_DIR)/v2u_kvm
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
SRC_C = v2u_kvm.c

#
# Compiler settings
#

INCLUDES = -I../../include $(V2ULIB_INCLUDES)

#
# Common rules
#

include $(V2ULIB_DIR)/Rules.mak
include $(CONFIG_DIR)/Rules.mak

#
# Dependencies
#

include $(DEPENDS)
