# -*- Makefile -*- rules common to both Xorg & Xvnc builds
#
# Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
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

PWD:sh=pwd
TOP=$(PWD)/../..

MODTYPE=xserver

### Include common definitions
include $(TOP)/common/Makefile.init

# Xorg server source tarball to use sources from
XORGSERVER_VERS=1.7.7

# Patches shared between Xorg & Xvnc builds
XORG_SOURCE_PATCHES:sh=cat -s ../xorg/patch-list ../xorg/closed-src/patch-list | sed '/^\#/ d'

SERVER_MODULES_DIR='$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)'
DRI_MODULES_DIR='$(X11_SERVERMODS_DIR)/dri$(SERVERMOD_subdir)'

# Changes to apply to man pages
MODTYPE_SUNTOUCH_MAN_FLAGS= -p $(MODULE_PREFIX)/bin/

# Command line options to GNU autoconf configure script for xorg-server
XSERVER_CONFIG_OPTS_COMMON = \
	--bindir='$${exec_prefix}/bin/$(ARCHBINSUBDIR)' \
	--docdir='$(X11_DOC_DIR)' \
        --enable-xdm-auth-1 --enable-xdmcp --enable-xcsecurity \
	--enable-shared --disable-static \
	--enable-glx --enable-dri2 \
	--disable-xsdl --disable-xwin --disable-xnest \
	--disable-install-setuid \
	--with-vendor-name='Sun Microsystems, Inc., based on X.Org Foundation sources' \
	--with-vendor-web='http://sunsolve.sun.com/' \
	--with-builderstring='Solaris ABI: $(BUILD_TYPE)-bit' \
	--with-os-name="`uname -srm`" \
	--with-log-dir=/var/log --with-xkb-output=/var/run/xkb \
	--with-module-dir=$(SERVER_MODULES_DIR) \
	--with-dri-driver-path=$(DRI_MODULES_DIR) \
	--with-default-font-path='catalogue:$(X11_FONT_CATALOGUE)' \
	--with-xkb-bin-directory='$${exec_prefix}/bin' \
	--with-default-xkb-model=pc105 \
	$(ADDITIONAL_CONFIG_OPTS)

XSERVER_CONFIG_OPTS_sparc=$(XSERVER_CONFIG_OPTS_COMMON)
XSERVER_CONFIG_OPTS_i386=$(XSERVER_CONFIG_OPTS_COMMON)
XSERVER_CONFIG_OPTS=$(XSERVER_CONFIG_OPTS_$(MACH))

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
MODTYPE_DEBUG_FLAGS=$(MODTYPE_DEBUG_FLAGS_$(MACH)_$(MODULE_COMPILER))

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
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR)

MODTYPE_ADD_CONFIGURE_TARGETS = fix-moduledirs

### Include common rulesets
include $(TOP)/common/Makefile.inc

## mdb module rules
# Build & install mdb modules & scripts that aren't integrated to
# normal build process - these are here since they are built in
# the Xorg build for 32-bit & 64-bit x86 and 64-bit SPARC, but
# in the Xvnc build for 32-bit SPARC
MDB_SOURCE_DIR=$(XORG_BUILD_DIR)/os/solaris/mdb
MDB_MODULE_DIR=/usr/lib/mdb/proc/$(ARCHLIBSUBDIR)
MDB_SCRIPT_DIR=/usr/demo/Xserver/mdb
MDB_SCRIPTS=list_Xserver_clients \
	list_Xserver_devicegrab_client \
	list_Xserver_servergrab_client
MDB_MODULE_NAMES=Xephyr Xorg Xvfb Xvnc

BUILT_MDB_MODULE=$(MDB_SOURCE_DIR)/modules/Xserver.so
BUILT_MDB_SCRIPTS=$(MDB_SCRIPTS:%=$(MDB_SOURCE_DIR)/scripts/%)

INSTALLED_MDB_LINKS=$(MDB_MODULE_NAMES:%=$(PROTODIR)$(MDB_MODULE_DIR)/%.so)
INSTALLED_MDB_MODULE=$(PROTODIR)$(MDB_MODULE_DIR)/Xserver.so
INSTALLED_MDB_SCRIPTS=$(MDB_SCRIPTS:%=$(PROTODIR)$(MDB_SCRIPT_DIR)/%)
INSTALLED_MDB_ALL=$(INSTALLED_MDB_MODULE) $(INSTALLED_MDB_LINKS) \
	$(INSTALLED_MDB_SCRIPTS)

# Since configure options aren't used to build the mdb modules, pass
# those variables at build time
MDB_BUILD_ENV=$(CONFIG_ENV) $(BUILD_ENV)

build_mdb: $(BUILT_MDB_MODULE)

$(BUILT_MDB_SCRIPTS): $(SOURCE_TARGETS)

$(BUILT_MDB_MODULE): $(CONFIGURE_TARGETS)
	(cd $(MDB_SOURCE_DIR) ; \
         $(MDB_BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS))

$(PROTODIR)$(MDB_MODULE_DIR) $(PROTODIR)$(MDB_SCRIPT_DIR):
	mkdir -p $@

$(INSTALLED_MDB_MODULE): $(PROTODIR)$(MDB_MODULE_DIR) $(BUILT_MDB_MODULE)
	$(INSTALL_SCRIPT) -c -m 0555 $(BUILT_MDB_MODULE) $@

$(INSTALLED_MDB_LINKS): $(PROTODIR)$(MDB_MODULE_DIR)
	@rm -f $@
	ln -s Xserver.so $@

$(INSTALLED_MDB_SCRIPTS): $(BUILT_MDB_SCRIPTS) $(PROTODIR)$(MDB_SCRIPT_DIR)
	$(INSTALL_SCRIPT) -c -m 0555 $(MDB_SOURCE_DIR)/scripts/$(@F) \
		$(PROTODIR)$(MDB_SCRIPT_DIR)/

install_mdb: $(INSTALLED_MDB_ALL)
	$(INSTALL_SCRIPT) -c -m 0444 $(MDB_SOURCE_DIR)/README \
		$(PROTODIR)$(MDB_SCRIPT_DIR)/
