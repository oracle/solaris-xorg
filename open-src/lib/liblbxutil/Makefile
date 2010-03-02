###############################################################################
# lbxutil 1.x Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
MODULE_NAME=liblbxutil

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5  = 273329a78c2e9ea189ac416c7fde94a1
TARBALL_SHA1 = 99616bab0ca0afeaa9c0a19222091b1ff4f18680

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=lbxutil

# Man pages to apply Sun footer to & attributes to list
#SUN_PACKAGE=SUNWxwplt
#LIB_STABILITY=Committed
#LIB_MAN_SUFFIX=3lbxutil

# Only need 32-bit since we only ship lbxproxy as 32-bit
BUILD_TYPES=32
BUILD_TYPES_SET=yes

# pkg-config files that need to have -R added
FIX_PC_FILES=lbxutil.pc.in

# Install in private subdir for use by lbxproxy
MODULE_ADD_LIB_SUBDIR=/X11/lbxproxy

# Link with mapfile to allow referring to symbols defined in callers
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-externs

include ../Makefile.inc

# Clean up old 64-bit builds from before we stopped building them
clean: clean_64
