###############################################################################
#
# FreeGLUT 2.6.0 Makefile
#
# Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=freeglut

# Version number (used in path names)
MODULE_VERSION=2.6.0

# Checksums for upstream tarball
TARBALL_MD5  = 39f0f2de89f399529d2b981188082218
TARBALL_SHA1 = 68306c4486c13d005a4e4d54035e0c0b1bdc220b

# ARC cases that covered this module
MODULE_ARC_CASES = LSARC/2010/016

# Package metadata
UPSTREAM = The FreeGLUT Project
UPSTREAM_URL = http://freeglut.sourceforge.net/
UPSTREAM_SET = yes

# Download site for source
SOURCE_URL = $(SF_DOWNLOADS_URL)/freeglut/freeglut/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = Makefile.in.0.patch

# Source tarball
SOURCE_TARBALL_NAME = freeglut-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Library name (used for specfiles/mapfiles)
LIBNAME=glut

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=

# We must build with GNU make
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

#  FreeGLUT really wants to know that we are POSIX friendly
MODULE_CPPFLAGS = -D__posix__ -D__unix__

include ../Makefile.inc

# FreeGLUT's $(VERSION_INFO) breaks delibtoolize.pl
DELIBTOOLIZE=no

install: install_extra_symlinks

install_extra_symlinks:
	( cd $(PROTODIR)/usr/lib/GL ; \
	    ln -sf ../libglut.so.3.9.0 libglut.so.3 ; \
	    ln -sf ../libglut.so.3.9.0 libglut.so ; \
	    cd - )
	( cd $(PROTODIR)/usr/lib/GL/$(SUBDIR64) ; \
	    ln -sf ../../$(SUBDIR64)/libglut.so.3.9.0 libglut.so.3 ; \
	    ln -sf ../../$(SUBDIR64)/libglut.so.3.9.0 libglut.so ; \
	    cd - )

