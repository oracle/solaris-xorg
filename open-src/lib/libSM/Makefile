###############################################################################
#
# Session Management protocol library
#
# Copyright (c) 2008, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.2.2

# Checksums for upstream tarball
TARBALL_MD5   = 499a7773c65aba513609fe651853c5f3
TARBALL_SHA1  = e6d5dab6828dfd296e564518d2ed0a349a25a714
TARBALL_SHA256= 0baca8c9f5d934450a70896c4ad38d06475521255ca63b717a6510fdb6e287bd

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
