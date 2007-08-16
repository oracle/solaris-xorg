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
# @(#)Makefile	1.67	07/08/10
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
SOURCE_PATCHES=freetype-$(FT_VERS).patch 6425531.patch 6542352.patch freetype-$(FT_VERS).hint.patch 6539893.patch,-p1 CVE-2007-2754.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/freetype-$(FT_VERS)

# Command line options to GNU autoconf configure script
FT_CFG="--enable-shared=yes --enable-static=no --prefix=/usr/sfw"

# Library built in tree
FT_LIB=$(FT_DIR)/objs/.libs/libfreetype.so.6.3.8

# Where to install most files
FT_prefix=$(PROTODIR)/usr/sfw

# Where to install libraries
FT_libdir=$(FT_prefix)/lib

# What to build
BUILD_TARGETS=$(GNUMAKE) $(FT_LIB)

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Run configure script (Freetype helpfully does this the first time you call
# gmake - this isn't normal for autoconf programs)
$(SOURCE_DIR)/config.mk: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x builds/unix/configure builds/unix/mkinstalldirs builds/unix/install-sh ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LD="$(CC) $(LIB_CFLAGS) $(LDFLAGS)" LDFLAGS="$(LIB_LDFLAGS)" MAKE=$(GNUMAKE) CFG=$(FT_CFG) \
	 $(GNUMAKE) $(MFLAGS) )

# Build Freetype
$(FT_LIB): $(SOURCE_DIR)/config.mk
	(cd $(SOURCE_DIR) ; LD_OPTIONS="$(LIB_LDFLAGS)" $(GNUMAKE) )

install_gen: $(FT_LIB) $(SOURCE_DIR)/builds/unix/SUNW-freetype-config
	(cd $(SOURCE_DIR) ; $(GNUMAKE) prefix=$(FT_prefix) libdir=$(FT_libdir)/$(ARCHLIBSUBDIR) install )
	mkdir -p $(PROTODIR)/usr/sfw/bin$(ARCHLIBSUBDIR)
	-rm -f $(PROTODIR)/usr/sfw/bin$(ARCHLIBSUBDIR)/freetype-config 
	rm $(PROTODIR)/usr/sfw/lib$(ARCHLIBSUBDIR)/pkgconfig/freetype2.pc $(PROTODIR)/usr/sfw/lib$(ARCHLIBSUBDIR)/libfreetype.la
	$(SOURCE_DIR)/builds/unix/install-sh -m 755 $(SOURCE_DIR)/builds/unix/SUNW-freetype-config $(PROTODIR)/usr/sfw/bin$(ARCHLIBSUBDIR)/freetype-config
	$(SOURCE_DIR)/builds/unix/install-sh -m 755 $(SOURCE_DIR)/builds/unix/SUNW-freetype2.pc $(PROTODIR)/usr/sfw/lib$(ARCHLIBSUBDIR)/pkgconfig/freetype2.pc

install_64::
	-/bin/rm -f $(FT_libdir)/64 $(PROTODIR)/usr/sfw/bin/64
	ln -s $(FT_libdir)/$(SUBDIR64) $(FT_libdir)/64
	ln -s $(PROTODIR)/usr/sfw/bin/$(SUBDIR64) $(PROTODIR)/usr/sfw/bin/64

# Hack to add -R option required for linking on Solaris and 64-bit library path
# and remove unnecessary -lz
$(SOURCE_DIR)/builds/unix/freetype-config: $(SOURCE_DIR)/config.mk

$(SOURCE_DIR)/builds/unix/SUNW-freetype-config: $(SOURCE_DIR)/builds/unix/freetype-config
	sed -e 's|libdir=$${exec_prefix}/lib$$|libdir=$${exec_prefix}/lib${LIBSUBDIR}|' -e 's|-lz||' $(SOURCE_DIR)/builds/unix/freetype-config > $(SOURCE_DIR)/builds/unix/SUNW-freetype-config
	sed -e 's|-L$${exec_prefix}/lib|-L$${exec_prefix}/lib -R$${exec_prefix}/lib|' -e 's|-L$${libdir}|-L$${libdir} -R$${libdir}|' -e 's|$${exec_prefix}/lib|$${exec_prefix}/lib${LIBSUBDIR}|' -e 's|-lz||' -e 's|-I$${includedir}/freetype2|-I$${includedir} -I$${includedir}/freetype2|' $(SOURCE_DIR)/builds/unix/freetype2.pc > $(SOURCE_DIR)/builds/unix/SUNW-freetype2.pc
