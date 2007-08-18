###############################################################################
#
# Freetype 2.x Makefile
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
# @(#)Makefile	1.68	07/08/17
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Build 32-bit, 64-bit, or both?
BUILD_TYPES=32 64
BUILD_TYPES_SET=yes

# Version number (used in path names)
FT_VERS=2.1.10

# Source tarball
SOURCE_TARBALL_NAME=freetype-$(FT_VERS).tar.bz2

# Download site for source
SOURCE_URL=http://savannah.nongnu.org/download/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= freetype-$(FT_VERS).patch 6425531.patch 6542352.patch \
	freetype-$(FT_VERS).hint.patch 6539893.patch,-p1 CVE-2007-2754.patch \
	freetype-config.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/freetype-$(FT_VERS)

# Command line options to GNU autoconf configure script
FT_CFG=--enable-shared=yes --enable-static=no --prefix=/usr/sfw \
	--libdir=/usr/sfw/lib$(LIBSUBDIR) \
	--bindir=/usr/sfw/bin$(LIBSUBDIR)

# Library built in tree
FT_LIB=$(FT_DIR)/objs/.libs/libfreetype.so.6.3.8

# Where to install most files
FT_prefix=$(PROTODIR)/usr/sfw

# Where to install libraries
FT_libdir=$(FT_prefix)/lib

# What to build
BUILD_TARGETS=$(GNUMAKE) $(FT_LIB)

# pkg-config files that need to have -R added
FIX_PC_FILES=builds/unix/freetype2.in

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Run configure script (Freetype helpfully does this the first time you call
# gmake - this isn't normal for autoconf programs)
$(SOURCE_DIR)/config.mk: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x builds/unix/configure builds/unix/mkinstalldirs builds/unix/install-sh ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LD="$(CC) $(LIB_CFLAGS) $(LDFLAGS)" \
	 LDFLAGS="$(LIB_LDFLAGS)" MAKE=$(GNUMAKE) CFG="$(FT_CFG)" \
	 INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	 $(GNUMAKE) $(MFLAGS) )

# Build Freetype
$(FT_LIB): $(SOURCE_DIR)/config.mk
	(cd $(SOURCE_DIR) ; LD_OPTIONS="$(LIB_LDFLAGS)" $(GNUMAKE) )

install_gen: $(FT_LIB)
	(cd $(SOURCE_DIR) ; $(GNUMAKE) prefix=$(FT_prefix) libdir=$(FT_libdir)$(ARCHLIBSUBDIR) bindir=$(FT_prefix)/bin$(ARCHLIBSUBDIR) install )
	mkdir -p $(PROTODIR)/usr/sfw/bin$(ARCHLIBSUBDIR)

install_64::
	-/bin/rm -f $(FT_libdir)/64 $(PROTODIR)/usr/sfw/bin/64
	ln -s $(FT_libdir)/$(SUBDIR64) $(FT_libdir)/64
	ln -s $(PROTODIR)/usr/sfw/bin/$(SUBDIR64) $(PROTODIR)/usr/sfw/bin/64

