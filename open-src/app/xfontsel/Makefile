###############################################################################
#
# xfontsel - simple X11 font selection/display application
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.0.4

# Checksums for upstream tarball
TARBALL_MD5   = 3f7e8ba9e34589a07d190575ae6c86cf
TARBALL_SHA1  = 8e3da9d050ad71531bfeb98d5f1071c7b392b3bd
TARBALL_SHA256= 25491e9b7762607299f8a7099c97452d9b83e92266f8b0579b380ca01e5a5f7e

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

install_po:
	mkdir -p $(PROTODIR)$(X11_LC_MESSAGES_DIR)
	cp -f xfontsel.po $(PROTODIR)$(X11_LC_MESSAGES_DIR)
