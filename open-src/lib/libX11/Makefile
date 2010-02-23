###############################################################################
#
# X11 1.x Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
#

# Package name used in tarballs
MODULE_NAME=libX11

# Version number (used in path names)
MODULE_VERSION=1.3.2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	lintlib.patch,-p1 \
	1234757.patch \
	4010755.patch \
	4614834.patch \
	5039226.patch \
	solaris-abi.patch \
	solaris-kbd.patch \
	SolarisIA.patch \
	4076297.patch \
	1261271.patch \
	6714036.patch \
	6789219.patch \
	sme.patch \
	im-suneu-greek.patch \
	im-ximcp.patch \
	lc-jisx0212.patch \
	lc-jisx0208.patch \
	lc-ksc5601.1992-3.patch \
	lc-gb18030.patch \
	om-generic.patch \
	compose.dir.patch \
	compose-iso8859-1.patch \
	compose-iso8859-2.patch \
	compose-iso8859-5.patch \
	compose-iso8859-6.patch \
	compose-iso8859-7.patch \
	compose-iso8859-8.patch \
	compose-iso8859-9.patch \
	compose-iso8859-13.patch \
	compose-iso8859-15.patch \
	compose-el_GR.UTF-8.patch \
	compose-en_US.UTF-8.patch \
	compose-ja_JP.UTF-8.patch \
	compose-ko_KR.UTF-8.patch \
	compose-pt_BR.UTF-8.patch \
	compose-th_TH.UTF-8.patch \
	compose-zh_CN.UTF-8.patch \
	compose-zh_HK.UTF-8.patch \
	compose-zh_TW.UTF-8.patch \
	locale.aliases.patch \
	locale.paths.patch \
	locale-C.patch \
	locale-iso8859-1.patch \
	locale-iso8859-6.patch \
	locale-iso8859-8.patch \
	locale-en_US.UTF-8.patch \
	locale-ja.SJIS.patch \
	locale-ja.patch \
	locale-ja_JP.UTF-8.patch \
	locale-ko.patch \
	locale-ko_KR.UTF-8.patch \
	locale-th_TH.UTF-8.patch \
	locale-zh_CN.UTF-8.patch \
	locale-zh_CN.gb18030.patch \
	locale-zh_HK.UTF-8.patch \
	locale-zh_TW.UTF-8.patch \
	locale-zh_TW.patch \
	locale-ru-RU.UTF-8.patch \
	6913809.patch 

# Library name (used for specfiles/mapfiles)
LIBNAME=X11

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man man/xkb/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3X11

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --enable-xthreads --enable-loadable-i18n \
	--disable-specs \
	--with-xcb=no --enable-loadable-xcursor --enable-xcms \
	--enable-xlocaledir --enable-xkb --enable-man-pages=$(LIB_MAN_SUFFIX) \
	--with-locale-lib-dir='$${exec_prefix}/lib/X11/locale$(LIBSUBDIR)'

# -xF is needed for mapfile reordering to work
MODULE_CFLAGS = -xF

# -DSUNSOFT enables SUN_SME (Shared Memory transport extension)
# -DMOTIFBC enables compatibility with bugs in Motif & other apps
# -DSUNSOFT_KBD enables Solaris traditional keyboard handling
MODULE_CPPFLAGS = -DSUNSOFT -DMOTIFBC -DSUNSOFT_KBD

# Backwards compatibility for old paths until g11n moves all input methods
MODULE_CONFIG_ENV = \
 XLOCALEDIR="/usr/share/X11/locale:/usr/X11/share/X11/locale" \
 XLOCALELIBDIR="/usr/lib/X11/locale$(LIBSUBDIR):/usr/X11/lib/X11/locale$(LIBSUBDIR)"

# Work around libtool's interference
DELIBTOOLIZE=yes

# Use -KPIC instead of -Kpic because we have too many symbols on SPARC
DELIBTOOLIZE_FLAGS_sparc = -P
DELIBTOOLIZE_FLAGS_i386 = 
DELIBTOOLIZE_FLAGS = $(DELIBTOOLIZE_FLAGS_$(MACH))

# Need additional mapfile for additional symbols in 64-bit versions
MODULE_BUILD_64_FLAGS = ADD_libX11_LDFLAGS="-M $(PWD)/mapfile-vers-64"

# pkg-config files that need to have -R added
FIX_PC_FILES=x11.pc.in

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

include ../Makefile.inc

# Install additional headers we need in other parts of the build or ship in
# packages, but which X.Org doesn't
install_32: install_extra_headers

install_extra_headers: build_32
	mkdir -p $(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions
	cp -pf sun-src/include/X11/Xvarargs.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/
	cp -pf sun-src/include/X11/extensions/XInteractive.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions/
	cp -pf $(BUILD_DIR_32)$(SOURCE_DIR)/src/ks_tables.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/
	cp -pf $(BUILD_DIR_32)$(SOURCE_DIR)/src/xlibi18n/Xlc*.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/
