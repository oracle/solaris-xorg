###############################################################################
#
# Freetype 2.x font rendering library
#
# Copyright (c) 2002, 2014, Oracle and/or its affiliates. All rights reserved.
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
ORACLE_TPNO		= 18003
MODULE_DESC		= the FreeType 2 font engine

# Library name used in man pages & spec files
LIBNAME=freetype

# Version number (used in path names)
MODULE_VERSION=2.4.11

# Checksums for upstream tarball
TARBALL_MD5   = b93435488942486c8d0ca22e8f768034
TARBALL_SHA1  = b15e5a0b30c1e854484b410340f6429ad1253c74
TARBALL_SHA256= ef9d0bcb64647d9e5125dc7534d7ca371c98310fec87677c410f397f71ffbe3f

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-config.patch,-p1 \
	freetype-GS-conflict.patch,-p1 \
	unix-cc.in.9.patch,-p1

# Where to install freetype files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= \
	--enable-biarch-config \
	--with-zlib \
	--with-pic

# pkg-config files that need to have -R added
FIX_PC_FILES=builds/unix/freetype2.in
install_pkgconfig_uninstalled:= FIX_PC_FILES=freetype2.in

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Environment variables to pass to configure & build steps
MODULE_CONFIG_ENV = GNUMAKE=$(GNUMAKE)
MODULE_BUILD_ENV = $(MODULE_CONFIG_ENV)

MODULE_ADD_INSTALL_TARGETS = install_freetype_docs install_freetype_links

# Don't use -I paths to proto area, since freetype doesn't depend on
# anything there, but can break if it finds old freetype headers during
# an incremental rebuild.
USE_DEFAULT_CONFIG_CPPFLAGS=no

MODULE_INSTALL_MAKEFLAGS=bindir=$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)

# Don't touch apinames when running set-elf-comments or it will trigger
# a rebuild during "make install" that loses the comments.
MODULE_ELF_COMMENTS_FLAGS = -X apinames

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

FT_libdir=$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)

install_freetype_links: $(PROTODIR)$(FT_libdir)/libfreetype.so

$(PROTODIR)$(FT_libdir)/libfreetype.so: default_install
	mkdir -p $(@D)
	-rm -f $@
	ln -s libfreetype.so.6 $@

install_freetype_docs: $(PROTODIR)$(MODULE_DOC_DIR)/index.html

$(PROTODIR)$(MODULE_DOC_DIR)/index.html: $(BUILD_TARGETS)
	-rm -rf $(PROTODIR)$(MODULE_DOC_DIR)
	mkdir -p $(PROTODIR)$(MODULE_DOC_DIR)
	$(INSTALL_SCRIPT) -m 0444 \
		$(SOURCE_DIR)/docs/reference/*.html \
		$(SOURCE_DIR)/docs/formats.txt \
		$(SOURCE_DIR)/docs/VERSION.DLL \
		$(PROTODIR)$(MODULE_DOC_DIR)/
	ln -sf ft2-toc.html $(PROTODIR)$(MODULE_DOC_DIR)/index.html
