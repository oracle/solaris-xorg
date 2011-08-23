#
# Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.
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

# No upstream sources, only local sources
MODULE_NAME		= build-tools
MODULE_VERSION 		= src
SOURCE_TARBALL_NAME	= NONE
SOURCE_TARBALL_NAME_SET	= yes
ADDITIONAL_SOURCE_DIR	= sun-src

# No configure script to run
CONFIGURE_TARGETS	=
CONFIGURE_TARGETS_SET	= yes

# Build both 32-bit & 64-bit versions
BUILD_TYPES		= 32 64
BUILD_TYPES_SET		= yes

# Since we don't have a configure script, pass configure flags to make
MODULE_CONFIG_ENV	= MACH="$(MACH$(BUILD_TYPE))" DESTDIR="$(TOOLS_DIR)"
MODULE_BUILD_MAKEFLAGS	= $(CONFIG_ENV)
MODULE_INSTALL_MAKEFLAGS= $(CONFIG_ENV)

include ../Makefile.inc
