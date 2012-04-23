###############################################################################
#
# libXi Makefile
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXi

# Version number (used in path names)
MODULE_VERSION=1.6.0

# Checksums for upstream tarball
TARBALL_MD5   = 2e7f84711035e672c34549eac5660a0f
TARBALL_SHA1  = b7edf48f93e8abd13ca688fa7f597452c4b74346
TARBALL_SHA256= b2fc65a24a269405c5bbe27152e97ed4c5987ca728b9d7e7576e4c9543c4a7af

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = solaris-abi.patch,-p1
SOURCE_PATCHES += man-fixme.patch,-p1 shadow-man-pages.patch,-p1

# Need to regen Makefile.in after man-fixme.patch changes Makefile.am
AUTORECONF=yes

# Library name (used for specfiles/mapfiles)
LIBNAME=Xi

# ARC cases that covered this module
# PSARC/1992/172  X Input Extensions
MODULE_ARC_CASES = PSARC/1992/172
# PSARC/2009/303  X Input Extension 1.5
MODULE_ARC_CASES += PSARC/2009/303
# PSARC/2009/678  X Input Extension version 2.0 (XI2)
MODULE_ARC_CASES += PSARC/2009/678

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3xi

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xi.pc.in

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
MODULE_MAKEFLAGS=-j 1

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXi.so libXi.so.5

include ../Makefile.inc
