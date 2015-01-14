# -*- Mode: makefile -*-
#
# $Id: Rules.mak 8624 2010-02-01 19:43:04Z monich $
#
# Copyright (C) 2003-2010 Epiphan Systems Inc. All rights reserved.
#
# libv2u build rules
#

v2u_clean:
	$(MAKE) -C $(V2ULIB_DIR) clean

v2u_debug:
	$(MAKE) -C $(V2ULIB_DIR) debug

v2u_profile:
	$(MAKE) -C $(V2ULIB_DIR) profile

v2u_release:
	$(MAKE) -C $(V2ULIB_DIR) release

v2u_debug_%:
	$(MAKE) -C $(V2ULIB_DIR) ARCH=$* debug

v2u_profile_%:
	$(MAKE) -C $(V2ULIB_DIR) ARCH=$* profile

v2u_release_%:
	$(MAKE) -C $(V2ULIB_DIR) ARCH=$* release
