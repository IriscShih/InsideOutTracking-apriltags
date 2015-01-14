# -*- Mode: makefile -*-
#
# $Id: Rules.mak 1495 2013-05-31 17:07:52Z monich $
#
# Copyright (C) 2001-2013 Epiphan Systems Inc. All rights reserved.
#
# Build rules
#
# Environment: 
#
# GNU make on Unix. Not guaranteed to work with non-GNU make utilities
# and especially on non-Unix platforms
#

#
# Input:
#
# SRC_C      list of the C source files (no directory names)
# SRC_CPP    list of the C++ source files (no directory names)
# SRC_OBJC   list of the Objective C source files (no directory names)
# SRC_OBJCPP list of the Objective C/C++ source files (no directory names)
# SRC_DIR    directory where source files are located
# 

SRC_FILES = \
$(SRC_C:%=$(SRC_DIR)/%) \
$(SRC_C1:%=$(SRC_DIR1)/%) \
$(SRC_C2:%=$(SRC_DIR2)/%) \
$(SRC_C3:%=$(SRC_DIR3)/%) \
$(SRC_C4:%=$(SRC_DIR4)/%) \
$(SRC_C5:%=$(SRC_DIR5)/%) \
$(SRC_C6:%=$(SRC_DIR6)/%) \
$(SRC_CPP:%=$(SRC_DIR)/%) \
$(SRC_CPP1:%=$(SRC_DIR1)/%) \
$(SRC_CPP2:%=$(SRC_DIR2)/%) \
$(SRC_OBJC:%=$(SRC_DIR)/%) \
$(SRC_OBJCPP:%=$(SRC_DIR)/%)

DEBUG_OBJS = \
$(SRC_C:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_C1:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_C2:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_C3:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_C4:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_C5:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_C6:%.c=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_CPP:%.cpp=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_CPP1:%.cpp=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_CPP2:%.cpp=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_OBJC:%.m=$(DEBUG_BUILD_DIR)/%.o) \
$(SRC_OBJCPP:%.mm=$(DEBUG_BUILD_DIR)/%.o)

RELEASE_OBJS = \
$(SRC_C:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_C1:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_C2:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_C3:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_C4:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_C5:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_C6:%.c=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_CPP:%.cpp=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_CPP1:%.cpp=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_CPP2:%.cpp=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_OBJC:%.m=$(RELEASE_BUILD_DIR)/%.o) \
$(SRC_OBJCPP:%.mm=$(RELEASE_BUILD_DIR)/%.o)

PROFILE_OBJS = \
$(SRC_C:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_C1:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_C2:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_C3:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_C4:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_C5:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_C6:%.c=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_CPP:%.cpp=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_CPP1:%.cpp=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_CPP2:%.cpp=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_OBJC:%.m=$(PROFILE_BUILD_DIR)/%.o) \
$(SRC_OBJCPP:%.mm=$(PROFILE_BUILD_DIR)/%.o)

#
# Create build directories
#

$(DEBUG_BUILD_DIR):
	@mkdir -p $@

$(RELEASE_BUILD_DIR):
	@mkdir -p $@

$(PROFILE_BUILD_DIR):
	@mkdir -p $@

#
# These targets are used for building Mac OS X universal binaries
#

debug_%:
	$(call RUN,$(MAKE) -f $(MAKEFILE) V="$(V)" ARCH=$* debug,)

profile_%:
	$(call RUN,$(MAKE) -f $(MAKEFILE) V="$(V)" ARCH=$* profile,)

release_%:
	$(call RUN,$(MAKE) -f $(MAKEFILE) V="$(V)" ARCH=$* release,)

# ============================================================================
# Build static library
# ============================================================================

ifdef DEBUG_LIB
ifdef DEBUG_UNIVERSAL

ifndef X86_DEBUG_LIB
ifdef DEBUG_LIB_FILE
X86_DEBUG_LIB = $(X86_DEBUG_BUILD_DIR)/$(DEBUG_LIB_FILE)
else # DEBUG_LIB_FILE
${error You must define either X86_DEBUG_LIB or DEBUG_LIB_FILE}
endif # DEBUG_LIB_FILE
endif # X86_DEBUG_LIB

