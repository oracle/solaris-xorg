###############################################################################
#
# Session Management protocol library
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.2.1

# Checksums for upstream tarball
TARBALL_MD5   = 766de9d1e1ecf8bf74cebe2111d8e2bd
TARBALL_SHA1  = 8d354a165e330022ffd3606aba562b8c8154213d
TARBALL_SHA256= 93c11d569c64f40723b93b44af1efb474a0cfe92573b0c8c330343cabb897f1d

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Library name (used for specfiles/mapfiles)
LIBNAME=SM

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Committed
MODULE_MTLEVEL=Unsafe
MODULE_MTLEVEL_SET=yes
LIB_MAN_SUFFIX=3SM

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS =  --enable-unix-transport \
		      --enable-tcp-transport \
		      --enable-IPv6 \
		      --without-libuuid

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=sm.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libSM.so libSM.so.6

include ../Makefile.inc
