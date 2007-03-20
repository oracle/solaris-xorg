###############################################################################
#
# Xorg server Makefile
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.36	07/03/19
#

PWD:sh=pwd
TOP=$(PWD)/../..

BUILD_TYPES_sun4=64
BUILD_TYPES_i86pc=32 64
BUILD_TYPES:sh=arch | sed 's/^\(.*\)$/\$\(BUILD_TYPES_\1\)/'

# Package name used in tarballs
MODULE_NAME=xorg-server

# Version number (used in path names)
XORGSERVER_VERS=1.2.0

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(XORGSERVER_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(XORG_RELEASES_URL)/xserver/$(SOURCE_TARBALL_NAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	xf1bpp.patch,-p1 \
	xephyr.patch,-p1 \
	immu.h.patch,-p1 \
	server-libs.patch \
	dtrace.patch \
	client-privates-leak.patch,-p1 \
	ddc.patch,-p1 \
	ast-driver.patch \
	sun-paths.patch \
	sun-extramodes.patch \
	sun-apm.patch \
	sun-kb-autodetect.patch \
	sun-keymappings.patch \
	sun-manpage.patch \
	cli-nolock.patch \
	cli-nobanner.patch \
	keep-aperture-open.patch \
	sun-virtual-mouse.patch \
	dtlogin-userinfo.patch \
	IA.patch \
	xtsol.patch \
	sun-loginfo.patch \
	xorgconfig.patch \
	xorgcfg.patch \
	amd64-loader-path.patch \
	Xorg-mapfile.patch \
	bitstream.patch \
	xevie.patch \
	sparc.patch,-p1 \
	wsfb.patch \
	sparc-probe.patch \
	glx-byteswap.patch \
	glx-impure.patch \
	dri.patch \
	nvidia-autoconfig.patch \
	autoconfig.patch \
	autoconfig2.patch \
	6406044.patch \
	cfbmskbits.patch \
	xf86ModesAdd.patch \
	lg3d.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(MODULE_NAME)-$(XORGSERVER_VERS)

# Prefix for installation
XORGSERVER_PREFIX=$(X11_DIR)

# Man pages to apply Sun footer to & attributes to list
# XXX: Need to adjust some of these to correct packages/stability
SUNTOUCHED_MANPAGES= \
	hw/xfree86/utils/gtf/gtf.man.pre \
	hw/xfree86/utils/cvt/cvt.man.pre \
	hw/xfree86/utils/pcitweak/pcitweak.man.pre \
	hw/xfree86/utils/scanpci/scanpci.man.pre \
	hw/xfree86/utils/xorgcfg/xorgcfg.man.pre \
	hw/xfree86/utils/xorgconfig/xorgconfig.man.pre \
	hw/xfree86/doc/man/Xorg.man.pre \
	hw/xfree86/doc/man/xorg.conf.man.pre 

SUNTOUCH_MAN_FLAGS= -p $(XORGSERVER_PREFIX)/bin/ \
 -a '{Availability, SUNWxorg-server} {Interface Stability, Volatile}'

# Binary built in tree
XORGSERVER_BIN=$(SOURCE_DIR)/hw/xfree86/Xorg

BUILD_TARGETS=$(XORGSERVER_BIN) $(BUILD_DIR)/mapfile-Xorg-externs

include $(TOP)/common/Makefile.inc

# Merge in additional sources from sun-src directory
source_gen:: $(LNDIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)

FONTDIR=$(X11_DIR)/lib/X11/fonts

MESA_SRC_DIR=$(TOP)/lib/mesa/$(BUILD_DIR)/Mesa-6.5.2

MODULES_DIR='$${libdir}/modules$(MODULE_subdir)'

# Command line options to GNU autoconf configure script
# XXX: enable xorgcfg --disable-dga enable security
CONFIG_OPTS_COMMON = \
	--prefix=$(XORGSERVER_PREFIX) --mandir='$${prefix}/share/man' \
	--bindir='$${exec_prefix}/bin/$(ARCHBINSUBDIR)' \
        --enable-builddocs --disable-xdm-auth-1 --enable-xcsecurity \
	--enable-shared --disable-static \
	--enable-kdrive --enable-xephyr \
	--disable-install-setuid --enable-xorgcfg \
	--with-vendor-name="Sun Microsystems, Inc." \
	--with-vendor-web="http://sunsolve.sun.com/" \
	--with-os-name="`uname -srmv`" \
	--with-log-dir=/var/log --with-xkb-output=/var/run/xkb \
	--with-module-dir=$(MODULES_DIR) \
	--with-xkb-path='$${libdir}/X11/xkb' \
	--with-default-font-path="$(FONTDIR)/TrueType/,$(FONTDIR)/Type1/,$(FONTDIR)/Type1/sun/,$(FONTDIR)/F3bitmaps/,$(FONTDIR)/misc/,$(FONTDIR)/100dpi/,$(FONTDIR)/75dpi/" 

CONFIG_OPTS_sun4=$(CONFIG_OPTS_COMMON)
CONFIG_OPTS_i86pc=$(CONFIG_OPTS_COMMON) --with-mesa-source=$(MESA_SRC_DIR)
CONFIG_OPTS:sh=arch | sed 's/^\(.*\)$/\$\(CONFIG_OPTS_\1\)/'


# Need to append header path with gcc headers for MMX/SSE intrinsics
GCC_HEADERS:sh=gcc -print-search-dirs | awk -F: '$1 == "install" {printf "-idirafter %s/install-tools/include", $2}'

# Default DPMS settings for Solaris
DPMS_DEFINES=   -DDEFAULT_STANDBY_TIME=1440000 \
		-DDEFAULT_SUSPEND_TIME=1620000 \
                -DDEFAULT_OFF_TIME=1800000

XORG_CPPFLAGS= \
	-I $(MESA_SRC_DIR)/include \
	-I $(PROTODIR)$(X11_INCLUDES_DIR) \
	-I $(PROTODIR)$(X11_INCLUDES_DIR)/drm \
	-I $(PROTODIR)$(X11_INCLUDES_DIR)/X11/dri \
	-I $(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions \
	$(DPMS_DEFINES) -DSUNSOFT -DTSOL

# Uncomment to build debug
# OPT_CFLAGS = -g
# DEFAULT_GCC_CFLAGS = -g -O3 -fno-omit-frame-pointer -Wall

# Environment variable options to GNU autoconf configure script
CONFIG_ENV_common = \
        PKG_CONFIG_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR):/usr/lib$(ARCHLIBSUBDIR)/pkgconfig \
	INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	PATH=$(PATH):/usr/sbin

LDPATH_ADD = -L$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

CONFIG_ENV_cc = CC=$(CC) CFLAGS="$(PROG_CFLAGS)" \
	LDFLAGS="$(PROG_LDFLAGS) $(LDPATH_ADD)" \
	CPPFLAGS="$(XORG_CPPFLAGS)"

CONFIG_ENV_gcc = CC=$(GCC) CFLAGS="$(DEFAULT_GCC_CFLAGS) $(ARCH_GCC_FLAGS)" \
	LDFLAGS="$(GCC_PROG_LDFLAGS) -L/usr/sfw/lib$(ARCHLIBSUBDIR) -R/usr/sfw/lib$(ARCHLIBSUBDIR) $(LDPATH_ADD)" \
	CPPFLAGS="$(GCC_HEADERS) $(XORG_CPPFLAGS)"

CONFIG_ENV_sun4= $(CONFIG_ENV_cc) $(CONFIG_ENV_common)
CONFIG_ENV_i86pc= $(CONFIG_ENV_gcc) $(CONFIG_ENV_common)
CONFIG_ENV:sh=arch | sed 's/^\(.*\)$/\$\(CONFIG_ENV_\1\)/'

BUILD_32_FLAGS=ARCH_GCC_FLAGS="$(ARCH32_GCC_FLAGS)" ARCHBINSUBDIR="$(SUBDIR32)"

BUILD_64_FLAGS_sun4=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)"
BUILD_64_FLAGS_i86pc=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)" ARCHBINSUBDIR="$(SUBDIR64)"
BUILD_64_FLAGS:sh=arch | sed 's/^\(.*\)$/\$\(BUILD_64_FLAGS_\1\)/'

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) && automake && autoconf && \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )
	(cd $(SOURCE_DIR)/hw/xfree86/xf1bpp && \
	 $(MAKE) $(MFLAGS) maintainer-clean-generic)

