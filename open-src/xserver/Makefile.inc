# -*- Makefile -*- rules common to all xserver build trees
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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
TOP=$(PWD)/../../..

MODTYPE=xserver

# Defaults for pkg names in this area
MODTYPE_PKGHIER=x11/server
MODTYPE_PKGHIER_SET=yes
MODULE_PKGTAIL_DEFAULT		= $(MODULE_PKGTAIL_SET:yes=$(POUND_SIGN))
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL:sh=basename $(pwd)
MODULE_PKGTAIL_SET	      	= yes

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# Xorg server source tarball to use sources from
XORGSERVER_VERS=1.9.4

# Patches shared between Xorg & Xvnc builds
XORG_SOURCE_PATCHES:sh=sed '/^\#/ d' ../xorg/patch-list

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
	--with-vendor-name='$(VENDOR_NAME)' \
	--with-vendor-web='$(VENDOR_SUPPORT_URL)' \
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
	$(DPMS_DEFINES) -DSUNSOFT -DTSOL \
	$(MODTYPE_CPPFLAGS_$(MODULE_COMPILER))

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
MODTYPE_LDFLAGS=$(PROG_LDFLAGS)

XSERVER_LDFLAGS = \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR) \
	-R$(X11_LIB_DIR)$(ARCHLIBSUBDIR)

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
include $(TOP)/open-src/common/Makefile.inc

# Fix paths in proto area to match paths in packages / installed systems
install_fix_paths: install_fix_paths_$(MACH)_$(BUILD_TYPE)
install_fix_paths_$(MACH)_: $(BUILD_TYPES:%=install_fix_paths_$(MACH)_%)

# When both 32-bit & 64-bit are built, only one copy of the programs in
# NOT_LINKED_TO_ISAEXEC are needed, so move the 32-bit copy to /usr/bin
# instead of linking to isaexec, and delete the 64-bit copy.
UTILS_IN_BIN = $(NOT_LINKED_TO_ISAEXEC:%=$(PROTODIR)$(MODULE_PREFIX)/bin/%)
UTILS_IN_ARCHBIN = \
 $(NOT_LINKED_TO_ISAEXEC:%=$(PROTODIR)$(MODULE_PREFIX)/bin/$(ARCHBINSUBDIR)/%)

install_fix_paths_use_utilities: $(INSTALL_FIX_PATHS_DEPS)
	rm -f $(UTILS_IN_BIN)
	mv -f $(UTILS_IN_ARCHBIN) $(PROTODIR)$(MODULE_PREFIX)/bin

install_fix_paths_remove_utilities: $(INSTALL_FIX_PATHS_DEPS)
	rm -f $(UTILS_IN_ARCHBIN)

install_fix_paths_i386_32: install_fix_paths_use_utilities
install_fix_paths_i386_64: install_fix_paths_remove_utilities
install_fix_paths_sparc_64: # not needed

# When both 32-bit & 64-bit are built, we install both versions of the 
# programs in LINKED_TO_ISAEXEC, and use isaexec to pick 32-bit or 64-bit
# binaries to execute.
ISAEXEC_LINKS = $(LINKED_TO_ISAEXEC:%=$(PROTODIR)$(MODULE_PREFIX)/bin/%)

$(PROTODIR)/usr/lib/isaexec:
	@rm -f $@
	cp -pf /usr/lib/isaexec $@

$(ISAEXEC_LINKS): $(PROTODIR)/usr/lib/isaexec
	@rm -f $@
	ln $(PROTODIR)/usr/lib/isaexec $@

install_fix_isaexec: $(ISAEXEC_LINKS)

install_fix_paths_i386_32: install_fix_isaexec
# Not needed in 64-bit builds on sparc or x86
