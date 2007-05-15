###############################################################################
#
# Xft 2.x Makefile
#
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
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
# @(#)Makefile	1.7	07/05/15
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Package name used in tarballs
LIB_MODULE=libXft

# Version number (used in path names)
LIB_VERS=2.1.6

# Download site for source
SOURCE_URL=http://xlibs.freedesktop.org/release/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	xft.pc.patch \
	xftcore-6261221.patch \
	cjk-embolden.patch \
	xft.hint.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xft

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=Xft.3.in Xft.3
SUN_PACKAGE=SUNWxwxft
LIB_STABILITY=Volatile
LIB_MAN_SUFFIX=3

# Where to install files
LIB_PREFIX=$(OW_DIR)
LIB_PREFIX_SET=yes

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = --mandir=$(X11_DIR)/share/man

# Paths to find libraries at build time (LD_LIBRARY_PATH) 
# and run time (LD_RUN_PATH) and freetype-config binary at build time (PATH)
LIBPATHS = LD_LIBRARY_PATH=$(PROTODIR)/usr/sfw/lib$(LIBSUBDIR):$(PROTODIR)/usr/openwin/lib$(LIBSUBDIR):/usr/sfw/lib$(LIBSUBDIR) LD_RUN_PATH=/usr/sfw/lib$(LIBSUBDIR) PATH=$(PROTODIR)/usr/sfw/bin$(LIBSUBDIR):$(PATH)
LIB_ADD_MAKEFLAGS = bindir=$(XFT_prefix)/bin$(ARCHLIBSUBDIR)

# Include Paths - need headers for fontconfig & freetype from build area
LIB_ADD_CFLAGS = -I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/sfw/include

# Library built in tree
LIB_BUILT=$(BUILD_DIR)/.libs/libXft.so.2

# Mapfile used to control which symbols are exported
XFT_MAPFILE=$(PWD)/mapfile.scope
LIB_ADD_LD_OPTIONS=-M$(XFT_MAPFILE)

# pkg-config files that need to have -R added
# FIX_PC_FILES=xft.pc.in - already has -R

include ../Makefile.inc

