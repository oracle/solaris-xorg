###############################################################################
#
# Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libX11

# Version number (used in path names)
MODULE_VERSION = 1.4.3

# Checksums for upstream tarball
TARBALL_MD5  = 85e942627aaa020813e0eb8433353563
TARBALL_SHA1 = 174270a0e51614b5077026fc6c20701d4e09aef8

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	xml-install.patch,-p1 \
	xml-install-paths.patch,-p1 \
	7038737.patch,-p1 \
	1234757.patch \
	4010755.patch \
	4614834.patch \
	5039226.patch \
	solaris-abi.patch \
	solaris-kbd.patch \
	SolarisIA.patch \
	6714036.patch \
	6789219.patch \
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
	locale.dir.patch \
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
	locale-pt_BR.UTF-8.patch \
	locale-th_TH.UTF-8.patch \
	locale-zh_CN.UTF-8.patch \
	locale-zh_CN.gb18030.patch \
	locale-zh_HK.UTF-8.patch \
	locale-zh_TW.UTF-8.patch \
	locale-zh_TW.patch \
	locale-ru-RU.UTF-8.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=X11

# ARC cases that covered this module
# PSARC/1992/077 X11/R5 Xlib API
MODULE_ARC_CASES = PSARC/1992/077
# PSARC/1992/082 Dynamic Locale Access For X11/R5 Xlib
MODULE_ARC_CASES += PSARC/1992/082
# PSARC/1997/141 Sym links needed in /usr/lib for SPARC ABI compliance
MODULE_ARC_CASES += PSARC/1997/141
# PSARC/2006/448 UTF8_STRING support in Solaris libX11 and XLC modules
MODULE_ARC_CASES += PSARC/2006/448
# PSARC/2008/062  Porting of Xorg XI18N
MODULE_ARC_CASES += PSARC/2008/062
# PSARC/2009/666 libX11 1.3 
MODULE_ARC_CASES += PSARC/2009/666
# PSARC/2010/109 Introduce libXcb in Solaris
MODULE_ARC_CASES += PSARC/2010/109

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man man/xkb/*.man
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3x11

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --enable-xthreads --enable-loadable-i18n \
	--disable-xf86bigfont \
	--with-xcb=yes --enable-loadable-xcursor --enable-xcms \
	--enable-xlocaledir --enable-xkb --enable-man-pages=$(LIB_MAN_SUFFIX) \
	--with-locale-lib-dir='$${exec_prefix}/lib/X11/locale$(LIBSUBDIR)'

# -xF is needed for mapfile reordering to work
MODULE_CFLAGS = -xF

# -DSUNSOFT enables Solaris extensions
# -DMOTIFBC enables compatibility with bugs in Motif & other apps
# -DSUNSOFT_KBD enables Solaris traditional keyboard handling
MODULE_CPPFLAGS = -DSUNSOFT -DMOTIFBC -DSUNSOFT_KBD \
		  -I$(PROTODIR)/usr/include/X11 \
		  -I$(PROTODIR)/usr/include/xcb

MODULE_LDFLAGS = -R$(X11_LIB_DIR)$(ARCHLIBSUBDIR)

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

# Install additional headers we need in other parts of the build or ship in
# packages, but which X.Org doesn't
MODULE_ADD_INSTALL_TARGETS_32 = install_extra_headers
MODULE_ADD_INSTALL_TARGETS = $(MODULE_ADD_INSTALL_TARGETS_$(BUILD_TYPE))

# Extra links to libX11 for compatibility with SVID / SPARC ABI / etc.
EXTRA_LINKS = libX.so libX.so.4 libX11.so.5
MODULE_ADD_INSTALL_TARGETS += install_extra_links

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libX11.so libX11.so.4 $(EXTRA_LINKS)

include ../Makefile.inc

install_extra_headers: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions
	cp -pf sun-src/include/X11/Xvarargs.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/
	cp -pf sun-src/include/X11/extensions/XInteractive.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions/
	cp -pf $(SOURCE_DIR)/src/xlibi18n/Xlc*.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/

# Install additional links to libX11 that we include in our packages
EXTRA_PROTO_LINKS = $(EXTRA_LINKS:%=$(PROTODIR)$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)/%)

$(EXTRA_PROTO_LINKS): $(INSTALL_DEPS)
	@rm -f $@
	ln -s libX11.so.4 $@

install_extra_links: $(EXTRA_PROTO_LINKS)
