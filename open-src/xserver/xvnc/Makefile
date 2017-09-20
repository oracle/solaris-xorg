###############################################################################
#
# TigerVNC (Xvnc server & vncviewer) Makefile
# Based on Fedora's tigervnc.spec revision 1.20 from
# http://cvs.fedoraproject.org/viewvc/rpms/tigervnc/devel/tigervnc.spec
#
# Copyright (c) 2007, 2017, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=tigervnc

# Package metadata
UPSTREAM	= TigerVNC
UPSTREAM_URL	= http://www.tigervnc.com/
UPSTREAM_SET	= yes
GIT_URL		= https://github.com/TigerVNC/tigervnc.git
GIT_URL_SET	= yes

MODULE_LICENSE          = GPLv2
MODULE_LICENSE_SET      = yes
MODULE_LICENSE_PREPEND	= $(GPL_CHOICE_FILE)
MODULE_LICENSE_DESC	= portions of the TigerVNC software
ORACLE_TPNO		= 34678

# Version number (used in path names)
MODULE_VERSION=1.7.1

# Checksums for upstream tarball
TARBALL_MD5  = 82dbb9d178b8e313b51f1651e0398dfe
TARBALL_SHA1 = e6924a1c0a092d3f4b4d389087592a1279cf1728

# Source tarball
SOURCE_TARBALL_NAME=v$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Xorg server source tarball to use sources from
XORG_TARBALL=$(TARBALLDIR)/xorg-server-$(XORGSERVER_VERS).tar.bz2
XORG_SOURCE_DIR = $(SOURCE_DIR)/unix/xserver
XORG_BUILD_DIR  = $(BUILD_DIR)/unix/xserver

# Java VNCviewer applet for connecting from a web browser
JAVA_SOURCE_DIR = $(SOURCE_DIR)/java/com/tigervnc
JAVA_BUILD_DIR  = $(BUILD_DIR)/java

# Download site for source
SOURCE_URL=https://github.com/TigerVNC/tigervnc/archive/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += $(SOURCE_DIR)/unix/xserver119.patch,'-p1 -d unix/xserver'
SOURCE_PATCHES += vnc-nobuildtime.patch,-p1
SOURCE_PATCHES += vnc-nohttpd.patch,-p1
SOURCE_PATCHES += solaris-port.patch,-p1
SOURCE_PATCHES += solaris-audit.patch,-p1
SOURCE_PATCHES += gnutls-drop-ecdh.patch,-p1
SOURCE_PATCHES += screenlock.patch,-p1
SOURCE_PATCHES += 25022410.patch,-p1
SOURCE_PATCHES += unicode.patch,-p1
SOURCE_PATCHES += tigervnc-xserver119.patch
SOURCE_PATCHES += cursor-upstream.patch,-p1
SOURCE_PATCHES += 25818978.patch,-p1

# Need to regenerate autoconf/automake files after patching
AUTORECONF=no

# TigerVNC 1.1 doesn't fully support out-of-tree builds, so clone the source
CLONE_SRC = yes

# Disable parfait build with 1.3 - 17759056
USE_PARFAIT=no

# ARC cases that covered this module
# PSARC/2007/545 Xvnc
MODULE_ARC_CASES = PSARC/2007/545
# LSARC/2007/625 vncviewer
MODULE_ARC_CASES += LSARC/2007/625
# PSARC/2009/592 TigerVNC 1.0
MODULE_ARC_CASES += PSARC/2009/592
# PSARC/2017/212 TigerVNC 1.7.1
MODULE_ARC_CASES += PSARC/2017/212

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= \
	unix/xserver/hw/vnc/Xvnc.man vncviewer/vncviewer.man \
	unix/vncconfig/vncconfig.man unix/vncpasswd/vncpasswd.man \
	unix/vncserver.man unix/x0vncserver/x0vncserver.man
MODULE_STABILITY = Volatile

# Additional pkg(5) package name used for vncviewer package
VNCVIEWER_PKGNAME=desktop/remote-desktop/$(MODULE_NAME)

# Override specific attributes for specific man pages
MODULE_SUNTOUCH_MAN_FLAGS= \
	$(SUNTOUCH_MAN_FLAGS_$(@F:%.man.$(FIXED_MAN_SUFFIX)=%))
