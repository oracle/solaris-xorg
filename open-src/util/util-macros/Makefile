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

# Package name used in tarballs
MODULE_NAME=util-macros

# Version number (used in path names)
MODULE_VERSION=1.8.0

# Checksums for upstream tarball
TARBALL_MD5  = 0c9334201b0ed5ec187faab0bb713329
TARBALL_SHA1 = cbe9920712789ecd25386956649db0d666642f0f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Additional rules for generating sources
MODULE_ADD_SOURCE_TARGETS=source_fix_pkgname

# Need to autoreconf after running source_fix_pkgname
AUTORECONF=yes

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_libtool_fix

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

# Libtool versions before 2.2.2 fail to work with LD=cc in 64 bit builds, 
# so install fixed libtool m4 macros here as well, so that modules that
# run autoreconf get the fixed sources
install_libtool_fix: $(TOOLS_ACLOCAL_DIR)/libtool.m4

$(TOOLS_ACLOCAL_DIR)/libtool.m4: $(TOOLS_ACLOCAL_DIR) libtool-64.patch
	-rm -f $(TOOLS_ACLOCAL_DIR)/libtool.m4
	SYSTEM_ACLOCAL_DIR=$$($(ACLOCAL_PATH) --print-ac-dir) ; \
	cp $${SYSTEM_ACLOCAL_DIR}/libtool.m4 \
		$(TOOLS_ACLOCAL_DIR)/libtool.m4
	gpatch -d $(TOOLS_ACLOCAL_DIR) < libtool-64.patch

# autoreconf fails if the aclocal include directories don't exist yet
$(AUTOCONF_TARGET): $(TOOLS_ACLOCAL_DIR) $(PROTODIR)$(X11_ACLOCAL_DIR)

$(TOOLS_ACLOCAL_DIR) $(PROTODIR)$(X11_ACLOCAL_DIR):
	mkdir -p $@
