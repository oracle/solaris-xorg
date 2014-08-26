#
# Copyright (c) 2006, 2014, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libdrm

# Package metadata
UPSTREAM	= Direct Rendering Infrastructure project
UPSTREAM_URL	= http://dri.freedesktop.org/
UPSTREAM_SET	= yes
ORACLE_TPNO	= 9900

# Version number (used in path names)
MODULE_VERSION=2.4.43

# Checksums for upstream tarball
TARBALL_MD5  = 7de4afd3756726d11f1bf75bd91ba716
TARBALL_SHA1 = 80e810e0476a0c479a84b74f22aa1bc228484ce6
TARBALL_SHA256 = 7fb8cd89df06e236a66a1a6a9f6d058271c1b1ec92631e08c66dc9e12abd4975

# Library name (used for specfiles/mapfiles)
LIBNAME=drm

# Download site for source
SOURCE_URL=http://dri.freedesktop.org/libdrm/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/drm
GIT_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += solaris-drm-port.patch,-p1
SOURCE_PATCHES += atomic.patch

# Can't build tests with parfait yet, due to parfait-ld error:
# llvm-ld: error: Cannot link file '../../libkms/libkms.so.1.bc: 
#    Linking globals named 'drmSetServerInfo': symbol multiply defined!
$(WITH_PARFAIT) SOURCE_PATCHES += no-tests.patch,-p1
$(WITH_PARFAIT) AUTORECONF = yes

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Need to build with gcc
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --enable-libkms --disable-nouveau

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY = Volatile
LIB_MAN_SUFFIX = 3
MODULE_MTLEVEL=Unknown
MODULE_MTLEVEL_SET=yes
MODULE_PKGCONFIG_MAN=libdrm
MODULE_PKGCONFIG_MAN_SET=yes

# pkg-config files that need to be updated
FIX_PC_FILES=libdrm.pc.in intel/libdrm_intel.pc.in libkms/libkms.pc.in \
	radeon/libdrm_radeon.pc.in

# Where to install files
MODULE_ADD_LIB_SUBDIR = $(X11_SERVERLIBS_SUBDIR)
MODULE_LDFLAGS = -R $(MODULE_PREFIX)/lib$(MODULE_ADD_LIB_SUBDIR)$(ARCHLIBSUBDIR)
MODULE_PKGCONFIG_DIR = $(PKGCONFIG_DIR)
MODULE_PKGCONFIG_DIR_SET = yes

include ../Makefile.inc

install: $(PROTODIR)/usr/include/drm

$(PROTODIR)/usr/include/drm:
	@rm -f $@
	ln -s libdrm $@

# Upstream seems to install in $(X11_SERVERLIBS_SUBDIR)/pkgconfig no matter
# what, so just move them afterwards
PC_FILES_i386 = libdrm_intel.pc libdrm_radeon.pc
PC_FILES = libdrm.pc libkms.pc $(PC_FILES_$(MACH))
MOVED_PC_FILES = $(PC_FILES:%=$(PROTODIR)/$(PKGCONFIG_DIR)/%)
XSERVER_PC_DIR = $(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)/pkgconfig

install-gen: $(MOVED_PC_FILES) install_fixed_manpages

$(PROTODIR)/$(PKGCONFIG_DIR)/%.pc: $(PROTODIR)/$(XSERVER_PC_DIR)/%.pc
	-rm -f $@
	mv $< $@

$(PC_FILES:%=$(PROTODIR)/$(XSERVER_PC_DIR)/%): default_install

# Need to apply suntouch changes to man pages generated during build
MANDIR = $(PROTODIR)$(X11_MAN_DIR)
install_fixed_manpages: default_install
	$(PERL) $(TOP)/open-src/common/suntouch-manpages.pl \
		$(SUNTOUCH_MAN_FLAGS) $(BUILD_DIR)/man/*.{3,7}
	ginstall -m 0444 $(BUILD_DIR)/man/*.3 $(MANDIR)/man3
	ginstall -m 0444 $(BUILD_DIR)/man/*.7 $(MANDIR)/man7
