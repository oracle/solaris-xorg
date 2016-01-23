###############################################################################
#
# Fontconfig - Font configuration and customization library
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

# Package name used in tarballs
MODULE_NAME=fontconfig

# Package metadata
MODULE_DESC = the FontConfig font configuration and customization library

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=system/library/fontconfig
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=System/Libraries
MODULE_PKGCLASS_SET=yes

# ARC cases that covered this module
# LSARC/2003/273 fontconfig library
MODULE_ARC_CASES = LSARC/2003/273
# PSARC/2004/752 X Consolidation conversion to SMF
MODULE_ARC_CASES += PSARC/2004/752
# LSARC/2006/485 fontconfig Hide/Only/Poor (HOP) language matching
MODULE_ARC_CASES += LSARC/2006/485
# LSARC/2007/532 fc-match
MODULE_ARC_CASES += LSARC/2007/532
# LSARC/2008/140 fontconfig 2.5.0
MODULE_ARC_CASES += LSARC/2008/140
# PSARC/2010/162 fontconfig 2.8.0
MODULE_ARC_CASES += PSARC/2010/162
# PSARC/2015/035 Moving fontconfig utilities to LP64 by default
MODULE_ARC_CASES += PSARC/2015/035
# PSARC/2015/527 Fontconfig 2.11.94  
MODULE_ARC_CASES += PSARC/2015/527

# Package metadata
UPSTREAM	= fontconfig
UPSTREAM_URL	= http://fontconfig.org/
UPSTREAM_SET	= yes
GIT_URL		= $(FDO_GIT_URL)/fontconfig
GIT_URL_SET	= yes
ORACLE_TPNO	= 23955

# Version number (used in path names)
MODULE_VERSION=2.11.94

# Checksums for upstream tarball
TARBALL_MD5  = 479be870c7f83f15f87bac085b61d641
TARBALL_SHA1 = 3748d8a2b9cf8052dbd003f524d829157f1ead83
TARBALL_SHA256 = 73f6d323c7bcfbde25d78397675191d55b8f4139132c6a9444410f3a2d8a9a95

# Source tarball
SOURCE_TARBALL_NAME=fontconfig-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=http://fontconfig.org/release/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
		arial-narrow.conf.patch \
		cjk-fonts.conf.patch,-p1 \
		Makefile.am.0.patch \
		bitmap-hacks.patch,-p1 \
		6447517.patch,-p1 \
		6770058.patch \
		7026635.patch,-p1 \
		7058808.patch \
		font-links.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=fontconfig