ifndef MACOSX_LION_OR_LATER
ifndef PPC_DEBUG_LIB
ifdef DEBUG_LIB_FILE
PPC_DEBUG_LIB = $(PPC_DEBUG_BUILD_DIR)/$(DEBUG_LIB_FILE)
else # DEBUG_LIB_FILE
${error You must define either PPC_DEBUG_LIB or DEBUG_LIB_FILE}
endif # DEBUG_LIB_FILE
endif # PPC_DEBUG_LIB

$(PPC_DEBUG_LIB): debug_ppc

else # MACOSX_LION_OR_LATER
override PPC_DEBUG_LIB=
endif # MACOSX_LION_OR_LATER

debug: universal_debug

universal_debug: $(DEBUG_LIB)

$(X86_DEBUG_LIB): debug_i386

$(DEBUG_LIB): $(X86_DEBUG_LIB) $(PPC_DEBUG_LIB) $(DEBUG_BUILD_DIR)
	$(call RUN,lipo -create $(X86_DEBUG_LIB) $(PPC_DEBUG_LIB) -output $@,"  LIPO  $@")

else # !DEBUG_UNIVERSAL

debug: $(DEBUG_PREBUILD_TARGET) $(DEBUG_BUILD_DIR) $(DEBUG_LIB)

$(DEBUG_LIB): $(DEBUG_OBJS)
	$(call RUN,$(AR) $(ARFLAGS) $@ $?,"  AR    $@")
	@ranlib $@

endif # !DEBUG_UNIVERSAL

endif # DEBUG_LIB

ifdef RELEASE_LIB
ifdef RELEASE_UNIVERSAL

ifndef X86_RELEASE_LIB
ifdef RELEASE_LIB_FILE
X86_RELEASE_LIB = $(X86_RELEASE_BUILD_DIR)/$(RELEASE_LIB_FILE)
else # RELEASE_LIB_FILE
${error You must define either X86_RELEASE_LIB or RELEASE_LIB_FILE}
endif # RELEASE_LIB_FILE
endif # X86_RELEASE_LIB

ifndef MACOSX_LION_OR_LATER
ifndef PPC_RELEASE_LIB
ifdef RELEASE_LIB_FILE
PPC_RELEASE_LIB = $(PPC_RELEASE_BUILD_DIR)/$(RELEASE_LIB_FILE)
else # RELEASE_LIB_FILE
${error You must define either PPC_RELEASE_LIB or RELEASE_LIB_FILE}
endif # RELEASE_LIB_FILE
endif # PPC_RELEASE_LIB

$(PPC_RELEASE_LIB): release_ppc

else # MACOSX_LION_OR_LATER
override PPC_RELEASE_LIB=
endif # MACOSX_LION_OR_LATER

release: universal_release

universal_release: $(RELEASE_LIB)

$(X86_RELEASE_LIB): release_i386

$(RELEASE_LIB): $(X86_RELEASE_LIB) $(PPC_RELEASE_LIB) $(RELEASE_BUILD_DIR)
	$(call RUN,lipo -create $(X86_RELEASE_LIB) $(PPC_RELEASE_LIB) -output $@,"  LIPO  $@")

else # !RELEASE_UNIVERSAL

release: $(RELEASE_PREBUILD_TARGET) $(RELEASE_BUILD_DIR) $(RELEASE_LIB)

$(RELEASE_LIB): $(RELEASE_OBJS)
	$(call RUN,$(AR) $(ARFLAGS) $@ $?,"  AR    $@")
	@ranlib $@

endif # !RELEASE_UNIVERSAL
endif # RELEASE_LIB

ifdef PROFILE_LIB
ifdef PROFILE_UNIVERSAL

ifndef X86_PROFILE_LIB
ifdef PROFILE_LIB_FILE
X86_PROFILE_LIB = $(X86_PROFILE_BUILD_DIR)/$(PROFILE_LIB_FILE)
else # PROFILE_LIB_FILE
${error You must define either X86_PROFILE_LIB or PROFILE_LIB_FILE}
endif # PROFILE_LIB_FILE
endif # X86_PROFILE_LIB

ifndef MACOSX_LION_OR_LATER
ifndef PPC_PROFILE_LIB
ifdef PROFILE_LIB_FILE
PPC_PROFILE_LIB = $(PPC_PROFILE_BUILD_DIR)/$(PROFILE_LIB_FILE)
else # PROFILE_LIB_FILE
${error You must define either PPC_PROFILE_LIB or PROFILE_LIB_FILE}
endif # PROFILE_LIB_FILE
endif # PPC_PROFILE_LIB

$(PPC_PROFILE_LIB): profile_ppc

