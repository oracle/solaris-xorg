###############################################################################
#
# xf86-video-ati Makefile
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
MODULE_NAME=xf86-video-ati

# Version number (used in path names)
MODULE_VERSION=7.7.0

# Checksums for upstream tarball
TARBALL_MD5   = 59bfe0feba875c5cd28b03766aaa3b98
TARBALL_SHA1  = 75621e6d41d0958d95dcd9b742bff3b81e853f8f
TARBALL_SHA256= 844d1c577b145c90dc8ef027678f0c27f554363f782cd696a3aea26415b2c1c3

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = solaris-port.patch disable-kms.patch

# Need to regenerate Makefile.in after patching Makefile.am in mapfiles.patch
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# AtomBios code won't build with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Cos of 23185711 - disable parfait temporarily
USE_PARFAIT=no

# Extra flags to pass to configure script
MODULE_CONFIG_OPTS=--disable-kms --disable-glamor

# Paths to find libraries/modules to link with
radeon_drv_la_LDFLAGS_add = \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-L$(PROTODIR)$(MESA_XSERVERMODS_DIR)/extensions \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERMODS_DIR) \
	-R$(MESA_XSERVERMODS_DIR)/extensions \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)
MODULE_BUILD_ENV = radeon_drv_la_LDFLAGS_add='$(radeon_drv_la_LDFLAGS_add)'

# Library paths at build time
MODULE_LD_OPTIONS = \
        -L$(PROTODIR)$(X11_SERVERMODS_DIR) \
        -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
        -L$(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR) \
        -lexa -lpixman-1 -lfb -lshadow

include ../Makefile.inc
