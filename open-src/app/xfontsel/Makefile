###############################################################################
#
# xfontsel - simple X11 font selection/display application
#
# Copyright (c) 2008, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xfontsel

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Checksums for upstream tarball
TARBALL_MD5   = a40302b88c599b63e3d3d412e02871e6
TARBALL_SHA1  = fbd8fa8a1c87d8da2e71def964e97bca1d4c4adf
TARBALL_SHA256= 170d8550b96fb47b04db6181b947e6f6b8b97f4df5d691c351a278480cc931bf

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES  = xfontsel.patch,-p1

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xfontsel.man
MODULE_STABILITY=Committed

MODULE_CPPFLAGS= -DXLOCALEDIR=\"$(XLOCALEDIR)\"

MODULE_ADD_INSTALL_TARGETS = install_po

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xfontsel

include ../Makefile.inc

install_po: $(SOURCE_TARGETS) $(BUILD_DIR)
	mkdir -p $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
	(cd $(SOURCE_DIR) ; xgettext -n -d xfontsel -p ../$(BUILD_DIR) *.c)
	ginstall -m 0444 $(BUILD_DIR)/xfontsel.po $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
