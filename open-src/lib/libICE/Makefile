###############################################################################
#
# libICE - Inter-Client Exchange library
#
# Copyright (c) 2008, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libICE

# Package metadata
MODULE_DESC = the X11 Inter-Client Exchange library (libICE)

# Version number (used in path names)
MODULE_VERSION=1.0.9

# Checksums for upstream tarball
TARBALL_MD5   = addfb1e897ca8079531669c7c7711726
TARBALL_SHA1  = 3c3a857a117ce48a1947a16860056e77cd494fdf
TARBALL_SHA256= 8f7032f2c1c64352b5423f6b48a8ebdc339cc63064af34d66a6c9aa79759e202

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =  define.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=ICE

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Committed
MODULE_MTLEVEL=Unsafe
MODULE_MTLEVEL_SET=yes
LIB_MAN_SUFFIX=3ICE

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS =  --enable-unix-transport \
		      --enable-tcp-transport \
		      --disable-local-transport \
		      --enable-ipv6

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=ice.pc.in

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libICE.so libICE.so.6

include ../Makefile.inc
