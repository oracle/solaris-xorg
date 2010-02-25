###############################################################################
#
# xterm Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
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
#

# Package name used in tarballs
MODULE_NAME=xterm

# Version number (used in path names)
MODULE_VERSION=253

# Checksums for upstream tarball
TARBALL_MD5  = 363cc4865b7691bd0ae5eebb42cde9c9
TARBALL_SHA1 = 4234376b43a0c60f2516df0c0abe26037671d50f

# Source tarball
SOURCE_URL=ftp://invisible-island.net/xterm/$(SOURCE_TARBALL_NAME:.tar.gz=.tgz)
SOURCE_URL_SET=yes

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed

# Paths to find Xft/fontconfig/freetype headers libraries at build time
# and run time 
FT_CFLAGS= -I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/include/freetype2
FT_LIBS= \
  -L$(PROTODIR)/usr/lib$(LIBSUBDIR) -R/usr/lib$(LIBSUBDIR) -lXft -lfontconfig

# Additional flags to pass to configure script
MODULE_CONFIG_OPTS = --enable-256-color --enable-88-color --enable-wide-chars \
  --with-freetype-cflags="$(FT_CFLAGS)" --with-freetype-libs="$(FT_LIBS)" \
  --disable-setuid --disable-setgid  --enable-luit --disable-imake \
  --x-includes=$(PROTODIR)$(X11_INCLUDES_DIR) --x-libraries=$(X11_LIB_DIR) \
  --with-app-defaults=/usr/share/X11/app-defaults \
  --with-icondir=/usr/share/pixmaps --enable-logging

# Sort-of emulate automake silent rules - run make configure V=1 to see 
# full compiler flags
MODULE_CONFIG_OPTS_VERBOSE_0 = --disable-echo
MODULE_CONFIG_OPTS_VERBOSE_1 = --enable-echo
MODULE_CONFIG_OPTS_VERBOSE_ = $(MODULE_CONFIG_OPTS_VERBOSE_0)
MODULE_CONFIG_OPTS += $(MODULE_CONFIG_OPTS_VERBOSE_$(V))

# Tell configure where to find luit binary
MODULE_CONFIG_ENV = LUIT=$(X11_BIN_DIR)/luit

MODULE_ADD_BUILD_TARGETS=fix_manpages
MODULE_ADD_INSTALL_TARGETS=install_docs

include ../Makefile.inc

# Fix man page footers to not say "Last changed: X Window System"
XORGRELSTRING = $(MODULE_NAME) $(MODULE_VERSION)
  XORGMANNAME = X Version 11
  XVENDORVERS = "$(XORGRELSTRING)" "$(XORGMANNAME)"

fix_manpages: $(UNPACK_TARGET)
	/usr/perl5/bin/perl -i -p -e 's{__vendorversion__}{$(XVENDORVERS)}g' \
	  $(SOURCE_DIR)/*.man

# Install documentation of xterm control sequences for packaging
MODULE_DOCDIR=/usr/share/doc/xterm

install_docs: $(UNPACK_TARGET)
	mkdir -p $(PROTODIR)$(MODULE_DOCDIR)
	rm -f $(PROTODIR)$(MODULE_DOCDIR)/ctlseqs.txt
	cp -pf $(SOURCE_DIR)/ctlseqs.txt $(PROTODIR)$(MODULE_DOCDIR)/
