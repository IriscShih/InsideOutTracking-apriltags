# -*- Mode: makefile -*-
#
# $Id: Config.mak 1495 2013-05-31 17:07:52Z monich $
#
# Copyright (C) 2001-2013 Epiphan Systems Inc. All rights reserved.
#
# Platform-specific configuration
#
# Environment: 
#
# GNU make on Unix. Not guaranteed to work with non-GNU make utilities
# and especially on non-Unix platforms 
#

ifndef CONFIG_DIR
${error CONFIG_DIR must point to the directory containing Config.mak}
endif # CONFIG_DIR

COMMON_CONFIG_INCLUDED = YES

#
# Root of the opensource tree
#

ifndef OPENSOURCE_DIR
OPENSOURCE_DIR = $(shell (if [ -d "$(CONFIG_DIR)/.." ] ; then cd "$(CONFIG_DIR)/.."; pwd; else echo "OPENSOURCE_DIR"; fi))
endif # OPENSOURCE_DIR

#
# Platform-specific targets require that MAKEFILE is defined
#

ifndef MAKEFILE
MAKEFILE = Makefile
endif # Makefile

#
# Determine the platform
#

OS = $(shell uname -s | tr '[A-Z]' '[a-z]' )

#
# Determine the CPU type
#

ifeq ($(OS),linux)
CPU = ${shell uname -i}
else
CPU = unknown
endif

ifeq ($(CPU),unknown)
CPU = ${shell uname -p}
ifeq ($(CPU),unknown)
CPU = ${shell uname -m}
endif
endif

# Substitiute i*86 with i386
ifeq ($(CPU),i486)
CPU = i386
endif
ifeq ($(CPU),i586)
CPU = i386
endif
ifeq ($(CPU),i686)
CPU = i386
endif

#
# Macro to quietly run a command
#

RUN = $(if $(V),$1,$(if $(2),@echo $2 && $1, @$1))

#
# Compiler settings
#

CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)gcc
STRIP = $(CROSS_COMPILE)strip
DEBUG_FLAGS = -g $(USER_DEBUG_ADD_FLAGS)
RELEASE_FLAGS = -O3 $(USER_RELEASE_ADD_FLAGS)
PROFILE_FLAGS = -pg $(USER_PROFILE_ADD_FLAGS)
DEBUG_DEFINES = -DDEBUG=1 -D_DEBUG=1
RELEASE_DEFINES = -DNDEBUG=1
BASE_WARNINGS = -Wall -Wwrite-strings -Wnested-externs
WARNINGS = $(BASE_WARNINGS) # -Waggregate-return

ARFLAGS = rc

#
# Platform specific settings
#

ifeq ($(OS),darwin)

SYSTEM_FRAMEWORKS = /System/Library/Frameworks
CORE_FRAMEWORKS = $(SYSTEM_FRAMEWORKS)/CoreServices.framework/Frameworks
KERNEL_FRAMEWORK = $(SYSTEM_FRAMEWORKS)/Kernel.framework
CARBON_FRAMEWORK = $(CORE_FRAMEWORKS)/CarbonCore.framework
JAVA_FRAMEWORK = $(SYSTEM_FRAMEWORKS)/JavaVM.framework
APPLICATION_FRAMEWORK = $(SYSTEM_FRAMEWORKS)/ApplicationServices.framework
DEFAULT_PLATFORM_LIBS = -lobjc -framework Cocoa -framework ApplicationServices

BASE_CPPFLAGS = -F$(APPLICATION_FRAMEWORK)/Frameworks -F$(SYSTEM_FRAMEWORKS)
BASE_DEFINES = -D_UNIX -DAPPLE -DNeXT

JAVA_INCLUDES = -I$(JAVA_FRAMEWORK)/Headers

#
# Locations of SDKs and tools
#

MACOSX_SDK_10_4 = /Developer/SDKs/MacOSX10.4u.sdk
MACOSX_SDK_10_6 = /Developer/SDKs/MacOSX10.6.sdk
GCC_4_2 = /Developer/usr/bin/gcc-4.2
GCC_4_0 = /Developer/usr/bin/gcc-4.0
PACKAGEMAKER = /Developer/Applications/Utilities/PackageMaker.app/Contents/MacOS/PackageMaker
XCODEBUILD = xcodebuild # It's usually in the PATH

# Extension for shared libraries
SO_EXT = dylib

# Mac OS X version
OS_RELEASE   = $(shell uname -r | cut -f1 -d.)

ifeq ($(shell [ $(OS_RELEASE) -ge 8 ] ; echo $$?),0)
TIGER_OR_LATER = YES
ifeq ($(shell [ $(OS_RELEASE) -eq 8 ] ; echo $$?),0)
TIGER = YES
endif
endif # $(OS_RELEASE) >= 8

