###############################################################################
#
# libepoxy - library for handling OpenGL function pointers
#
# Copyright (c) 2015, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libepoxy

# Version number (used in path names)
MODULE_VERSION=1.4.0

# Checksums for upstream tarball
TARBALL_MD5   = 678299d756c64ca9bda4fc7579b93b32
TARBALL_SHA1  = 9c08295880642e49f7f6a1f6f1dcaf3f1c9f04d5
TARBALL_SHA256= 935c74d4f8bdb1d2435f58ea95ac2a470368123e8e6f0618f6bc147c0956701b

# ARC cases that covered this module
# PSARC/2015/382 libepoxy 1.2
MODULE_ARC_CASES = PSARC/2015/382

# Package metadata
MODULE_DESC = library for handling OpenGL function pointers
UPSTREAM = libepoxy
UPSTREAM_URL = https://github.com/anholt/libepoxy/
UPSTREAM_SET = yes
ORACLE_TPNO = 34679
ORACLE_TPNO_SET = yes

# Download site for source
SOURCE_URL = https://github.com/anholt/$(MODULE_NAME)/archive/$(MODULE_VERSION).tar.gz
SOURCE_URL_SET = yes

# Source tarball
SOURCE_TARBALL_NAME = $(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Need to build with gcc
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Cos of 23185711 - disable parfait temporarily
USE_PARFAIT=no

# regenerate configure
AUTORECONF = yes

# libtool interferes with libepoxy
DELIBTOOLIZE = no

# Library name
LIBNAME=epoxy

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Pass-through Volatile

# pkg-config files that need to have -R added
FIX_PC_FILES=epoxy.pc.in

include ../Makefile.inc
