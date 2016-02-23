###############################################################################
#
# Freetype 2.x font rendering library
#
# Copyright (c) 2002, 2016, Oracle and/or its affiliates. All rights reserved.
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
# PSARC/2010/342 FreeType 2.4.2
MODULE_ARC_CASES += PSARC/2010/342
# PSARC/2015/148 FreeType 2.5.5
MODULE_ARC_CASES += PSARC/2015/148

# Package metadata
UPSTREAM		= The FreeType Project
UPSTREAM_URL		= http://freetype.org/
UPSTREAM_SET		= yes
GIT_URL			= http://git.sv.nongnu.org/r/freetype/freetype2.git
GIT_URL_SET		= yes

MODULE_LICENSE		= The FreeType Project License
MODULE_LICENSE_SET	= yes
ORACLE_TPNO		= 21936
MODULE_DESC		= the FreeType 2 font engine

# Library name used in man pages & spec files
LIBNAME=freetype

# Version number (used in path names)
MODULE_VERSION=2.5.5

# Checksums for upstream tarball
TARBALL_MD5   = 2a7a314927011d5030903179cf183be0
TARBALL_SHA1  = 7b7460ef51a8fdb17baae53c6658fc1ad000a1c2
TARBALL_SHA256= 387bcc4b780b12484aa2ec9f7db1a55d8286eb5639f45fbc0fbba7a4e5a1afb9

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-GS-conflict.patch,-p1 \
	unix-cc.in.9.patch,-p1

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

MODULE_INSTALL_MAKEFLAGS=bindir=$(MODULE_BIN_DIR)$(ARCHLIBSUBDIR)

# Don't touch apinames when running set-elf-comments or it will trigger
# a rebuild during "make install" that loses the comments.
MODULE_ELF_COMMENTS_FLAGS = -X apinames

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

install_freetype_links: $(PROTODIR)$(MODULE_FULLARCH_LIB_DIR)/libfreetype.so

$(PROTODIR)$(MODULE_FULLARCH_LIB_DIR)/libfreetype.so: default_install
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
