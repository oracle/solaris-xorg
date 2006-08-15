###############################################################################
#
# Xdmcp 1.x Makefile
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
# @(#)Makefile	1.1	06/08/15
#

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32 64

# Version number (used in path names)
XDMCP_VERS=1.0.2

# Source tarball
SOURCE_TARBALL_NAME=libXdmcp-$(XDMCP_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(XORG_RELEASES_URL)/lib/$(SOURCE_TARBALL_NAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/libXdmcp-$(XDMCP_VERS)

# Library name (used for specfiles/mapfiles)
LIBNAME=Xdmcp

# libXdmcp has no man pages to apply Sun footer to
#SUNTOUCHED_MANPAGES=Xdmcp.man
#SUNTOUCH_MAN_FLAGS= -l Xdmcp \
# -a '{Availability, SUNWxwplt} {Interface Stability, Committed} {MT-Level, Unsafe}'

include $(TOP)/common/Makefile.inc

# Where to install files
XDMCP_prefix=$(X11_DIR)

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --enable-shared=yes --enable-static=no \
	--prefix=$(XDMCP_prefix) --libdir='$${exec_prefix}/lib$(LIBSUBDIR)' \
	--with-lint --enable-lint-library

# Environment variable options to GNU autoconf configure script
CONFIG_ENV = CC=$(CC) CFLAGS="$(LIB_CFLAGS)" \
	LD="$(LD) $(LDFLAGS)"  LDFLAGS="$(LIB_LDFLAGS)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig \
	LIB_MAN_SUFFIX=3Xdmcp

# Remove encryption source before building
source_gen::
	-rm $(SOURCE_DIR)/Wraphelp.*

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET) $(XPROTO_DEP)
	(cd $(SOURCE_DIR) ; chmod a+x configure ; \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )

# Library built in tree
XDMCP_LIB=$(BUILD_DIR)/.libs/libXdmcp.so.6.0.0

build_gen: $(XDMCP_LIB)

$(XDMCP_LIB): $(SOURCE_DIR)/Makefile $(BUILD_DIR)/$(SPECMAP)
	(cd $(SOURCE_DIR) ; LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS) -M $(PWD)/$(BUILD_DIR)/$(SPECMAP)" $(MAKE) $(MFLAGS) )


install_gen: $(XDMCP_LIB)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS)" $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) libdir=$(XDMCP_prefix)/lib$(ARCHLIBSUBDIR) install pkgconfigdir=/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig)
