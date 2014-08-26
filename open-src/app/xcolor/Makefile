###############################################################################
#
# xcolor - utility to display used & available entries in PseudoColor colormaps
#
# Copyright (c) 2008, 2014, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xcolor

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/colormap-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xcolor colormap display program
MODULE_LICENSE = Oracle
MODULE_LICENSE_SET = yes

# Dates to show in Oracle copyright notice in pkg license file
# Earliest & latest of the copyrights in the Oracle files in this pkg
ORACLE_COPYRIGHT_YEARS = 1995, 2001

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=sun-src

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# No configure script to run, but need to make build dirs
CONFIGURE_TARGETS = $(BUILD_DIR)
CONFIGURE_TARGETS_SET=yes

# Since we don't have a configure script, pass configure flags to make
# and pass path to $(SOURCE_DIR) for makefiles & source files
REL_SOURCE_DIR = ../$(SOURCE_DIR)
MODULE_MAKEFLAGS  = VPATH=$(REL_SOURCE_DIR) -f $(REL_SOURCE_DIR)/Makefile
MODULE_BUILD_ENV  = $(CONFIG_ENV) PREFIX=$(MODULE_PREFIX)
MODULE_INSTALL_MAKEFLAGS = bindir=$(X11_BIN_DIR) mandir=$(X11_MAN_DIR)

# Allow linker to skip linking to libm if compiler inlined all math functions
MODULE_LDFLAGS = $(ZDISCARD_UNUSED_DEP)

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=xcolor.1
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xcolor

include ../Makefile.inc
