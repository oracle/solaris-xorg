###############################################################################
#
# xfd - simple X11/Xft font display application
#
# Copyright (c) 2008, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xfd

# Version number (used in path names)
MODULE_VERSION=1.1.2

# Checksums for upstream tarball
TARBALL_MD5   = f19de745d181f5547ed59d6d9ed7a588
TARBALL_SHA1  = cac5ca0f5fb6044840b15cf6fb9c541bd4fbce4a
TARBALL_SHA256= c1a32222fbb891e46f9a82ebab005bc5e54ce006a2586195ec9ea7ab308e2759

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# ARC cases that covered this module
# PSARC/2008/137  xfd -fa 
MODULE_ARC_CASES = PSARC/2008/137

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xfd.man
MODULE_STABILITY=Committed

# Allow linker to drop direct dependency on libfreetype
MODULE_LDFLAGS = $(ZDISCARD_UNUSED_DEP)

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_po

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xfd

include ../Makefile.inc

install_po: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
	ginstall -m 0444 $(BUILD_DIR)/xfd.po $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
