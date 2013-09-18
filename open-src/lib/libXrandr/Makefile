###############################################################################
#
# Xrandr 1.x Makefile
#
# Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.4.2

# Checksums for upstream tarball
TARBALL_MD5   = 210ed9499a3d9c96e3a221629b7d39b0
TARBALL_SHA1  = 00865ccefe1040e4d9866d878556710cdc4b2ae6
TARBALL_SHA256= caa7b31ac769be51a532343c65376f1d4df3f307afaed58e34fb5e82e8b825ad

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

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
