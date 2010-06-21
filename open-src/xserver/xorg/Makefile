###############################################################################
#
# Xorg server Makefile
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
TARBALL_MD5  = 8c0146330fb155c23d947ac37d431d4b
TARBALL_SHA1 = 0f1b71363843b7e394d6de62d0db5b74d3e645ef

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

MODULE_LDFLAGS = $(XSERVER_LDFLAGS)
MODULE_LD_OPTIONS = $(MAPFILES_FOR_ALL)

# pkg-config files that need to have -R added
FIX_PC_FILES=xorg-server.pc.in
# Remove compiler specific visibility flag that breaks builds of drivers
# with other compilers
MODULE_FIX_PC_FLAGS= -e 's|\@symbol_visibility\@||'

# Need to use GNU Make to build due to hw/xfree86/loader
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Additional install targets beyond the defaults
MODULE_ADD_INSTALL_TARGETS=install_extras install_mdb install_pc_common \
	install_fix_paths
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
	mkdir -p $(PROTODIR)$(X11_MAN_DIR)/man1
	cp -pf $(SOURCE_DIR)/doc/Xnest.1 \
	       $(SOURCE_DIR)/hw/xfree86/doc/man/xorgcfg.1 \
	       $(SOURCE_DIR)/hw/xfree86/doc/man/xorgconfig.1 \
	       $(PROTODIR)$(X11_MAN_DIR)/man1
	mkdir -p $(PROTODIR)/etc/hal/fdi/preprobe/10osvendor
	cp -pf $(SOURCE_DIR)/config/x11-input.fdi \
	 $(PROTODIR)/etc/hal/fdi/preprobe/10osvendor/10-x11-input.fdi

# Workaround bug in installing pkgconfig file to wrong directory in 64-bit
# builds
install_pc_common: default_install
	-rm -f $(PROTODIR)$(PKGCONFIG_DIR)/xorg-server.pc
	mkdir -p $(PROTODIR)$(PKGCONFIG_DIR)
	cp -pf $(SOURCE_DIR)/xorg-server.pc \
		$(PROTODIR)$(PKGCONFIG_DIR)/xorg-server.pc

# Install xorg-server.pc in 32-bit pkgconfig dir on SPARC too
install_pc_copy: install_pc_common install_pkgconfig_uninstalled 
	-rm -f $(PROTODIR)$(PKGCONFIG_DIR_32)/xorg-server.pc \
		$(PROTODIR)$(PKGCONFIG_DIR_32)/xorg-server-uninstalled.pc
	mkdir -p $(PROTODIR)$(PKGCONFIG_DIR_32)
	ln -s ../$(SUBDIR64)$(PKGCONFIG_DIR_suffix)/xorg-server.pc \
	    ../$(SUBDIR64)$(PKGCONFIG_DIR_suffix)/xorg-server-uninstalled.pc \
		$(PROTODIR)$(PKGCONFIG_DIR_32)/

$(BUILD_DIR)/mapfile-Xorg-externs: $(XORGSERVER_BIN)
	-rm -f $@
	printf '# Functions exported by Xorg server to loadable modules\n\n' > $@
	printf '{\n  global:\n' >> $@
	/usr/ccs/bin/nm -g $(XORGSERVER_BIN) $(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)/libXfont.so | grep -v UNDEF | sed -n -e 's/^.*\|FUNC \|GLOB \|.*\|\([A-Za-z].*\)$$/    \1 = FUNCTION extern;/p' -e 's/^.*\|OBJT \|GLOB \|.*\|\([A-Za-z].*\)$$/    \1 = DATA extern;/p' >> $@
	printf '};\n' >> $@

# Fix paths in proto area to match paths in packages / installed systems
install_fix_paths: install_fix_paths_$(MACH)_$(BUILD_TYPE) install_fix_paths_X
install_fix_paths_$(MACH)_: $(BUILD_TYPES:%=install_fix_paths_$(MACH)_%)

install_fix_paths_i386_32: install_fix_paths_use_utilities
install_fix_paths_i386_64: install_fix_paths_remove_utilities
install_fix_paths_sparc_64: # not needed

install_fix_paths_use_utilities: default_install
	rm -f $(PROTODIR)$(MODULE_PREFIX)/bin/cvt \
	      $(PROTODIR)$(MODULE_PREFIX)/bin/gtf
	mv -f $(PROTODIR)$(MODULE_PREFIX)/bin/$(ARCHBINSUBDIR)/cvt \
	      $(PROTODIR)$(MODULE_PREFIX)/bin/$(ARCHBINSUBDIR)/gtf \
	      $(PROTODIR)$(MODULE_PREFIX)/bin

install_fix_paths_remove_utilities: default_install
	rm -f $(PROTODIR)$(MODULE_PREFIX)/bin/$(ARCHBINSUBDIR)/cvt \
	      $(PROTODIR)$(MODULE_PREFIX)/bin/$(ARCHBINSUBDIR)/gtf

install_fix_paths_X: default_install
	rm -f $(PROTODIR)$(MODULE_PREFIX)/bin/X \
	      $(PROTODIR)$(MODULE_PREFIX)/bin/$(ARCHBINSUBDIR)/X
	ln -s Xserver $(PROTODIR)$(MODULE_PREFIX)/bin/X

XSERVERS = Xorg Xephyr Xvfb
XSERVER_LINKS = $(XSERVERS:%=$(PROTODIR)$(MODULE_PREFIX)/bin/%)

$(PROTODIR)/usr/lib/isaexec:
	@rm -f $@
	cp -pf /usr/lib/isaexec $@

$(XSERVER_LINKS): $(PROTODIR)/usr/lib/isaexec
	@rm -f $@
	ln $(PROTODIR)/usr/lib/isaexec $@

install_fix_paths_i386_32: install_fix_paths_Xservers_isaexec
install_fix_paths_Xservers_isaexec: $(XSERVER_LINKS)
