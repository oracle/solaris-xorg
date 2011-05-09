###############################################################################
#
# Xaw4 Makefile
#
# Copyright (c) 2009, 2011, Oracle and/or its affiliates. All rights reserved.
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

# libXaw4 was replaced by libXaw5 well before 64-bit libraries were shipped
# in Solaris, and since it's just kept for binary compatibility of old apps,
# we don't need a 64-bit version.
BUILD_TYPES_SET=yes
BUILD_TYPES=32

MODULE_NAME=libXaw4

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/toolkit/libxaw4
MODULE_PKGNAME_SET=yes

# No upstream sources, only local sources
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=sun-src

# Library name (used for specfiles/mapfiles)
LIBNAME=Xaw4

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= # None
MODULE_STABILITY=Committed

# ARC cases that covered this module
# PSARC/1992/173 - Update MIT-based Clients to X11R5 Level
MODULE_ARC_CASES = PSARC/1992/173

# No configure script to run
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

# Since we don't have a configure script, pass configure flags to make
MODULE_BUILD_MAKEFLAGS=$(CONFIG_ENV) PREFIX=$(MODULE_PREFIX)
MODULE_INSTALL_MAKEFLAGS=$(CONFIG_ENV) PREFIX=$(MODULE_PREFIX)

MODULE_CFLAGS += -I.

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXaw.so.4

include ../Makefile.inc
