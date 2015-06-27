###############################################################################
#
# xfsinfo - Utility to display information about an X font server
#
# Copyright (c) 2008, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xfsinfo

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/xfs/xfs-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xfsinfo X font server query utility

# ARC cases that covered this module
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES = PSARC/2015/221

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Checksums for upstream tarball
TARBALL_MD5  = 64fc7318602b4b2751e2bd7c33bf061c
TARBALL_SHA1 = 7513e16ba5c8fef31aeaff4a42bf131859e75ee0
TARBALL_SHA256 = d38f4b6db4038f52ba1a09123ef497993194288454c0c13d16debe246f120d8f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xfsinfo

include ../Makefile.inc

# Special compatibility link from /usr/X11/bin/fsinfo to /usr/bin/xfsinfo
install: $(X11_BINCOMPAT_DIR)/fsinfo

$(X11_BINCOMPAT_DIR)/fsinfo: $(X11_BINCOMPAT_DIR)
	@rm -f $@
	ln -s ../../bin/xfsinfo $@
