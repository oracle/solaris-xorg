###############################################################################
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xorg-sgml-doctools

# pkg(5) name we deliver the files in (override default)
MODULE_PKGTAIL=xorg-docs
MODULE_PKGTAIL_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.8

# Checksums for upstream tarball
TARBALL_MD5  = c6ad2cca27c24115e3e74a34beb9f4d5
TARBALL_SHA1 = 524c994832508d3097c1a42bd3623251c9f87d77

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# doctools pkg-config file needs to have sgml path in share adjusted
# so that xorg-docs and other parts of build can find it
FIX_PC_FILES = xorg-sgml-doctools.pc.in
MODULE_ADD_UNINSTALLED_PC_OPS = -e 's|sgmlrootdir=|sgmlrootdir=$(PROTODIR)|g'

# doctools pkg-config file is architecture independent, so install in share
MODULE_PKGCONFIG_DIR=$(PKGCONFIG_DIR_SHARE)
MODULE_PKGCONFIG_DIR_SET=yes

### Include common rulesets
include ../Makefile.inc
