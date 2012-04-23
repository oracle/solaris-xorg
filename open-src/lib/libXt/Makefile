###############################################################################
#
# X Toolkit Intrinsics library (libXt)
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXt

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/library/toolkit/libxt
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.1.3

# Checksums for upstream tarball
TARBALL_MD5   = a6f137ae100e74ebe3b71eb4a38c40b3
TARBALL_SHA1  = 2cef068bc4d7170e31b89b5c06aeaf4451a7699a
TARBALL_SHA256= 8db593c3fc5ffc4e9cd854ba50af1eac9b90d66521ba17802b8f1e0d2d7f05bd

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 6671721.patch,-p1

# Regenerate configure script & Makefile.in's after applying patches
AUTORECONF=yes

# Work around libtool's interference
DELIBTOOLIZE=yes

# Library name (used for specfiles/mapfiles)
LIBNAME=Xt

# ARC cases that covered this module
# PSARC/1997/141 Sym links needed in /usr/lib for SPARC ABI compliance
MODULE_ARC_CASES = PSARC/1997/141
# PSARC/2012/151 libXt 1.1 [XtAsprintf()]
MODULE_ARC_CASES += PSARC/2012/151

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed
MODULE_MTLEVEL=See XtToolkitThreadInitialize(3XT)
MODULE_MTLEVEL_SET=yes
LIB_MAN_SUFFIX=3xt

# pkg-config files that need to have -R added
FIX_PC_FILES=xt.pc.in

# the xinline option is here to fix bug 6748586, No Hands lib
# needs to interpose on these functions, and if they are inline
# then the loader can't find them.  It complains silently.
MODULE_CFLAGS = -DGETVALUES_BUG -xinline=no%_XtAppCreateShell,no%XtAppNextEvent,no%XtAppProcessEvent

# Extra links to libXt for compatibility with SVID / SPARC ABI / etc.
EXTRA_LINKS = libXt.so.5
MODULE_ADD_INSTALL_TARGETS = install_extra_links

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXt.so libXt.so.4 $(EXTRA_LINKS)

include ../Makefile.inc

# Install additional links to libXt that we include in our packages
EXTRA_PROTO_LINKS = $(EXTRA_LINKS:%=$(PROTODIR)$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)/%)

$(EXTRA_PROTO_LINKS): $(INSTALL_DEPS)
	@rm -f $@
	ln -s libXt.so.4 $@

install_extra_links: $(EXTRA_PROTO_LINKS)