else # MACOSX_LION_OR_LATER
override PPC_PROFILE_LIB=
endif # MACOSX_LION_OR_LATER

profile: universal_profile

universal_profile: $(PROFILE_LIB)

$(X86_PROFILE_LIB): profile_i386

$(PROFILE_LIB): $(X86_PROFILE_LIB) $(PPC_PROFILE_LIB) $(PROFILE_BUILD_DIR)
	$(call RUN,lipo -create $(X86_PROFILE_LIB) $(PPC_PROFILE_LIB) -output $@,"  LIPO  $@")

else # !PROFILE_UNIVERSAL

profile: $(PROFILE_PREBUILD_TARGET) $(PROFILE_BUILD_DIR) $(PROFILE_LIB)

$(PROFILE_LIB): $(PROFILE_OBJS)
	$(call RUN,$(AR) $(ARFLAGS) $@ $?,"  AR    $@")
	ranlib $@

endif # !PROFILE_UNIVERSAL
endif # RELEASE_LIB

# ============================================================================
# Build an application
# ============================================================================

ifdef DEBUG_APP
ifdef DEBUG_UNIVERSAL

ifndef X86_DEBUG_APP
ifdef DEBUG_APP_FILE
X86_DEBUG_APP = $(X86_DEBUG_BUILD_DIR)/$(DEBUG_APP_FILE)
else # DEBUG_APP_FILE
${error You must define either X86_DEBUG_APP or DEBUG_APP_FILE}
endif # DEBUG_APP_FILE
endif # X86_DEBUG_APP

ifndef MACOSX_LION_OR_LATER
ifndef PPC_DEBUG_APP
ifdef DEBUG_APP_FILE
PPC_DEBUG_APP = $(PPC_DEBUG_BUILD_DIR)/$(DEBUG_APP_FILE)
else # DEBUG_APP_FILE
${error You must define either PPC_DEBUG_APP or DEBUG_APP_FILE}
endif # DEBUG_APP_FILE
endif # PPC_DEBUG_APP

$(PPC_DEBUG_APP): debug_ppc

else # MACOSX_LION_OR_LATER
override PPC_DEBUG_APP=
endif # MACOSX_LION_OR_LATER

debug: $(DEBUG_PREBUILD_TARGET) universal_debug

universal_debug: $(DEBUG_APP)

$(X86_DEBUG_APP): debug_i386

$(DEBUG_APP): $(X86_DEBUG_APP) $(PPC_DEBUG_APP) $(DEBUG_BUILD_DIR)
	$(call RUN,lipo -create $(X86_DEBUG_APP) $(PPC_DEBUG_APP) -output $@,"  LIPO  $@")

else # !DEBUG_UNIVERSAL

debug: $(DEBUG_PREBUILD_TARGET) $(DEBUG_BUILD_DIR) $(DEBUG_APP)

$(DEBUG_APP): $(DEBUG_OBJS) $(DEBUG_DEPENDENCIES)
	$(call RUN,$(LD) $(LDFLAGS) -o $@ $(DEBUG_OBJS) $(DEBUG_LIBS) $(SYSTEM_LIBS),"  LINK  $@")

endif # !DEBUG_UNIVERSAL
endif # DEBUG_APP

ifdef RELEASE_APP
ifdef RELEASE_UNIVERSAL

ifndef X86_RELEASE_APP
ifdef RELEASE_APP_FILE
X86_RELEASE_APP = $(X86_RELEASE_BUILD_DIR)/$(RELEASE_APP_FILE)
else # RELEASE_APP_FILE
${error You must define either X86_RELEASE_APP or RELEASE_APP_FILE}
endif # RELEASE_APP_FILE
endif # X86_RELEASE_APP

ifndef MACOSX_LION_OR_LATER
ifndef PPC_RELEASE_APP
ifdef RELEASE_APP_FILE
PPC_RELEASE_APP = $(PPC_RELEASE_BUILD_DIR)/$(RELEASE_APP_FILE)
else # RELEASE_APP_FILE
${error You must define either PPC_RELEASE_APP or RELEASE_APP_FILE}
endif # RELEASE_APP_FILE
endif # PPC_RELEASE_APP

$(PPC_RELEASE_APP): release_ppc

else # MACOSX_LION_OR_LATER
override PPC_RELEASE_APP=
endif # MACOSX_LION_OR_LATER

release: $(RELEASE_PREBUILD_TARGET) universal_release

