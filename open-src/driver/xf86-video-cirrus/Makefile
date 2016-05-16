###############################################################################
#
# xf86-video-cirrus 1.x Makefile
#
# Copyright (c) 2006, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-cirrus

# Version number (used in path names)
MODULE_VERSION=1.5.3

# Checksums for upstream tarball
TARBALL_MD5   = 7d7dfd4cdc42aa8b6e923510fa00ad2b
TARBALL_SHA1  = 545cbedb42e5a5ce6bdf0b9bc30a826908f284e5
TARBALL_SHA256= edc87b20a55259126b5239b5c1ef913419eab7ded0ed12ae9ae989460d7351ab

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = mapfiles.patch solaris-port.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Paths to find libraries/modules to link with - libraries themselves listed
# in src/Makefile.am since cirrus has submodules with different needs
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR)

AUTORECONF=yes

include ../Makefile.inc


