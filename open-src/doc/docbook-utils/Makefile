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
# @(#)Makefile	1.4	08/03/07
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Version number (used in path names)
DOCBOOK_TOOLS_VERS=0.6.14

# Source tarball
SOURCE_TARBALL_NAME=docbook-utils-$(DOCBOOK_TOOLS_VERS).tar.gz
SOURCE_UNCOMPRESS=gzcat

# Download site for source
SOURCE_URL=ftp://sources.redhat.com/pub/docbook-tools/new-trials/SOURCES/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= solaris-port.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/docbook-utils-$(DOCBOOK_TOOLS_VERS)

# What to build
BUILD_TARGETS=$(APP_BIN)

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --prefix=$(PROTODIR)$(TOOLS_DIR)

# Environment variable options to GNU autoconf configure script
CONFIG_ENV = CC=$(CC) CFLAGS="$(PROG_CFLAGS)" LD="$(LD) $(LDFLAGS)" \
	CPPFLAGS="-I $(PROTODIR)$(X11_INCLUDES_DIR)" \
	LDFLAGS="$(PROG_LDFLAGS) -L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR) $(APP_ADD_LDFLAGS)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR)

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; autoreconf && \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )

# Binary built in tree
APP_BIN=$(SOURCE_DIR)/src/$(APP_NAME)

# Catalog generated to find SGML files
CATALOG=$(BUILD_DIR)/catalog

build_gen: $(APP_BIN) $(CATALOG)

$(APP_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(GNUMAKE) $(MFLAGS) )

install_gen:: $(APP_BIN) $(CATALOG)
	(cd $(SOURCE_DIR) ; $(GNUMAKE) $(MFLAGS) install)
	cp -p $(CATALOG) $(PROTODIR)$(TOOLS_DIR)/share/sgml

$(CATALOG):
	echo "OVERRIDE YES" > $@
	echo "" >> $@
	echo 'SYSTEM "X11/defs.ent" "$(PROTODIR)$(X11_DIR)/share/sgml/X11/defs.ent"' >> $@
	echo "CATALOG /etc/sgml/catalog" >> $@
	echo "CATALOG /usr/share/sgml/docbook/dsssl-stylesheets/catalog" >> $@
	echo "CATALOG /usr/share/sgml/openjade/catalog" >> $@