# Man pages to apply Sun footer to & attributes to list
FONTCONFIG_CMDS = fc-cache fc-list fc-query fc-cat fc-match fc-scan fc-pattern fc-validate
SUNTOUCHED_MANPAGES = $(FONTCONFIG_CMDS:%=%/%.1) doc/*.3 doc/*.5
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3fontconfig
MODULE_MTLEVEL=Unknown
MODULE_MTLEVEL_SET=yes

# Override specific attributes for specific man pages
SUNTOUCH_MAN_FLAGS = $(COMMON_SUNTOUCH_MAN_FLAGS) \
	$(SUNTOUCH_MAN_FLAGS_$(@F:%.$(FIXED_MAN_SUFFIX)=%))
SUNTOUCH_MAN_FLAGS_SET = yes
# API docs are in doc/*.3
SUNTOUCH_MAN_FLAGS_*.3 = $(MODTYPE_SUNTOUCH_MAN_FLAGS)
# Command docs are in per-command dirs
SUNTOUCH_MAN_FLAGS_CMDS = -p $(MODULE_PREFIX)/bin/
SUNTOUCH_MAN_FLAGS_fc-cache.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-list.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-query.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-cat.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-match.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-scan.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-pattern.1	= $(SUNTOUCH_MAN_FLAGS_CMDS)
SUNTOUCH_MAN_FLAGS_fc-validate.1= $(SUNTOUCH_MAN_FLAGS_CMDS)

# pkg-config files to update
FIX_PC_FILES=fontconfig.pc.in
# Don't include path to freetype headers in our proto area in the *.pc files.
# The 'Requires: freetype' will handle the right -I flags for us.
MODULE_FIX_PC_FLAGS += -e 's| \@FREETYPE_CFLAGS\@||g;'

BINSUBDIR       = $(BINSUBDIR_$(BUILD_TYPE))
BINSUBDIR_32    = $(SUBDIR32)
BINSUBDIR_64    = $(SUBDIR64)

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = \
    --bindir='$${exec_prefix}/bin/$(BINSUBDIR)' \
    --with-baseconfigdir='/etc/fonts' \
    --with-templatedir='/etc/fonts/conf.avail' \
    --with-xmldir='/etc/fonts' \
    --with-default-fonts='/usr/share/fonts' \
    --with-add-fonts='/etc/X11/fontpath.d,/usr/share/ghostscript/fonts,/usr/X11/lib/X11/fonts' \
    --with-cache-dir=/var/cache/fontconfig

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Paths to find libraries
MODULE_BUILD_ENV = LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(LIBSUBDIR) \
	LD_RUN_PATH=/usr/lib$(LIBSUBDIR) $(DOC_PATH)

# Need to rebuild autoconf/automake files
AUTORECONF=yes

# Override linker flags for programs
FONTCONFIG_PROG_LDFLAGS=$(MAPFILES_FOR_PROGS) ../src/libfontconfig.so.1
PROGRAM_VARS = fc_cache fc_cat fc_list fc_match fc_query fc_scan
MODULE_BUILD_MAKEFLAGS = $(PROGRAM_VARS:%=%_LDADD="$(FONTCONFIG_PROG_LDFLAGS)")

# 22507047 libfontconfig should not need a libpthread dependency
# Upstream uses ac_pthread.m4 which insists on using -mt & -lpthread on Solaris
# despite them not being necessary any more
MODULE_LD_OPTIONS += $(ZDISCARD_UNUSED_DEP)

# Include common rulesets
include ../Makefile.inc

# Path to install fc-cache SMF files and the RBAC files they use
FONTCONFIG_SMF_MANIFEST_dir=$(PROTODIR)$(SMF_MANIFEST_DIR)/application/font
FONTCONFIG_SMF_MANIFEST=$(FONTCONFIG_SMF_MANIFEST_dir)/fc-cache.xml
FONTCONFIG_SMF_METHOD_dir=$(PROTODIR)$(SMF_METHOD_DIR)
FONTCONFIG_SMF_METHOD=$(FONTCONFIG_SMF_METHOD_dir)/fc-cache
FONTCONFIG_RBAC_AUTH_dir=$(PROTODIR)/etc/security/auth_attr.d
FONTCONFIG_RBAC_AUTH=$(FONTCONFIG_RBAC_AUTH_dir)/fontconfig

# Sun specification configuration files to be installed in /etc/fonts/conf.d
FONTCONFIG_CONFDIR = $(PROTODIR)/etc/fonts/conf.avail
SUN_CONFFILES = 20-indic.conf 49-sun-preuser.conf 90-sun-prefer-bitmap.conf
FONTCONFIG_SUN_CONFFILES = $(SUN_CONFFILES:%=$(FONTCONFIG_CONFDIR)/%)

# Link /usr/bin/$cmdname to the 64-bit version
FONTCONFIG_CMD_LINKS = $(FONTCONFIG_CMDS:%=$(PROTODIR)/usr/bin/%)
$(FONTCONFIG_CMD_LINKS): $(PROTODIR)/usr/bin
	-rm -f $@
	ln -s $(BINSUBDIR_64)/${@F} $@

$(PROTODIR)/usr/bin:
	mkdir $@

# Additional targets to install beyond the default_install,
# run just once, not per bitwidth
FONTCONFIG_INSTALL_RULES = fontconfig-sunman-install \
	$(FONTCONFIG_SMF_MANIFEST) $(FONTCONFIG_SMF_METHOD) \
	$(FONTCONFIG_RBAC_AUTH) \
	$(FONTCONFIG_SUN_CONFFILES) $(FONTCONFIG_CMD_LINKS)

install: $(FONTCONFIG_INSTALL_RULES)

$(FONTCONFIG_SMF_MANIFEST): fc-cache.xml
	/usr/sbin/svccfg validate fc-cache.xml
	mkdir -p $(FONTCONFIG_SMF_MANIFEST_dir)
	$(INSTALL_SCRIPT) -c -m 0444 fc-cache.xml $@

$(FONTCONFIG_SMF_METHOD): fc-cache.sh
	mkdir -p $(FONTCONFIG_SMF_METHOD_dir)
	$(INSTALL_SCRIPT) -c -m 0555 fc-cache.sh $@

$(FONTCONFIG_RBAC_AUTH): auth_attr
	mkdir -p $(FONTCONFIG_RBAC_AUTH_dir)
	$(INSTALL_SCRIPT) -c -m 0444 auth_attr $@

$(FONTCONFIG_SUN_CONFFILES): $(SUN_CONFFILES)
	mkdir -p $(PROTODIR)/etc/fonts/conf.d $(PROTODIR)/etc/fonts/conf.avail
	cp -fp $(@F) $@
	rm -f $(PROTODIR)/etc/fonts/conf.d/$(@F)
	ln -sf ../conf.avail/$(@F) $(PROTODIR)/etc/fonts/conf.d/$(@F)

FC_MAN_DIR=$(PROTODIR)/usr/share/man

fontconfig-sunman-install: $(INSTALL_RULES)
	mkdir -p $(FC_MAN_DIR)/man5 $(FC_MAN_DIR)/man3lib
	-rm -f $(FC_MAN_DIR)/man5/fonts.conf.5
	ln -s fonts-conf.5 $(FC_MAN_DIR)/man5/fonts.conf.5
	-rm -f $(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	$(PERL) -p -e 's/^$$/.LP/;' -e 's/.?\251/\\(co/;' 	\
	    libfontconfig.3lib > $(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	cat sunman-notes-fc-cache >> $(FC_MAN_DIR)/man1/fc-cache.1
	-rm -f $(PROTODIR)/etc/fonts/fonts.conf.bak

