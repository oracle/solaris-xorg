###############################################################################
#
# xf86-video-ast 1.x Makefile
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
MODULE_NAME=xf86-video-ast

# Version number (used in path names)
MODULE_VERSION=0.89.0

# Checksums for upstream tarball
TARBALL_MD5  = 9d98ac74ea7feb9f1f4783b59eccdb2b
TARBALL_SHA1 = dca605c1d7202209bc0f3ecec10ae979cbf71e14

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
		xf86UDelay.patch,-p1 \
		sparc-ast.patch \
		sparc-ast-6860461.patch \
		6924667.patch port.patch

# Link in additional source files from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-graphics-ddx
MODULE_STABILITY=Volatile

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-lfb -lint10 -lpciaccess -lvbe -lvgahw -lxaa

include ../Makefile.inc
