###############################################################################
#
# xhost - utility for mananging X server access control lists
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
MODULE_NAME=xhost

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xhost utility for mananging X server access control lists

# Version number (used in path names)
MODULE_VERSION=1.0.7

# Checksums for upstream tarball
TARBALL_MD5   = f5d490738b148cb7f2fe760f40f92516
TARBALL_SHA1  = 8abfb0554e14a074f0dbfdda4919880e088545cb
TARBALL_SHA256= 93e619ee15471f576cfb30c663e18f5bc70aca577a63d2c2c03f006a7837c29a

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += xhost.patch,-p1
SOURCE_PATCHES += manual.patch,-p1

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xhost.man
MODULE_STABILITY=Committed
# The only instance in the current manpage is getaddrinfo(__libmansuffix__),
# which is in section 3c on Solaris 11.4 & later.
MODULE_SUNTOUCH_MAN_FLAGS += -r '{__libmansuffix__, 3c}'

# ARC cases that covered this module
# PSARC/2001/108 X11 Over IPv6
MODULE_ARC_CASES = PSARC/2001/108

MODULE_CONFIG_OPTS = --disable-secure-rpc

MODULE_ADD_INSTALL_TARGETS = install_po

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xhost

include ../Makefile.inc

install_po:
	mkdir -p $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
	ginstall -m 0444 xhost.po $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
