###############################################################################
#
# xload 1.x Makefile
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xload

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=diagnostic/xload
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=Applications/System Utilities
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.2

# Checksums for upstream tarball
TARBALL_MD5  = b41ed6b4bcfc9897366c27a94d2bf150
TARBALL_SHA1 = cbbbf23531e0d781b556f5864cc3b83874f786d2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = patch.xload


# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Committed

# Specify which cpp to use instead of depending on builder's $PATH
MODULE_CONFIG_OPTS = --with-cpp=/usr/lib/cpp

MODULE_ADD_INSTALL_TARGETS=install_po

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xload

include ../Makefile.inc

install_po:
	mkdir -p $(PROTODIR)$(X11_LC_MESSAGES_DIR)
	cp -f xload.po $(PROTODIR)$(X11_LC_MESSAGES_DIR)

