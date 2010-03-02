###############################################################################
#
# Xft 2.x Makefile
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
MODULE_NAME=libXft

# Version number (used in path names)
MODULE_VERSION=2.1.14

# Checksums for upstream tarball
TARBALL_MD5  = 254e62a233491e0e1251636536163e20
TARBALL_SHA1 = e08ae6b1f56e160179b9f141b4ab799333ec725e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	xftcore-6261221.patch \
	6872780.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xft

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/Xft.3.in man/Xft.3
SUN_PACKAGE=SUNWxwxft
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3

# Paths to find libraries at build time (LD_LIBRARY_PATH) 
# and run time (LD_RUN_PATH) and freetype-config binary at build time (PATH)
BUILD_ENV = LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(LIBSUBDIR):$(PROTODIR)/usr/openwin/lib$(LIBSUBDIR):/usr/sfw/lib$(LIBSUBDIR) LD_RUN_PATH=/usr/sfw/lib$(LIBSUBDIR) PATH=$(PROTODIR)/usr/bin$(LIBSUBDIR):$(PATH)
MODULE_LD_OPTIONS= -L$(PROTODIR)/usr/lib$(LIBSUBDIR)

# Mapfile used to control which symbols are exported
XFT_MAPFILE=$(PWD)/mapfile.scope
MODULE_LD_OPTIONS += -M$(XFT_MAPFILE) -lX11

# pkg-config files that need to have -R added
FIX_PC_FILES=xft.pc.in

include ../Makefile.inc

