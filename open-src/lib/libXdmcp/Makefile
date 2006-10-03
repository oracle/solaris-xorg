###############################################################################
#
# Xdmcp 1.x Makefile
#
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.2	06/10/03
#

# Package name used in tarballs
LIB_MODULE=libXdmcp

# Version number (used in path names)
LIB_VERS=1.0.2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name
LIBNAME=Xdmcp

# libXdmcp has no man pages to apply Sun footer to
#SUNTOUCHED_MANPAGES=Xdmcp.man
SUN_PACKAGE=SUNWxwplt
LIB_STABILITY=Committed
LIB_MAN_SUFFIX=3Xdmcp
LIB_SUNTOUCH_MAN_FLAGS= -a '{Availability, $(SUN_PACKAGE)} {Interface Stability, $(LIB_STABILITY)} {MT-Level, Unsafe}'

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = --with-lint --enable-lint-library

# Library built in tree
LIB_BUILT=$(BUILD_DIR)/.libs/libXdmcp.so.6.0.0

# Use specfile to make mapfile & link with it
LIB_ADD_BUILD_DEPS = $(BUILD_DIR)/$(SPECMAP)
LIB_ADD_LD_OPTIONS = -M $(PWD)/$(BUILD_DIR)/$(SPECMAP)

# pkg-config files that need to have -R added
FIX_PC_FILES=xdmcp.pc.in

include ../Makefile.inc

# Remove encryption source before building
source_gen::
	-rm $(SOURCE_DIR)/Wraphelp.*
