###############################################################################
#
# X11 1.x Makefile
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.7	08/05/02
#

# Package name used in tarballs
LIB_MODULE=libX11

# Version number (used in path names)
LIB_VERS=1.1.3

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	lintlib.patch,-p1 \
	manpages.patch,-p1 \
	XErrorDB.patch,-p1 \
	1234757.patch \
	4010755.patch \
	4614834.patch \
	5039226.patch \
	6409332.patch \
	solaris-abi.patch \
	solaris-i18n.patch \
	solaris-kbd.patch \
	SolarisIA.patch \
	sme.patch \
	4076297.patch \
	1261271.patch \
	6686591.patch \
	6690054.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=X11

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man man/xkb/*.man
SUN_PACKAGE=SUNWxwplt
LIB_STABILITY=Committed
LIB_MAN_SUFFIX=3X11

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = --enable-xthreads --enable-loadable-i18n \
	--with-xcb=no --enable-loadable-xcursor --enable-xcms \
	--enable-xlocaledir --enable-xkb --enable-man-pages=$(LIB_MAN_SUFFIX) \
	--with-lint --enable-lint-library \
	--with-locale-lib-dir='$${exec_prefix}/lib/X11/locale$(LIBSUBDIR)'

# -xF is needed for mapfile reordering to work
LIB_ADD_CFLAGS = -xF -g

# -DSUNSOFT enables SUN_SME (Shared Memory transport extension)
# -DMOTIFBC enables compatibility with bugs in Motif & other apps
# -DSUNSOFT_KBD enables Solaris traditional keyboard handling
LIB_ADD_CPPFLAGS = -DSUNSOFT -DMOTIFBC -DSUNSOFT_KBD

# Work around libtool's interference
LIB_ADD_BUILD_DEPS = delibtoolize

# Use -KPIC instead of -Kpic because we have too many symbols on SPARC
DELIBTOOLIZE_FLAGS_sparc = -P
DELIBTOOLIZE_FLAGS_i386 = 
DELIBTOOLIZE_FLAGS = $(DELIBTOOLIZE_FLAGS_$(MACH))

# Need additional mapfile for additional symbols in 64-bit versions
BUILD_64_FLAGS = ADD_libX11_LDFLAGS="-M $(PWD)/mapfile-vers-64"

# Library built in tree
LIB_BUILT=$(SOURCE_DIR)/src/.libs/libX11.so.4

# pkg-config files that need to have -R added
FIX_PC_FILES=x11.pc.in

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=true

include ../Makefile.inc

# Merge in additional sources from sun-src directory
source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)

install_gen::
	mkdir -p $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)
	-rm -f $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)/libX11.so
	ln -s libX11.so.4 $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)/libX11.so

# Install additional headers we need in other parts of the build or ship in
# packages, but which X.Org doesn't
install_32::
	mkdir -p $(PROTODIR)/usr/X11/include/X11/extensions
	cp -pf sun-src/include/X11/Xvarargs.h $(PROTODIR)/usr/X11/include/X11/
	cp -pf sun-src/include/X11/extensions/XInteractive.h \
		$(PROTODIR)/usr/X11/include/X11/extensions/
	cp -pf sun-src/include/X11/extensions/XInteractive.h \
		$(PROTODIR)/usr/X11/include/X11/extensions/
	cp -pf $(BUILD_DIR_32)$(SOURCE_DIR)/src/ks_tables.h \
		$(PROTODIR)/usr/X11/include/
	cp -pf $(BUILD_DIR_32)$(SOURCE_DIR)/src/xlibi18n/Xlc*.h \
		$(PROTODIR)/usr/X11/include/X11/
