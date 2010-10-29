###############################################################################
#
# xcbutil 0.3.6 Makefile
#
# Copyright 2010, Oracle and/or its affiliates. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
# 
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#

# Package name used in tarballs
MODULE_NAME=xcb-util

# Version number (used in path names)
MODULE_VERSION=0.3.6

# Checksums for upstream tarball
TARBALL_MD5  = dd8968b8ee613cb027a8ef1fcbdc8fc9
TARBALL_SHA1 = 8585a421c077382937d181b921263fac2e448d31

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109
#
# Package metadata
UPSTREAM = The XCB Project
UPSTREAM_URL = http://xcb.freedesktop.org/
UPSTREAM_SET = yes

# Download site for source
SOURCE_URL = http://xcb.freedesktop.org/dist/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = xcb_icccm.h.0.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=xcbutil

# We need GNU M4 and not SVR4 M4
GM4 = /usr/gnu/bin/m4

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
GNUMAKEFLAGS=-j1 --jobs=1

# Environment variables to pass to configure & build steps
MODULE_ENV = \
	     M4=$(GM4) \
	     MAKE=$(GNUMAKE) \
	     GNUMAKE=$(GNUMAKE)

include ../Makefile.inc

# We need libtool
DELIBTOOLIZE=no

# But we don't want autoreconf 
AUTORECONF=no

MODULE_CONFIG_ENV = env \
		    M4=$(GM4) \
		    PKG_CONFIG_PATH=$(DEFAULT_PKG_CONFIG_PATH) \
		    MAKE="$(GNUMAKE) $(GNUMAKEFLAGS)" \
		    GNUMAKE=$(GNUMAKE)

MODULE_BUILD_ENV = env \
		   M4=$(GM4) \
		   PKG_CONFIG_PATH=$(DEFAULT_PKG_CONFIG_PATH) \
		   MAKE="$(GNUMAKE) $(GNUMAKEFLAGS)" \
		   GNUMAKE=$(GNUMAKE)

