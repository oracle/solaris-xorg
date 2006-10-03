###############################################################################
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile.inc	1.1	06/10/02
#

### Makefile settings/rules common to all X.Org application modules

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32

# Source tarball
SOURCE_TARBALL_NAME=$(APP_NAME)-$(APP_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(XORG_RELEASES_URL)/app/$(SOURCE_TARBALL_NAME)

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(APP_NAME)-$(APP_VERS)

include $(TOP)/common/Makefile.inc

# Where to install files
APP_prefix=$(X11_DIR)

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --prefix=$(APP_prefix) --mandir='$${prefix}/share/man' \
	$(APP_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
CONFIG_ENV = CC=$(CC) CFLAGS="$(PROG_CFLAGS)" \
	LD="$(LD) $(LDFLAGS)"  LDFLAGS="$(PROG_LDFLAGS)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR) 

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; chmod a+x configure ; \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )

# Binary built in tree
APP_BIN=$(SOURCE_DIR)/src/$(APP_NAME)

build_gen: $(APP_BIN)

$(APP_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) )

install_gen: $(APP_BIN)
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)
