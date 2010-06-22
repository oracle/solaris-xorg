# -*- Makefile -*- rules common to most X.Org library modules
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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

PWD:sh=pwd
TOP=$(PWD)/../../..

MODTYPE=lib

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# Set default to build both 32-bit & 64-bit if not set in module makefile
BUILD_TYPES_DEFAULT     = $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
$(BUILD_TYPES_DEFAULT)  BUILD_TYPES=32 64
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES_SET=yes

# Changes to apply to man pages
MODULE_MTLEVEL_DEFAULT	= $(MODULE_MTLEVEL_SET:yes=$(POUND_SIGN))
$(MODULE_MTLEVEL_DEFAULT)	MODULE_MTLEVEL=See XInitThreads(3X11)
MODTYPE_SUNTOUCH_MAN_FLAGS= -l $(LIBNAME) -a '{MT-Level, $(MODULE_MTLEVEL)}'

# Add -lc to library link except for the handful of libraries that don't 
# need it
LIBC_DEFAULT		= $(NEED_LIBC:no=$(POUND_SIGN))
$(LIBC_DEFAULT)		LIBC=-lc

# Flags to build & link libraries with
MODTYPE_CPPFLAGS=$(LIB_CPPFLAGS)
MODTYPE_CFLAGS=$(LIB_CFLAGS)
MODTYPE_CXXFLAGS=$(LIB_CXXFLAGS)
MODTYPE_LDFLAGS=$(LIB_LDFLAGS) $(LIBC)
MODTYPE_LD_OPTIONS=$(LIB_REQUIRED_LDFLAGS)

# Additional command line options to GNU autoconf configure script
MODTYPE_CONFIG_OPTS = --enable-shared=yes --enable-static=no \
    --with-lint --enable-lint-library --with-pic \
    --libdir='$${exec_prefix}/lib$(MODULE_ADD_LIB_SUBDIR)$(ARCHLIBSUBDIR)' \
    --bindir='$${exec_prefix}/bin$(ARCHLIBSUBDIR)'

# Additional environment variables to GNU autoconf configure script
MODTYPE_CONFIG_ENV = LIB_MAN_SUFFIX=$(LIB_MAN_SUFFIX)

# Set comments in ELF binaries to help identify versions/builds
MODTYPE_ADD_BUILD_TARGETS=set-elf-comments

# Additional arguments to pass to make when running make install
MODTYPE_INSTALL_MAKEFLAGS = \
	libdir=$(MODULE_PREFIX)/lib$(MODULE_ADD_LIB_SUBDIR)$(ARCHLIBSUBDIR)

# Need to fix libtool interference with our linker flags
DELIBTOOLIZE=yes

include $(TOP)/open-src/common/Makefile.inc
