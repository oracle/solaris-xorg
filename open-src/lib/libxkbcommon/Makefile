###############################################################################
#
# libxkbcommon - keyboard keymap compiler and support library
#
# Copyright (c) 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libxkbcommon

# Version number (used in path names)
MODULE_VERSION=0.5.0

# Checksums for upstream tarball
TARBALL_MD5   = 2e1faeafcc609c30af3a561a91e84158
TARBALL_SHA1  = 7127993bfb69e13cdff25fb8b3c8f26ce6be5bfa
TARBALL_SHA256= 90bd7824742b9a6f52a6cf80e2cadd6f5349cf600a358d08260772615b89d19c

# ARC cases that covered this module
# PSARC/2016/062 xkbcommon
MODULE_ARC_CASES = PSARC/2016/062

# Package metadata
UPSTREAM = XKBcommon
UPSTREAM_URL = http://xkbcommon.org
UPSTREAM_SET = yes
ORACLE_TPNO = 23564

# Download site for source
SOURCE_URL = http://xkbcommon.org/download/$(MODULE_NAME)-$(MODULE_VERSION).tar.xz
SOURCE_URL_SET = yes

# Source tarball
SOURCE_TARBALL_NAME = $(MODULE_NAME)-$(MODULE_VERSION).tar.xz
SOURCE_TARBALL_NAME_SET=yes

MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

MODULE_CONFIG_OPTS += \
		--with-default-rules=/usr/share/X11/xkb/rules \
		--enable-docs

# Regenerate Makefile.in after patching Makefile.am
AUTORECONF=yes 

# generate a .so
DELIBTOOLIZE_FLAGS = \
		--shared libxkbcommon.so.0 \
	 	--shared libxkbcommon-x11.so.0

MODULE_LD_SHARED_OPTIONS = -z gnu-version-script-compat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = doxyfile.patch,-p1

# Library name
LIBNAME=xkbcommon

# pkg-config files that need to have -R added
FIX_PC_FILES=xkbcommon.pc.in xkbcommon-x11.pc.in

include ../Makefile.inc
