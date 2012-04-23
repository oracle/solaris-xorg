###############################################################################
#
# Xlib-based client library for common X11 protocol extensions
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
MODULE_NAME=libXext

# Version number (used in path names)
MODULE_VERSION=1.3.1

# Checksums for upstream tarball
TARBALL_MD5   = 71251a22bc47068d60a95f50ed2ec3cf
TARBALL_SHA1  = 764ac472ae19a0faade193717a9e0938d3430aaa
TARBALL_SHA256= 56229c617eb7bfd6dec40d2805bc4dfb883dfe80f130d99b9a2beb632165e859

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES  = shm.patch,-p1
SOURCE_PATCHES += sun-exts.patch,-p1

# Library name (used for specfiles/mapfiles)
LIBNAME=Xext

# ARC cases that covered this module
# PSARC 1999/188 FBPM (Frame Buffer Power Management) Extension
MODULE_ARC_CASES = PSARC/1999/188
# PSARC/2005/328 Xinerama Protocol Compatibility
MODULE_ARC_CASES += PSARC/2005/328
# PSARC/2007/500 X11 MIT-SHM Extension across zones
MODULE_ARC_CASES += PSARC/2007/500
# PSARC/2008/194 X11R7 update to libXext, libXevie & libXss
MODULE_ARC_CASES += PSARC/2008/194
# PSARC/2009/293 X Generic Event Extension (XGE), Version 1.0
MODULE_ARC_CASES += PSARC/2009/293
# PSARC/2011/213 X Synchronization Extension version 3.1
MODULE_ARC_CASES += PSARC/2011/213

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3xext

# pkg-config files that need to have -R added
FIX_PC_FILES=xext.pc.in

# Enable Solaris-specific code
MODULE_CPPFLAGS = -DSUNSOFT

# Link with version mapfile and libraries that contain sources that used
# to be in libXext
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXext.so libXext.so.0

include ../Makefile.inc


