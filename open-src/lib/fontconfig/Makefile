###############################################################################
#
# Fontconfig 2.x Makefile
#
# Copyright 2005 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.62	06/08/29
#

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32 64

# Version number (used in path names)
FONTCONFIG_VERS=2.3.2

# Source tarball
SOURCE_TARBALL_NAME=fontconfig-$(FONTCONFIG_VERS).tar.gz

# Download site for source
SOURCE_URL=http://fontconfig.org/release/fontconfig-2.3.2.tar.gz

SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=fontconfig-$(FONTCONFIG_VERS).patch \
	       fontconfig-$(FONTCONFIG_VERS).patch.hide

include $(TOP)/common/Makefile.inc

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/fontconfig-$(FONTCONFIG_VERS)

# Special version of freetype config to force certain arguments
FONTCONFIG_FT2CFG=$(BUILD_DIR)/freetype-config

# Command line options to GNU autoconf configure script
FONTCONFIG_CFG=--enable-shared=yes --enable-static=no --with-expat-includes=/usr/sfw/include --with-expat-lib=/usr/sfw/lib$(LIBSUBDIR) --prefix=/usr --with-confdir=/etc/fonts --with-default-fonts='--font-dirs-go-here--' --with-freetype-config=../freetype-config --libdir='$${exec_prefix}/lib$(LIBSUBDIR)'

# Library built in tree
FONTCONFIG_LIB=$(SOURCE_DIR)/src/.libs/libfontconfig.so.1.0.4

# Where to install most files
FONTCONFIG_prefix=$(PROTODIR)

# Where to install libraries
FONTCONFIG_libdir=$(FONTCONFIG_prefix)/usr/lib

# Paths to find libraries
FONTCONFIG_libpaths= LD_LIBRARY_PATH=$(PROTODIR)/usr/sfw/lib$(LIBSUBDIR) LD_RUN_PATH=/usr/sfw/lib$(LIBSUBDIR)

# Path to install fc-cache manifest
FONTCONFIG_SMF_MANIFEST_dir=$(PROTODIR)/var/svc/manifest/application/font
FONTCONFIG_SMF_MANIFEST=$(FONTCONFIG_SMF_MANIFEST_dir)/fc-cache.xml

build_gen: $(FONTCONFIG_LIB)

$(FONTCONFIG_FT2CFG): $(PROTODIR)/usr/sfw/bin$(LIBSUBDIR)/freetype-config
	-if [ -f $@ ] ; then rm $@ ; fi
	sed -e 's|-L$$libdir|-L$(PROTODIR)$${exec_prefix}/lib$(LIBSUBDIR) -L$$libdir|' -e 's|^includedir=$${prefix}|includedir=$(PROTODIR)$${prefix}|' $(PROTODIR)/usr/sfw/bin$(LIBSUBDIR)/freetype-config > $@
	chmod +x $@

# Run configure script
$(SOURCE_DIR)/Makefile:= OPT_CFLAGS=$(FC_OPTFLAGS)
FC_OPTFLAGS:sh=arch | sed 's/^\(.*\)$/\$\(FC_\1_OPT_FLAGS\)/'
FC_i386_OPT_FLAGS = ""
FC_sun4_OPT_FLAGS = $(DEFAULT_OPT_CFLAGS)

$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET) $(FONTCONFIG_FT2CFG)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x configure ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS)" LD="$(CC) $(LIB_CFLAGS) $(LDFLAGS)" LDFLAGS="$(LIB_LDFLAGS)" $(FONTCONFIG_libpaths) ./configure $(FONTCONFIG_CFG) )
	if [ -f $(SOURCE_DIR)/src/Makefile.bak ] ; then 	\
		rm -f $(SOURCE_DIR)/src/Makefile.bak ; 	\
	fi

$(SOURCE_DIR)/fonts.conf-SUNW: $(SOURCE_DIR)/fonts.conf fonts.conf.append fonts.conf.preuser fix-fonts.conf.pl
	perl fix-fonts.conf.pl < $(SOURCE_DIR)/fonts.conf > $(SOURCE_DIR)/fonts.conf-SUNW

# Build Fontconfig 
$(FONTCONFIG_LIB): $(SOURCE_DIR)/Makefile $(SOURCE_DIR)/fonts.conf-SUNW
	(cd $(SOURCE_DIR) ; LD_OPTIONS="$(LIB_LDFLAGS)" $(FONTCONFIG_libpaths) $(MAKE) $(MFLAGS) )

install_gen: $(FONTCONFIG_LIB) $(SOURCE_DIR)/fonts.conf-SUNW $(FONTCONFIG_SMF_MANIFEST)
	(cd $(SOURCE_DIR) ; $(FONTCONFIG_libpaths) $(MAKE) -e DESTDIR=$(FONTCONFIG_prefix) libdir=/usr/lib$(ARCHLIBSUBDIR) bindir=/usr/bin$(ARCHLIBSUBDIR) install )
	rm $(PROTODIR)/etc/fonts/fonts.conf
	cp $(SOURCE_DIR)/fonts.conf-SUNW $(PROTODIR)/etc/fonts/fonts.conf
	$(MAKE) $(MFLAGS) fontconfig-sunman-install

$(FONTCONFIG_SMF_MANIFEST): fc-cache.xml
	/usr/sbin/svccfg validate fc-cache.xml
	mkdir -p $(FONTCONFIG_SMF_MANIFEST_dir)
	if [ -f $@ ] ; then /bin/rm -f $@ ; fi
	cp fc-cache.xml $@

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

fontconfig-sunman-install: $(PROTODIR)/usr/man/man5/fonts-conf.5
	mkdir -p $(PROTODIR)/usr/man/man1 $(PROTODIR)/usr/man/man4 $(PROTODIR)/usr/man/man3lib
	$(FC_MAN_FIX) $(PROTODIR)/usr/man/man5/fonts-conf.5 sunman-notes-fonts.conf $(TOP)/common/sunman-stability | sed -e 's/.TH "FONTS-CONF" "5"/.TH "fonts.conf" "4"/' -e 's/SUNWfontconfig/SUNWfontconfig-root/' > $(PROTODIR)/usr/man/man4/fonts.conf.4
	-if [ -f $(PROTODIR)/usr/man/man3lib/libfontconfig.3lib ] ; then \
		rm -f $(PROTODIR)/usr/man/man3lib/libfontconfig.3lib ; \
	fi
	perl -p -e 's/^$$/.LP/;' -e 's/\251/\\(co/;' libfontconfig.3lib $(SOURCE_DIR)/COPYING >> $(PROTODIR)/usr/man/man3lib/libfontconfig.3lib
	for i in fc-cache fc-list ; do 			\
	    if [ -f $(PROTODIR)/usr/man/man1/$$i.1 ] ; then 	\
		rm -f $(PROTODIR)/usr/man/man1/$$i.1 ; 		\
	    fi ;						\
	    if [ -f sunman-notes-$$i ] ; then			\
		APPEND="sunman-notes-$$i $(TOP)/common/sunman-stability" ;	\
	    else						\
		APPEND=$(TOP)/common/sunman-stability ;		\
	    fi ;						\
	    $(FC_MAN_FIX) $(SOURCE_DIR)/$$i/$$i.man $$APPEND > \
		$(PROTODIR)/usr/man/man1/$$i.1 ;		\
	done

install_64::
	-/bin/rm -f $(FONTCONFIG_libdir)/64
	ln -s $(FONTCONFIG_libdir)/$(SUBDIR64) $(FONTCONFIG_libdir)/64


