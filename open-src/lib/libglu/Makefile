###############################################################################
#
# Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=glu

# Package metadata
UPSTREAM	= The Mesa 3-D Graphics Library
UPSTREAM_URL	= http://mesa3d.org/
UPSTREAM_SET	= yes

MODULE_LICENSE          = Mesa License (Mixed: MIT, LGPLv2, others)
MODULE_LICENSE_SET      = yes
MODULE_LICENSE_PREPEND	= $(GPL_CHOICE_FILE)
MODULE_DESC		= the Mesa 3-D Graphics Libraries
ORACLE_TPNO		= 9926

# ARC cases that covered this module
# LSARC/2005/109 MESA, Open Source OpenGL clone
MODULE_ARC_CASES = LSARC/2005/109

# Version number (used in path names)
MODULE_VERSION=9.0.0

# Checksums for upstream tarball
TARBALL_MD5  = be9249132ff49275461cf92039083030
TARBALL_SHA1 = c2814bbaf1e60e28a75ec80f4646047c0da742bd

# Source tarball
SOURCE_TARBALL_NAME=glu-$(MODULE_VERSION).tar.bz2
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=ftp://ftp.freedesktop.org/pub/mesa/glu/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/glu
GIT_URL_SET=yes

# Directory created by unpacking source
SOURCE_DIRNAME=glu-$(MODULE_VERSION)
SOURCE_DIRNAME_SET=yes
SOURCE_DIR = $(BUILD_DIR)/$(SOURCE_DIRNAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=glu-exports.patch

# Build mesa with this define (CR 7032292)
MODULE_CFLAGS = -D__EXTENSIONS__

# Override LDFLAGS passed to configure to not include CFLAGS, since that
# breaks building libGLU with Sun Studio C++ compiler
MODULE_CONFIG_ENV = LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(ARCH_FLAGS) $(MODTYPE_LDFLAGS) $(MODULE_CFLAGS) $(MODULE_LDFLAGS)"

# Add proto area to path to find makedepend
MODULE_BUILD_ENV = PATH="$(PROTODIR)/$(X11_BIN_DIR):$(PATH)"
MODULE_CONFIG_ENV += $(MODULE_BUILD_ENV)

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Need to use GNU indent
MODULE_INDENT=$(GNUINDENT)
MODULE_INDENT_SET=yes

# To build with gcc, uncomment these lines and comment out the MODULE_CFLAGS
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Tell Sun compiler to not apply optimizations that increase binary sizes
# Without this flag, Studio 12 binaries are almost 2x bigger (CR 6808868)
#MODULE_CFLAGS = -xspace

# Add include paths and linker paths needed for DRM modules
MODULE_CPPFLAGS = -I$(PROTODIR)/usr/include/libdrm

MODULE_LDFLAGS= -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
		-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) -z ignore

### Include common rulesets
include ../Makefile.inc

HEADERS_TO_MOVE = glu.h glu_mangle.h
HEADERS_TO_DIRECT_LINK = glu.h
LIBRARY_TO_LINK = libGLU.so.1.3.1

# Run just once after all other install steps are done for both 32/64-bit
install: install_ogl_layout

install_ogl_layout:
	-rm -f $(PROTODIR)/usr/lib/libGLU.so.1
	-rm -f $(PROTODIR)/usr/lib/libGLU.so
	-rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLU.so.1
	-rm -f $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLU.so
	mv $(PROTODIR)/usr/include/GL/glu.h $(PROTODIR)/usr/include/mesa
	mv $(PROTODIR)/usr/include/GL/glu_mangle.h $(PROTODIR)/usr/include/mesa
	ln -s $(HEADERS_TO_DIRECT_LINK:%=../mesa/%) \
		$(PROTODIR)/usr/include/GL
	mkdir -p $(PROTODIR)/usr/lib/GL/$(SUBDIR64) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)
	mv $(LIBRARY_TO_LINK:%=$(PROTODIR)/usr/lib/%) \
		$(PROTODIR)/usr/lib/mesa/
	mv $(LIBRARY_TO_LINK:%=$(PROTODIR)/usr/lib/$(SUBDIR64)/%) \
		$(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/lib/mesa/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/lib/mesa/libGLU.so
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/lib/mesa/$(SUBDIR64)/libGLU.so
	(cd $(PROTODIR)/usr/lib/GL; ln -s $(LIBRARY_TO_LINK:%=../mesa/%) .)
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/lib/GL/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/lib/GL/libGLU.so
	(cd $(PROTODIR)/usr/lib/GL/$(SUBDIR64); ln -s $(LIBRARY_TO_LINK:%=../../mesa/$(SUBDIR64)/%) .)
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/lib/GL/$(SUBDIR64)/libGLU.so
	(cd $(PROTODIR)/usr/lib; ln -s $(LIBRARY_TO_LINK:%=GL/%) .)
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/lib/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/lib/libGLU.so
	(cd $(PROTODIR)/usr/lib/$(SUBDIR64); ln -s $(LIBRARY_TO_LINK:%=../GL/$(SUBDIR64)/%) .)
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/lib/$(SUBDIR64)/libGLU.so
	(cd $(PROTODIR)/usr/X11/lib; ln -s $(LIBRARY_TO_LINK:%=GL/%) .)
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/X11/lib/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/X11/lib/libGLU.so
	(cd $(PROTODIR)/usr/X11/lib/$(SUBDIR64); ln -s $(LIBRARY_TO_LINK:%=../GL/$(SUBDIR64)/%) .)
	ln -s libGLU.so.1.3.1 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGLU.so.1
	ln -s libGLU.so.1 $(PROTODIR)/usr/X11/lib/$(SUBDIR64)/libGLU.so
