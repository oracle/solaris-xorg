###############################################################################
#
# xf86-video-cirrus 1.x Makefile
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
MODULE_NAME=xf86-video-cirrus

# Version number (used in path names)
MODULE_VERSION=1.3.2

# Checksums for upstream tarball
TARBALL_MD5  = 8195d03ed0be0975c03441e66a9f53b3
TARBALL_SHA1 = 85bc5833075f9ddadb902f71a40249aa769aa4d1

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = mapfiles.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Paths to find libraries/modules to link with - libraries themselves listed
# in src/Makefile.am since cirrus has submodules with different needs
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)

AUTORECONF=yes

# Need to override install path for all the modules, not just cirrus_drv.so
MODULE_MAKEFLAGS = \
	cirrus_alpine_ladir='$(DRIVER_DIR)' \
	cirrus_laguna_ladir='$(DRIVER_DIR)'

include ../Makefile.inc