ifeq ($(shell [ $(OS_RELEASE) -ge 9 ] ; echo $$?),0)
LEOPARD_OR_LATER = YES
ifeq ($(shell [ $(OS_RELEASE) -eq 9 ] ; echo $$?),0)
LEOPARD = YES
endif
endif # $(OS_RELEASE) >= 9

ifeq ($(shell [ $(OS_RELEASE) -ge 10 ] ; echo $$?),0)
SNOW_LEOPARD_OR_LATER = YES
ifeq ($(shell [ $(OS_RELEASE) -eq 10 ] ; echo $$?),0)
SNOW_LEOPARD = YES
endif
endif # $(OS_RELEASE) >= 10

ifeq ($(shell [ $(OS_RELEASE) -ge 11 ] ; echo $$?),0)
MACOSX_LION_OR_LATER = YES
ifeq ($(shell [ $(OS_RELEASE) -eq 11 ] ; echo $$?),0)
MACOSX_LION = YES
endif
endif # $(OS_RELEASE) >= 11

# We support Mac OS X 10.4 and up. Without this flag, applications built on
# Mac OS X 10.6 don't load on 10.4 giving errors like "dyld: unknown required
# load command 0x80000022" (if they are linked without this flag) or "dyld:
# Symbol not found: ___stack_chk_guard" (if they are compiled without it).
# -mmacosx-version-min=10.4 seems to take care of that
ifdef TIGER_OR_LATER
BASE_CFLAGS += -mmacosx-version-min=10.4
BASE_LDFLAGS += -mmacosx-version-min=10.4
endif # TIGER_OR_LATER

# Supported architectures
ifdef ARCH
ifneq ($(ARCH),native)
BASE_CFLAGS += -arch $(ARCH)
BASE_LDFLAGS += -arch $(ARCH)
else
override ARCH=
endif
else # ARCH
ifdef TIGER_OR_LATER
BASE_CFLAGS  += -arch i386
BASE_LDFLAGS += -arch i386
X86_DEBUG_BUILD_DIR = $(BUILD_DIR)/i386/debug
X86_RELEASE_BUILD_DIR = $(BUILD_DIR)/i386/release
X86_PROFILE_BUILD_DIR = $(BUILD_DIR)/i386/release_profile
# Support for PowerPC was dropped in Lion
ifndef MACOSX_LION_OR_LATER
BASE_CFLAGS  += -arch ppc
BASE_LDFLAGS += -arch ppc
PPC_DEBUG_BUILD_DIR = $(BUILD_DIR)/ppc/debug
PPC_RELEASE_BUILD_DIR = $(BUILD_DIR)/ppc/release
PPC_PROFILE_BUILD_DIR = $(BUILD_DIR)/ppc/release_profile
endif # MACOSX_LION_OR_LATER
ifdef SNOW_LEOPARD_OR_LATER
ifdef DISABLE_64_BIT
DISABLE_X86_64 = YES
endif # DISABLE_X86_64
ifndef DISABLE_X86_64
BASE_CFLAGS  += -arch x86_64
BASE_LDFLAGS += -arch x86_64
X86_64_DEBUG_BUILD_DIR = $(BUILD_DIR)/x86_64/debug
X86_64_RELEASE_BUILD_DIR = $(BUILD_DIR)/x86_64/release
X86_64_PROFILE_BUILD_DIR = $(BUILD_DIR)/x86_64/release_profile
endif # DISABLE_X86_64
endif # SNOW_LEOPARD_OR_LATER
endif # TIGER_OR_LATER
endif # ARCH

else  # darwin

# Assume Linux
ifndef JAVA_HOME
JAVA_HOME = /usr/lib/jvm/java
endif
JAVA_INCLUDE_ROOT = $(JAVA_HOME)/include
BASE_CFLAGS += -fPIC
BASE_DEFINES = -D_REENTRANT -D_UNIX -D_LARGEFILE_SOURCE -D_LARGEFILE64_SOURCE
JAVA_INCLUDES = -I$(JAVA_INCLUDE_ROOT) -I$(JAVA_INCLUDE_ROOT)/$(OS)
# Extension for shared libraries
SO_EXT = so
endif # darwin

ifeq ($(OS),sunos)
X11LIBDIR = /usr/openwin/lib
DEFAULT_PLATFORM_LIBS = -lkstat -lsocket -lnsl -lrt
LD_SHARED_OPT = -G
else
ifeq ($(OS),darwin)
LD_SHARED_OPT = -dynamic
else  # darwin
LD_SHARED_OPT = -shared
endif # darwin
X11LIBDIR = /usr/X11R6/lib
endif # sunos

