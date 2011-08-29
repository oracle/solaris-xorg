###############################################################################
#
# Fontconfig 2.x Makefile
#
# Copyright (c) 2002, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Package metadata
UPSTREAM	= fontconfig
UPSTREAM_URL	= http://fontconfig.org/
UPSTREAM_SET	= yes

# Version number (used in path names)
MODULE_VERSION=2.8.0

# Checksums for upstream tarball
TARBALL_MD5  = 77e15a92006ddc2adbb06f840d591c0e
TARBALL_SHA1 = 570fb55eb14f2c92a7b470b941e9d35dbfafa716

# Source tarball
SOURCE_TARBALL_NAME=fontconfig-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Download site for source
SOURCE_URL=http://fontconfig.org/release/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
		arial-narrow.conf.patch \
		cjk-fonts.conf.patch \
		Makefile.am.0.patch \
		bitmap-hacks.patch \
		6447517.patch \
		6770058.patch \
		7026635.patch \
		7058808.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=fontconfig

# Man pages to apply Sun footer to & attributes to list
FONTCONFIG_CMDS = fc-cache fc-list fc-query fc-cat fc-match fc-scan
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

# Install to /usr, not /usr/X11
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = \
    --with-confdir=/etc/fonts \
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

# Include common rulesets
include ../Makefile.inc

# Path to install fc-cache SMF files and the RBAC files they use
FONTCONFIG_SMF_MANIFEST_dir=$(PROTODIR)$(SMF_MANIFEST_DIR)/application/font
FONTCONFIG_SMF_MANIFEST=$(FONTCONFIG_SMF_MANIFEST_dir)/fc-cache.xml
FONTCONFIG_SMF_METHOD_dir=$(PROTODIR)$(SMF_METHOD_DIR)
FONTCONFIG_SMF_METHOD=$(FONTCONFIG_SMF_METHOD_dir)/fc-cache
FONTCONFIG_RBAC_AUTH_dir=$(PROTODIR)/etc/security/auth_attr.d
FONTCONFIG_RBAC_AUTH=$(FONTCONFIG_RBAC_AUTH_dir)/fontconfig
FONTCONFIG_RBAC_HELP_dir=$(PROTODIR)/usr/lib/help/auths/locale/C
FONTCONFIG_RBAC_HELP=$(FONTCONFIG_RBAC_HELP_dir)/FontStates.html

# Sun specification configuration files to be installed in /etc/fonts/conf.d
FONTCONFIG_CONFDIR = $(PROTODIR)/etc/fonts/conf.avail
SUN_CONFFILES = 20-indic.conf 49-sun-preuser.conf 90-sun-prefer-bitmap.conf
FONTCONFIG_SUN_CONFFILES = $(SUN_CONFFILES:%=$(FONTCONFIG_CONFDIR)/%)

# Additional targets to install beyond the default_install,
# run just once, not per bitwidth
FONTCONFIG_INSTALL_RULES = fontconfig-sunman-install \
	$(FONTCONFIG_SMF_MANIFEST) $(FONTCONFIG_SMF_METHOD) \
	$(FONTCONFIG_RBAC_AUTH) $(FONTCONFIG_RBAC_HELP) \
	$(FONTCONFIG_SUN_CONFFILES)

install: $(FONTCONFIG_INSTALL_RULES)

$(FONTCONFIG_SMF_MANIFEST): fc-cache.xml
	/usr/sbin/svccfg validate fc-cache.xml
	mkdir -p $(FONTCONFIG_SMF_MANIFEST_dir)
	$(INSTALL_SCRIPT) -c -m 0444 fc-cache.xml $@

$(FONTCONFIG_SMF_METHOD): fc-cache.sh
	mkdir -p $(FONTCONFIG_SMF_METHOD_dir)
	$(INSTALL_SCRIPT) -c -m 0755 fc-cache.sh $@

$(FONTCONFIG_RBAC_AUTH): auth_attr
	mkdir -p $(FONTCONFIG_RBAC_AUTH_dir)
	$(INSTALL_SCRIPT) -c -m 0444 auth_attr $@

$(FONTCONFIG_RBAC_HELP): FontStates.html
	mkdir -p $(FONTCONFIG_RBAC_HELP_dir)
	$(INSTALL_SCRIPT) -c -m 0444 FontStates.html $@

$(FONTCONFIG_SUN_CONFFILES): $(SUN_CONFFILES)
	mkdir -p $(PROTODIR)/etc/fonts/conf.d $(PROTODIR)/etc/fonts/conf.avail
	cp -fp $(@F) $@
	rm -f $(PROTODIR)/etc/fonts/conf.d/$(@F)
	ln -sf ../conf.avail/$(@F) $(PROTODIR)/etc/fonts/conf.d/$(@F)

FC_MAN_DIR=$(PROTODIR)/usr/share/man

fontconfig-sunman-install: $(INSTALL_RULES)
	mkdir -p $(FC_MAN_DIR)/man4 $(FC_MAN_DIR)/man3lib
	-rm -f $(FC_MAN_DIR)/man4/fonts.conf.4
	sed -e 's/.TH "FONTS-CONF" "5"/.TH "fonts.conf" "4"/' 	\
	  $(FC_MAN_DIR)/man5/fonts-conf.5 > $(FC_MAN_DIR)/man4/fonts.conf.4
	-rm -f $(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	$(PERL) -p -e 's/^$$/.LP/;' -e 's/.?\251/\\(co/;' 	\
	    libfontconfig.3lib > $(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	cat sunman-notes-fc-cache >> $(FC_MAN_DIR)/man1/fc-cache.1
	-rm -f $(PROTODIR)/etc/fonts/fonts.conf.bak

