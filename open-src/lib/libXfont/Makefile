###############################################################################
#
# Xfont Makefile
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

# Package name used in tarballs
MODULE_NAME=libXfont

# Version number (used in path names)
MODULE_VERSION=1.4.1

# Checksums for upstream tarball
TARBALL_MD5  = 4f2bed2a2be82e90a51a24bb3a22cdf0
TARBALL_SHA1 = f8dc669760975b41885143f828b54164224c8a31

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	ps-fonts.patch \
	htons-inline.patch

# Library name
LIBNAME=Xfont

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Project Private
LIB_MAN_SUFFIX=3xfont

# ARC cases that covered this module
# PSARC 2003/094 Bitmap Font File Compression Formats
MODULE_ARC_CASES = PSARC/2003/094

# Additional configure script options to select the modules we use
MODULE_CONFIG_OPTS= --enable-freetype --disable-type1 --disable-cid \
	--disable-speedo --enable-fontcache --enable-fc --enable-builtins \
	--with-bzip2

# pkg-config files that need to have -R added
FIX_PC_FILES=xfont.pc.in

# Install in private subdir
MODULE_ADD_LIB_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

# Need to link with libfontenc in server private directory
MODULE_LDFLAGS= -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXfont.so libXfont.so.1

include ../Makefile.inc

