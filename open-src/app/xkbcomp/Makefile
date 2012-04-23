###############################################################################
#
# xkbcomp - XKB keyboard layout parser/compiler
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
MODULE_NAME=xkbcomp

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/keyboard/$(MODULE_NAME)
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.2.4

# Checksums for upstream tarball
TARBALL_MD5   = a0fc1ac3fc4fe479ade09674347c5aa0
TARBALL_SHA1  = 695d2f2824884d9e54aa1419282ae167fdea163b
TARBALL_SHA256= 91d0c9ab445d21dfe1892dbae5ae5264f39bae68223dd092ffc547c9450b5a2d

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xkbcomp.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xkbcomp

# Also need an extra link for the pkgconfig file
MODULE_ADD_INSTALL_TARGETS = install_extra_link

include ../Makefile.inc

EXTRA_LINK_DIR = $(PROTODIR)$(PKGCONFIG_DIR_32)
EXTRA_LINK = $(EXTRA_LINK_DIR)/xkbcomp.pc

$(EXTRA_LINK_DIR):
	mkdir -p $@

$(EXTRA_LINK): $(EXTRA_LINK_DIR)
	ln -s ../$(SUBDIR64)/pkgconfig/xkbcomp.pc $@

install_extra_link: $(EXTRA_LINK)
