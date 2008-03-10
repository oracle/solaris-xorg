###############################################################################
#
# Really Slick Screensavers for GLX Makefile
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
# @(#)Makefile	1.1	08/03/07
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Version number (used in path names)
RSS_GLX_VERS=0.8.1

# Source tarball
SOURCE_TARBALL_NAME=rss-glx_$(RSS_GLX_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/rss-glx/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = busyspheres.patch matrixview.patch \
	install-util.patch cflags.patch kde-config.patch \
	rss-glx-0.8.1-gcc43-buildfix.patch,-p1

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/rss-glx_$(RSS_GLX_VERS)

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*src/*.1 reallyslick/*src/*.1
SUNTOUCH_MAN_FLAGS= \
 -a '{Availability, SUNWxscreensaver-hacks-gl} {Interface Stability, Volatile}'

# The COPYING file in the release only contains the GPL license, not the
# Copyright notices, so use the manually gathered one instead
LICENSE_FILE=../../Copyright-notices
LICENSE_FILE_SET=yes

include $(TOP)/common/Makefile.inc

# Where to find OpenGL headers/libraries
OPENGL_DIR_sparc=/usr/openwin
OPENGL_DIR_i386=/usr/X11
OPENGL_DIR=$(OPENGL_DIR_$(MACH))

# Where to find/install Xscreensaver files
XSS_DIR=$(X11_DIR)/lib/xscreensaver

# We don't use normal $(PROG_CFLAGS) since this is built with gcc/g++
RSS_GLX_CFLAGS=-O3 $(ARCH32_GCC_FLAGS)
RSS_GLX_LDFLAGS=$(GCC_PROG_LDFLAGS)

# Comment out or override to enable building with -g instead of -O2
RSS_GLX_DEBUGFLAGS= --disable-debug 

# Binary built in tree
RSS_GLX_BIN=$(SOURCE_DIR)/reallyslick/cpp_src/solarwinds

build_gen:   $(RSS_GLX_BIN)

# Regen configure script
$(SOURCE_DIR)/configure: $(UNPACK_TARGET) $(SOURCE_DIR)/configure.in
	(cd $(SOURCE_DIR) ; \
	 rm -f config.guess config.sub ; \
	 ./autogen.sh )

RSS_GLX_CONFIG_FLAGS= --prefix=$(XSS_DIR) --disable-sound \
	--bindir='$$(prefix)/hacks' --mandir=$(X11_DIR)/share/man \
	 --with-configdir=$(XSS_DIR)/config/control-center-2.0 \
	 $(RSS_GLX_DEBUGFLAGS) CC=$(GCC) CXX=$(GXX) \
	 CFLAGS="$(RSS_GLX_CFLAGS)" CXXFLAGS="$(RSS_GLX_CFLAGS)" \
	 LDFLAGS="$(RSS_GLX_LDFLAGS)" 

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET) $(SOURCE_DIR)/configure
	rm $(SOURCE_DIR)/other_src/matrixview_textures/cpics
	touch $(SOURCE_DIR)/other_src/matrixview_textures/cpics
	(cd $(SOURCE_DIR) ; ./configure $(RSS_GLX_CONFIG_FLAGS))

$(RSS_GLX_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) )

# XScreenSaver app-defaults file
XSS_AD=$(PROTODIR)$(X11_DIR)/lib/X11/app-defaults/XScreenSaver

install_gen:: $(RSS_GLX_BIN) $(XSS_AD)
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS) install DESTDIR=$(PROTODIR) )
	$(PROTODIR)$(XSS_DIR)/hacks/rss-glx_install.pl $(XSS_AD)
	-case '$(MFLAGS)' in *[ik]*) set +e;; esac;
	(cd $(SOURCE_DIR) ; \
	 for m in $(SUNTOUCHED_MANPAGES) ; do \
	    mv $(PROTODIR)$(X11_DIR)/share/man/man1/`basename $$m` \
	       $(PROTODIR)$(X11_DIR)/share/man/man6/`basename $$m .1`.6 ; \
	 done )

$(XSS_AD):
	(cd ../xscreensaver ; $(MAKE) $(MFLAGS) install)
