###############################################################################
#
# Copyright (c) 2008, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION = 1.6.3

# Checksums for upstream tarball
TARBALL_MD5   = 2e36b73f8a42143142dda8129f02e4e0
TARBALL_SHA1  = 6f2aadf8346ee00b7419bd338461c6986e274733
TARBALL_SHA256= cf31a7c39f2f52e8ebd0db95640384e63451f9b014eed2bb7f5de03e8adc8111

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	1234757.patch,-p1 \
	4010755.patch,-p1 \
	4614834.patch,-p1 \
	5039226.patch,-p1 \
	solaris-abi.patch,-p1 \
	solaris-kbd.patch,-p1 \
	SolarisIA.patch,-p1 \
	6789219.patch,-p1 \
	im-suneu-greek.patch,-p1 \
	im-ximcp.patch,-p1 \
	lc-jisx0212.patch,-p1 \
	lc-jisx0208.patch,-p1 \
	lc-ksc5601.1992-3.patch,-p1 \
	lc-gb18030.patch,-p1 \
	om-generic.patch,-p1 \
	locale.aliases.patch,-p1 \
	locale.paths.patch,-p1 \
	locale-iso8859-6.patch,-p1 \
	locale-en_US.UTF-8.patch,-p1 \
	locale-ja.SJIS.patch,-p1 \
	locale-ja.patch,-p1 \
	locale-ja_JP.UTF-8.patch,-p1 \
	locale-ko.patch,-p1 \
	locale-ko_KR.UTF-8.patch,-p1 \
	locale-th_TH.UTF-8.patch,-p1 \
	locale-zh_CN.UTF-8.patch,-p1 \
	locale-zh_CN.gb18030.patch,-p1 \
	locale-zh_HK.UTF-8.patch,-p1 \
	locale-zh_TW.UTF-8.patch,-p1 \
	locale-zh_TW.patch,-p1

# Library name (used for specfiles/mapfiles)
LIBNAME=X11

# Dates to show in Oracle copyright notice in pkg license file
# Earliest & latest of the copyrights in the Oracle files in this pkg
ORACLE_COPYRIGHT_YEARS = 1986, 2016

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
MODULE_PKGCONFIG_MAN=x11
MODULE_PKGCONFIG_MAN_SET=yes

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --enable-xthreads --enable-loadable-i18n \
	--disable-xf86bigfont --disable-secure-rpc \
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

# Need to discard -lc in .so's that don't need it, but leave it for those
# which do need to link to it.
MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP)

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

# pkg-config files that need to be updated
FIX_PC_FILES=x11.pc.in x11-xcb.pc.in

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
	mkdir -p $(TOOLS_DIR)$(X11_INCLUDES_DIR)/X11/xcms
	cp -pf $(SOURCE_DIR)/src/xcms/*.h \
		$(TOOLS_DIR)$(X11_INCLUDES_DIR)/X11/xcms

# Install additional links to libX11 that we include in our packages
EXTRA_PROTO_LINKS = $(EXTRA_LINKS:%=$(PROTODIR)$(MODULE_FULLARCH_LIB_DIR)/%)

$(EXTRA_PROTO_LINKS): $(INSTALL_DEPS)
	@rm -f $@
	ln -s libX11.so.4 $@

install_extra_links: $(EXTRA_PROTO_LINKS)
