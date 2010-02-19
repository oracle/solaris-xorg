###############################################################################
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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

# Package name used in tarballs
MODULE_NAME=docbook-utils

# Version number (used in path names)
MODULE_VERSION=0.6.14

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
MODULE_PREFIX=$(PROTODIR)$(TOOLS_DIR)
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

$(CATALOG): catalog.in
	sed 's%__sgmldir__%$(PROTODIR)$(X11_DIR)/share/sgml%' < catalog.in > $@

install_catalog: $(CATALOG)
	mkdir -p $(MODULE_PREFIX)/share/sgml
	cp -p $(CATALOG) $(MODULE_PREFIX)/share/sgml
