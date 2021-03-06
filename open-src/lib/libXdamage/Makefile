###############################################################################
#
# Xlib-based client library for the X Damage extension
#
# Copyright (c) 2007, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXdamage

# Version number (used in path names)
MODULE_VERSION=1.1.4

# Checksums for upstream tarball
TARBALL_MD5  = 0cf292de2a9fa2e9a939aefde68fd34f
TARBALL_SHA1 = c3fc0f4b02dce2239bf46c82a5f06b06585720ae

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name
LIBNAME=Xdamage

# ARC cases that covered this module
# LSARC 2003/506 X Damage Extension
MODULE_ARC_CASES = LSARC/2003/506
# LSARC/2007/421 X Damage Extension Version 1.1
MODULE_ARC_CASES += LSARC/2007/421

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xdamage

# pkg-config files that need to have -R added
FIX_PC_FILES=xdamage.pc.in

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# configure includes xfixes in PKG_CHECK_MODULES since libXfixes headers & 
# macros are used, but currently no functions are actually called, so we can
# ignore it when linking
MODULE_LD_OPTIONS += $(ZDISCARD_UNUSED_DEP)

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXdamage.so libXdamage.so.1

include ../Makefile.inc
