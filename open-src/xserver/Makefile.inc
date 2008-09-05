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
# @(#)Makefile.inc	1.7	08/09/05
#

PWD:sh=pwd
TOP=$(PWD)/../..

MODTYPE=xserver

### Include common definitions
include $(TOP)/common/Makefile.init

# Xorg server source tarball to use sources from
XORGSERVER_VERS=1.3.0.0

# Patches shared between Xorg & Xvnc builds
XORG_SOURCE_PATCHES:sh=cat -s ../xorg/patch-list ../xorg/closed-src/patch-list | sed '/^\#/ d'

XSERVER_FONTDIR=$(X11_DIR)/lib/X11/fonts
MESA_SRC_DIR=$(TOP)/lib/mesa/$(BUILD_DIR)/Mesa-7.0.3

SERVER_MODULES_DIR='$${libdir}/modules$(SERVERMOD_subdir)'
DRI_MODULES_DIR='$${libdir}/modules/dri$(SERVERMOD_subdir)'

# Command line options to GNU autoconf configure script for xorg-server
XSERVER_CONFIG_OPTS_COMMON = \
	--bindir='$${exec_prefix}/bin/$(ARCHBINSUBDIR)' \
        --disable-xdm-auth-1 --enable-xcsecurity \
	--enable-shared --disable-static \
	--disable-xsdl --disable-xwin --disable-xgl \
	--disable-kbd_mode \
	--disable-install-setuid \
	--with-vendor-name="Sun Microsystems, Inc." \
	--with-vendor-web="http://sunsolve.sun.com/" \
	--with-os-name="`uname -srmv`" \
	--with-log-dir=/var/log --with-xkb-output=/var/run/xkb \
	--with-module-dir=$(SERVER_MODULES_DIR) \
	--with-dri-driver-path=$(DRI_MODULES_DIR) \
	--with-xkb-path='$${libdir}/X11/xkb' \
	--with-rgb-path='$${libdir}/X11/rgb' \
	--with-default-font-path="$(XSERVER_FONTDIR)/TrueType/,$(XSERVER_FONTDIR)/Type1/,$(XSERVER_FONTDIR)/Type1/sun/,$(XSERVER_FONTDIR)/F3bitmaps/,$(XSERVER_FONTDIR)/misc/,$(XSERVER_FONTDIR)/100dpi/,$(XSERVER_FONTDIR)/75dpi/" 

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

MODTYPE_CPPFLAGS_suncc= # No extra flags needed
MODTYPE_CPPFLAGS_gcc= $(GCC_HEADERS)

MODTYPE_CPPFLAGS=  \
	-I$(MESA_SRC_DIR)/include \
	-I$(PROTODIR)$(X11_INCLUDES_DIR) \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions \
	$(DPMS_DEFINES) -DSUNSOFT -DTSOL \
	$(MODTYPE_CPPFLAGS_$(MODULE_COMPILER))

XSERVER_LDFLAGS_suncc= # No extra flags needed
XSERVER_LDFLAGS_gcc= -L/usr/sfw/lib$(ARCHLIBSUBDIR) \
		     -R/usr/sfw/lib$(ARCHLIBSUBDIR)

# Environment variable options to GNU autoconf configure script
#  - XSERVERCFLAGS_CFLAGS - force CFLAGS to avoid -I /usr/X11/include breaking
#    use of headers from proto area
MODTYPE_CONFIG_ENV = \
	XSERVERCFLAGS_CFLAGS="-DHAS_FCHOWN -DHAS_STICKY_DIR_BIT -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -DXTHREADS -DXUSE_MTSAFE_API" \
	PATH=$(PATH):/usr/sbin

MODTYPE_CFLAGS=$(PROG_CFLAGS)
MODTYPE_CXXFLAGS=$(PROG_CXXFLAGS)
MODTYPE_LDFLAGS=$(PROG_LDFLAGS) $(XSERVER_LDFLAGS_$(MODULE_COMPILER)) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR) \
	-R$(X11_DIR)/lib$(ARCHLIBSUBDIR)

MODTYPE_BUILD_32_FLAGS=ARCHBINSUBDIR="$(SUBDIR32)"

MODTYPE_BUILD_64_FLAGS_sparc=ARCHBINSUBDIR=""
MODTYPE_BUILD_64_FLAGS_i386=ARCHBINSUBDIR="$(SUBDIR64)"
MODTYPE_BUILD_64_FLAGS=$(MODTYPE_BUILD_64_FLAGS_$(MACH))

# Environment variable options to use when building
MODTYPE_BUILD_ENV= PATH=$(PATH):/usr/sbin \
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib/xorgcfg$(ARCHLIBSUBDIR)

### Include common rulesets
include $(TOP)/common/Makefile.inc

