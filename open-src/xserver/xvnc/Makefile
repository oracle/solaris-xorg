###############################################################################
#
# TigerVNC (Xvnc server & vncviewer) Makefile
# Based on Fedora's tigervnc.spec revision 1.20 from
# http://cvs.fedoraproject.org/viewvc/rpms/tigervnc/devel/tigervnc.spec
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

# Package name used in tarballs
MODULE_NAME=tigervnc

# Version number (used in path names)
MODULE_VERSION=1.0.0

# Checksums for upstream tarball
TARBALL_MD5  = 622c6689a66651419b5431efdee99d5a
TARBALL_SHA1 = 2435ea2e3c34a4055350f7680a94cea580b92979

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Xorg server source tarball to use sources from
XORG_TARBALL=$(TARBALLDIR)/xorg-server-$(XORGSERVER_VERS).tar.bz2
XORG_SOURCE_DIR=$(BUILD_DIR)/xorg-server-$(XORGSERVER_VERS)
XORG_BUILD_DIR=$(SOURCE_DIR)/unix/xserver

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/project/tigervnc/tigervnc/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	upstream-xorg-version.patch,-p1 \
	upstream-input-refactor.patch,-p1 \
	tigervnc-port-to-1.7.patch \
	tigervnc-xserver17.patch \
	vnc-samemachine-ipv6.patch,-p1 \
	c++public.patch,-p1 \
	vncserver-urandom.patch,-p1 \
	vnc-nobuildtime.patch \
	vnc-nohttpd.patch \
	vncviewer-connect.patch,-p1 \
	upstream-modkeystick.patch,-p0 \
	solaris-port.patch


# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= \
	unix/xserver/hw/vnc/Xvnc.man unix/vncviewer/vncviewer.man \
	unix/vncconfig/vncconfig.man unix/vncpasswd/vncpasswd.man \
	unix/vncserver.man unix/x0vncserver/x0vncserver.man
SUN_PACKAGE = SUNWxvnc
MODULE_STABILITY = Volatile

# Additional package name used for vncviewer package
VNCVIEWER_PKGNAME=SUNWvncviewer

# Override specific attributes for specific man pages
MODULE_SUNTOUCH_MAN_FLAGS= \
	$(SUNTOUCH_MAN_FLAGS_$(@F:%.man.$(FIXED_MAN_SUFFIX)=%))
SUNTOUCH_MAN_FLAGS_vncviewer = -o '{Availability, $(VNCVIEWER_PKGNAME)}' \
	-o '{Interface Stability, Path: Committed, Options: Volatile}'

# Compiler to use
MODULE_COMPILER=suncc
MODULE_COMPILER_SET=yes

# Xvnc has to configure, build, and install in different directories due to 
# the mixed codebases and unusual directory layout
MODULE_ADD_SOURCE_TARGETS=Xvnc_source_init
MODULE_SOURCE_DEPS = Xvnc_source_init
CONFIGURE_TARGETS=$(SOURCE_DIR)/unix/Makefile $(XORG_BUILD_DIR)/Makefile
CONFIGURE_TARGETS_SET=yes
BUILD_TARGETS_common=Xvnc_build set-elf-comments
BUILD_TARGETS_32=java_build
BUILD_TARGETS=$(BUILD_TARGETS_common) $(BUILD_TARGETS_$(BUILD_TYPE))
BUILD_TARGETS_SET=yes
INSTALL_TARGETS_common=Xvnc_install_module
INSTALL_TARGETS_32=Xvnc_install_programs java_install
INSTALL_TARGETS=$(INSTALL_TARGETS_common) $(INSTALL_TARGETS_$(BUILD_TYPE))
INSTALL_TARGETS_SET=yes

# Since we only build 64-bit versions of the Xorg servers on SPARC,
# need to build a 32-bit version of the Xserver mdb module to allow
# use with Xvnc on SPARC
BUILD_TARGETS_sparc = build_mdb
BUILD_TARGETS += $(BUILD_TARGETS_$(MACH))
INSTALL_TARGETS_sparc = install_mdb
INSTALL_TARGETS += $(INSTALL_TARGETS_$(MACH))

include ../Makefile.inc

