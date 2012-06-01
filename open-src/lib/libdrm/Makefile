#
# Copyright (c) 2006, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=2.4.32

# Checksums for upstream tarball
TARBALL_MD5  = b731faf4a49f89c318d84378cb563cf8
TARBALL_SHA1 = 4aab6cc1c0d2a4822ffd83bd3da0e7a5479bf842
TARBALL_SHA256 = 9ba8a1613de57af8f501a162747ed69aff30d63fc1316602770974bdd9ee984e

# Library name (used for specfiles/mapfiles)
LIBNAME=drm

# Download site for source
SOURCE_URL=http://dri.freedesktop.org/libdrm/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/drm
GIT_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += solaris-drm-port.patch,-p1
SOURCE_PATCHES += atomic.patch

# Can't build tests with parfait yet, due to parfait-ld error:
# llvm-ld: error: Cannot link file '../../libkms/libkms.so.1.bc: 
#    Linking globals named 'drmSetServerInfo': symbol multiply defined!
$(WITH_PARFAIT) SOURCE_PATCHES += no-tests.patch,-p1
$(WITH_PARFAIT) AUTORECONF = yes

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

MODULE_BUILD_MAKEFLAGS = libkms_la_LIBADD=../libdrm.so.2

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --disable-radeon --enable-libkms

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=*.man
#MODULE_STABILITY=Volatile
#LIB_MAN_SUFFIX=3drm

# pkg-config files that need to have -R added
FIX_PC_FILES=libdrm.pc.in intel/libdrm_intel.pc.in libkms/libkms.pc.in

# Where to install files
MODULE_ADD_LIB_SUBDIR = $(X11_SERVERLIBS_SUBDIR)
MODULE_PKGCONFIG_DIR = $(PKGCONFIG_DIR)
MODULE_PKGCONFIG_DIR_SET = yes

include ../Makefile.inc

install: $(PROTODIR)/usr/include/drm

$(PROTODIR)/usr/include/drm:
	@rm -f $@
	ln -s libdrm $@

# Upstream seems to install in $(X11_SERVERLIBS_SUBDIR)/pkgconfig no matter
# what, so just move them afterwards
PC_FILES_i386 = libdrm_intel.pc
PC_FILES = libdrm.pc libkms.pc $(PC_FILES_$(MACH))
MOVED_PC_FILES = $(PC_FILES:%=$(PROTODIR)/$(PKGCONFIG_DIR)/%)
XSERVER_PC_DIR = $(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)/pkgconfig

install-gen: $(MOVED_PC_FILES)

$(PROTODIR)/$(PKGCONFIG_DIR)/%.pc: $(PROTODIR)/$(XSERVER_PC_DIR)/%.pc
	-rm -f $@
	mv $< $@

$(PC_FILES:%=$(PROTODIR)/$(XSERVER_PC_DIR)/%): default_install
