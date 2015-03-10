###############################################################################
#
# fbconsole - fallback console utility
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
#

# Package name used in tarballs
MODULE_NAME=fbconsole

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=terminal/fbconsole
MODULE_PKGNAME_SET=yes
# Package classification (override default)
MODULE_PKGCLASS=System/Hardware
MODULE_PKGCLASS_SET=yes

# Package metadata
MODULE_LICENSE = Oracle
MODULE_LICENSE_SET = yes
MODULE_DESC = the fbconsole console fallback utility

# Dates to show in Oracle copyright notice in pkg license file
# Earliest & latest of the copyrights in the Oracle files in this pkg
ORACLE_COPYRIGHT_YEARS = 1992, 2015

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/1992/130 LSARC/2005/590

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
SOURCE_TARGETS =
SOURCE_TARGETS_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# No configure script to run, but need to make build dirs
CONFIGURE_TARGETS = $(BUILD_DIR)
CONFIGURE_TARGETS_SET=yes

# Since we don't have a configure script, pass configure flags to make
# and pass path to $(REL_SOURCE_DIR) for makefiles & source files
REL_SOURCE_DIR = ../sun-src
MODULE_MAKEFLAGS  = VPATH=$(REL_SOURCE_DIR) -f $(REL_SOURCE_DIR)/Makefile
MODULE_BUILD_ENV  = $(CONFIG_ENV) PREFIX=$(MODULE_PREFIX)

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = fbconsole

include ../Makefile.inc