# Merge in additional sources from Xorg tarball & sun-src directory
# This step has to be done before default_source, so that patches applied
# in that step work correctly.
XORG_SOURCE_INIT_TARGET = $(XORG_BUILD_DIR)/$(SOURCE_INIT_TARGET_FILE)
XORG_PATCHED_TARGET = $(XORG_BUILD_DIR)/$(PATCHED_TARGET_FILE)
Xvnc_source_init: $(XORG_PATCHED_TARGET)

$(XORG_TARBALL):
	(cd ../xorg && $(MAKE) $(MAKEFLAGS) download)

$(XORG_SOURCE_INIT_TARGET): $(XORG_TARBALL)
	mkdir -p $(SOURCE_DIR)/unix
	$(MAKE) $(MAKEFLAGS) init_source \
		SOURCE_TARBALL="$(XORG_TARBALL)" \
		SOURCE_UNCOMPRESS="bzcat" \
		SOURCE_DIR="$(XORG_SOURCE_DIR)"	
	mv $(XORG_SOURCE_DIR) $(XORG_BUILD_DIR)
	(cd $(XORG_BUILD_DIR) && $(LNDIR) ../../../../../xorg/sun-src)

$(XORG_PATCHED_TARGET): $(XORG_SOURCE_INIT_TARGET)
	$(MAKE) $(MAKEFLAGS) patched_source \
		SOURCE_PATCHES="$(XORG_SOURCE_PATCHES:%=../xorg/%)" \
		SOURCE_DIR="$(XORG_BUILD_DIR)"


# Command line options to GNU autoconf configure script for vnc clients
VNC_CONFIG_OPTS= --with-installed-zlib --prefix=/usr \
	--mandir='$${prefix}/share/man' --libdir='$${exec_prefix}/lib/vnc' \
	--localstatedir=/var --with-system-jpeg --without-simd

# Command line options to GNU autoconf configure script for Xvnc
XVNC_CONFIG_OPTS = $(DEFAULT_CONFIG_OPTS) $(XSERVER_CONFIG_OPTS) \
	--disable-builddocs --without-int10 \
	--disable-dga --disable-xfree86-utils --disable-config-hal \
	--disable-xorg --disable-xnest --disable-xvfb --disable-dmx \
	--disable-xwin --disable-xephyr --disable-kdrive


MODULE_CPPFLAGS = \
	-D_XOPEN_SOURCE=500 -D__EXTENSIONS__ \
	-D__extension__=\"\" \
	-I$(PWD)/$(SOURCE_DIR)/common \
	-I$(PWD)/$(SOURCE_DIR)/unix/vncconfig

CFLAGS_gcc += -fPIC

# Environment variable options to GNU autoconf configure script
XVNC_CONFIG_ENV = LIBS="-lsocket -lnsl" \
	LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(MODTYPE_LDFLAGS) $(MODULE_LDFLAGS)"
CONFIG_ENV += $(XVNC_CONFIG_ENV) 

LDPATH_ADDITIONS_sparc = \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions$(ARCHLIBSUBDIR) \
        -R$(X11_SERVERMODS_DIR)/extensions$(ARCHLIBSUBDIR)
LDPATH_ADDITIONS_i386 = \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions$(ARCHLIBSUBDIR) \
        -R$(X11_SERVERMODS_DIR)/extensions$(ARCHLIBSUBDIR)
LDPATH_ADDITIONS=$(LDPATH_ADDITIONS_$(MACH))

MODULE_LDFLAGS = $(LDPATH_ADDITIONS)

# Needed by unix/xorg-x11-server-source/hw/vnc/Makefile.am to find libpixman
# headers in proto area after solaris-port.patch is applied
MODULE_MAKEFLAGS = PROTODIR=$(PROTODIR)

# Run configure scripts for vnc sources
$(SOURCE_DIR)/unix/Makefile: $(SOURCE_TARGETS)
	(cd $(SOURCE_DIR)/common && \
		$(AUTORECONF_ENV) autoreconf --install --force && \
		$(CONFIG_ENV) $(SHELL) ./configure $(VNC_CONFIG_OPTS) )
	(cd $(SOURCE_DIR)/unix && \
		$(AUTORECONF_ENV) autoreconf --install --force && \
		$(CONFIG_ENV) $(SHELL) ./configure $(VNC_CONFIG_OPTS) )


