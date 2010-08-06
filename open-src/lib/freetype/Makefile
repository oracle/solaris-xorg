###############################################################################
#
# Freetype 2.x Makefile
#
# Copyright (c) 2002, 2010, Oracle and/or its affiliates. All rights reserved.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#

PWD:sh=pwd
TOP=$(PWD)/../../..

# Package name used in tarballs
MODULE_NAME=freetype

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=system/library/freetype-2
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=System/Libraries
MODULE_PKGCLASS_SET=yes

# ARC cases that covered this module
# LSARC/2002/291 FreeType 2.1.0
MODULE_ARC_CASES = LSARC/2002/291
# LSARC/2003/400 FreeType 2.1.4
MODULE_ARC_CASES += LSARC/2003/400
# LSARC/2004/568 FreeType 2.1.9
MODULE_ARC_CASES += LSARC/2004/568
# LSARC/2005/397 FreeType 2.1.10
MODULE_ARC_CASES += LSARC/2005/397
# LSARC/2007/662 FreeType move from /usr/sfw to /usr
MODULE_ARC_CASES += LSARC/2007/662

# Package metadata
UPSTREAM		= The FreeType Project
UPSTREAM_URL		= http://freetype.org/
UPSTREAM_SET		= yes

MODULE_LICENSE		= The FreeType Project License
MODULE_LICENSE_SET	= yes

# Library name used in man pages & spec files
LIBNAME=freetype

# Version number (used in path names)
MODULE_VERSION=2.4.1

# Checksums for upstream tarball
TARBALL_MD5  = 567a27e5189ed581396f69fb51faf2d8
TARBALL_SHA1 = 1870cc142be52a721031fcf16c4f8110dfb84fdb

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-CVE-2010-1797.patch,-p1 \
	freetype-config.patch,-p1 \
	freetype-GS-conflict.patch,-p1 \
	unix-cc.in.9.patch,-p1

# Where to install freetype files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= \
	CONFIG_SHELL="/bin/bash" \
	--enable-biarch-config \
	--libdir='$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)' \
	--with-zlib \
	--with-pic

# pkg-config files that need to have -R added
FIX_PC_FILES=builds/unix/freetype2.in

# License file
LICENSE_FILE=docs/FTL.TXT
LICENSE_FILE_SET=yes

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Don't run normal configure/install rules - freetype is different
CONFIGURE_TARGETS=$(SOURCE_DIR)/config.mk
CONFIGURE_TARGETS_SET=yes
INSTALL_TARGETS=install_freetype install_freetype_docs
INSTALL_TARGETS_SET=yes

# Environment variables to pass to configure & build steps
MODULE_ENV = \
    SHELL="/bin/bash" \
    MAKESHELL="/bin/bash" \
    CONFIG_SHELL="/bin/bash" \
    CPP="$(CC) -E $(LIB_CFLAGS)" \
    CXXCPP="$(CXX) -E $(CXXFLAGS)" \
    MAKEFLAGS= \
    MAKE=$(GNUMAKE) GNUMAKE=$(GNUMAKE) CFG="$(CONFIG_OPTS)"

# Don't use -I paths to proto area, since freetype doesn't depend on
# anything there, but can break if it finds old freetype headers during
# an incremental rebuild.
USE_DEFAULT_CONFIG_CPPFLAGS=no

MODULE_CONFIG_ENV = $(MODULE_ENV)
MODULE_BUILD_ENV = $(CONFIG_ENV)

MODULE_INSTALL_MAKEFLAGS=bindir=$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

# Run configure script (Freetype helpfully does this the first time you call
# gmake - this isn't normal for autoconf programs)
$(SOURCE_DIR)/config.mk: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	chmod a+x builds/unix/configure builds/unix/mkinstalldirs builds/unix/install-sh ; \
	$(CONFIG_ENV) $(GNUMAKE) )

FT_bindir=$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)
FT_libdir=$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)
FT_docdir=$(MODULE_PREFIX)/share/doc/$(MODULE_NAME)

# Have to override default_install since the -e causes our BUILD_DIR to
# clash with the one in freetype's Makefiles
install_freetype: $(BUILD_TARGETS)
	mkdir -p $(PROTODIR)$(FT_bindir)
	( cd $(SOURCE_DIR) ; \
	  $(GNUMAKE) DESTDIR=$(PROTODIR) prefix=$(MODULE_PREFIX) libdir=$(FT_libdir) bindir=$(FT_bindir) install )

install_freetype_docs: $(PROTODIR)$(FT_docdir)/index.html

$(PROTODIR)$(FT_docdir)/index.html: $(BUILD_TARGETS)
	-rm -rf $(PROTODIR)$(FT_docdir)
	mkdir -p $(PROTODIR)$(FT_docdir)
	( cd $(SOURCE_DIR) ; \
	    cp -fp docs/reference/*.html $(PROTODIR)$(FT_docdir)/ )
	cp -fp $(SOURCE_DIR)/docs/formats.txt $(PROTODIR)$(FT_docdir)/
	cp -fp $(SOURCE_DIR)/docs/VERSION.DLL $(PROTODIR)$(FT_docdir)/
	ln -sf ft2-toc.html $(PROTODIR)$(FT_docdir)/index.html

