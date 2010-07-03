###############################################################################
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
MODULE_NAME=docbook-utils

# Package metadata
UPSTREAM                = Red Hat
UPSTREAM_URL            = http://sourceware.org/docbook-tools/
UPSTREAM_SET            = yes

MODULE_LICENSE          = GPLv2
MODULE_LICENSE_SET      = yes

# Version number (used in path names)
MODULE_VERSION=0.6.14

# Checksums for upstream tarball
TARBALL_MD5  = 6b41b18c365c01f225bc417cf632d81c
TARBALL_SHA1 = aae596844930e0e4bb7f8824024be0982cc4fa85

# Source tarball
SOURCE_TARBALL_NAME=docbook-utils-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Download site for source
SOURCE_URL=ftp://sources.redhat.com/pub/docbook-tools/new-trials/SOURCES/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= solaris-port.patch

# Regenerate Makefile.in's from Makefile.am's after solaris-port.patch
AUTORECONF=yes

# Only used during build, so install in tools directory in proto area
MODULE_PREFIX=$(TOOLS_DIR)
MODULE_PREFIX_SET=yes
MODULE_MAKEFLAGS= DESTDIR=""

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Catalog generated to find SGML files
CATALOG=$(SOURCE_DIR)/catalog

# Need to use local catalog during build due to empty /etc/sgml/catalog
# on OpenSolaris systems
MODULE_BUILD_DEPS = $(CATALOG)
MODULE_ADD_INSTALL_TARGETS = install_catalog

### Include common rulesets
include ../Makefile.inc

$(CATALOG): catalog.in $(SOURCE_TARGETS)
	sed 's%__sgmldir__%$(PROTODIR)$(X11_DIR)/share/sgml%' < catalog.in > $@

install_catalog: $(CATALOG)
	mkdir -p $(MODULE_PREFIX)/share/sgml
	cp -p $(CATALOG) $(MODULE_PREFIX)/share/sgml
