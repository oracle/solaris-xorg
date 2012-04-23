###############################################################################
#
# Copyright (c) 2006, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=7.0.23

# Checksums for upstream tarball
TARBALL_MD5   = d4d241a4849167e4e694fe73371c328c
TARBALL_SHA1  = 5d7f00d1dbe6cf8e725841ef663f0ee2491ba5b2
TARBALL_SHA256= ade04a0949ebe4e3ef34bb2183b1ae8e08f6f9c7571729c9db38212742ac939e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	X.h-patch,-p1

# ARC cases that covered this module
# PSARC/2001/108 X11 Over IPv6
MODULE_ARC_CASES = PSARC/2001/108

### Include common rulesets
include ../Makefile.inc

# Special compatibility link from /usr/X11/include/X11 to /usr/include/X11
X11_INC_COMPAT_LINK = $(PROTODIR)/usr/X11/include/X11

install_X11_compat_links: $(X11_INC_COMPAT_LINK)

$(X11_INC_COMPAT_LINK):
	mkdir -p $(@D)
	ln -s ../../include/X11 $@
