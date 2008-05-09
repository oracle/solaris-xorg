###############################################################################
#
# Freetype 2.x Makefile
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.73	08/05/02
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Build 32-bit, 64-bit, or both?
BUILD_TYPES=32 64
BUILD_TYPES_SET=yes

# Version number (used in path names)
FT_VERS=2.3.5

# Source tarball
SOURCE_TARBALL_NAME=freetype-$(FT_VERS).tar.bz2

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-config.patch \
	CVE-2007-2754.patch \
	freetype-2.3.5.ftoption.h.patch \
	freetype-2.3.5.configure.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/freetype-$(FT_VERS)

# Where to install freetype files
FT_prefix=/usr
FT_bindir=$(FT_prefix)/bin$(ARCHLIBSUBDIR)
FT_libdir=$(FT_prefix)/lib$(ARCHLIBSUBDIR)

# Command line options to GNU autoconf configure script
FT_CFG= --prefix=$(FT_prefix) \
	--libdir=$(FT_libdir) \
	--bindir=$(FT_bindir) \
	--localstatedir=/var \
	--enable-shared \
	--disable-static \
	--disable-libtool-lock \
	--with-zlib \
	--with-pic

# Library built in tree
FT_LIB=$(FT_DIR)/objs/.libs/libfreetype.so.6.3.16

# What to build
BUILD_TARGETS=$(GNUMAKE) $(FT_LIB)

# pkg-config files that need to have -R added
FIX_PC_FILES=builds/unix/freetype2.in

### Include common rulesets
include $(TOP)/common/Makefile.inc

# GNU make wants bash
SHELL = /bin/bash

# License file
LICENSE_FILE=docs/FTL.TXT
LICENSE_FILE_SET=yes

### Rules specific to this directory:

# Run configure script (Freetype helpfully does this the first time you call
# gmake - this isn't normal for autoconf programs)
$(SOURCE_DIR)/config.mk: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	chmod a+x builds/unix/configure builds/unix/mkinstalldirs builds/unix/install-sh ; \
	    SHELL="/bin/bash" \
	    MAKESHELL="/bin/bash" \
	    CONFIG_SHELL="/bin/bash" \
	    CC="$(CC)" \
	    CXX="$(CXX)" \
	    CPP="$(CC) -E $(LIB_CFLAGS)" \
	    CXXCPP="$(CXX) -E $(CXXFLAGS)" \
	    CFLAGS="$(LIB_CFLAGS)" \
	    CXXFLAGS="$(LIB_CXXFLAGS)" \
	    LDFLAGS="$(LIB_LDFLAGS)" \
	    CFG="$(FT_CFG)" \
	    LD="$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS)" \
	    MAKE=$(GNUMAKE) GNUMAKE=$(GNUMAKE) CFG="$(FT_CFG)" \
	    INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	    $(GNUMAKE) )

# Build Freetype
$(FT_LIB): $(SOURCE_DIR)/config.mk
	(cd $(SOURCE_DIR) ; \
	    SHELL="/bin/bash" \
	    MAKESHELL="/bin/bash" \
	    CONFIG_SHELL="/bin/bash" \
	    CC="$(CC)" \
	    CXX="$(CXX)" \
	    CPP="$(CC) -E $(LIB_CFLAGS)" \
	    CXXCPP="$(CXX) -E $(CXXFLAGS)" \
	    CFLAGS="$(LIB_CFLAGS)" \
	    CXXFLAGS="$(LIB_CXXFLAGS)" \
	    LDFLAGS="$(LIB_LDFLAGS)" \
	    MAKE=$(GNUMAKE) GNUMAKE=$(GNUMAKE) \
	    LDFLAGS="$(LIB_LDFLAGS)" \
	    CFG="$(FT_CFG)" \
	    LD="$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS)" \
	    INSTALL="$(TOP)/common/install-sh -c" \
	    $(GNUMAKE) )

install_gen:: $(FT_LIB)
	mkdir -p $(PROTODIR)/usr/bin$(ARCHLIBSUBDIR)
	(cd $(SOURCE_DIR) ; $(GNUMAKE) prefix=$(PROTODIR)$(FT_prefix) libdir=$(PROTODIR)$(FT_libdir) bindir=$(PROTODIR)$(FT_bindir) install )

install_64::
	-/bin/rm -f $(PROTODIR)$(FT_prefix)/lib/64 $(PROTODIR)$(FT_prefix)/bin/64
	ln -s $(SUBDIR64) $(PROTODIR)$(FT_prefix)/lib/64
	ln -s $(SUBDIR64) $(PROTODIR)$(FT_prefix)/bin/64
