###############################################################################
#
# xcb library Makefile
#
# Copyright (c) 2010, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libxcb

# Version number (used in path names)
MODULE_VERSION=1.8.1

# Checksums for upstream tarball
TARBALL_MD5  = 9da03df9e2f4c048202920d9f6a7e123
TARBALL_SHA1 = 98199b6054750a06cddd4e77baa4354af547ce6f

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES  = Makefile.am.0.patch,-p1
SOURCE_PATCHES += Makefile.am.4.patch,-p1

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Library name (used for specfiles/mapfiles)
LIBNAME=xcb

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=

MODULE_LD_OPTIONS = -z ignore -lc
LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = $(MODTYPE_CONFIG_OPTS) \
		     --disable-libtool-lock \
		     --enable-build-docs=no \
		     --enable-composite \
		     --enable-damage \
		     --enable-dpms \
		     --enable-dri2 \
		     --enable-glx \
		     --enable-randr \
		     --enable-record \
		     --enable-render \
		     --enable-resource \
		     --enable-screensaver \
		     --enable-shape \
		     --enable-shm \
		     --enable-sync \
		     --enable-xevie \
		     --enable-xfixes \
		     --enable-xfree86-dri \
		     --enable-xinerama \
		     --enable-xinput \
		     --enable-xprint \
		     --disable-selinux \
		     --enable-xtest \
		     --enable-xv \
		     --enable-xvmc \
		     --with-queue-size=32768 \
		     --with-pic

# Work around libtool's interference
DELIBTOOLIZE=yes

# pkg-config files that need to have -R added
FIX_PC_FILES=*.pc.in

include ../Makefile.inc
