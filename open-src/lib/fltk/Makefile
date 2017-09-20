###############################################################################
#
# Fltk - Fast Light Toolkit (pronounced fulltick)
#
# Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=fltk

# Version number (used in path names)
MODULE_VERSION=1.3.4

# Checksums for upstream tarball
TARBALL_MD5 = d7fcd27ab928648e1a1366dd2e273970
TARBALL_SHA1 = 0a2a40e34094b9758dc8552ffb32ae444bce8577
TARBALL_SHA256 = 7fb2c8882433ce694e6900c94fda505e8f4ed3fa9c7e597007098a33b85c53f4

# make links to source
CLONE_SRC=yes

# ARC cases that covered this module
# PSARC/2017/212 Xorg server 1.19.3
MODULE_ARC_CASES = PSARC/2017/212

# Package metadata
MODULE_DESC = the fast light toolkit library
ORACLE_TPNO = 11272
ORACLE_TPNO_SET = yes

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/toolkit/fltk
MODULE_PKGNAME_SET=yes

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Need to build with gcc
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes
USE_PARFAIT=no

# Source tarball
SOURCE_TARBALL_NAME=fltk-1.3.4-1-source.tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=http://fltk.org/pub/fltk/1.3.4/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

MODULE_CONFIG_OPTS += \
		--enable-shared \
		--disable-static

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = configure.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=libfltk

include ../Makefile.inc

install: install_extra_symlinks

install_extra_symlinks:
	( cd $(PROTODIR)/usr/lib; \
		ln -sf libfltk_forms.so.1.3 libfltk_forms.so.1; \
		ln -sf libfltk_gl.so.1.3 libfltk_gl.so.1; \
		ln -sf libfltk.so.1.3 libfltk.so.1; \
		ln -sf libfltk_images.so.1.3 libfltk_images.so.1;)
	( cd $(PROTODIR)/usr/lib/$(SUBDIR64) ; \
		ln -sf libfltk_forms.so.1.3 libfltk_forms.so.1; \
		ln -sf libfltk_gl.so.1.3 libfltk_gl.so.1; \
		ln -sf libfltk.so.1.3 libfltk.so.1; \
		ln -sf libfltk_images.so.1.3 libfltk_images.so.1;)