SUNTOUCH_MAN_FLAGS_vncviewer = -o '{Availability, $(VNCVIEWER_PKGNAME)}' \
	-o '{Interface Stability, Path: Committed, Options: Volatile}'

# Compiler to use

MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Make command to use
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Xvnc has to configure, build, and install in different directories due to 
# the mixed codebases and unusual directory layout
$(NO_RECURSE)MODULE_ADD_SOURCE_TARGETS = Xvnc_source_init 
$(NO_RECURSE)MODULE_SOURCE_DEPS = Xvnc_source_init
$(NO_RECURSE)ADDITIONAL_SOURCE_DIR = sun-src
MODULE_ADD_CONFIGURE_TARGETS = $(XORG_BUILD_DIR)/Makefile
BUILD_TARGETS=Xvnc_build set-elf-comments
BUILD_TARGETS_SET=yes
INSTALL_TARGETS=Xvnc_install_server Xvnc_install_programs \
	 Xvnc_install_data install_fix_paths
INSTALL_TARGETS_SET=yes

CONFIGURE_TARGETS = $(BUILD_DIR) cmake $(XORG_BUILD_DIR)/Makefile
CONFIGURE_TARGETS_SET=yes

# Environment variable options to GNU autoconf configure script
# Have to remove CFLAGS entries from default LDFLAGS to avoid conflicts
# with the C++ compiler we use instead
MODULE_CONFIG_ENV = \
    LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(MODTYPE_LDFLAGS) $(MODULE_LDFLAGS)"

# Command line options to GNU autoconf configure script for vnc clients
MODULE_CONFIG_OPTS  = --with-installed-zlib --with-system-jpeg --without-simd
MODULE_CONFIG_OPTS += --libdir='$${exec_prefix}/lib/vnc'
MODULE_CONFIG_OPTS += --enable-gnutls

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = Xvnc

# Binaries that were linked to isaexec on x86 to choose 32-bit or 64-bit,
# now just symlinks from amd64 to parent directory
LINKED_TO_ISAEXEC = Xvnc

include ../Makefile.inc

# Merge in additional sources from Xorg tarball & sun-src directory
# This step has to be done before default_source, so that patches applied
# in that step work correctly.
$(NO_RECURSE)XORG_SOURCE_INIT_TARGET = $(XORG_SOURCE_DIR)/$(SOURCE_INIT_TARGET_FILE)
$(NO_RECURSE)XORG_PATCHED_TARGET = $(XORG_SOURCE_DIR)/$(PATCHED_TARGET_FILE)
Xvnc_source_init: $(XORG_PATCHED_TARGET)

$(XORG_TARBALL):
	(cd ../xorg && $(MAKE) $(MAKEFLAGS) download)

$(XORG_SOURCE_INIT_TARGET): $(XORG_TARBALL)
	mkdir -p $(SOURCE_DIR)/unix
	$(MAKE) $(MAKEFLAGS) init_source \
		NO_RECURSE="$(POUND_SIGN)" \
		BUILD_TYPE="$(BUILD_TYPE)" \
		SOURCE_TARBALL="$(XORG_TARBALL)" \
		SOURCE_DEPS="$(XORG_TARBALL)" \
		SOURCE_DIR="$(XORG_SOURCE_DIR)" \
		BUILD_DIR="$(XORG_BUILD_DIR)"
	(cd $(XORG_SOURCE_DIR) && $(LNDIR) ../../../../xorg/sun-src)
	touch $(XORG_SOURCE_INIT_TARGET)

$(XORG_PATCHED_TARGET): $(XORG_SOURCE_INIT_TARGET)
	$(MAKE) $(MAKEFLAGS) patched_source \
		NO_RECURSE="$(POUND_SIGN)" \
		BUILD_TYPE="$(BUILD_TYPE)" \
		SOURCE_PATCHES="$(XORG_SOURCE_PATCHES:%=../xorg/%)" \
		SOURCE_DIR="$(XORG_SOURCE_DIR)" \
		BUILD_DIR="$(XORG_BUILD_DIR)" \
		XORG_BUILD_DIR="$(XORG_BUILD_DIR)"
	(cd $(SOURCE_DIR) && $(LNDIR) ../sun-src)
	touch $(XORG_PATCHED_TARGET)

