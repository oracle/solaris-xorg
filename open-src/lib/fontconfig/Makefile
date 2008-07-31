###############################################################################
#
# Fontconfig 2.x Makefile
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
# @(#)Makefile	1.76	08/07/24
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Build 32-bit, 64-bit, or both?
BUILD_TYPES=32 64
BUILD_TYPES_SET=yes

# Package name used in tarballs
LIB_MODULE=fontconfig

# Version number (used in path names)
LIB_VERS=2.5.0

# Source tarball
SOURCE_TARBALL_NAME=fontconfig-$(LIB_VERS).tar.gz
SOURCE_TARBALL_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Download site for source
SOURCE_URL=http://fontconfig.org/release/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=fontconfig-$(LIB_VERS).patch,-p1 \
	       fontconfig-$(LIB_VERS).patch.hide,-p1 \
	       fontconfig-$(LIB_VERS)-fonts.conf.patch \
	       6729284.patch,-p1

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(LIB_MODULE)-$(LIB_VERS)

# Library name (used for specfiles/mapfiles)
LIBNAME=fontconfig

# Special version of freetype config to force certain arguments
FONTCONFIG_FT2CFG=$(BUILD_DIR)/freetype-config

# Install to /usr, not /usr/X11
LIB_PREFIX=/usr
LIB_PREFIX_SET=yes

# Path needed to find docbook2man, etc.
# DOC_PATH= PATH="$(PROTODIR)$(X11_DIR)/bin:$(PROTODIR)$(TOOLS_DIR)/bin:$$PATH"

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = \
    --with-confdir=/etc/fonts --with-default-fonts='--font-dirs-go-here--' \
    --with-freetype-config=../freetype-config

LIB_ADD_CONFIG_ENV = $(LIBPATHS)

# Library built in tree
LIB_BUILT=$(SOURCE_DIR)/src/.libs/libfontconfig.so.1.3.0

# What to build
BUILD_TARGETS=$(GNUMAKE) $(LIB_BUILT)

# Options to autoconf configure script
FONTCONFIG_CFG=--enable-shared=yes --enable-static=no --with-expat-includes=/usr/sfw/include \
	--with-expat-lib=/usr/sfw/lib$(LIBSUBDIR) --prefix=/usr --with-confdir=/etc/fonts \
	--with-cache-dir=/var/cache/fontconfig --with-default-fonts=/usr/openwin/lib/X11/fonts \
	--with-add-fonts='--font-dirs-go-here--' --with-freetype-config=../freetype-config \
	--libdir='$${exec_prefix}/lib$(LIBSUBDIR)' --bindir='$${exec_prefix}/bin$(ARCHLIBSUBDIR)' \
	--mandir='$${prefix}/share/man' 

# Where to install most files
FONTCONFIG_prefix=$(PROTODIR)

# Where to install libraries
FONTCONFIG_libdir=$(FONTCONFIG_prefix)/usr/lib

# Paths to find libraries
LIBPATHS = LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(LIBSUBDIR) LD_RUN_PATH=/usr/lib$(LIBSUBDIR) $(DOC_PATH)

# Include common rulesets
include $(TOP)/common/Makefile.inc

# Path to install fc-cache manifest & method script
FONTCONFIG_SMF_MANIFEST_dir=$(PROTODIR)/var/svc/manifest/application/font
FONTCONFIG_SMF_MANIFEST=$(FONTCONFIG_SMF_MANIFEST_dir)/fc-cache.xml
FONTCONFIG_SMF_METHOD_dir=$(PROTODIR)/lib/svc/method
FONTCONFIG_SMF_METHOD=$(FONTCONFIG_SMF_METHOD_dir)/fc-cache

$(FONTCONFIG_FT2CFG): $(PROTODIR)/usr/bin$(LIBSUBDIR)/freetype-config
	-if [ -f $@ ] ; then rm $@ ; fi
	sed -e 's|-L$$libdir|-L$(PROTODIR)$${exec_prefix}/lib$(LIBSUBDIR) -L$$libdir|' -e 's|^includedir=$${prefix}|includedir=$(PROTODIR)$${prefix}|' $(PROTODIR)/usr/bin$(LIBSUBDIR)/freetype-config > $@
	chmod +x $@

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET) autoreconf $(FONTCONFIG_FT2CFG)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x configure ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LDFLAGS="$(LIB_LDFLAGS)" $(LIBPATHS) ./configure $(FONTCONFIG_CFG))
	if [ -f $(SOURCE_DIR)/src/Makefile.bak ] ; then     \
		rm -f $(SOURCE_DIR)/src/Makefile.bak ;      \
	fi

$(SOURCE_DIR)/fonts.conf-SUNW: $(SOURCE_DIR)/fonts.conf fonts.conf.append fix-fonts.conf.pl
	perl fix-fonts.conf.pl < $(SOURCE_DIR)/fonts.conf > $(SOURCE_DIR)/fonts.conf-SUNW

