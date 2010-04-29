###############################################################################
#
# Copyright (c) 2004, 2010, Oracle and/or its affiliates. All rights reserved.
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
#
# Files for integration with Solaris Service Management Framework (SMF) and
# Role Based Access Control (RBAC)

MODULE_NAME=smf-rbac

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=NONE
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
SOURCE_TARGETS=
SOURCE_TARGETS_SET=yes

CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

BUILD_TARGETS = x11-server.xml check
BUILD_TARGETS_SET=yes

INSTALL_TARGETS = install_common install_ogl-select
INSTALL_TARGETS_SET=yes

### Include common rulesets
include ../Makefile.inc

SUN_SMF_MANIFEST_DIR=/var/svc/manifest/application
SUN_SMF_METHOD_DIR=/lib/svc/method
DESTDIR=$(PROTODIR)

DEFAULT_XSERVER=$(X11_BIN_DIR)/Xorg

x11-server.xml: x11-server.xml.in
	sed -e 's|@DEFAULT_XSERVER@|$(DEFAULT_XSERVER)|' < x11-server.xml.in > $@

check: x11-server.xml ogl-select.xml
	/usr/sbin/svccfg validate x11-server.xml
	/usr/sbin/svccfg validate ogl-select.xml

clean_common:
	$(RM) x11-server.xml

clean: clean_common

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

OGL_VENDOR_SCRIPTS_sparc=sun_vendor_select
OGL_VENDOR_SCRIPTS_i386=nvidia_vendor_select
OGL_VENDOR_SCRIPTS=mesa_vendor_select $(OGL_VENDOR_SCRIPTS_$(MACH))

install_ogl-select: install_common
	mkdir -p $(DESTDIR)$(SUN_SMF_MANIFEST_DIR)/opengl \
		$(DESTDIR)$(SUN_SMF_METHOD_DIR) \
		$(DESTDIR)/lib/opengl/ogl_select/
	cp -pf ogl-select.xml $(DESTDIR)$(SUN_SMF_MANIFEST_DIR)/opengl
	cp -pf ogl-select $(DESTDIR)$(SUN_SMF_METHOD_DIR)/ogl-select
	cp -pf $(OGL_VENDOR_SCRIPTS) $(DESTDIR)/lib/opengl/ogl_select/

