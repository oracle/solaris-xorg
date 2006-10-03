###############################################################################
#
# Xft 2.x Makefile
#
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.5	06/10/02
#

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32 64

# Version number (used in path names)
XFT_VERS=2.1.6

# Source tarball
SOURCE_TARBALL_NAME=libXft-$(XFT_VERS).tar.bz2

# Download site for source
SOURCE_URL=http://xlibs.freedesktop.org/release/$(SOURCE_TARBALL_NAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	xft.pc.patch \
	xftcore-6261221.patch \
	cjk-embolden.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/libXft-$(XFT_VERS)

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=Xft.3.in Xft.3
SUNTOUCH_MAN_FLAGS= -l Xft \
 -a '{Availability, SUNWxwxft} {Interface Stability, Volatile} {MT-Level, See XInitThreads(3X11)}'

include $(TOP)/common/Makefile.inc

# Where to install files
XFT_prefix=$(OW_DIR)

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --enable-shared=yes --enable-static=no --prefix=$(XFT_prefix) --x-libraries="/usr/openwin/lib$(LIBSUBDIR) -L/usr/openwin/sfw/lib$(LIBSUBDIR) -R/usr/openwin/lib$(LIBSUBDIR) -R/usr/openwin/sfw/lib$(LIBSUBDIR)" --libdir='$${exec_prefix}/lib$(LIBSUBDIR)' --mandir=$(X11_DIR)/share/man

# Paths to find libraries at build time (LD_LIBRARY_PATH) 
# and run time (LD_RUN_PATH) and freetype-config binary at build time (PATH)
LIBPATHS = LD_LIBRARY_PATH=$(PROTODIR)/usr/sfw/lib$(LIBSUBDIR):$(PROTODIR)/usr/openwin/lib$(LIBSUBDIR):/usr/sfw/lib$(LIBSUBDIR) LD_RUN_PATH=/usr/sfw/lib$(LIBSUBDIR) PATH=$(PROTODIR)/usr/sfw/bin$(LIBSUBDIR):$(PATH)

# Include Paths - need headers for fontconfig & freetype from build area
INCLUDES = -I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/sfw/include

# Library built in tree
XFT_LIB=$(BUILD_DIR)/.libs/libXft.so.2

# Mapfile used to control which symbols are exported
XFT_MAPFILE=$(PWD)/mapfile.scope

build_gen: $(XFT_LIB)

$(XFT_LIB): $(SOURCE_DIR)/Makefile $(XFT_MAPFILE)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS) -M$(XFT_MAPFILE)" $(MAKE) $(MFLAGS))

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; chmod a+x configure ; \
	 CC=$(CC) CFLAGS="$(LIB_CFLAGS) $(INCLUDES)" LD="$(CC) $(LIB_CFLAGS) $(LDFLAGS)"  LDFLAGS="$(LIB_LDFLAGS)" $(LIBPATHS) ./configure $(CONFIG_OPTS) )

install_gen: $(XFT_LIB)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS)" $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) libdir=$(XFT_prefix)/lib$(ARCHLIBSUBDIR) bindir=$(XFT_prefix)/bin$(ARCHLIBSUBDIR) install)
