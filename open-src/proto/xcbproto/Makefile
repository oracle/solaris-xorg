###############################################################################
#
# xcbproto Makefile
#
# Copyright (c) 2010, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xcb-proto

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/libxcb
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=System/X11
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.11

# Checksums for upstream tarball
TARBALL_MD5  = 6bf2797445dc6d43e9e4707c082eff9c
TARBALL_SHA1 = 608bd60663e223464d38acec0183ddb827776401
TARBALL_SHA256 = b4aceee6502a0ce45fc39b33c541a2df4715d00b72e660ebe8c5bb444771e32e

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes
MODULE_DESC = X Window System protocol descriptions used by xcb

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += xia.patch,-p1 \
		  py-compile.patch,-p1

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

### Include common rulesets
include ../Makefile.inc

