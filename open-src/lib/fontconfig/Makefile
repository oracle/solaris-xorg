###############################################################################
#
# Fontconfig 2.x Makefile
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
# ident	"@(#)Makefile	1.88	09/12/01 SMI"
#

# Package name used in tarballs
MODULE_NAME=fontconfig

# Version number (used in path names)
MODULE_VERSION=2.7.3

# Source tarball
SOURCE_TARBALL_NAME=fontconfig-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Download site for source
SOURCE_URL=http://fontconfig.org/release/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=fontconfig-$(MODULE_VERSION)-fonts.conf.patch,-p1 \
	       Makefile.am.0.patch,-p1 \
	       6770058.patch,-p1

# Library name (used for specfiles/mapfiles)
LIBNAME=fontconfig

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*/*.1 doc/*.3 doc/*.5
SUN_PACKAGE=SUNWfontconfig
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3fontconfig
MODULE_MTLEVEL=Unknown
MODULE_MTLEVEL_set=yes

# Install to /usr, not /usr/X11
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = \
    --with-confdir=/etc/fonts \
    --with-default-fonts='/usr/share/fonts' \
    --with-add-fonts='/etc/X11/fontpath.d,/usr/share/ghostscript/fonts,/usr/X11/lib/X11/fonts' \
    --with-freetype-config=$(PROTODIR)/usr/bin$(LIBSUBDIR)/freetype-config \
    --with-cache-dir=/var/cache/fontconfig

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Paths to find libraries
MODULE_BUILD_ENV = LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(LIBSUBDIR) \
	LD_RUN_PATH=/usr/lib$(LIBSUBDIR) $(DOC_PATH)

# Need to rebuild autoconf/automake files
AUTORECONF=yes

# Path to install fc-cache manifest & method script
FONTCONFIG_SMF_MANIFEST_dir=$(PROTODIR)/var/svc/manifest/application/font
FONTCONFIG_SMF_MANIFEST=$(FONTCONFIG_SMF_MANIFEST_dir)/fc-cache.xml
FONTCONFIG_SMF_METHOD_dir=$(PROTODIR)/lib/svc/method
FONTCONFIG_SMF_METHOD=$(FONTCONFIG_SMF_METHOD_dir)/fc-cache

# Sun specification configuration files to be installed in /etc/fonts/conf.d
FONTCONFIG_CONFDIR = $(PROTODIR)/etc/fonts/conf.avail
SUN_CONFFILES = 20-indic.conf 49-sun-preuser.conf 90-sun-prefer-bitmap.conf
FONTCONFIG_SUN_CONFFILES = $(SUN_CONFFILES:%=$(FONTCONFIG_CONFDIR)/%)

# Additional targets to install beyond the default_install
MODULE_ADD_INSTALL_TARGETS = fontconfig-sunman-install \
	$(FONTCONFIG_SMF_MANIFEST) $(FONTCONFIG_SMF_METHOD) \
	$(FONTCONFIG_SUN_CONFFILES)

# Include common rulesets
include ../Makefile.inc

$(FONTCONFIG_SMF_MANIFEST): fc-cache.xml
	/usr/sbin/svccfg validate fc-cache.xml
	mkdir -p $(FONTCONFIG_SMF_MANIFEST_dir)
	$(INSTALL_SCRIPT) -c -m 0444 fc-cache.xml $@

$(FONTCONFIG_SMF_METHOD): fc-cache.sh
	mkdir -p $(FONTCONFIG_SMF_METHOD_dir)
	$(INSTALL_SCRIPT) -c -m 0755 fc-cache.sh $@

$(FONTCONFIG_SUN_CONFFILES): $(SUN_CONFFILES)
	mkdir -p $(PROTODIR)/etc/fonts/conf.d $(PROTODIR)/etc/fonts/conf.avail
	@f=`basename $@` ; $(START_CMD_ECHO) ; \
	cp -fp $$f $@ ; \
	rm -f $(PROTODIR)/etc/fonts/conf.d/$$f ; \
	ln -sf ../conf.avail/$$f $(PROTODIR)/etc/fonts/conf.d/$$f

FC_MAN_DIR=$(PROTODIR)/usr/share/man

fontconfig-sunman-install: default_install
	mkdir -p $(FC_MAN_DIR)/man4 $(FC_MAN_DIR)/man3lib
	sed -e 's/.TH "FONTS-CONF" "5"/.TH "fonts.conf" "4"/' 	\
	    -e 's/SUNWfontconfig/SUNWfontconfig-root/' 		\
	  $(FC_MAN_DIR)/man5/fonts-conf.5 > $(FC_MAN_DIR)/man4/fonts.conf.4
	-rm -f $(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	perl -p -e 's/^$$/.LP/;' -e 's/.?\251/\\(co/;' 		\
		libfontconfig.3lib $(SOURCE_DIR)/COPYING >> 	\
		$(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	-rm -f $(FC_MAN_DIR)/man1/fc-cache.1
	cat $(SOURCE_DIR)/fc-cache/fc-cache.1 sunman-notes-fc-cache >> \
		$(FC_MAN_DIR)/man1/fc-cache.1