# Configuration for xcodebuild
ifdef TIGER_OR_LATER

XPROJ = xcodeproj
XCONF = -configuration
BUILD_32BIT = ARCHS="i386 ppc" GCC_VERSION=4.0 MACOSX_DEPLOYMENT_TARGET=10.4

ifdef SNOW_LEOPARD_OR_LATER
BUILD_64BIT = ARCHS=x86_64 GCC_VERSION=4.2
endif # SNOW_LEOPARD_OR_LATER
else  # TIGER_OR_LATER

XPROJ = xcode
XCONF = -buildstyle

endif # TIGER_OR_LATER

BASE_CFLAGS += $(WARNINGS)
CFLAGS += $(BASE_CFLAGS)
LDFLAGS = $(BASE_LDFLAGS)

DEFAULT_SYSTEM_LIBS = -lpthread -lm -lz
DEFINES = $(BASE_DEFINES)
SYSTEM_LIBS = $(DEFAULT_SYSTEM_LIBS) $(DEFAULT_PLATFORM_LIBS)

#
# Directories. Note that BUILD_DIR can be redefined prior to including 
# Config.mak
#

ifndef BUILD_DIR
BUILD_DIR = build
endif # BUILD_DIR

ifdef ARCH

DEBUG_BUILD_DIR = $(BUILD_DIR)/$(ARCH)/debug
RELEASE_BUILD_DIR = $(BUILD_DIR)/$(ARCH)/release
PROFILE_BUILD_DIR = $(BUILD_DIR)/$(ARCH)/release_profile
ARCH_TARGET_SUFFIX = _$(ARCH)
ARCH_FILE_SUFFIX = -$(ARCH)

else  # !ARCH

DEBUG_BUILD_DIR = $(BUILD_DIR)/debug
RELEASE_BUILD_DIR = $(BUILD_DIR)/release
PROFILE_BUILD_DIR = $(BUILD_DIR)/release_profile

endif # !ARCH

#
# Components
#

# SLIB

ifndef SLIB_DIR
SLIB_DIR = $(OPENSOURCE_DIR)/slib
endif # SLIB_DIR

SLIB_INCLUDES = -I$(SLIB_DIR)/include

SLIB_RELEASE_LIB = $(SLIB_DIR)/libslava$(ARCH_FILE_SUFFIX).a
SLIB_PROFILE_LIB = $(SLIB_DIR)/libslavap$(ARCH_FILE_SUFFIX).a
SLIB_DEBUG_LIB = $(SLIB_DIR)/libslavad$(ARCH_FILE_SUFFIX).a

SLIB_DEBUG_TARGET = slib_debug$(ARCH_TARGET_SUFFIX)
SLIB_PROFILE_TARGET = slib_profile$(ARCH_TARGET_SUFFIX)
SLIB_RELEASE_TARGET = slib_release$(ARCH_TARGET_SUFFIX)

# JPEG

ifndef JPEG_DIR
JPEG_DIR = $(OPENSOURCE_DIR)/jpeg
endif # JPEG_DIR
# check if this directory exists, if it doesn't - unset the variable
JPEG_DIR := $(shell (if [ -d "$(JPEG_DIR)" ] ; then echo "$(JPEG_DIR)"; fi))

ifeq ($(JPEG_DIR),)

JPEG_RELEASE_LIB = -ljpeg
JPEG_PROFILE_LIB = -ljpeg
JPEG_DEBUG_LIB = -ljpeg

else # ($(JPEG_DIR),)
JPEG_INCLUDES = -I$(JPEG_DIR)

JPEG_RELEASE_LIB = $(JPEG_DIR)/libjpeg$(ARCH_FILE_SUFFIX).a
JPEG_PROFILE_LIB = $(JPEG_DIR)/libjpegp$(ARCH_FILE_SUFFIX).a
JPEG_DEBUG_LIB = $(JPEG_DIR)/libjpegd$(ARCH_FILE_SUFFIX).a

JPEG_DEBUG_TARGET = jpeg_debug$(ARCH_TARGET_SUFFIX)
JPEG_PROFILE_TARGET = jpeg_profile$(ARCH_TARGET_SUFFIX)
JPEG_RELEASE_TARGET = jpeg_release$(ARCH_TARGET_SUFFIX)

endif # ($(JPEG_DIR),)
# PNG

ifndef PNG_DIR
PNG_DIR = $(OPENSOURCE_DIR)/png
endif # PNG_DIR

