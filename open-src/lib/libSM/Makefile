###############################################################################
#
# SM 1.x Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
MODULE_NAME=libSM

# Version number (used in path names)
MODULE_VERSION=1.0.3

# Checksums for upstream tarball
TARBALL_MD5  = 184cbf502b3cd5d7ba5f9d1290a99606
TARBALL_SHA1 = ccf38d4575da92b750640525af55d770b85765e1

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = libsm.globals.h.patch libsm.SMlibint.h.patch libsm.sm_client.c.patch libsm.sm_manager.c.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=SM

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
MODULE_MTLEVEL=Unsafe
LIB_MAN_SUFFIX=3SM

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS =  --enable-unix-transport \
		      --enable-tcp-transport \
		      --enable-IPv6



# Use specfile to make mapfile & link with it
MODULE_BUILD_DEPS = $(BUILD_DIR)/$(SPECMAP)

MODULE_LD_OPTIONS = -M $(PWD)/$(BUILD_DIR)/$(SPECMAP)

# pkg-config files that need to have -R added
FIX_PC_FILES=sm.pc.in

include ../Makefile.inc
