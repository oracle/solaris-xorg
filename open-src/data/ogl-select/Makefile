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

MODULE_NAME=ogl-select

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=service/opengl/ogl-select
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# LSARC 2005/700 OpenGL boot time selection of libraries and headers
MODULE_ARC_CASES = LSARC/2005/700

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=NONE
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
SOURCE_TARGETS=
SOURCE_TARGETS_SET=yes

CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

BUILD_TARGETS = check
BUILD_TARGETS_SET=yes

INSTALL_TARGETS = install_ogl-select
INSTALL_TARGETS_SET=yes

### Include common rulesets
include ../Makefile.inc

OGL_SMF_MANIFEST_DIR=$(SMF_MANIFEST_DIR)/application/opengl
DESTDIR=$(PROTODIR)

check: ogl-select.xml
	/usr/sbin/svccfg validate ogl-select.xml

OGL_VENDOR_SCRIPTS_sparc=sun_vendor_select
OGL_VENDOR_SCRIPTS_i386=nvidia_vendor_select
OGL_VENDOR_SCRIPTS=mesa_vendor_select $(OGL_VENDOR_SCRIPTS_$(MACH))

install_ogl-select:
	mkdir -p $(DESTDIR)$(OGL_SMF_MANIFEST_DIR) \
		$(DESTDIR)$(SMF_METHOD_DIR) \
		$(DESTDIR)$(X11_DIR)/share/man/man1 \
		$(DESTDIR)/lib/opengl/ogl_select/
	$(INSTALL_SCRIPT) -c -m 0444 ogl-select.xml \
		$(DESTDIR)$(OGL_SMF_MANIFEST_DIR)
	$(INSTALL_SCRIPT) -c -m 0555 ogl-select \
		$(DESTDIR)$(SMF_METHOD_DIR)/ogl-select
	$(INSTALL_SCRIPT) -c -m 0444 ogl-select.man \
		$(DESTDIR)$(X11_DIR)/share/man/man1/ogl-select.1
	$(INSTALL_SCRIPT) -c -m 0555 $(OGL_VENDOR_SCRIPTS) \
		$(DESTDIR)/lib/opengl/ogl_select/
