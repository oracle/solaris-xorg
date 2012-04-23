###############################################################################
#
# Copyright (c) 2007, 2012, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=util-macros

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=developer/build/autoconf/xorg-macros
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.17

# Checksums for upstream tarball
TARBALL_MD5   = 4f41667e1bf4938bb2b24fa09d517f77
TARBALL_SHA1  = c424a09fa6f628e24eff74496acebef13e8093b9
TARBALL_SHA256= 7d79edde72fc17dcdc56cfbe2f8ff384d1bffd1d8c58c0fd25970f5b2ccb75ed

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Additional rules for generating sources
MODULE_ADD_SOURCE_TARGETS=source_fix_pkgname

# Need to autoreconf after running source_fix_pkgname
AUTORECONF=yes

# xorg-macro pkg-config files are architecture independent, so install in share
MODTYPE_PKGCONFIG_DIR=$(PKGCONFIG_DIR_SHARE)
MODTYPE_PKGCONFIG_DIR_SET=yes

### Include common rulesets
include ../Makefile.inc

# Install additional data files under xorg-macros instead of the
# too-generic "util-macros"
source_fix_pkgname: $(PATCHED_TARGET)
	$(PERL) -i.orig -p -e 's{util-macros}{xorg-macros}g' \
		$(SOURCE_DIR)/configure.ac

# autoreconf fails if the aclocal include directories don't exist yet
$(AUTOCONF_TARGET): $(PROTODIR)$(X11_ACLOCAL_DIR)

$(PROTODIR)$(X11_ACLOCAL_DIR):
	mkdir -p $@
