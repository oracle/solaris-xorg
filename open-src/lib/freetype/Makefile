###############################################################################
#
# Freetype 2.x font rendering library
#
# Copyright (c) 2002, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_ARC_CASES += LSARC/2002/291	# FreeType 2.1.0
MODULE_ARC_CASES += LSARC/2003/400	# FreeType 2.1.4
MODULE_ARC_CASES += LSARC/2004/568	# FreeType 2.1.9
MODULE_ARC_CASES += LSARC/2005/397	# FreeType 2.1.10
MODULE_ARC_CASES += LSARC/2007/662	# FreeType move from /usr/sfw to /usr
MODULE_ARC_CASES += PSARC/2010/342	# FreeType 2.4.2
MODULE_ARC_CASES += PSARC/2015/148	# FreeType 2.5.5
MODULE_ARC_CASES += PSARC/2016/127	# FreeType 2.6.3
MODULE_ARC_CASES += PSARC/2017/130	# FreeType 2.8

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES 	= docs/freetype-config.1 docs/libfreetype.3lib
MODULE_STABILITY	= Pass-through Volatile
MODULE_MTLEVEL		= MT-Safe
MODULE_MTLEVEL_SET	= yes
MODULE_PKGCONFIG_MAN	= freetype2
MODULE_PKGCONFIG_MAN_SET= yes
MODULE_SUNTOUCH_MAN_FLAGS = -r '{__version__, FreeType $(MODULE_VERSION)}'
# Don't want to add MT-Level & -l flags to command man pages
OVERRIDE_LIB_SUNTOUCH_DEFAULT = yes
# But do want to add them to the library man page
MODULE_SUNTOUCH_MAN_FLAGS += $(SUNTOUCH_MAN_FLAGS_$(@F:%.$(FIXED_MAN_SUFFIX)=%))
SUNTOUCH_MAN_FLAGS_libfreetype.3lib = $(LIB_SUNTOUCH_DEFAULTS)

# Package metadata
UPSTREAM		= The FreeType Project
UPSTREAM_URL		= http://freetype.org/
UPSTREAM_SET		= yes
GIT_URL			= http://git.sv.nongnu.org/r/freetype/freetype2.git
GIT_URL_SET		= yes

MODULE_LICENSE		= The FreeType Project License
MODULE_LICENSE_SET	= yes
ORACLE_TPNO		= 35528
MODULE_DESC		= the FreeType 2 font engine

# Library name used in man pages & spec files
LIBNAME=freetype

# Version number (used in path names)
MODULE_VERSION=2.8

# Checksums for upstream tarball
TARBALL_MD5   = 2413ac3eaf508ada019c63959ea81a92
TARBALL_SHA1  = 42c6b1f733fe13a3eba135f5025b22cb68450f91
TARBALL_SHA256= a3c603ed84c3c2495f9c9331fe6bba3bb0ee65e06ec331e0a0fb52158291b40b

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-GS-conflict.patch,-p1 \
	unix-cc.in.9.patch,-p1

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS  = --enable-biarch-config
MODULE_CONFIG_OPTS += --with-zlib --with-bzip2 --with-png
MODULE_CONFIG_OPTS += --with-pic

# Disable harfbuzz temporarily until package is on all the build machines
MODULE_CONFIG_OPTS += --without-harfbuzz

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
		$(SOURCE_DIR)/docs/VERSIONS.TXT \
		$(PROTODIR)$(MODULE_DOC_DIR)/
	ln -sf ft2-toc.html $(PROTODIR)$(MODULE_DOC_DIR)/index.html
	mkdir -p $(PROTODIR)$(MODULE_MAN_DIR)/man3lib
	$(INSTALL_SCRIPT) -m 0444 \
		$(SOURCE_DIR)/docs/libfreetype.3lib \
		$(PROTODIR)$(MODULE_MAN_DIR)/man3lib/