# check if this directory exists, if it doesn't - unset the variable
PNG_DIR := $(shell (if [ -d "$(PNG_DIR)" ] ; then echo "$(PNG_DIR)"; fi))

ifeq ($(PNG_DIR),)

PNG_RELEASE_LIB = -lpng
PNG_PROFILE_LIB = -lpng
PNG_DEBUG_LIB = -lpng

else # ($(PNG_DIR),)

PNG_INCLUDES = -I$(PNG_DIR)

PNG_RELEASE_LIB = $(PNG_DIR)/libpng$(ARCH_FILE_SUFFIX).a
PNG_PROFILE_LIB = $(PNG_DIR)/libpngp$(ARCH_FILE_SUFFIX).a
PNG_DEBUG_LIB = $(PNG_DIR)/libpngd$(ARCH_FILE_SUFFIX).a

PNG_DEBUG_TARGET = png_debug$(ARCH_TARGET_SUFFIX)
PNG_PROFILE_TARGET = png_profile$(ARCH_TARGET_SUFFIX)
PNG_RELEASE_TARGET = png_release$(ARCH_TARGET_SUFFIX)

endif # ($(PNG_DIR),)

# CURL

ifndef CURL_DIR
CURL_DIR = $(OPENSOURCE_DIR)/curl
endif # CURL_DIR

ifeq ($(CURL_DIR),)

CURL_RELEASE_LIB = -lcurl
CURL_PROFILE_LIB = -lcurl
CURL_DEBUG_LIB = -lcurl

else # ($(CURL_DIR),)

CURL_INCLUDES = -I$(CURL_DIR)/include
CURL_LIB_DIR =  $(CURL_DIR)/lib
CURL_RELEASE_LIB = $(CURL_LIB_DIR)/libcurl.a
CURL_PROFILE_LIB = $(CURL_LIB_DIR)/libcurlp.a
CURL_DEBUG_LIB = $(CURL_LIB_DIR)/libcurld.a

endif # ($(CURL_DIR),)

# EXPAT

ifeq ($(EXPAT_DIR),)

EXPAT_RELEASE_LIB = -lexpat
EXPAT_PROFILE_LIB = -lexpat
EXPAT_DEBUG_LIB = -lexpat

else # ($(EXPAT_DIR),)

# check if this directory exists
EXPAT_DIR := $(shell (if [ -d "$(EXPAT_DIR)" ] ; then echo "$(EXPAT_DIR)"; fi))

EXPAT_INCLUDES = -I$(EXPAT_DIR)/lib
EXPAT_LIB_DIR =  $(EXPAT_DIR)/lib
EXPAT_RELEASE_LIB = $(EXPAT_LIB_DIR)/libexpat$(ARCH_FILE_SUFFIX).a
EXPAT_PROFILE_LIB = $(EXPAT_LIB_DIR)/libexpatp$(ARCH_FILE_SUFFIX).a
EXPAT_DEBUG_LIB = $(EXPAT_LIB_DIR)/libexpatd$(ARCH_FILE_SUFFIX).a

EXPAT_DEBUG_TARGET = expat_debug$(ARCH_TARGET_SUFFIX)
EXPAT_PROFILE_TARGET = expat_profile$(ARCH_TARGET_SUFFIX)
EXPAT_RELEASE_TARGET = expat_release$(ARCH_TARGET_SUFFIX)

endif # ($(EXPAT_DIR),)

# ZLIB

ifndef ZLIB_DIR
ZLIB_DIR = $(OPENSOURCE_DIR)/zlib
endif # ZLIB_DIR

ifeq ($(ZLIB_DIR),)

EXPAT_RELEASE_LIB = -lz
EXPAT_PROFILE_LIB = -lz
EXPAT_DEBUG_LIB = -lz

else # ($(ZLIB_DIR),)

# check if this directory exists
ZLIB_DIR := $(shell (if [ -d "$(ZLIB_DIR)" ] ; then echo "$(ZLIB_DIR)"; fi))

ZLIB_INCLUDES = -I$(ZLIB_DIR)
ZLIB_LIB_DIR =  $(ZLIB_DIR)
ZLIB_RELEASE_LIB = $(ZLIB_LIB_DIR)/libz.a
ZLIB_PROFILE_LIB = $(ZLIB_RELEASE_LIB)
ZLIB_DEBUG_LIB = $(ZLIB_RELEASE_LIB)

ZLIB_DEBUG_TARGET = expat_debug$(ARCH_TARGET_SUFFIX)
ZLIB_PROFILE_TARGET = expat_profile$(ARCH_TARGET_SUFFIX)
ZLIB_RELEASE_TARGET = expat_release$(ARCH_TARGET_SUFFIX)

endif # ($(ZLIB_DIR),)
