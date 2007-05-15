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
# @(#)Makefile	1.3	07/05/15
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Version number (used in path names)
VERSION=1.0.1

# Source tarball
SOURCE_TARBALL_NAME=lndir-$(VERSION).tar.bz2
SOURCE_TARBALL_DIR=util

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/lndir-$(VERSION)

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Command line options to GNU autoconf configure script
LNDIR_CFG=--prefix=$(X11_DIR)

build_gen: $(SOURCE_DIR)/Makefile

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x configure ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LD="$(CC) $(PROG_CFLAGS) $(LDFLAGS)" LDFLAGS="$(PROG_LDFLAGS)" PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/pkgconfig:$(PROTODIR)/usr/X11/lib/pkgconfig ./configure $(LNDIR_CFG) )

# Install
install_gen: $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)
