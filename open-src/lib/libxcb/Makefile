###############################################################################
#
# xcb 1.7 Makefile
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
MODULE_NAME=libxcb

# Version number (used in path names)
MODULE_VERSION=1.7

# Checksums for upstream tarball
TARBALL_MD5  = 925699df361b99491165ebc12068056b
TARBALL_SHA1 = 7540f0587907bce421914f0ddb813810cb2f36f8

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
UPSTREAM = The XCB Project
UPSTREAM_URL = http://xcb.freedesktop.org/
UPSTREAM_SET = yes

# Download site for source
SOURCE_URL = http://xcb.freedesktop.org/dist//$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = Makefile.am.0.patch \
		 configure.1.patch \
		 aclocal.m4.2.patch \
		 xcb_conn.c.3.patch \
		 Makefile.am.4.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=xcb

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=

MODULE_LD_OPTIONS = -L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)
LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)

# We need Python
PYTHON_VERSION=2.6
PYTHON=/usr/bin/python$(PYTHON_VERSION)
PYTHON_PATH=$(PROTODIR)/usr/lib/python$(PYTHON_VERSION)/vendor-packages:$(PROTODIR)/usr/lib/python$(PYTHON_VERSION)/site-packages:/usr/lib/python$(PYTHON_VERSION)/site-packages:/usr/lib/python$(PYTHON_VERSION)/vendor-packages
PYTHONPATH=$(PYTHON_PATH)

# We want GNU cp
GNUCP=/usr/gnu/bin/cp

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

MODULE_CPPFLAGS = -I$(PROTODIR)/usr/include/X11 \
		  -I$(PROTODIR)/usr/include

# pkg-config files that need to have -R added
FIX_PC_FILES=*.pc.in

include ../Makefile.inc

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes
AUTORECONF_FLAGS = -vif

# We need PYTHON and PYTHONPATH in our environment
MODULE_BUILD_ENV = PYTHON="$(PYTHON)" \
		   PYTHONPATH="$(PYTHON_PATH)" \
		   PROTODIR="$(PROTODIR)" \
		   CC="$(CC)" \
		   CFLAGS="$(CFLAGS)" \
		   CPPFLAGS="$(CPPFLAGS)" \
		   LDFLAGS="$(LDFLAGS)" \
		   LD_OPTIONS="$(MODULE_LD_OPTIONS)" \
		   LD="$(CC) $(CFLAGS) $(LDFLAGS)" \
		   LD_LIBRARY_PATH="$(LD_LIBRARY_PATH)" \
		   LD_OPTIONS="$(MODULE_LD_OPTIONS)"

