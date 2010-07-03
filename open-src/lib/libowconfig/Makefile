###############################################################################
#
# libowconfig Makefile
#
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
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

BUILD_TYPES_SET=yes
BUILD_TYPES=32

MODULE_NAME=libowconfig

# No upstream sources, only local sources
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=sun-src

# Library name (used for specfiles/mapfiles)
LIBNAME=owconfig

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= # None
MODULE_STABILITY=Obsolete

# No configure script to run
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

# Since we don't have a configure script, pass configure flags to make
MODULE_BUILD_MAKEFLAGS=$(CONFIG_ENV) PREFIX=$(MODULE_PREFIX) \
	X11_INCLUDES_DIR=$(X11_INCLUDES_DIR)
MODULE_INSTALL_MAKEFLAGS=$(MODULE_BUILD_MAKEFLAGS)

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libowconfig.so libowconfig.so.0

include ../Makefile.inc
