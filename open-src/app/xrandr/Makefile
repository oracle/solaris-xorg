###############################################################################
#
# xrandr 1.x Makefile
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
MODULE_NAME=xrandr

# Version number (used in path names)
MODULE_VERSION=1.3.2

# Checksums for upstream tarball
TARBALL_MD5  = 2cb19bb1c19ccf77c40032b03dbe06f0
TARBALL_SHA1 = 0e49b0a0889ae8a590452c6cd0d60a2253a8d940

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# PSARC/2005/189 X client support for XFree86 extensions
MODULE_ARC_CASES = PSARC/2005/189
# PSARC/2007/430 X Resize, Rotate and Reflect (RandR) 1.2
MODULE_ARC_CASES += PSARC/2007/430
# PSARC/2009/269 X Resize, Rotate and Reflect (RandR) 1.3
MODULE_ARC_CASES += PSARC/2009/269

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xrandr

include ../Makefile.inc

# Remove xkeystone utility that requires nickle interpreter we don't ship yet
remove_xkeystone: $(INSTALL_RULES)
	-rm -f $(PROTODIR)$(MODULE_PREFIX)/bin/xkeystone

install: remove_xkeystone
