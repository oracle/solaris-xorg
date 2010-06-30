###############################################################################
#
# libXres 1.x Makefile
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXres

# Version number (used in path names)
MODULE_VERSION=1.0.4

# Checksums for upstream tarball
TARBALL_MD5  = 4daf91f93d924e693f6f6ed276791be2
TARBALL_SHA1 = d5ee9560a61666e6bb3d2285b9634fccd7211d65

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=XRes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3xres



# pkg-config files that need to have -R added
FIX_PC_FILES=xres.pc.in

include ../Makefile.inc
