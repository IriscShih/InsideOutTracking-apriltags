# -*- Mode: makefile -*-
#
# $Id: Makefile 1307 2012-10-22 22:09:38Z monich $
#

.PHONY: all clean 

#
# Include master config file
#

CONFIG_DIR = ../config
EXPAT_DIR = .
include $(CONFIG_DIR)/Config.mak

#
# Directories
#

SRC_DIR = lib

#
# Targets
#

DEBUG_LIB = $(EXPAT_DEBUG_LIB)
RELEASE_LIB = $(EXPAT_RELEASE_LIB)
PROFILE_LIB = $(EXPAT_PROFILE_LIB)
RESULTS = $(DEBUG_LIB) $(RELEASE_LIB) $(PROFILE_LIB) 

#
# Source files
#

SRC_C = xmlparse.c xmlrole.c xmltok.c

#
# Build settings
#

DEFINES += -DHAVE_EXPAT_CONFIG_H
INCLUDES = -I. -I$(SRC_DIR)

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
