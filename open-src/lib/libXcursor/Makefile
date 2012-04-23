###############################################################################
#
# Xcursor library for alpha-blended & animated cursors via Xrender extension
#
# Copyright (c) 2007, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXcursor

# Version number (used in path names)
MODULE_VERSION=1.1.13

# Checksums for upstream tarball
TARBALL_MD5   = 52efa81b7f26c8eda13510a2fba98eea
TARBALL_SHA1  = 4e461d9e4324689ca8a84706926a8c0fe9470e47
TARBALL_SHA256= f78827de4a1b7ce8cceca24a9ab9d1b1d2f6a61362f505166ffc19b07c0bad8f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name
LIBNAME=Xcursor

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=3xcursor

# ARC cases that covered this module
# LSARC/2007/491 libXcursor
MODULE_ARC_CASES = LSARC/2007/491


# pkg-config files that need to have -R added
FIX_PC_FILES=xcursor.pc.in

# Use /usr/share/icons only, not /usr/X11/share, for consistency with GNOME
MODULE_CONFIG_OPTS=--with-icondir=/usr/share/icons \
	--with-cursorpath='~/.icons:/usr/share/icons:/usr/share/pixmaps'

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXcursor.so libXcursor.so.1

include ../Makefile.inc
