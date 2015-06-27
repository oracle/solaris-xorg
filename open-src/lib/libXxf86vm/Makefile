###############################################################################
#
# libXxf86vm - Xlib-based client library for XFree86-VidMode extension protocol
#
# Copyright (c) 2006, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXxf86vm

# Version number (used in path names)
MODULE_VERSION=1.1.4

# Checksums for upstream tarball
TARBALL_MD5   = 298b8fff82df17304dfdb5fe4066fe3a
TARBALL_SHA1  = 78ef18ca2be157131899e01c62679e247409a13f
TARBALL_SHA256= afee27f93c5f31c0ad582852c0fb36d50e4de7cd585fcf655e278a633d85cd57

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Library name
LIBNAME=Xxf86vm

# ARC cases that covered this module
# PSARC/2005/189 X client support for XFree86 extensions
MODULE_ARC_CASES = PSARC/2005/189

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xxf86vm

# pkg-config files that need to have -R added
FIX_PC_FILES=xxf86vm.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXxf86vm.so libXxf86vm.so.1

include ../Makefile.inc
