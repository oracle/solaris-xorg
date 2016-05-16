###############################################################################
#
# xkbcomp - XKB keyboard layout parser/compiler
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
MODULE_NAME=xkbcomp

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/keyboard/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.3.1

# Checksums for upstream tarball
TARBALL_MD5   = a4d8353daf6cb0a9c47379b7413c42c6
TARBALL_SHA1  = 0295b57a4c4087b7a4d8d0ce6764039c24efb98c
TARBALL_SHA256= 0304dc9e0d4ac10831a9ef5d5419722375ddbc3eac3ff4413094d57bc1f1923d

# ARC cases that covered this module
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES = PSARC/2015/221

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xkbcomp.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xkbcomp

# Also need an extra link for the pkgconfig file
MODULE_ADD_INSTALL_TARGETS = install_extra_link

# Silence warnings from unnecessary inclusion of Xalloca.h in parseutils.c
MODULE_CPPFLAGS = -DNO_ALLOCA

include ../Makefile.inc

EXTRA_LINK_DIR = $(PROTODIR)$(PKGCONFIG_DIR_32)
EXTRA_LINK = $(EXTRA_LINK_DIR)/xkbcomp.pc

$(EXTRA_LINK_DIR):
	mkdir -p $@

$(EXTRA_LINK): $(EXTRA_LINK_DIR)
	ln -s ../$(SUBDIR64)/pkgconfig/xkbcomp.pc $@

install_extra_link: $(EXTRA_LINK)
