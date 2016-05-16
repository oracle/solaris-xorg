###############################################################################
#
# Copyright (c) 2006, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=7.0.28

# Checksums for upstream tarball
TARBALL_MD5   = 3ce2f230c5d8fa929f326ad1f0fa40a8
TARBALL_SHA1  = d62c43e1b3619ab85732e0113eaa2104920730ac
TARBALL_SHA256= 29e85568d1f68ceef8a2c081dad9bc0e5500a53cfffde24b564dc43d46ddf6ca

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	X.h-patch,-p1

# ARC cases that covered this module
# PSARC/2001/108 X11 Over IPv6
MODULE_ARC_CASES = PSARC/2001/108

# Package metadata
MODULE_DESC = headers & documentation for the X11 protocol

### Include common rulesets
include ../Makefile.inc

# Special compatibility link from /usr/X11/include/X11 to /usr/include/X11
X11_INC_COMPAT_LINK = $(PROTODIR)/usr/X11/include/X11

install_X11_compat_links: $(X11_INC_COMPAT_LINK)

$(X11_INC_COMPAT_LINK):
	mkdir -p $(@D)
	ln -s ../../include/X11 $@