# Command line options to GNU autoconf configure script for Xvnc
XVNC_CONFIG_OPTS = $(DEFAULT_CONFIG_OPTS) $(XSERVER_CONFIG_OPTS)
# Disable all the things we don't need in the Xvnc build since we
# get them in the normal Xorg build instead
XVNC_CONFIG_OPTS += --disable-builddocs --disable-xfree86-utils
XVNC_CONFIG_OPTS += --disable-xorg --disable-xnest --disable-xvfb --disable-dmx
XVNC_CONFIG_OPTS += --disable-xwin --disable-xephyr --disable-kdrive 
XVNC_CONFIG_OPTS += --disable-dri3 --disable-present
XVNC_CONFIG_OPTS += --disable-all-encodings
# Disable building Xorg hardware device support that Xvnc can't use
XVNC_CONFIG_OPTS += --without-int10 --disable-config-hal --disable-dga 
# The X server unit tests currently depend on the Xorg ddx being built
XVNC_CONFIG_OPTS += --disable-unit-tests
XVNC_CONFIG_OPTS += AR_FLAGS="cruS"

# Force use of gnutls-3 pkgconfig during 2.x->3.x transition (bug 22144600)
GNUTLS_PKGCONFIG_CMD = \
    PKG_CONFIG_PATH=/usr/lib/$(SUBDIR64)/pkgconfig/gnutls-3/ pkg-config
XVNC_CONFIG_OPTS += GNUTLS_CFLAGS="$$($(GNUTLS_PKGCONFIG_CMD) --cflags gnutls)"
XVNC_CONFIG_OPTS += GNUTLS_LDFLAGS="$$($(GNUTLS_PKGCONFIG_CMD) --libs gnutls)"

MODULE_CPPFLAGS = \
	-D__extension__=\"\" \
	-I$(PWD)/$(SOURCE_DIR)/common \
	-I$(PWD)/$(SOURCE_DIR)/unix/vncconfig \
	-I$(PWD)/$(SOURCE_DIR)/unix/xserver/miext/damage

CFLAGS_gcc += -fPIC 

# Needed by unix/xorg-x11-server-source/hw/vnc/Makefile.am to find libpixman
# headers in proto area after solaris-port.patch is applied
MODULE_MAKEFLAGS = PROTODIR=$(PROTODIR)

# Run configure script for Xvnc
$(XORG_BUILD_DIR)/Makefile: $(SOURCE_TARGETS) $(BUILD_DIR)
	(cd $(XORG_BUILD_DIR) &&  \
		$(AUTORECONF_ENV) autoreconf --install --force && \
		$(CONFIG_ENV) $(CONFIG_SHELL) ./configure $(XVNC_CONFIG_OPTS)) 

Xvnc_build: $(CONFIGURE_TARGETS)
	$(PERL) -i -p -e 's{/usr/bin/env perl}{$(PERL)}' \
		$(SOURCE_DIR)/unix/vncserver
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(BUILD_DIR))
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(XORG_BUILD_DIR))

cmake: $(BUILD_DIR)
	(cd $(BUILD_DIR) && \
	PKG_CONFIG_PATH=/usr/lib/$(SUBDIR64)/pkgconfig/gnutls-3 \
	cmake -Wno-dev \
  	-DFLTK_INCLUDE_DIR:PATH="$(PROTODIR)/usr/include" \
	-DFLTK_LIBRARIES:FILEPATH="$(PROTODIR)/usr/lib/64/libfltk.so.1.3" \
	-DFLTK_IMAGES_LIBRARY:FILEPATH="$(PROTODIR)/usr/lib/64/libfltk_images.so.1.3" \
	-DFLTK_BASE_LIBRARY:FILEPATH="$(PROTODIR)/usr/lib/64/libfltk.so.1.3" \
	-DENABLE_NLS:BOOL=OFF \
	-DGNUTLS_INCLUDE_DIR:PATH="/usr/include/gnutls-3" \
	-DGNUTLS_LIBRARY:FILEPATH="/usr/lib/64/gnutls-3/libgnutls.so.30" \
	-DCMAKE_C_COMPILER:STRING="$(GCC)" \
	-DCMAKE_CXX_COMPILER:FILEPATH="$(GXX)" \
	-DCMAKE_C_ARCHIVE_CREATE:STRING="crS" \
	-DCMAKE_CXX_ARCHIVE_CREATE:STRING="crS" \
	-DCMAKE_C_FLAGS:STRING="$(CFLAGS) $(ARCH_FLAGS)" \
	-DCMAKE_CXX_FLAGS:STRING="$(CXXFLAGS) $(ARCH_FLAGS)" \
	-DBUILD_JAVA:BOOL=TRUE \
	-DCMAKE_SKIP_RPATH:BOOL=TRUE \
	-G "Unix Makefiles" . )
	(cd $(BUILD_DIR) && cmake -G "Unix Makefiles")

