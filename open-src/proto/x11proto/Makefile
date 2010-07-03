###############################################################################
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xproto

# Version number (used in path names)
MODULE_VERSION=7.0.16

# Checksums for upstream tarball
TARBALL_MD5  = 75c9edff1f3823e5ab6bb9e66821a901
TARBALL_SHA1 = 0eda5588d42f2c810d8491da44fe84a4093e12bc

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	X.h-patch

# ARC cases that covered this module
# PSARC/2001/108 X11 Over IPv6
MODULE_ARC_CASES = PSARC/2001/108

# Additional install targets beyond the defaults
MODULE_ADD_INSTALL_TARGETS = install_x11_compat_link

### Include common rulesets
include ../Makefile.inc

# Special compatibility link from /usr/X11/include/X11 to /usr/include/X11
X11_INC_COMPAT_LINK = $(PROTODIR)/usr/X11/include/X11

install_x11_compat_link: $(X11_INC_COMPAT_LINK)

$(X11_INC_COMPAT_LINK):
	mkdir -p $(@D)
	ln -s ../../include/X11 $@
