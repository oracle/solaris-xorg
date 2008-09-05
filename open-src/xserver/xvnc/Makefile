###############################################################################
#
# Xvnc server Makefile
# Based on Fedora's vnc-4.1.2-21.fc8/vnc.spec
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
# @(#)Makefile	1.13 08/09/05
#

# Package name used in tarballs
MODULE_NAME=vnc

# Version number (used in path names)
MODULE_VERSION=4_1_2-unixsrc

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Xorg server source tarball to use sources from
XORG_TARBALL=$(TARBALLDIR)/xorg-server-$(XORGSERVER_VERS).tar.bz2
XORG_SOURCE_DIR=$(BUILD_DIR)/xorg-server-$(XORGSERVER_VERS)
XORG_BUILD_DIR=$(SOURCE_DIR)/unix/xorg-x11-server-source

# Download site for source
# - Master source site doesn't allow wget downloads, so we use a mirror
#SOURCE_URL=http://www.realvnc.com/dist/$(SOURCE_TARBALL_NAME)
SOURCE_URL=http://ltsp.mirrors.tds.net/pub/ltsp/tarballs/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	vnc-use-fb.patch,-p1 \
	vnc-restart.patch,-p1 \
	vnc-modular-xorg.patch,-p1 \
	vnc-nohttpd.patch,-p1 \
	vnc-int10.patch,-p1 \
	vnc-64bit.patch,-p1 \
	vnc-render.patch,-p1 \
	vnc-autotools.patch,-p1 \
	vnc-autotools-compile.patch,-p1 \
	vnc-always_use_fb.patch,-p1 \
	vnc-24bit.patch,-p1 \
	vnc-ipv6.patch,-p1 \
	c++public.patch \
	build-without-Xorg.patch \
	solaris-port.patch

# Man pages to apply Sun footer to & attributes to list
# Common ones are here - source_gen target below has more with
# different attributes/path set
SUNTOUCHED_MANPAGES= \
	unix/vncconfig/vncconfig.man unix/vncpasswd/vncpasswd.man \
	unix/vncserver.man unix/x0vncserver/x0vncserver.man
SUNTOUCH_MAN_FLAGS= -p /usr/bin/ \
 -a '{Availability, SUNWxvnc} {Interface Stability, Volatile}'

# Compiler to use
MODULE_COMPILER=suncc
MODULE_COMPILER_SET=yes

# Xvnc has to configure, build, and install in different directories due to 
# the mixed codebases and unusual directory layout
CONFIGURE_TARGETS=$(SOURCE_DIR)/unix/Makefile $(XORG_BUILD_DIR)/Makefile
CONFIGURE_TARGETS_SET=yes
BUILD_TARGETS=Xvnc_build
BUILD_TARGETS_SET=yes
INSTALL_TARGETS=Xvnc_install
INSTALL_TARGETS_SET=yes

# Default rule to make, so that the source_gen rule below doesn't become
# default by virtue of being first
Xvnc_default: all

# Merge in additional sources from Xorg tarball & sun-src directory
# Use layout from Fedora RPM so their patches apply
# This step has to be done before including common/Makefile.inc so
# that patches applied in that step work correctly.
source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)/unix
	bzcat $(XORG_TARBALL) | (cd $(BUILD_DIR) ; tar -xf - )
	@if [ "x$(XORG_SOURCE_PATCHES)" != "x" ] ; then \
	    for p in $(XORG_SOURCE_PATCHES) x ; do \
	        if [ "$$p" != "x" ] ; then \
		    PATCH_CMD=`echo $$p | awk -F, '{if (NF > 1) FLAGS = $$2; else FLAGS="-p0" ; printf "gpatch -d $(XORG_SOURCE_DIR) %s < ../xorg/%s\n", FLAGS, $$1}'` ; \
		    echo "$${PATCH_CMD}" ; \
		    eval $${PATCH_CMD} ; \
		fi ; \
	    done ; \
	fi
	mv $(XORG_SOURCE_DIR) $(XORG_BUILD_DIR)
	(cd $(XORG_BUILD_DIR) && $(LNDIR) ../../../../../xorg/sun-src)
	(mkdir -p $(XORG_BUILD_DIR)/GL/mesa/vbo)
	(cp ../xorg/sun-src/mesa/Makefile.am $(XORG_BUILD_DIR)/GL/mesa/vbo)


include ../Makefile.inc

