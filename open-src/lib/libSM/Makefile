###############################################################################
#
# SM 1.x Makefile
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
# @(#)Makefile	1.2	08/02/09
#

# Package name used in tarballs
LIB_MODULE=libSM

# Version number (used in path names)
LIB_VERS=1.0.3

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = libsm.globals.h.patch libsm.SMlibint.h.patch libsm.sm_client.c.patch libsm.sm_manager.c.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=SM

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
SUN_PACKAGE=SUNWxwplt
LIB_STABILITY=Committed
LIB_MAN_SUFFIX=3SM
LIB_SUNTOUCH_MAN_FLAGS= -a '{Availability, $(SUN_PACKAGE)} {Interface Stability, $(LIB_STABILITY)} {MT-Level, Unsafe}'

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = --disable-libtool-lock \
		      --enable-unix-transport \
		      --enable-tcp-transport \
		      --enable-IPv6 \
		      --with-pic \
		      --with-release-version="1.0.3"

# Library built in tree
LIB_BUILT=$(BUILD_DIR)/.libs/libSM.so.6.0.0

# Use specfile to make mapfile & link with it
LIB_ADD_BUILD_DEPS = $(BUILD_DIR)/$(SPECMAP)

LIB_ADD_LD_OPTIONS = -M $(PWD)/$(BUILD_DIR)/$(SPECMAP)

LIB_ADD_LDFLAGS = -lsocket -lnsl

# pkg-config files that need to have -R added
FIX_PC_FILES=sm.pc.in

include ../Makefile.inc