Xvnc_install_server: $(BUILD_TARGETS)
	$(DEFAULT_INSTALL_COMMAND:@DIR@=$(XORG_BUILD_DIR)/hw/vnc) install
	rm -f $(PROTODIR)/$(X11_SERVERMODS_DIR)/extensions/libvnc.so

Xvnc_install_programs: $(BUILD_TARGETS)
	mkdir -p $(PROTODIR)/usr/bin/$(ARCHBINSUBDIR)
	mkdir -p $(PROTODIR)/usr/share/vnc/classes
	$(INSTALL_SCRIPT) -m 0755 $(BUILD_DIR)/unix/vncserver \
		$(BUILD_DIR)/unix/vncpasswd/vncpasswd \
		$(BUILD_DIR)/unix/vncconfig/vncconfig \
		$(BUILD_DIR)/vncviewer/vncviewer \
		$(BUILD_DIR)/unix/x0vncserver/x0vncserver \
		$(PROTODIR)/usr/bin/$(ARCHBINSUBDIR)
	$(INSTALL_SCRIPT) -m 0444 $(JAVA_SOURCE_DIR)/vncviewer/README \
		$(JAVA_SOURCE_DIR)/vncviewer/LICENCE.TXT \
		$(JAVA_BUILD_DIR)/VncViewer.jar $(PROTODIR)/usr/share/vnc/classes

Xvnc_install_data: $(SOURCE_TARGETS)
	mkdir -p $(PROTODIR)/usr/share/man/man1 \
		$(PROTODIR)$(X11_SMF_MANIFEST_DIR)
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/unix/vncserver.man \
		$(PROTODIR)/usr/share/man/man1/vncserver.1
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/unix/vncconfig/vncconfig.man \
		$(PROTODIR)/usr/share/man/man1/vncconfig.1
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/unix/vncpasswd/vncpasswd.man \
		$(PROTODIR)/usr/share/man/man1/vncpasswd.1
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/vncviewer/vncviewer.man \
		$(PROTODIR)/usr/share/man/man1/vncviewer.1
	$(INSTALL_SCRIPT) -m 0444 \
	    $(SOURCE_DIR)/unix/x0vncserver/x0vncserver.man \
		$(PROTODIR)/usr/share/man/man1/x0vncserver.1
	$(INSTALL_SCRIPT) -m 0444 xvnc-inetd.xml \
		$(PROTODIR)$(X11_SMF_MANIFEST_DIR)/
	$(INSTALL_SCRIPT) -m 0555 vnclock.sh \
		$(PROTODIR)/usr/lib/vnclock

# Metadata for vncviewer package
VNCVIEWER_PKG_METADATA_DIR	= $(PROTOMETA)/desktop-remote-desktop-tigervnc
VNCVIEWER_PKG_ATTRDATA_FILE	= $(VNCVIEWER_PKG_METADATA_DIR)/$(ATTRDATA_FILE_NAME)
VNCVIEWER_PKG_LICENSE_FILE	= $(VNCVIEWER_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME)
VNCVIEWER_METADATA_TARGETS = \
	$(VNCVIEWER_PKG_METADATA_DIR) \
	$(VNCVIEWER_PKG_ATTRDATA_FILE) \
	$(VNCVIEWER_PKG_LICENSE_FILE)

install_metadata: $(VNCVIEWER_METADATA_TARGETS)

$(VNCVIEWER_PKG_METADATA_DIR):
	mkdir -p $@

$(VNCVIEWER_PKG_ATTRDATA_FILE) $(VNCVIEWER_PKG_LICENSE_FILE): $(VNCVIEWER_PKG_METADATA_DIR)
