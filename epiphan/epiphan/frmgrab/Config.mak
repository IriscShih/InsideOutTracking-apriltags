# -*- Mode: makefile -*-
#
# $Id: Config.mak 11264 2010-10-17 07:24:27Z monich $
#
# Copyright (C) 2008-2010 Epiphan Systems Inc. All rights reserved.
#
# libfrmgrab build configuration
#

ifndef FRMGRAB_DIR
${error FRMGRAB_DIR must point to the directory containing Config.mak}
endif # FRMGRAB_DIR

# Make sure that opensource/config/Config.mak is included
ifndef COMMON_CONFIG_INCLUDED
${error You must include master Config.mak prior to including this file}
endif # COMMON_CONFIG_INCLUDED

#
# File names
#

FRMGRAB_BASE = libfrmgrab
FRMGRAB_RELEASE_FILE = $(FRMGRAB_BASE).a
FRMGRAB_PROFILE_FILE = $(FRMGRAB_BASE)p.a
FRMGRAB_DEBUG_FILE = $(FRMGRAB_BASE)d.a

#
# Directories
#

FRMGRAB_RELEASE_BUILD_DIR = $(FRMGRAB_DIR)/$(RELEASE_BUILD_DIR)
FRMGRAB_PROFILE_BUILD_DIR = $(FRMGRAB_DIR)/$(PROFILE_BUILD_DIR)
FRMGRAB_DEBUG_BUILD_DIR = $(FRMGRAB_DIR)/$(DEBUG_BUILD_DIR)

#
# Includes
#

FRMGRAB_INCLUDES = -I$(FRMGRAB_DIR)/include

#
# Targets
#

FRMGRAB_RELEASE_LIB = $(FRMGRAB_RELEASE_BUILD_DIR)/$(FRMGRAB_RELEASE_FILE)
FRMGRAB_PROFILE_LIB = $(FRMGRAB_PROFILE_BUILD_DIR)/$(FRMGRAB_PROFILE_FILE)
FRMGRAB_DEBUG_LIB = $(FRMGRAB_DEBUG_BUILD_DIR)/$(FRMGRAB_DEBUG_FILE)

FRMGRAB_RELEASE_TARGET = frmgrab_release$(ARCH_TARGET_SUFFIX)
FRMGRAB_PROFILE_TARGET = frmgrab_profile$(ARCH_TARGET_SUFFIX)
FRMGRAB_DEBUG_TARGET = frmgrab_debug$(ARCH_TARGET_SUFFIX)

#
# SDK lib
#

ifeq ($(OS),darwin)

FRMGRAB_SDK_LIB = $(FRMGRAB_DIR)/lib/macosx/$(FRMGRAB_RELEASE_FILE)

else  # OS

ifdef ARCH
FRMGRAB_SDK_LIB = $(FRMGRAB_DIR)/lib/$(OS)/$(ARCH)/$(FRMGRAB_RELEASE_FILE)
else  # ARCH
FRMGRAB_SDK_LIB = $(FRMGRAB_DIR)/lib/$(OS)/$(CPU)/$(FRMGRAB_RELEASE_FILE)
endif # ARCH

endif # OS
