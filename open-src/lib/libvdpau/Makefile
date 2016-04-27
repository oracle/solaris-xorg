###############################################################################
#
# Copyright (c) 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libvdpau

# Package metadata
UPSTREAM	= Video Decode and Presentation API for UNIX 
UPSTREAM_URL	= http://freedesktop.org/wiki/Software/VDPAU/ 
UPSTREAM_SET	= yes

MODULE_LICENSE          = MIT
MODULE_LICENSE_SET      = yes
MODULE_LICENSE_PREPEND	= 
MODULE_DESC		= VDPAU libaries and headers
ORACLE_TPNO		= 24731

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2009/059

# Version number (used in path names)
MODULE_VERSION=1.1.1

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=library/graphics/libvdpau
MODULE_PKGNAME_SET=yes
# Package classification (override default)
MODULE_PKGCLASS=System/Libraries
MODULE_PKGCLASS_SET=yes

# Checksums for upstream tarball
TARBALL_MD5  = 2fa0b05a4f4d06791eec83bc9c854d14
TARBALL_SHA1 = 86516e2a962fd34f65d49115d6ddf15fd912f579

# Download site for source
SOURCE_URL=http://people.freedesktop.org/~aplattner/vdpau/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=git://people.freedesktop.org/~aplattner/libvdpau
GIT_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=

# Don't use libtool so force -lX11 and -lXext
MODULE_LD_OPTIONS += -lX11 -lXext 

# Add proto area to path to find makedepend
MODULE_BUILD_ENV = PATH="$(PROTODIR)/$(X11_BIN_DIR):$(PATH)"
MODULE_CONFIG_ENV += $(MODULE_BUILD_ENV)

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Need to build with gcc
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Cos of 23185711 - disable parfait temporarily
USE_PARFAIT=no

### Include common rulesets
include ../Makefile.inc