universal_release: $(RELEASE_APP)

$(X86_RELEASE_APP): release_i386

$(RELEASE_APP): $(X86_RELEASE_APP) $(PPC_RELEASE_APP) $(RELEASE_BUILD_DIR)
	$(call RUN,lipo -create $(X86_RELEASE_APP) $(PPC_RELEASE_APP) -output $@,"  LIPO  $@")

else # !RELEASE_UNIVERSAL

release: $(RELEASE_PREBUILD_TARGET) $(RELEASE_BUILD_DIR) $(RELEASE_APP)

$(RELEASE_APP): $(RELEASE_OBJS) $(RELEASE_DEPENDENCIES)
	$(call RUN,$(LD) $(LDFLAGS) -o $@ $(RELEASE_OBJS) $(RELEASE_LIBS) $(SYSTEM_LIBS),"  LINK  $@")
	@$(STRIP) $@

endif # !RELEASE_UNIVERSAL
endif # RELEASE_APP

ifdef PROFILE_APP
ifdef PROFILE_UNIVERSAL

ifndef X86_PROFILE_APP
ifdef PROFILE_APP_FILE
X86_PROFILE_APP = $(X86_PROFILE_BUILD_DIR)/$(PROFILE_APP_FILE)
else # PROFILE_APP_FILE
${error You must define either X86_PROFILE_APP or PROFILE_APP_FILE}
endif # PROFILE_APP_FILE
endif # X86_PROFILE_APP

ifndef MACOSX_LION_OR_LATER
ifndef PPC_PROFILE_APP
ifdef PROFILE_APP_FILE
PPC_PROFILE_APP = $(PPC_PROFILE_BUILD_DIR)/$(PROFILE_APP_FILE)
else # PROFILE_APP_FILE
${error You must define either PPC_PROFILE_APP or PROFILE_APP_FILE}
endif # PROFILE_APP_FILE
endif # PPC_PROFILE_APP

$(PPC_PROFILE_APP): profile_ppc

else # MACOSX_LION_OR_LATER
override PPC_PROFILE_APP=
endif # MACOSX_LION_OR_LATER

profile: $(PROFILE_PREBUILD_TARGET) universal_profile

universal_profile: $(PROFILE_APP)

$(X86_PROFILE_APP): profile_i386

$(PROFILE_APP): $(X86_PROFILE_APP) $(PPC_PROFILE_APP) $(PROFILE_BUILD_DIR)
	$(call RUN,lipo -create $(X86_PROFILE_APP) $(PPC_PROFILE_APP) -output $@,"  LIPO  $@")

else # !PROFILE_UNIVERSAL

profile: $(PROFILE_PREBUILD_TARGET) $(PROFILE_BUILD_DIR) $(PROFILE_APP)

$(PROFILE_APP): $(PROFILE_OBJS) $(PROFILE_DEPENDENCIES)
	$(call RUN,$(LD) $(LDFLAGS) -pg -o $@ $(PROFILE_OBJS) $(PROFILE_LIBS) $(SYSTEM_LIBS),"  LINK  $@")

endif # !PROFILE_UNIVERSAL
endif # PROFILE_APP

# ============================================================================
# Build a shared library
# ============================================================================

ifdef DEBUG_SO

debug: $(DEBUG_PREBUILD_TARGET) $(DEBUG_BUILD_DIR) $(DEBUG_SO)

$(DEBUG_SO): $(DEBUG_OBJS) $(DEBUG_DEPENDENCIES)
	$(call RUN,$(LD) $(LD_SHARED_OPT) $(LDFLAGS) -o $@ $(DEBUG_OBJS) $(DEBUG_LIBS) $(SYSTEM_LIBS),"  LINK  $@")

endif # DEBUG_SO

ifdef RELEASE_SO

release: $(RELEASE_PREBUILD_TARGET) $(RELEASE_BUILD_DIR) $(RELEASE_SO)

$(RELEASE_SO): $(RELEASE_OBJS) $(RELEASE_DEPENDENCIES)
	$(call RUN,$(LD) $(LD_SHARED_OPT) $(LDFLAGS) -o $@ $(RELEASE_OBJS) $(RELEASE_LIBS) $(SYSTEM_LIBS),"  LINK  $@")

endif # RELEASE_SO

ifdef PROFILE_SO

profile: $(PROFILE_PREBUILD_TARGET) $(PROFILE_BUILD_DIR) $(PROFILE_SO)

