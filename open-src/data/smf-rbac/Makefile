###############################################################################
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
#
# ident "@(#)Makefile 1.6     07/10/09 SMI"
#
# Files for integration with Solaris Service Management Framework (SMF) and
# Role Based Access Control (RBAC)

# No upstream sources for these files, since they're Solaris-specific
SOURCE_TARBALL_NAME=NONE

# Build per-arch, not per-bitsize
BUILD_TYPES_SET=yes
BUILD_TYPES=$(MACH)

### Include common rulesets
PWD:sh=pwd
TOP=$(PWD)/../..
include $(TOP)/common/Makefile.inc

source_i386:

source_sparc:

SUN_SMF_MANIFEST_DIR=/var/svc/manifest/application
SUN_SMF_METHOD_DIR=/lib/svc/method
DESTDIR=$(PROTODIR)

BUILD_TARGETS = x11-server.xml check

build_common: $(BUILD_TARGETS)
build_sparc: build_common
build_i386: build_common

DEFAULT_XSERVER_sparc=/usr/openwin/bin/Xsun
DEFAULT_XSERVER_i386=/usr/X11/bin/Xorg
DEFAULT_XSERVER=$(DEFAULT_XSERVER_$(MACH))

x11-server.xml: x11-server.xml.in
	sed -e 's|@DEFAULT_XSERVER@|$(DEFAULT_XSERVER)|' < x11-server.xml.in > $@

check:: x11-server.xml ogl-select.xml
	/usr/sbin/svccfg validate x11-server.xml
	/usr/sbin/svccfg validate ogl-select.xml

clean_common: 
	$(RM) x11-server.xml

clean_sparc: clean_common
clean_i386: clean_common

RBAC_ATTR_FILES=auth_attr exec_attr prof_attr 
RBAC_AUTH_HELP=X11States.html FontStates.html
RBAC_PROFILE_HELP=RtDesktopConfiguration.html

install_common: $(BUILD_TARGETS)
	mkdir -p $(DESTDIR)$(SUN_SMF_MANIFEST_DIR)/x11 \
		$(DESTDIR)$(SUN_SMF_METHOD_DIR) \
		$(DESTDIR)$(X11_DIR)/bin \
		$(DESTDIR)$(X11_DIR)/share/man/man1 \
		$(DESTDIR)/etc/security \
		$(DESTDIR)/usr/lib/help/auths/locale/C \
		$(DESTDIR)/usr/lib/help/profiles/locale/C
	cp -pf x11-server.xml $(DESTDIR)$(SUN_SMF_MANIFEST_DIR)/x11
	cp -pf x11-server $(DESTDIR)$(SUN_SMF_METHOD_DIR)
	cp -pf Xserver $(DESTDIR)$(X11_DIR)/bin
	cp -pf $(RBAC_ATTR_FILES) $(DESTDIR)/etc/security
	cp -pf $(RBAC_AUTH_HELP) $(DESTDIR)/usr/lib/help/auths/locale/C
	cp -pf $(RBAC_PROFILE_HELP) $(DESTDIR)/usr/lib/help/profiles/locale/C
	cp -pf ogl-select.man $(DESTDIR)$(X11_DIR)/share/man/man1/ogl-select.1

install_sparc: install_common

OGL_VENDOR_SCRIPTS=nvidia_vendor_select mesa_vendor_select

install_i386: install_common
	mkdir -p $(DESTDIR)$(SUN_SMF_MANIFEST_DIR)/opengl \
		$(DESTDIR)$(SUN_SMF_METHOD_DIR) \
		$(DESTDIR)/lib/opengl/ogl_select/
	cp -pf ogl-select.xml $(DESTDIR)$(SUN_SMF_MANIFEST_DIR)/opengl
	cp -pf ogl-select $(DESTDIR)$(SUN_SMF_METHOD_DIR)/ogl-select
	cp -pf $(OGL_VENDOR_SCRIPTS) $(DESTDIR)/lib/opengl/ogl_select/

