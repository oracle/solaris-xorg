###############################################################################
#
# libXi Makefile
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
MODULE_NAME=libXi

# Version number (used in path names)
MODULE_VERSION=1.3

# Checksums for upstream tarball
TARBALL_MD5  = 8df4ece9bd1efb02c28acb2b6f485e09
TARBALL_SHA1 = 7685f2881ce40b13028d9409eedbb9cf1ed0d8ef

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	man.patch,-p1 \
	solaris-abi.patch

# Need to autoreconf after patching man/Makefile.am in man.patch
AUTORECONF = yes

# Library name (used for specfiles/mapfiles)
LIBNAME=Xi

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3Xi

# Use specfile to make mapfile & link with it
MODULE_BUILD_DEPS = $(BUILD_DIR)/$(SPECMAP)
MODULE_LD_OPTIONS = -M $(PWD)/$(BUILD_DIR)/$(SPECMAP)

# pkg-config files that need to have -R added
FIX_PC_FILES=xi.pc.in

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
MODULE_MAKEFLAGS=-j 1

include ../Makefile.inc