$(PROFILE_SO): $(PROFILE_OBJS) $(PROFILE_DEPENDENCIES)
	$(call RUN,$(LD) $(LD_SHARED_OPT) $(LDFLAGS) -pg -o $@ $(PROFILE_OBJS) $(PROFILE_LIBS) $(SYSTEM_LIBS),"  LINK  $@")

endif # PROFILE_SO

# ============================================================================
#
# Components
#
# ============================================================================

.PHONY: slib slib_clean slib_debug slib_profile slib_release

slib: slib_debug slib_release

slib_clean:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" clean,)

slib_debug:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" debug,)

slib_profile:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" profile,)

slib_release:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" release,)

slib_debug_%:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" ARCH=$* debug,)

slib_profile_%:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" ARCH=$* profile,)

slib_release_%:
	$(call RUN,$(MAKE) -C $(SLIB_DIR) V="$(V)" ARCH=$* release,)

.PHONY: jpeg jpeg_clean jpeg_debug jpeg_profile jpeg_release

jpeg: jpeg_debug jpeg_release

ifdef JPEG_DIR

jpeg_clean:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" clean,)

jpeg_debug:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" debug,)

jpeg_profile:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" profile),

jpeg_release:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" release,)

jpeg_debug_%:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" ARCH=$* debug,)

jpeg_profile_%:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" ARCH=$* profile,)

jpeg_release_%:
	$(call RUN,$(MAKE) -C $(JPEG_DIR) V="$(V)" ARCH=$* release,)

else  # JPEG_DIR
jpeg_clean:

jpeg_debug:

jpeg_profile:

jpeg_release:

endif # JPEG_DIR

.PHONY: png png_clean png_debug png_profile png_release

png: png_debug png_release

ifdef PNG_DIR

png_clean:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" clean,)

png_debug:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" debug,)

png_profile:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" profile,)

png_release:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" release,)

png_debug_%:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" ARCH=$* debug,)

png_profile_%:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" ARCH=$* profile,)

png_release_%:
	$(call RUN,$(MAKE) -C $(PNG_DIR) V="$(V)" ARCH=$* release,)

else  # PNG_DIR
png_clean:

png_debug:

png_profile:

png_release:

endif # PNG_DIR

.PHONY: curl_clean curl_debug curl_profile curl_release

ifdef CURL_DIR
curl_clean:
	$(call RUN,$(MAKE) -C $(CURL_LIB_DIR) V="$(V)" clean,)

curl_debug:
	$(call RUN,$(MAKE) -C $(CURL_LIB_DIR) V="$(V)" debug,)

curl_profile:
	$(call RUN,$(MAKE) -C $(CURL_LIB_DIR) V="$(V)" profile,)

curl_release:
	$(call RUN,$(MAKE) -C $(CURL_LIB_DIR) V="$(V)" release,)

else  # CURL_DIR
curl_clean:

curl_debug:

curl_profile:

curl_release:

endif # CURL_DIR

.PHONY: expat_clean expat_debug expat_profile expat_release

ifdef EXPAT_DIR
expat_clean:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" clean,)

expat_debug:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" debug,)

expat_profile:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" profile,)

expat_release:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" release,)

expat_debug_%:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" ARCH=$* debug,)

expat_profile_%:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" ARCH=$* profile,)

expat_release_%:
	$(call RUN,$(MAKE) -C $(EXPAT_DIR) V="$(V)" ARCH=$* release,)

else  # EXPAT_DIR
expat_clean:

expat_debug:

expat_profile:

expat_release:

endif # EXPAT_DIR

ifdef ZLIB_DIR
zlib_clean:
	$(call RUN,$(MAKE) -C $(ZLIB_DIR) V="$(V)" clean,)

zlib_debug:
	$(call RUN,$(MAKE) -C $(ZLIB_DIR) V="$(V)" debug,)

zlib_profile:
	$(call RUN,$(MAKE) -C $(ZLIB_DIR) V="$(V)" profile,)

zlib_release:
	$(call RUN,$(MAKE) -C $(ZLIB_DIR) V="$(V)" release,)
else  # ZLIB_DIR
zlib_clean:

zlib_debug:

zlib_profile:

zlib_release:

endif # ZLIB_DIR

#
# Pattern Rules
#

