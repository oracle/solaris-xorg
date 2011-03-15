#
# Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libdrm

# Package metadata
UPSTREAM	= Direct Rendering Infrastructure project
UPSTREAM_URL	= http://dri.freedesktop.org/
UPSTREAM_SET	= yes

# Version number (used in path names)
MODULE_VERSION=2.4.21

# Checksums for upstream tarball
TARBALL_MD5  = 273ed9dad986e3a931649f3d8762ff74
TARBALL_SHA1 = be7754008424a12e01ab0f0da3deb8de13ad2f0c

# Library name (used for specfiles/mapfiles)
LIBNAME=drm

# Download site for source
SOURCE_URL=http://dri.freedesktop.org/libdrm/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/drm
GIT_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	solaris-drm-port.patch atomic.patch

# Need to regenerate files after patching Makefile.am in solaris-drm-port.patch
AUTORECONF=yes

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --disable-radeon --enable-libkms

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=*.man
#MODULE_STABILITY=Volatile
#LIB_MAN_SUFFIX=3drm

# pkg-config files that need to have -R added
FIX_PC_FILES=libdrm.pc.in intel/libdrm_intel.pc.in libkms/libkms.pc.in

# Where to install files
MODULE_ADD_LIB_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

include ../Makefile.inc

install: $(PROTODIR)/usr/include/drm

$(PROTODIR)/usr/include/drm:
	@rm -f $@
	ln -s libdrm $@
