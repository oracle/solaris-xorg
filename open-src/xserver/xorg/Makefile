###############################################################################
#
# Xorg server Makefile
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.72	09/01/14
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

# Patches to apply to source after unpacking, in order
# *** Moved to patch-list file so they can be shared between Xorg & Xvnc builds
SOURCE_PATCHES=$(XORG_SOURCE_PATCHES)

# Man pages to apply Sun footer to & attributes to list
# XXX: Need to adjust some of these to correct packages/stability
SUNTOUCHED_MANPAGES= \
	hw/kdrive/ephyr/Xephyr.man.pre \
	hw/xfree86/utils/gtf/gtf.man.pre \
	hw/xfree86/utils/cvt/cvt.man.pre \
	hw/xfree86/utils/xorgcfg/xorgcfg.man.pre \
	hw/xfree86/utils/xorgconfig/xorgconfig.man.pre \
	hw/xfree86/doc/man/Xorg.man.pre \
	hw/xfree86/doc/man/xorg.conf.man.pre \
	hw/vfb/Xvfb.man.pre

SUN_PACKAGE=SUNWxorg-server
MODULE_STABILITY=Volatile
MODULE_SUNTOUCH_MAN_FLAGS= -p $(MODULE_PREFIX)/bin/

# Compiler to use - x86 MMX code only builds with gcc, not Sun Studio, so use
# gcc on x86 for now, but keep Sun Studio on SPARC
MODULE_COMPILER_sparc=suncc
MODULE_COMPILER_i386=gcc
MODULE_COMPILER=$(MODULE_COMPILER_$(MACH))
MODULE_COMPILER_SET=yes

# Binary built in tree
XORGSERVER_BIN=$(SOURCE_DIR)/hw/xfree86/Xorg

BUILD_TARGETS=$(XORGSERVER_BIN) set-elf-comments \
	$(BUILD_DIR)/mapfile-Xorg-externs
BUILD_TARGETS_SET=yes

# Additional license files to install
EXTRA_LICENSES = \
	COPYING.Xprint-fonts 

# Need to regenerate autoconf/automake files after patching
AUTORECONF=yes

# Command line options to GNU autoconf configure script
#  Need to override LD setting to add -xarch flags that libtool strips
# XXX: --disable-dga
MODULE_CONFIG_OPTS = $(XSERVER_CONFIG_OPTS) \
        --enable-builddocs --enable-xorgcfg --disable-xprint \
	--enable-kdrive --enable-xephyr --disable-xsdl \
	MKFONTDIR="$(MKFONTDIR)" \
	MKFONTSCALE="$(MKFONTSCALE)" \
	LD="$(CC) $(ARCH_FLAGS)"

# pkg-config files that need to have -R added
FIX_PC_FILES=xorg-server.pc.in

# Additional install targets beyond the defaults
EXTRA_INSTALL_sparc=install_pc
EXTRA_INSTALL_i386=
MODULE_ADD_INSTALL_TARGETS=install_extras $(EXTRA_INSTALL_$(MACH))

include ../Makefile.inc

# Merge in additional sources from sun-src directory
source_gen:: $(LNDIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)

$(XORGSERVER_BIN): default_build

SDK_INCLUDES_DIR=$(PROTODIR)$(X11_INCLUDES_DIR)/xorg/

install_extras: default_install
	sed -e 's%#define XF86DRI 1%/* #define XF86DRI 1 */%' \
	    -e 's%^.* _XSERVER64 .*$$%#include <sys/isa_defs.h>\
#ifdef _LP64\
#define _XSERVER64 1\
#endif%' \
	    $(SOURCE_DIR)/include/xorg-server.h > $(SDK_INCLUDES_DIR)/xorg-server.h
	cp -p $(SOURCE_DIR)/mi/mioverlay.h $(SDK_INCLUDES_DIR)/mioverlay.h
	cp -p $(SOURCE_DIR)/hw/xfree86/os-support/bus/xf86Sbus.h $(SDK_INCLUDES_DIR)/xf86Sbus.h
	cp -p $(SOURCE_DIR)/hw/xfree86/dri/*.h $(SDK_INCLUDES_DIR)/
	-rm -f $(SDK_INCLUDES_DIR)/xf1bpp.h $(SDK_INCLUDES_DIR)/xf4bpp.h
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	cp -p $(SOURCE_DIR)/hw/xfree86/doc/README.* $(PROTODIR)$(X11_DOC_DIR)/
	mkdir -p $(PROTODIR)$(X11_DIR)/share/hal/fdi/policy/10osvendor
	cp -p $(SOURCE_DIR)/config/x11-input.fdi \
	 $(PROTODIR)$(X11_DIR)/share/hal/fdi/policy/10osvendor/10-x11-input.fdi

# Install xorg-server.pc in 32-bit pkgconfig dir on SPARC too
install_pc: install_pkgconfig_uninstalled
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
