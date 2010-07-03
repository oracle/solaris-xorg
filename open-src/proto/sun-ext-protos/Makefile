###############################################################################
#
# Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#
#
# Definitions of Solaris-specific X Server extensions

MODULE_NAME=sun-ext-protos

# No upstream sources for these files, since they're Solaris-specific
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes

# ARC cases that covered this module
# PSARC 1994/377 SUN_SME extension to Xsun
MODULE_ARC_CASES = PSARC/1994/377
# PSARC 1999/188 FBPM (Frame Buffer Power Management) Extension
MODULE_ARC_CASES += PSARC/1999/188
# PSARC 1999/545 Xinerama addition to DGA Extension
MODULE_ARC_CASES += PSARC/1999/545

# Don't need to configure or build these or install pkg-config files for them
SOURCE_TARGETS=
SOURCE_TARGETS_SET=yes
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes
BUILD_TARGETS=
BUILD_TARGETS_SET=yes
INSTALL_TARGETS=install_sun_headers
INSTALL_TARGETS_SET=yes
FIX_PC_FILES=
FIX_PC_FILES_SET=yes

### Include common rulesets
include ../Makefile.inc

DESTDIR=$(PROTODIR)
INSTDIR=$(DESTDIR)$(X11_INCLUDES_DIR)/X11/extensions

install_sun_headers:
	mkdir -p $(INSTDIR)
	cp -pf include/*h $(INSTDIR)/