# Run configure script for Xvnc
$(XORG_BUILD_DIR)/Makefile: $(SOURCE_TARGETS)
	(cd $(XORG_BUILD_DIR) &&  \
		$(AUTORECONF_ENV) autoreconf --install --force && \
		$(CONFIG_ENV) $(SHELL) ./configure $(XVNC_CONFIG_OPTS) )

Xvnc_build: $(CONFIGURE_TARGETS)
	(cd $(SOURCE_DIR)/unix ; \
	 $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS))
	(cd $(XORG_BUILD_DIR) ; \
	 $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS))

Xvnc_install_module: $(BUILD_TARGETS)
	(cd $(XORG_BUILD_DIR)/hw/vnc \
	 &&  $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	 CPPROG="cp -p" install)

Xvnc_install_programs: $(BUILD_TARGETS)
	mkdir -p $(PROTODIR)/usr/bin/$(ARCHLIBSUBDIR) \
		$(PROTODIR)/usr/share/man/man1 \
		$(PROTODIR)/var/svc/manifest/application/x11 
	cp -pf $(SOURCE_DIR)/unix/vncserver \
		$(SOURCE_DIR)/unix/vncpasswd/vncpasswd \
		$(SOURCE_DIR)/unix/vncconfig/vncconfig \
		$(SOURCE_DIR)/unix/vncviewer/vncviewer \
		$(SOURCE_DIR)/unix/x0vncserver/x0vncserver \
		$(PROTODIR)/usr/bin/$(ARCHLIBSUBDIR)
	cp -pf $(SOURCE_DIR)/unix/vncserver.man \
		$(PROTODIR)/usr/share/man/man1/vncserver.1
	cp -pf $(SOURCE_DIR)/unix/vncconfig/vncconfig.man \
		$(PROTODIR)/usr/share/man/man1/vncconfig.1
	cp -pf $(SOURCE_DIR)/unix/vncpasswd/vncpasswd.man \
		$(PROTODIR)/usr/share/man/man1/vncpasswd.1
	cp -pf $(SOURCE_DIR)/unix/vncviewer/vncviewer.man \
		$(PROTODIR)/usr/share/man/man1/vncviewer.1
	cp -pf $(SOURCE_DIR)/unix/x0vncserver/x0vncserver.man \
		$(PROTODIR)/usr/share/man/man1/x0vncserver.1
	cp -pf xvnc-inetd.xml $(PROTODIR)/var/svc/manifest/application/x11/

# Java VNCviewer applet for connecting from a web browser
JAVA_SOURCE_DIR = $(SOURCE_DIR)/java/src
JAVA_INSTALL_DIR = $(PROTODIR)/usr/share/vnc/classes
JAVA_JAR_FILE = $(JAVA_SOURCE_DIR)/VncViewer.jar

# Temporary special handling of Java build fix submitted upstream
JAVA_MAKEFILE = $(JAVA_SOURCE_DIR)/Makefile
BUILD_TARGETS += $(JAVA_MAKEFILE)

$(JAVA_MAKEFILE): $(SOURCE_TARGETS)
	cp $(JAVA_SOURCE_DIR)/com/tigervnc/vncviewer/Makefile $@
	gpatch -d $(SOURCE_DIR) -p0 < applet-build.patch
	gpatch -d $(SOURCE_DIR) -p0 < icon.patch
	cp tigervnc_16.gif $(JAVA_SOURCE_DIR)

java_build: $(JAVA_JAR_FILE)

$(JAVA_JAR_FILE): $(SOURCE_TARGETS) $(JAVA_MAKEFILE)
	(cd $(JAVA_SOURCE_DIR) ; \
	 $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS))

java_install: $(JAVA_JAR_FILE)
	-rm -rf $(JAVA_INSTALL_DIR)
	mkdir -p $(JAVA_INSTALL_DIR)
	(cd $(JAVA_SOURCE_DIR) ; \
	 $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS) \
		install INSTALL_DIR=$(JAVA_INSTALL_DIR))
	cp -pf  $(JAVA_SOURCE_DIR)/com/tigervnc/vncviewer/README \
		$(JAVA_SOURCE_DIR)/com/tigervnc/vncviewer/LICENCE.TXT \
		$(JAVA_INSTALL_DIR)
