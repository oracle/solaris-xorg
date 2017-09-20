###############################################################################
#
# xcb-util-cursor - XCB cursor utility library
#
# Copyright (c) 2015, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xcb-util-cursor

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/xcb-util
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=0.1.3

# Checksums for upstream tarball
TARBALL_MD5 = 6ac3b17cba51aaaa36ba035a53527214
TARBALL_SHA1 = 26562eb6d4151307f7b6a53453d360ecfc0563ac 
TARBALL_SHA256 = 05a10a0706a1a789a078be297b5fb663f66a71fb7f7f1b99658264c35926394f

# ARC cases that covered this module
# PSARC/2015/251 Xorg server 1.17
MODULE_ARC_CASES = PSARC/2015/251

# Package metadata
MODULE_DESC = the xcb-util-cursor is a port of libXcursor
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes
ORACLE_TPNO = 34676
ORACLE_TPNO_SET = yes

MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP)  

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=xcbutil

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted

# pkg-config files to update
FIX_PC_FILES=cursor/xcb-cursor.pc.in

include ../Makefile.inc