# Merge in additional sources from Xorg tarball & sun-src directory
# Use layout from Fedora RPM so their patches apply
# This step has to be done after including common/Makefile.inc since
# it depends on files created in that stage
source_gen:: 
	(cd $(SOURCE_DIR) ; \
	cp -p unix/xc/programs/Xserver/vnc/Xvnc/xvnc.cc \
		unix/xc/programs/Xserver/Xvnc.man \
		unix/xc/programs/Xserver/vnc/*.h \
		unix/xc/programs/Xserver/vnc/*.cc \
		unix/xorg-x11-server-source/hw/vnc/ ; \
	cp -p unix/xorg-x11-server-source/cfb/cfb.h \
		unix/xorg-x11-server-source/fb/fb.h \
		unix/xorg-x11-server-source/fb/fbrop.h \
		unix/xorg-x11-server-source/hw/vnc/ ; \
	perl -i -p -e 's,\b(xor|and)\b,c_$$1,g' \
		unix/xorg-x11-server-source/hw/vnc/cfb.h \
		unix/xorg-x11-server-source/hw/vnc/fb.h \
		unix/xorg-x11-server-source/hw/vnc/fbrop.h )
	(cd $(SOURCE_DIR) && \
		/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
		-a '{Availability, SUNWvncviewer} {Interface Stability, Volatile}' \
		-p /usr/bin/ unix/vncviewer/vncviewer.man )
	(cd $(SOURCE_DIR) && \
		/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
		-a '{Availability, SUNWxvnc} {Interface Stability, Volatile}' \
		-p /usr/X11/bin/ unix/xc/programs/Xserver/Xvnc.man )

# Command line options to GNU autoconf configure script for vnc clients
VNC_CONFIG_OPTS= --with-installed-zlib --prefix=/usr \
	--mandir='$${prefix}/share/man' --libdir='$${exec_prefix}/lib/vnc' \
	--localstatedir=/var

# Command line options to GNU autoconf configure script for Xvnc
XVNC_CONFIG_OPTS = $(DEFAULT_CONFIG_OPTS) $(XSERVER_CONFIG_OPTS) \
        --disable-builddocs --with-int10=stub \
	--disable-dga --disable-xfree86-utils --disable-dri \
	--disable-xorg --disable-dmx --disable-xvfb --disable-xnest \
	--disable-xprint --disable-kdrive

MODULE_CPPFLAGS = \
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
        -R$(X11_SERVERMODS_DIR)/extensions/mesa/$(ARCHLIBSUBDIR) \
        -R$(X11_SERVERMODS_DIR)/extensions$(ARCHLIBSUBDIR)
LDPATH_ADDITIONS=$(LDPATH_ADDITIONS_$(MACH))

MODULE_LDFLAGS = $(LDPATH_ADDITIONS)

# Run configure scripts for vnc sources
$(SOURCE_DIR)/unix/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR)/common && autoreconf --install --force && \
		$(CONFIG_ENV) $(SHELL) ./configure $(VNC_CONFIG_OPTS) )
	(cd $(SOURCE_DIR)/unix && autoreconf --install --force && \
		$(CONFIG_ENV) $(SHELL) ./configure $(VNC_CONFIG_OPTS) )

# Run configure script for Xvnc
# Need to run delibtoolize to fix libtool interference with our linker 
# flags, but need to make sure we use large pic flags (-KPIC/-fPIC)
$(XORG_BUILD_DIR)/Makefile: $(UNPACK_TARGET) $(SOURCE_DIR)/unix/Makefile
	(cd $(XORG_BUILD_DIR) &&  \
	 ACLOCAL="$(ACLOCAL_PATH)" autoreconf --install --force && \
	 $(CONFIG_ENV) $(SHELL) ./configure $(XVNC_CONFIG_OPTS) )
	$(TOP)/common/delibtoolize.pl -P -s $(SOURCE_DIR)

Xvnc_build: $(SOURCE_DIR)/unix/Makefile $(XORG_BUILD_DIR)/Makefile
	(cd $(SOURCE_DIR)/common ; $(BUILD_ENV) $(MAKE) $(MAKEFLAGS))
	(cd $(SOURCE_DIR)/unix ; $(BUILD_ENV) $(MAKE) $(MAKEFLAGS))
	(cd $(XORG_BUILD_DIR) ; $(BUILD_ENV) $(MAKE) $(MAKEFLAGS))

Xvnc_install::
	(cd $(XORG_BUILD_DIR)/hw/vnc \
	 &&  $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) -e install \
	 DESTDIR=$(PROTODIR) CPPROG="cp -p" )
	mkdir -p $(PROTODIR)/usr/bin/$(ARCHLIBSUBDIR) \
		$(PROTODIR)/usr/share/man/man1 \
		$(PROTODIR)$(X11_MAN_DIR)/man/man1 \
		$(PROTODIR)/usr/share/vnc \
		$(PROTODIR)/var/svc/manifest/application/x11 
	cp -pf $(SOURCE_DIR)/unix/vncserver \
		$(SOURCE_DIR)/unix/vncpasswd/vncpasswd \
		$(SOURCE_DIR)/unix/vncconfig/vncconfig \
		$(SOURCE_DIR)/unix/vncviewer/vncviewer \
		$(PROTODIR)/usr/bin/$(ARCHLIBSUBDIR)
	cp -pf $(SOURCE_DIR)/unix/vncserver.man \
		$(PROTODIR)/usr/share/man/man1/vncserver.1
	cp -pf $(SOURCE_DIR)/unix/vncconfig/vncconfig.man \
		$(PROTODIR)/usr/share/man/man1/vncconfig.1
	cp -pf $(SOURCE_DIR)/unix/vncpasswd/vncpasswd.man \
		$(PROTODIR)/usr/share/man/man1/vncpasswd.1
	cp -pf $(SOURCE_DIR)/unix/vncviewer/vncviewer.man \
		$(PROTODIR)/usr/share/man/man1/vncviewer.1
	cp -pf $(SOURCE_DIR)/common/javabin/index.vnc \
		$(PROTODIR)/usr/share/vnc
	cp -pf $(XORG_BUILD_DIR)/hw/vnc/Xvnc.man \
		$(PROTODIR)$(X11_MAN_DIR)/man1/Xvnc.1
	cp -pf xvnc-inetd.xml $(PROTODIR)/var/svc/manifest/application/x11/
