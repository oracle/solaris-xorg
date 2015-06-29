###############################################################################
#
# Copyright (c) 2007, 2015, Oracle and/or its affiliates. All rights reserved.
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

# Package metadata
GIT_URL     = $(XORG_GIT_URL)/util/macros
GIT_URL_SET = yes

# Version number (used in path names)
MODULE_VERSION=1.19.0

# Checksums for upstream tarball
TARBALL_MD5   = 1cf984125e75f8204938d998a8b6c1e1
TARBALL_SHA1  = 00cfc636694000112924198e6b9e4d72f1601338
TARBALL_SHA256= 2835b11829ee634e19fa56517b4cfc52ef39acea0cd82e15f68096e27cbed0ba

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
$(PATCHED_TARGET) $(AUTOCONF_TARGET): $(PROTODIR)$(X11_ACLOCAL_DIR)

$(PROTODIR)$(X11_ACLOCAL_DIR):
	mkdir -p $@
