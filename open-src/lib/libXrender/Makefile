###############################################################################
#
# Xrender 1.x Makefile
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXrender

# Version number (used in path names)
MODULE_VERSION=0.9.6

# Checksums for upstream tarball
TARBALL_MD5  = 3b3b7d076c2384b6c600c0b5f4ba971f
TARBALL_SHA1 = 65bd96dc17da6b064f95109da02d9c9d14cb06dc

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name
LIBNAME=Xrender

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-clientlibs
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xrender

# Environment variable options to GNU autoconf configure script
MODULE_CFLAGS=-I../include

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers



# pkg-config files that need to have -R added
FIX_PC_FILES=xrender.pc.in

include ../Makefile.inc
