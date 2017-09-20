###############################################################################
#
# libXvMC - Xlib-based client library for X-Video Motion Compensation extension
#
# Copyright (c) 2006, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXvMC

# Version number (used in path names)
MODULE_VERSION=1.0.10

# Checksums for upstream tarball
TARBALL_MD5   = 4cbe1c1def7a5e1b0ed5fce8e512f4c6
TARBALL_SHA1  = 8c50ee4a43aff84d807da2122ec6b0d8e3ce4635
TARBALL_SHA256= e501a079b5dfaef0897c56152770c77e05e362065cec58910289aa567277ee2e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += 

# Library name
LIBNAME=XvMC

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xvMC

# pkg-config files that need to have -R added
FIX_PC_FILES=xvmc.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXvMC.so libXvMC.so.1 libXvMCW.so libXvMCW.so.1

# configure passes "xv" to PKG_CHECK_MODULES since libXv headers are needed,
# but we don't need to link with it, so ignore it at link time.
MODULE_LDFLAGS = $(ZDISCARD_UNUSED_DEP)

include ../Makefile.inc
