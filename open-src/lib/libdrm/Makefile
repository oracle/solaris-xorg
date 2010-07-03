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
MODULE_NAME=libdrm

# Package metadata
UPSTREAM	= Direct Rendering Infrastructure project
UPSTREAM_URL	= http://dri.freedesktop.org/
UPSTREAM_SET	= yes

# Version number (used in path names)
MODULE_VERSION=2.4.14

# Checksums for upstream tarball
TARBALL_MD5  = e785b6a6ebd6b8a49dd8210cd8742842
TARBALL_SHA1 = a18cc2cdfd02b8ba1c91c3e9ac609521ad3e2fe9

# Library name (used for specfiles/mapfiles)
LIBNAME=drm

# Download site for source
SOURCE_URL=http://dri.freedesktop.org/libdrm/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/drm
GIT_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	solaris-drm-port.patch \
	6802045.patch \
	gem.patch

# Need to regenerate files after patching Makefile.am in solaris-drm-port.patch
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=*.man
#MODULE_STABILITY=Volatile
#LIB_MAN_SUFFIX=3drm

# pkg-config files that need to have -R added
FIX_PC_FILES=libdrm.pc.in libdrm_intel.pc.in

# Where to install files
MODULE_ADD_LIB_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

include ../Makefile.inc
