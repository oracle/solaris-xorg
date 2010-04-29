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
MODULE_VERSION=1.3.0

# Checksums for upstream tarball
TARBALL_MD5  = 0837fe88af7d692308b25815457a3288
TARBALL_SHA1 = 2c6396976af20bb0c7b3b27b229876fe67c6b9dc

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_libtool_fix

### Include common rulesets
include ../Makefile.inc

# Libtool versions before 2.2.2 fail to work with LD=cc in 64 bit builds, 
# so install fixed libtool m4 macros here as well, so that modules that
# run autoreconf get the fixed sources
install_libtool_fix:
	mkdir -p $(PROTODIR)$(X11_ACLOCAL_DIR)/
	-rm -f $(PROTODIR)$(X11_ACLOCAL_DIR)/libtool.m4
	SYSTEM_ACLOCAL_DIR=$$($(ACLOCAL_PATH) --print-ac-dir) ; \
	cp $${SYSTEM_ACLOCAL_DIR}/libtool.m4 \
		$(PROTODIR)$(X11_ACLOCAL_DIR)/libtool.m4
	gpatch -d $(PROTODIR)$(X11_ACLOCAL_DIR) < libtool-64.patch
