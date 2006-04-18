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
# @(#)Makefile	1.1	06/04/11
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Not really 32 or 64-bit, but we call it 32-bit to avoid subdirs
BUILD_TYPES=32 

# Version number (used in path names)
VERSION=7.0.4

# Source tarball
SOURCE_TARBALL_NAME=xproto-$(VERSION).tar.bz2

# Download site for source
SOURCE_URL=$(XORG_RELEASES_URL)/proto/$(SOURCE_TARBALL_NAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	Xmd.h-patch Xos.h-patch Xpoll.h-patch X.h-patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/xproto-$(VERSION)

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Command line options to GNU autoconf configure script
XPROTO_CFG=--prefix=$(X11_DIR)

build_gen: $(SOURCE_DIR)/Makefile

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x configure ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LD="$(CC) $(LIB_CFLAGS) $(LDFLAGS)" LDFLAGS="$(LIB_LDFLAGS)" ./configure $(XPROTO_CFG) )

# Install - first is for packages, second is for building the old monolith
install_gen: $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)
	@if [ -d $(PROTODIR)/../xc/include ] ; then \
		(set -x ; cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) xprotodir=/../xc/include install) ;\
	fi


