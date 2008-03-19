# -*- Makefile -*- rules common to both Xorg & Xvnc builds
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
# @(#)Makefile.inc	1.3	08/03/19
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Xorg server source tarball to use sources from
XORGSERVER_VERS=1.3.0.0

include $(TOP)/common/Makefile.inc

# Prefix for installation
XSERVER_PREFIX=$(X11_DIR)

FONTDIR=$(X11_DIR)/lib/X11/fonts
MESA_SRC_DIR=$(TOP)/lib/mesa/$(BUILD_DIR)/Mesa-6.5.2

MODULES_DIR='$${libdir}/modules$(MODULE_subdir)'
DRI_MODULES_DIR='$${libdir}/modules/dri$(MODULE_subdir)'

# Command line options to GNU autoconf configure script for xorg-server
XSERVER_CONFIG_OPTS_COMMON = \
	--prefix=$(XSERVER_PREFIX) --mandir='$${prefix}/share/man' \
	--bindir='$${exec_prefix}/bin/$(ARCHBINSUBDIR)' \
        --disable-xdm-auth-1 --enable-xcsecurity \
	--enable-shared --disable-static \
	--disable-xsdl --disable-xwin --disable-xgl \
	--disable-install-setuid \
	--with-vendor-name="Sun Microsystems, Inc." \
	--with-vendor-web="http://sunsolve.sun.com/" \
	--with-os-name="`uname -srmv`" \
	--with-log-dir=/var/log --with-xkb-output=/var/run/xkb \
	--with-module-dir=$(MODULES_DIR) \
	--with-dri-driver-path=$(DRI_MODULES_DIR) \
	--with-xkb-path='$${libdir}/X11/xkb' \
	--with-rgb-path='$${libdir}/X11/rgb' \
	--with-default-font-path="$(FONTDIR)/TrueType/,$(FONTDIR)/Type1/,$(FONTDIR)/Type1/sun/,$(FONTDIR)/F3bitmaps/,$(FONTDIR)/misc/,$(FONTDIR)/100dpi/,$(FONTDIR)/75dpi/" 

XSERVER_CONFIG_OPTS_sparc=$(XSERVER_CONFIG_OPTS_COMMON)
XSERVER_CONFIG_OPTS_i386=$(XSERVER_CONFIG_OPTS_COMMON) \
	--with-mesa-source=$(MESA_SRC_DIR)
XSERVER_CONFIG_OPTS=$(XSERVER_CONFIG_OPTS_$(MACH))

# Need to append header path with gcc headers for MMX/SSE intrinsics
GCC_HEADERS:sh=gcc -print-search-dirs | awk -F: '$1 == "install" {printf "-idirafter %s/install-tools/include", $2}'

# Default DPMS settings for Solaris
DPMS_DEFINES=   -DDEFAULT_STANDBY_TIME=1440000 \
		-DDEFAULT_SUSPEND_TIME=1620000 \
                -DDEFAULT_OFF_TIME=1800000

XSERVER_CPPFLAGS= \
	-I$(MESA_SRC_DIR)/include \
	-I$(PROTODIR)$(X11_INCLUDES_DIR) \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions \
	$(DPMS_DEFINES) -DSUNSOFT -DTSOL

# Environment variable options to GNU autoconf configure script
#  - XSERVERCFLAGS_CFLAGS - force CFLAGS to avoid -I /usr/X11/include breaking
#    use of headers from proto area
CONFIG_ENV_common = \
        PKG_CONFIG_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR):/usr/lib$(ARCHLIBSUBDIR)/pkgconfig \
	INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	XSERVERCFLAGS_CFLAGS="-DHAS_FCHOWN -DHAS_STICKY_DIR_BIT -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -DXTHREADS -DXUSE_MTSAFE_API" \
	PATH=$(PATH):/usr/sbin

LDPATH_ADD = -L$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR) \
	-R$(X11_DIR)/lib$(ARCHLIBSUBDIR)

CONFIG_ENV_cc = CC=$(CC) CFLAGS="$(PROG_CFLAGS)" \
	CXX=$(CXX) CXXFLAGS="$(PROG_CXXFLAGS)" \
	LDFLAGS="$(PROG_LDFLAGS) $(LDPATH_ADD)" \
	CPPFLAGS="$($(XSERVER)_CPPFLAGS)"

CONFIG_ENV_gcc = CC=$(GCC) CFLAGS="$(DEFAULT_GCC_CFLAGS) $(ARCH_GCC_FLAGS)" \
	CXX=$(GCC:gcc=g++) CXXFLAGS="$(DEFAULT_GCC_CFLAGS) $(ARCH_GCC_FLAGS)" \
	LDFLAGS="$(GCC_PROG_LDFLAGS) -L/usr/sfw/lib$(ARCHLIBSUBDIR) -R/usr/sfw/lib$(ARCHLIBSUBDIR) $(LDPATH_ADD)" \
	CPPFLAGS="$(GCC_HEADERS) $($(XSERVER)_CPPFLAGS)"

CONFIG_ENV= $(CONFIG_ENV_$(COMPILER)) $(CONFIG_ENV_common)

BUILD_32_FLAGS=ARCH_GCC_FLAGS="$(ARCH32_GCC_FLAGS)" ARCHBINSUBDIR="$(SUBDIR32)"

BUILD_64_FLAGS_sparc=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)"
BUILD_64_FLAGS_i386=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)" ARCHBINSUBDIR="$(SUBDIR64)"
BUILD_64_FLAGS=$(BUILD_64_FLAGS_$(MACH))

# Environment variable options to use when building
BUILD_ENV= PATH=$(PATH):/usr/sbin \
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib/xorgcfg$(ARCHLIBSUBDIR)
