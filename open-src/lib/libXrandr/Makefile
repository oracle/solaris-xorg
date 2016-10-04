###############################################################################
#
# Xrandr 1.x Makefile
#
# Copyright (c) 2006, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXrandr

# Version number (used in path names)
MODULE_VERSION=1.5.0

# Checksums for upstream tarball
TARBALL_MD5   = 309762867e41c6fd813da880d8a1bc93
TARBALL_SHA1  = 79538220e567318c6fb3c5b9a0075184ad6e001f
TARBALL_SHA256= 6f864959b7fc35db11754b270d71106ef5b5cf363426aa58589cb8ac8266de58

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += CVE-2016-7947+CVE-2016-7948.patch,-p1

# Library name
LIBNAME=Xrandr

# ARC cases that covered this module
# PSARC/2004/187 Xorg Server for Solaris
MODULE_ARC_CASES = PSARC/2004/187
# PSARC/2007/430 X Resize, Rotate and Reflect (RandR) 1.2
MODULE_ARC_CASES += PSARC/2007/430
# PSARC/2009/269 X Resize, Rotate and Reflect (RandR) 1.3
MODULE_ARC_CASES += PSARC/2009/269
# PSARC/2012/318 X Resize, Rotate and Reflect (RandR) 1.4
MODULE_ARC_CASES += PSARC/2012/318
# PSARC/2016/252 X Resize, Rotate and Reflect (RandR) 1.5
MODULE_ARC_CASES += PSARC/2016/252

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xrandr

# pkg-config files that need to have -R added
FIX_PC_FILES=xrandr.pc.in

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXrandr.so libXrandr.so.2

include ../Makefile.inc
