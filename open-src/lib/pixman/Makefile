###############################################################################
#
# pixman Makefile
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=0.24.4

# Checksums for upstream tarball
TARBALL_MD5  = 89eb7db8853681bbcea626b161de0488
TARBALL_SHA1 = 683450f917015366ac7918fc517c76801aeff374

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Override default CFLAGS for x86 builds to work around assembler syntax 
# errors reported by Studio 12.1 when the -xspace flag is used
include ../../common/Makefile.init
NO_XSPACE_CFLAGS = $(LIB_CFLAGS:-xspace=)
MODULE_CONFIG_ENV_i386 = CFLAGS="$(NO_XSPACE_CFLAGS)"
MODULE_CONFIG_ENV_sparc = # not needed
MODULE_CONFIG_ENV = $(MODULE_CONFIG_ENV_$(MACH))

# Library name
LIBNAME=pixman

# Where to install pixman files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

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

include ../Makefile.inc
