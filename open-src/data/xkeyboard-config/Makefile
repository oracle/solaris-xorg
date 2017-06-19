###############################################################################
#
# xkeyboard-config Xserver keyboard layouts
#
# Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xkeyboard-config

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/keyboard/data-xkb
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = X.Org Foundation Xserver keyboard layouts

# Version number (used in path names)
MODULE_VERSION=2.16

# Checksums for upstream tarball
TARBALL_MD5   = bf6aa31195584cfce01b2194c3e5ea26
TARBALL_SHA1  = 6b2f374ccef542eed8ac29f81996cf9ca81cd3b6
TARBALL_SHA256= 1e8a1f212c96b7f71ca0b05da361cac1816d27baae1d8e03e7b53049b58d1d58

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
compat_ledcompose.patch \
keycodes_xfree86.patch \
rules_base_list.patch \
rules_base_o_c_part.patch \
rules_base_xml_in.patch \
symbols_at.patch \
symbols_lk.patch \
symbols_macintosh_vndr.patch \
symbols_pc.patch \
symbols.patch

ADDITIONAL_SOURCE_DIR=sun-src

MODULE_LICENSE          = MIT
MODULE_LICENSE_SET      = yes

LICENSE_FILE            = COPYING
LICENSE_FILE_SET        = yes

# ARC cases that covered this module
# PSARC/2009/117 XKB data migration to xkeyboard-config
MODULE_ARC_CASES = PSARC/2009/117
# PSARC/2009/440 xkeyboard-config update from v1.4 to v1.6
MODULE_ARC_CASES += PSARC/2009/440
# PSARC/2010/303 xkeyboard-config update from v1.6 to v1.9
MODULE_ARC_CASES += PSARC/2010/303

MODULE_STABILITY=Uncommited

# need gnu xgettext. msgfmt is picked from PATH
MODULE_CONFIG_ENV=XGETTEXT=gxgettext PATH=/usr/gnu/bin:$(PATH) MAKE=$(GNUMAKE)
# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

MODULE_ADD_INSTALL_TARGETS = sun_src

ORACLE_TPNO = 26022

include ../Makefile.inc

CP = cp -pf
PKGCONF32=$(PROTODIR)$(PKGCONFIG_DIR_32)

sun_src: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)/usr/share/X11/xkb/compat \
	    $(PROTODIR)/usr/share/X11/xkb/compiled
	$(CP) $(SOURCE_DIR)/compiled/README \
	    $(PROTODIR)/usr/share/X11/xkb/compiled/
	$(CP) $(SOURCE_DIR)/xkbtable.map $(PROTODIR)/usr/share/X11/xkb/
	$(CP) $(SOURCE_DIR)/compat/ledcompose \
	    $(PROTODIR)/usr/share/X11/xkb/compat/
	mkdir -p $(PKGCONF32)
	ln -s ../$(SUBDIR64)/pkgconfig/xkeyboard-config.pc $(PKGCONF32)
	$(CP) $(SOURCE_DIR)/README $(PROTODIR)/usr/share/X11/xkb/
	mkdir -p $(PROTODIR)/usr/X11/share/X11
	ln -s ../../../share/X11/xkb $(PROTODIR)/usr/X11/lib/X11/xkb
	ln -s ../../../share/X11/xkb $(PROTODIR)/usr/X11/share/X11/xkb
	mkdir -p $(PROTODIR)/usr/share/X11/xkb/rules
	ln -s base $(PROTODIR)/usr/share/X11/xkb/rules/xfree86
	ln -s base.lst $(PROTODIR)/usr/share/X11/xkb/rules/xfree86.lst
	ln -s base.xml $(PROTODIR)/usr/share/X11/xkb/rules/xfree86.xml
	ln -s base $(PROTODIR)/usr/share/X11/xkb/rules/xorg
	ln -s base.lst $(PROTODIR)/usr/share/X11/xkb/rules/xorg.lst
	ln -s base.xml $(PROTODIR)/usr/share/X11/xkb/rules/xorg.xml
	ln -s ../../../X11/bin/xkbcomp $(PROTODIR)/usr/share/X11/xkb/xkbcomp