CPPFLAGS = $(BASE_CPPFLAGS) $(INCLUDES) $(DEFINES)
DEBUG_OPTS = -c $(DEBUG_FLAGS) $(CFLAGS) $(CPPFLAGS) $(DEBUG_DEFINES) 
RELEASE_OPTS = -c $(RELEASE_FLAGS) $(CFLAGS) $(CPPFLAGS) $(RELEASE_DEFINES)
PROFILE_OPTS = -c $(PROFILE_FLAGS) $(CFLAGS) $(CPPFLAGS) $(PROFILE_DEFINES)

ifndef CPLUSPLUSFLAGS
CPLUSPLUSFLAGS = $(CFLAGS)
endif

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR1)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR2)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR3)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR4)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR5)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR6)/%.c
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR1)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR2)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR)/%.m
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(DEBUG_BUILD_DIR)/%.o : $(SRC_DIR)/%.mm
	$(call RUN,$(CC) $(DEBUG_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR1)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR2)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR3)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR4)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR5)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR6)/%.c
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR1)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR2)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR)/%.m
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(RELEASE_BUILD_DIR)/%.o : $(SRC_DIR)/%.mm
	$(call RUN,$(CC) $(RELEASE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR1)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR2)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR3)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR4)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR5)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR6)/%.c
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR1)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR2)/%.cpp
	$(call RUN,$(CC) $(CPLUSPLUSFLAGS) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR)/%.m
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

$(PROFILE_BUILD_DIR)/%.o : $(SRC_DIR)/%.mm
	$(call RUN,$(CC) $(PROFILE_OPTS) $< -o $@,"  CC    $@")

#
# Cancel some implicit rules
#

%: %.o

#
# Special target (dependency rules)
#

ifndef DEPENDS
DEPENDS = Makefile.dep
endif # DEPENDS

ifndef DEPENDS_DEPS
DEPENDS_DEPS = $(SRC_FILES)
endif # DEPENDS_DEPS

#
# sed script for filtering the dependency file. Makes the dependency file
# much smaller, but I'm not sure whether that makes build any faster
#

DEPENDS_SED += \
-e 's/'`echo $(SLIB_DIR)|sed 's/[\/\.]/\\\\&/g'`'/$$(SLIB_DIR)/g'

ifdef JPEG_DIR
DEPENDS_SED += \
-e 's/'`echo $(JPEG_DIR)|sed 's/[\/\.]/\\\\&/g'`'/$$(JPEG_DIR)/g'
endif

ifdef PNG_DIR
DEPENDS_SED += \
-e 's/'`echo $(PNG_DIR)|sed 's/[\/\.]/\\\\&/g'`'/$$(PNG_DIR)/g'
endif

ifdef CURL_DIR
DEPENDS_SED += \
-e 's/'`echo $(CURL_DIR)|sed 's/[\/\.]/\\\\&/g'`'/$$(CURL_DIR)/g'
endif

ifdef EXPAT_DIR
DEPENDS_SED += \
-e 's/'`echo $(EXPAT_DIR)|sed 's/[\/\.]/\\\\&/g'`'/$$(EXPAT_DIR)/g'
endif

ifdef ZLIB_DIR
DEPENDS_SED += \
-e 's/'`echo $(ZLIB_DIR)|sed 's/[\/\.]/\\\\&/g'`'/$$(ZLIB_DIR)/g'
endif

depends nothing:

nodepend:
	-rm -fr $(DEPENDS)

veryclean: clean nodepend

$(DEPENDS): $(DEPENDS_DEPS)
	@echo "$(MAKE): Updating dependencies"
	@echo "# -*- Mode: makefile -*-" > $@
	@echo "# This file is generated automatically." >> $@
	@echo "# Run 'make veryclean' and  'make' to update it." >> $@
	@$(CC) -MM $(CPPFLAGS) $(DEBUG_DEFINES) $(SRC_FILES) | sed -e "s/^.*:/\\$$\(DEBUG_BUILD_DIR)\\/&/g" $(DEPENDS_SED) >> $@
	@$(CC) -MM $(CPPFLAGS) $(RELEASE_DEFINES) $(SRC_FILES) | sed -e "s/^.*:/\\$$\(RELEASE_BUILD_DIR)\\/&/g" $(DEPENDS_SED) >> $@
	@$(CC) -MM $(CPPFLAGS) $(PROFILE_DEFINES) $(SRC_FILES) | sed -e "s/^.*:/\\$$\(PROFILE_BUILD_DIR)\\/&/g" $(DEPENDS_SED) >> $@
