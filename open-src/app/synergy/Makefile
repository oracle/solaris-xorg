###############################################################################
#
# synergy application to share input devices across computers
#
# Copyright (c) 2009, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=synergy

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=desktop/synergy
MODULE_PKGNAME_SET=yes

# Package classification (override default)
# more in line with tigervnc:
MODULE_PKGCLASS=System/X11
MODULE_PKGCLASS_SET=yes

# Package metadata
UPSTREAM		= Synergy
UPSTREAM_URL		= https://github.com/symless/synergy
UPSTREAM_SET		= yes
GIT_URL			= https://github.com/symless/synergy
GIT_URL_SET		= yes

MODULE_LICENSE		= GPLv2
MODULE_LICENSE_SET	= yes
MODULE_LICENSE_PREPEND	= $(GPL_CHOICE_FILE)

LICENSE_FILE		= LICENSE
LICENSE_FILE_SET	= yes

MODULE_DESC = the Synergy mouse and keyboard sharing system
ORACLE_TPNO = 29402

# Version number (used in path names)
MODULE_VERSION=1.7.6

# FYI ARC since first integration of V 1.3.8
MODULE_ARC_CASES=LSARC/2009/489
MODULE_ARC_CASES+=PSARC/2016/436

# Source tarball
SOURCE_TARBALL_NAME=synergy-v1.7.6-stable-bcb9da8-Source.tar.gz
SOURCE_TARBALL_NAME_SET=yes

SOURCE_URL		= $(UPSTREAM_URL)/archive/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET		= yes

# Checksums for upstream tarball
TARBALL_MD5  = a324798a47445de739a7ed4baf12638f
TARBALL_SHA1 = 01936476040b85c08d1077c4dd9614303c78f69b

# make links to source
CLONE_SRC=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = Werrors.cpp.patch documentation.patch in-help.patch ns.patch ellipse_error.patch

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Committed

INSTALL = /usr/bin/ginstall -c
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

MODULE_CONFIG_OPTS = --disable-debug

MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

USE_PARFAIT=no

MODULE_CONFIG_ENV = $(MODULE_ENV)
MODULE_BUILD_ENV = $(CONFIG_ENV)

$(CONFIGURE_TARGETS):
	unpacked-src/hm.sh  conf --skip-gui

$(BUILD_TARGETS): $(CONFIGURE_TARGETS)
	unpacked-src/hm.sh  build --skip-gui


# What to install
INSTALL_TARGETS= install_synergy
INSTALL_TARGETS_SET=yes

include ../Makefile.inc


DESTDIR=$(PROTODIR)

install_synergy: $(BUILD_TARGETS)
	mkdir -p $(PROTODIR)/usr/lib/synergy/plugins$(ARCHLIBSUBDIR) \
		$(PROTODIR)/usr/bin/$(ARCHBINSUBDIR) \
		$(PROTODIR)/usr/share/man/man1 
	$(INSTALL_SCRIPT) -m 0755 $(BUILD_DIR)/bin/synergyc \
		$(BUILD_DIR)/bin/synergys \
		$(PROTODIR)/usr/bin/$(ARCHBINSUBDIR)
	$(INSTALL_SCRIPT) -m 0444 \
		$(BUILD_DIR)/bin/plugins/libns.so \
		$(PROTODIR)/usr/lib/synergy/plugins$(ARCHLIBSUBDIR)
	$(INSTALL_SCRIPT) -m 0444 \
		$(BUILD_DIR)/doc/synergys.man \
		$(PROTODIR)/usr/share/man/man1/synergys.1
	$(INSTALL_SCRIPT) -m 0444 \
		$(BUILD_DIR)/doc/synergyc.man \
		$(PROTODIR)/usr/share/man/man1/synergyc.1
