# -*- Makefile -*- rules common to both Xorg & Xvnc builds
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile.inc	1.13	09/07/07
#

PWD:sh=pwd
TOP=$(PWD)/../..

MODTYPE=xserver

### Include common definitions
include $(TOP)/common/Makefile.init

# Xorg server source tarball to use sources from
XORGSERVER_VERS=1.6.2

# Patches shared between Xorg & Xvnc builds
XORG_SOURCE_PATCHES:sh=cat -s ../xorg/patch-list ../xorg/closed-src/patch-list | sed '/^\#/ d'

XSERVER_FONTDIR=$(X11_DIR)/lib/X11/fonts
MESA_SRC_DIR=$(TOP)/lib/mesa/$(BUILD_DIR)/Mesa-7.4.2

SERVER_MODULES_DIR='$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)'
DRI_MODULES_DIR='$(X11_SERVERMODS_DIR)/dri$(SERVERMOD_subdir)'

# Command line options to GNU autoconf configure script for xorg-server
XSERVER_CONFIG_OPTS_COMMON = \
	--bindir='$${exec_prefix}/bin/$(ARCHBINSUBDIR)' \
        --disable-xdm-auth-1 --enable-xcsecurity \
	--enable-shared --disable-static \
	--disable-xsdl --disable-xwin --disable-xnest \
	--disable-install-setuid \
	--with-vendor-name="Sun Microsystems, Inc." \
	--with-vendor-web="http://sunsolve.sun.com/" \
	--with-os-name="`uname -srm`" \
	--with-log-dir=/var/log --with-xkb-output=/var/run/xkb \
	--with-module-dir=$(SERVER_MODULES_DIR) \
	--with-dri-driver-path=$(DRI_MODULES_DIR) \
	--with-xkb-path='$${libdir}/X11/xkb' \
	--with-serverconfig-path='$${libdir}/X11/xserver' \
	--with-default-font-path="catalogue:/etc/X11/fontpath.d" \
	--with-default-xkb-rules=xorg \
	--with-default-xkb-model=pc105 \
	$(ADDITIONAL_CONFIG_OPTS)

XSERVER_CONFIG_OPTS_sparc=$(XSERVER_CONFIG_OPTS_COMMON) \
	--disable-glx --disable-dri2
XSERVER_CONFIG_OPTS_i386=$(XSERVER_CONFIG_OPTS_COMMON) \
	--with-mesa-source=$(MESA_SRC_DIR) --enable-dri2
XSERVER_CONFIG_OPTS=$(XSERVER_CONFIG_OPTS_$(MACH))

# Default DPMS settings for Solaris
DPMS_DEFINES=   -DDEFAULT_STANDBY_TIME=900000 \
		-DDEFAULT_SUSPEND_TIME=1080000 \
                -DDEFAULT_OFF_TIME=1260000

MODTYPE_CPPFLAGS_suncc= # No extra flags needed
MODTYPE_CPPFLAGS_gcc= # No extra flags needed

MODTYPE_CPPFLAGS=  \
	-I$(PROTODIR)/usr/include \
	-I$(PROTODIR)/usr/include/pixman-1 \
	-I$(PROTODIR)$(X11_INCLUDES_DIR) \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/drm \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/X11/dri \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/X11/extensions \
	$(DPMS_DEFINES) -DSUNSOFT -DTSOL \
	$(MODTYPE_CPPFLAGS_$(MODULE_COMPILER))

XSERVER_LDFLAGS_suncc= # No extra flags needed
XSERVER_LDFLAGS_gcc= -L/usr/sfw/lib$(ARCHLIBSUBDIR) \
		     -R/usr/sfw/lib$(ARCHLIBSUBDIR)

# Environment variable options to GNU autoconf configure script
#  - XSERVERCFLAGS_CFLAGS - force CFLAGS to avoid -I /usr/X11/include breaking
#    use of headers from proto area
#  - PATH - include /usr/sbin for dtrace command (needed for building probes)
MODTYPE_CONFIG_ENV = \
	XSERVERCFLAGS_CFLAGS="-DHAS_FCHOWN -DHAS_STICKY_DIR_BIT -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -DXTHREADS -DXUSE_MTSAFE_API" \
	PATH=$(PATH):/usr/sbin

# 64-bit X servers on SPARC crash on startup when built with Sun Studio 
# with -g unless -xmemalign=8i is also used (Sun bug #6545113)
MODTYPE_DEBUG_FLAGS_sparc_suncc = -xmemalign=8i
MODTYPE_DEBUG_FLAGS=$(MODTYPE_DEBUG_FLAGS_$(MACH)_$(MODULE_COMPILER)) -DDEBUG

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
#  - PATH - include /usr/sbin for dtrace command (needed for building probes)
MODTYPE_BUILD_ENV= PATH=$(PATH):/usr/sbin \
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib/xorgcfg$(ARCHLIBSUBDIR)

### Include common rulesets
include $(TOP)/common/Makefile.inc

