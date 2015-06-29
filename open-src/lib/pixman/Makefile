###############################################################################
#
# Pixman: The pixel-manipulation library for X and Cairo.
#
# Copyright (c) 2008, 2015, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=pixman

# Version number (used in path names)
MODULE_VERSION=0.32.6

# Checksums for upstream tarball
TARBALL_MD5  = 8a9e8f14743a39cf303803f369c1f344
TARBALL_SHA1 = 5b730399e1e212e5acaa69a4f1a2c7be1af1cdc4
TARBALL_SHA256 = 201fc0d7d6bc0017496f2bd27b3ca14224aea0df6b624c5ee2dc0307a4ff14a4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name
LIBNAME=pixman

# Where to install pixman files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Package metadata
MODULE_DESC = the pixman pixel-manipulation library
GIT_URL     = $(FDO_GIT_URL)/pixman
GIT_URL_SET = yes

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=library/graphics/pixman
MODULE_PKGNAME_SET=yes
# Package classification (override default)
MODULE_PKGCLASS=System/Libraries
MODULE_PKGCLASS_SET=yes

# ARC cases that covered this module
# LSARC/2008/637 pixman
MODULE_ARC_CASES = LSARC/2008/637
# LSARC/2009/223 pixman 0.14
MODULE_ARC_CASES += LSARC/2009/223
# PSARC/2010/282 pixman 0.18
MODULE_ARC_CASES += PSARC/2010/282

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3pixman

# pkg-config files that need to have -R added
FIX_PC_FILES=pixman-1.pc.in

# Command line options to GNU autoconf configure script
# --disable-gtk: Disable building of test programs that require gtk
#   and have problems with mismatches between system library & the one we're
#   building in our builds.
MODULE_CONFIG_OPTS = --disable-gtk

# Discard unneeded libraries & files that get linked into many test programs
MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP) $(ZDISCARD_UNUSED_FIL)

include ../Makefile.inc
