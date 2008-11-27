# -*- Makefile -*- rules common to most X.Org library modules
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
# 
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#
# @(#)Makefile.inc	1.12	08/11/26
#

PWD:sh=pwd
TOP=$(PWD)/../..

MODTYPE=lib

### Include common definitions
include $(TOP)/common/Makefile.init

# Set default to build both 32-bit & 64-bit if not set in module makefile
BUILD_TYPES_DEFAULT     = $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
$(BUILD_TYPES_DEFAULT)  BUILD_TYPES=32 64
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES_SET=yes

# Changes to apply to man pages
MODULE_MTLEVEL_DEFAULT	= $(MODULE_MTLEVEL_SET:yes=$(POUND_SIGN))
$(MODULE_MTLEVEL_DEFAULT)	MODULE_MTLEVEL=See XInitThreads(3X11)
MODTYPE_SUNTOUCH_MAN_FLAGS= -l $(LIBNAME) -a '{MT-Level, $(MODULE_MTLEVEL)}'

# Flags to build & link libraries with
MODTYPE_CPPFLAGS=$(LIB_CPPFLAGS)
MODTYPE_CFLAGS=$(LIB_CFLAGS)
MODTYPE_LDFLAGS=$(LIB_LDFLAGS) -lc
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

include $(TOP)/common/Makefile.inc

# Make 64-bit link when installing 64-bit libraries

LIB_INST_DIR=$(PROTODIR)$(MODULE_PREFIX)/lib$(MODULE_ADD_LIB_SUBDIR)

install_64::
	-/bin/rm -f $(LIB_INST_DIR)/64
	ln -s $(SUBDIR64) $(LIB_INST_DIR)/64

