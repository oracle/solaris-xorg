###############################################################################
#
# FreeGLUT 2.6.0 Makefile
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
SOURCE_URL = http://sourceforge.net/projects/freeglut/files/freeglut/$(SOURCE_TARBALL_NAME)
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

MODULE_LD_OPTIONS = -L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)
MODULE_LD_OPTIONS += -lGL -lGLU
MODULE_LD_OPTIONS += -z combreloc -z redlocsym -z ignore

# We want GNU cp
GNUCP=/usr/gnu/bin/cp

# And GNU sed
GSED=/usr/gnu/bin/sed

# We must build with GNU make
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = $(MODTYPE_CONFIG_OPTS) \
		     --enable-shared \
		     --disable-static \
		     --disable-libtool-lock \
		     --x-includes=/usr/include \
		     --x-libraries=/usr/lib$(ARCHLIBSUBDIR) \
		     --with-x \
		     --with-pic

# -xF is needed for mapfile reordering to work
MODULE_CFLAGS = -xF -D__posix__ -D__unix__

#  FreeGLUT really wants to know that we are POSIX friendly
MODULE_CPPFLAGS = -D__posix__ -D__unix__
MODULE_CPPFLAGS += -I$(PROTODIR)/usr/X11/include \
		   -I$(PROTODIR)/usr/X11/include/X11

# Add -lGL and -lGLU to LDFLAGS
MODULE_LDFLAGS = -L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)
MODULE_LDFLAGS += -lGL -lGLU
MODULE_LDFLAGS += -z combreloc -z redlocsym -z ignore

# We must use GNU make
MODULE_ENV = \
	     SHELL="/bin/bash" \
	     MAKESHELL="/bin/bash" \
	     CONFIG_SHELL="/bin/bash" \
	     PROTODIR="$(PROTODIR)" \
	     CC="$(CC)" \
	     CPPFLAGS="$(MODULE_CPPFLAGS) $(CPPFLAGS)" \
	     CFLAGS="$(MODULE_CPPFLAGS) $(CPPFLAGS) $(CFLAGS)" \
	     LDFLAGS="$(LDFLAGS) $(MODULE_LDFLAGS)" \
	     LD="$(CC) $(CFLAGS) $(LDFLAGS) $(MODULE_LDFLAGS)" \
	     CPP="$(CC) -E $(MODULE_CPPFLAGS) $(CPPFLAGS) $(CFLAGS)" \
	     CXXCPP="$(CXX) -E $(CXXFLAGS)" \
	     MAKEFLAGS= \
	     MAKE="$(GNUMAKE)" \
	     CPPROG="$(GNUCP)"

MODULE_CONFIG_ENV = $(MODULE_ENV)
MODULE_BUILD_ENV = $(MODULE_ENV)

include ../Makefile.inc

# Freeglut needs libtool
DELIBTOOLIZE=no

# But we don't want autoreconf
AUTORECONF=no

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

