###############################################################################
#
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
# @(#)Makefile.inc	1.1	06/09/15
#

### Makefile settings/rules common to all X.Org protocol modules

PWD:sh=pwd
TOP=$(PWD)/../..

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(VERSION).tar.bz2
SOURCE_TARBALL_DIR=data

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(MODULE_NAME)-$(VERSION)

### Include common rulesets
include $(TOP)/common/Makefile.inc

# Command line options to GNU autoconf configure script
DATA_CFG=--prefix=$(X11_DIR)

build_gen: $(SOURCE_DIR)/Makefile

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x configure ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LD="$(CC) $(LIB_CFLAGS) $(LDFLAGS)" LDFLAGS="$(LIB_LDFLAGS)" ./configure $(DATA_CFG) )

# Install
install_gen:: $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; \
	 $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) pkgconfigdir=/usr/lib/pkgconfig install ; \
	 $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) pkgconfigdir=/usr/lib/$(SUBDIR64)/pkgconfig install-pkgconfigDATA )