# Environment variable options to use when building
BUILD_ENV= PATH=$(PATH):/usr/sbin \
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib/xorgcfg$(ARCHLIBSUBDIR)

#LD_OPTIONS="-z textwarn"

$(XORGSERVER_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(BUILD_ENV) $(MAKE) $(MFLAGS))

install_gen: $(XORGSERVER_BIN)
	(cd $(SOURCE_DIR) ;  $(BUILD_ENV) $(MAKE) $(MFLAGS) -e install \
	 DESTDIR=$(PROTODIR) CPPROG="cp -p" \
	 pkgconfigdir=/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig )
	sed -e 's%#define XF86DRI 1%/* #define XF86DRI 1 */%' \
	    -e 's%^.* _XSERVER64 .*$$%#include <sys/isa_defs.h>\
#ifdef _LP64\
#define _XSERVER64 1\
#endif%' \
	    $(SOURCE_DIR)/include/xorg-server.h > $(PROTODIR)$(X11_INCLUDES_DIR)/xorg/xorg-server.h
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	cp -p $(SOURCE_DIR)/hw/xfree86/doc/README.* $(PROTODIR)$(X11_DOC_DIR)/

$(BUILD_DIR)/mapfile-Xorg-externs: $(XORGSERVER_BIN)
	-rm -f $@
	printf '# Functions exported by Xorg server to loadable modules\n\n' > $@
	printf '{\n  global:\n' >> $@
	/usr/ccs/bin/nm -g $(XORGSERVER_BIN) | grep -v UNDEF | sed -n -e 's/^.*\|FUNC \|GLOB \|.*\|\([A-Za-z].*\)$$/    \1 = FUNCTION extern;/p' -e 's/^.*\|OBJT \|GLOB \|.*\|\([A-Za-z].*\)$$/    \1 = DATA extern;/p' >> $@
	printf '};\n' >> $@
