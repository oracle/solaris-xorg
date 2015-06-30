###############################################################################
#
# xcb-util-keysyms - Library of xcb utility functions for handling X keysyms
#
# Copyright (c) 2011, 2015, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xcb-util-keysyms

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/xcb-util
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=0.4.0

# Checksums for upstream tarball
TARBALL_MD5  = 1022293083eec9e62d5659261c29e367
TARBALL_SHA1 = ff02ee8ac22c53848af50c0a6a6b00feb002c1cb
TARBALL_SHA256 = 0ef8490ff1dede52b7de533158547f8b454b241aa3e4dcca369507f66f216dd9

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
MODULE_DESC = the xcb-util-keysyms library of xcb utility functions for handling X keysyms
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes
ORACLE_TPNO = 22892
ORACLE_TPNO_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=xcb-keysyms

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted

# pkg-config files to update
FIX_PC_FILES=keysyms/xcb-keysyms.pc.in

include ../Makefile.inc
