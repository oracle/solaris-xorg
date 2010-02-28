###############################################################################
#
# Xorg server Makefile
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

# Build 32-bit or 64-bit?
BUILD_TYPES_SET=yes
BUILD_TYPES_sparc=64
BUILD_TYPES_i386=32 64
BUILD_TYPES=$(BUILD_TYPES_$(MACH))

# Package name used in tarballs
MODULE_NAME=xorg-server

# Version number (used in path names)
MODULE_VERSION=$(XORGSERVER_VERS)

# Checksums for upstream tarball
TARBALL_MD5  = 2856130aebf56e3df7b7d9be419bfb28
TARBALL_SHA1 = c385293b66a6b0bc6bad648588f611d844c26ee7

# Patches to apply to source after unpacking, in order
# *** Moved to patch-list file so they can be shared between Xorg & Xvnc builds
SOURCE_PATCHES=$(XORG_SOURCE_PATCHES)

# Used in rules shared between Xorg & Xvnc builds in Makefile.inc
# to allow for different source layouts
XORG_BUILD_DIR=$(SOURCE_DIR)

# Man pages to apply Sun footer to & attributes to list
# XXX: Need to adjust some of these to correct packages/stability
SUNTOUCHED_MANPAGES= \
	hw/kdrive/ephyr/Xephyr.man.pre \
	hw/xfree86/utils/gtf/gtf.man.pre \
	hw/xfree86/utils/cvt/cvt.man.pre \
	hw/xfree86/doc/man/Xorg.man.pre \
	hw/xfree86/doc/man/xorg.conf.man.pre \
	hw/vfb/Xvfb.man.pre

SUN_PACKAGE=SUNWxorg-server
MODULE_STABILITY=Volatile
MODULE_SUNTOUCH_MAN_FLAGS= -p $(MODULE_PREFIX)/bin/

# Binary built in tree
XORGSERVER_BIN=$(SOURCE_DIR)/hw/xfree86/Xorg

BUILD_TARGETS=$(XORGSERVER_BIN) build_mdb set-elf-comments \
	$(BUILD_DIR)/mapfile-Xorg-externs
BUILD_TARGETS_SET=yes

# Need to regenerate autoconf/automake files after patching
AUTORECONF=yes

# Command line options to GNU autoconf configure script
#  Need to override LD setting to add -xarch flags that libtool strips
# XXX: --disable-dga
MODULE_CONFIG_OPTS = $(XSERVER_CONFIG_OPTS) \
        --enable-builddocs \
	--enable-kdrive --enable-xephyr --disable-xsdl \
	MKFONTDIR="$(MKFONTDIR)" \
	MKFONTSCALE="$(MKFONTSCALE)" \
	LD="$(CC) $(ARCH_FLAGS)"

# pkg-config files that need to have -R added
FIX_PC_FILES=xorg-server.pc.in
# Remove compiler specific visibility flag that breaks builds of drivers
# with other compilers
MODULE_FIX_PC_FLAGS= -e 's|\@symbol_visibility\@||'

# Need to use GNU Make to build due to hw/xfree86/loader
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Additional install targets beyond the defaults
MODULE_ADD_INSTALL_TARGETS=install_extras install_mdb install_pc_common
EXTRA_INSTALL_sparc=install_pc_copy
EXTRA_INSTALL_i386=
MODULE_ADD_INSTALL_TARGETS += $(EXTRA_INSTALL_$(MACH))

# Have to avoid dependency loop between install_pc_copy and 
# install_pkgconfig_uninstalled
INSTALL_PCU_DEPS = $(INSTALL_TARGETS:install_pc_copy=)
INSTALL_PCU_DEPS_SET=yes

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

include ../Makefile.inc

$(XORGSERVER_BIN): default_build

SDK_INCLUDES_DIR=$(PROTODIR)$(X11_INCLUDES_DIR)/xorg/

install_extras: default_install
	-rm -f $(SDK_INCLUDES_DIR)/xorg-server.h
	sed -e 's%#define XF86DRI 1%/* #define XF86DRI 1 */%' \
	    -e 's%^.* _XSERVER64 .*$$%#include <sys/isa_defs.h>\
#ifdef _LP64\
#define _XSERVER64 1\
#endif%' \
	    $(SOURCE_DIR)/include/xorg-server.h > $(SDK_INCLUDES_DIR)/xorg-server.h
	cp -pf $(SOURCE_DIR)/mi/mioverlay.h $(SDK_INCLUDES_DIR)/mioverlay.h
	cp -pf $(SOURCE_DIR)/hw/xfree86/os-support/bus/xf86Sbus.h $(SDK_INCLUDES_DIR)/xf86Sbus.h
	cp -pf $(SOURCE_DIR)/hw/xfree86/dri/*.h $(SDK_INCLUDES_DIR)/
	-rm -f $(SDK_INCLUDES_DIR)/xf1bpp.h $(SDK_INCLUDES_DIR)/xf4bpp.h
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	cp -pf $(SOURCE_DIR)/hw/xfree86/doc/README.* $(PROTODIR)$(X11_DOC_DIR)/
	mkdir -p $(PROTODIR)$(X11_MAN_DIR)/man1
	cp -pf $(SOURCE_DIR)/doc/Xnest.1 \
	       $(SOURCE_DIR)/hw/xfree86/doc/man/xorgcfg.1 \
	       $(SOURCE_DIR)/hw/xfree86/doc/man/xorgconfig.1 \
	       $(PROTODIR)$(X11_MAN_DIR)/man1
	mkdir -p $(PROTODIR)$(X11_DIR)/share/hal/fdi/policy/10osvendor
	cp -pf $(SOURCE_DIR)/config/x11-input.fdi \
	 $(PROTODIR)$(X11_DIR)/share/hal/fdi/policy/10osvendor/10-x11-input.fdi

# Workaround bug in installing pkgconfig file to wrong directory in 64-bit
# builds
install_pc_common: default_install
	-rm -f $(PROTODIR)$(PKGCONFIG_DIR)/xorg-server.pc
	cp -pf $(SOURCE_DIR)/xorg-server.pc \
		$(PROTODIR)$(PKGCONFIG_DIR)/xorg-server.pc

# Install xorg-server.pc in 32-bit pkgconfig dir on SPARC too
install_pc_copy: install_pc_common install_pkgconfig_uninstalled 
	cp -pf $(PROTODIR)$(PKGCONFIG_DIR_64)/xorg-server.pc \
		$(PROTODIR)$(PKGCONFIG_DIR_32)/xorg-server.pc
	cp -pf $(PROTODIR)$(PKGCONFIG_DIR_64)/xorg-server-uninstalled.pc \
		$(PROTODIR)$(PKGCONFIG_DIR_32)/xorg-server-uninstalled.pc

$(BUILD_DIR)/mapfile-Xorg-externs: $(XORGSERVER_BIN)
	-rm -f $@
	printf '# Functions exported by Xorg server to loadable modules\n\n' > $@
	printf '{\n  global:\n' >> $@
	/usr/ccs/bin/nm -g $(XORGSERVER_BIN) $(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)/libXfont.so | grep -v UNDEF | sed -n -e 's/^.*\|FUNC \|GLOB \|.*\|\([A-Za-z].*\)$$/    \1 = FUNCTION extern;/p' -e 's/^.*\|OBJT \|GLOB \|.*\|\([A-Za-z].*\)$$/    \1 = DATA extern;/p' >> $@
	printf '};\n' >> $@