$(LIB_BUILT): $(SOURCE_DIR)/Makefile $(SOURCE_DIR)/fonts.conf-SUNW
	(cd $(SOURCE_DIR) ; LD_OPTIONS="$(LIB_LDFLAGS)" $(LIBPATHS) $(GNUMAKE) $(MFLAGS))

install_gen:: $(LIB_BUILT) $(SOURCE_DIR)/fonts.conf-SUNW $(FONTCONFIG_SMF_MANIFEST) $(FONTCONFIG_SMF_METHOD)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) $(GNUMAKE) -e DESTDIR=$(FONTCONFIG_prefix) libdir=/usr/lib$(ARCHLIBSUBDIR) bindir=/usr/bin$(ARCHLIBSUBDIR) install )
	rm $(PROTODIR)/etc/fonts/fonts.conf
	cp $(SOURCE_DIR)/fonts.conf-SUNW $(PROTODIR)/etc/fonts/fonts.conf
	$(MAKE) $(MFLAGS) fontconfig-sunman-install

$(FONTCONFIG_SMF_MANIFEST): fc-cache.xml
	/usr/sbin/svccfg validate fc-cache.xml
	mkdir -p $(FONTCONFIG_SMF_MANIFEST_dir)
	if [ -f $@ ] ; then /bin/rm -f $@ ; fi
	cp fc-cache.xml $@

$(FONTCONFIG_SMF_METHOD): fc-cache.sh
	mkdir -p $(FONTCONFIG_SMF_METHOD_dir)
	if [ -f $@ ] ; then /bin/rm -f $@ ; fi
	cp fc-cache.sh $@
	chmod 755 $@

# Quick inline perl script to fix up fc-cache & fc-list man pages
FC_MAN_FIX=perl -n -MPOSIX=strftime -e \
  'BEGIN { \
	$$date=strftime("%e %b %Y",localtime((stat($$ARGV[0]))[9])); \
	print "\047\\\" t\n"; \
   } \
   { \
	s|__vendorversion__|"$$date"| ; \
	s|__package__|SUNWfontconfig| ; \
	s|fontconfig\(3\)|libfontconfig\(3lib\)|; \
	s%^\\\|\.%.IR \\|.%; \
	print \
   }'
#
# 6368378: fc-cache and fc-list man pages are broken
#
FC_MAN_DIR=$(PROTODIR)/usr/share/man

fontconfig-sunman-install:
	mkdir -p $(FC_MAN_DIR)/man1 $(FC_MAN_DIR)/man4 $(FC_MAN_DIR)/man3lib
	$(FC_MAN_FIX) $(FC_MAN_DIR)/man5/fonts-conf.5 sunman-notes-fonts.conf $(TOP)/common/sunman-stability | sed -e 's/.TH "FONTS-CONF" "5"/.TH "fonts.conf" "4"/' -e 's/SUNWfontconfig/SUNWfontconfig-root/' > $(FC_MAN_DIR)/man4/fonts.conf.4
	-if [ -f $(FC_MAN_DIR)/man3lib/libfontconfig.3lib ] ; then \
		rm -f $(FC_MAN_DIR)/man3lib/libfontconfig.3lib ; \
	fi
	perl -p -e 's/^$$/.LP/;' -e 's/\251/\\(co/;' libfontconfig.3lib $(SOURCE_DIR)/COPYING >> $(FC_MAN_DIR)/man3lib/libfontconfig.3lib
	@case '$(MFLAGS)' in *[ik]*) set +e;; esac; set -x ;	\
	for i in fc-cache fc-list fc-match fc-cat ; do 		\
	    if [ -f $(FC_MAN_DIR)/man1/$$i.1 ] ; then 	\
		rm -f $(FC_MAN_DIR)/man1/$$i.1 ; 		\
	    fi ;						\
	    if [ -f sunman-notes-$$i ] ; then			\
		APPEND="sunman-notes-$$i $(TOP)/common/sunman-stability" ; \
		else						\
		APPEND=$(TOP)/common/sunman-stability ;		\
	    fi ;						\
	    if [ -f $(SOURCE_DIR)/$$i/$$i.1 ] ; then \
	    $(FC_MAN_FIX) $(SOURCE_DIR)/$$i/$$i.1 $$APPEND > \
		$(FC_MAN_DIR)/man1/$$i.1 ;		\
	    else						\
		$(FC_MAN_FIX) $$i.man $$APPEND > \
		$(FC_MAN_DIR)/man1/$$i.1 ;		\
		fi ;						\
	done

install_64::
	-/bin/rm -f $(FONTCONFIG_libdir)/64
	ln -s $(FONTCONFIG_libdir)/$(SUBDIR64) $(FONTCONFIG_libdir)/64


