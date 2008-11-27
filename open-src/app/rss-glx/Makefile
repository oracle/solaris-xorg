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
# @(#)Makefile	1.3	08/11/26
#

# Package name used in tarballs
MODULE_NAME=rss-glx

# Version number (used in path names)
MODULE_VERSION=0.8.1

# Source tarball/directory name - uses _ instead of default -
SOURCE_DIRNAME=$(MODULE_NAME)_$(MODULE_VERSION)
SOURCE_DIRNAME_SET=yes

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/rss-glx/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = busyspheres.patch matrixview.patch \
	install-util.patch cflags.patch kde-config.patch \
	rss-glx-0.8.1-gcc43-buildfix.patch,-p1

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*src/*.1 reallyslick/*src/*.1
SUN_PACKAGE=SUNWxscreensaver-hacks-gl
MODULE_STABILITY=Volatile
MODULE_SUNTOUCH_MAN_FLAGS= -p $(XSS_DIR)/hacks/

# The COPYING file in the release only contains the GPL license, not the
# Copyright notices, so use the manually gathered one instead
LICENSE_FILE=../../Copyright-notices
LICENSE_FILE_SET=yes

# Where to find OpenGL headers/libraries
OPENGL_DIR_sparc=/usr/openwin
OPENGL_DIR_i386=/usr/X11
OPENGL_DIR=$(OPENGL_DIR_$(MACH))

# Where to find/install Xscreensaver files
XSS_DIR=$(X11_DIR)/lib/xscreensaver
MODULE_PREFIX=$(XSS_DIR)
MODULE_PREFIX_SET=yes

# We don't use normal $(PROG_CFLAGS) since this is built with gcc/g++
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

include ../Makefile.inc

# Configure flag to enable building with -O2 instead of -g
# Disabled by running "make debug" target
$(BUILD_DEBUG:yes=$(POUND_SIGN)) RSS_GLX_DEBUGFLAGS= --disable-debug 

# Regen configure script
AUTORECONF=yes

MODULE_CONFIG_OPTS= --disable-sound \
	--bindir='$$(prefix)/hacks' --mandir=$(X11_DIR)/share/man \
	--with-configdir=$(XSS_DIR)/config/control-center-2.0 \
	$(RSS_GLX_DEBUGFLAGS)

# Clear pictures we don't want in build
source_gen::
	rm $(SOURCE_DIR)/other_src/matrixview_textures/cpics
	touch $(SOURCE_DIR)/other_src/matrixview_textures/cpics

# XScreenSaver app-defaults file
XSS_AD=$(PROTODIR)$(X11_DIR)/lib/X11/app-defaults/XScreenSaver
$(XSS_AD):
	(cd ../xscreensaver ; $(MAKE) $(MAKEFLAGS) install)

install_gen:: $(RSS_GLX_BIN) $(XSS_AD)
	$(PROTODIR)$(XSS_DIR)/hacks/rss-glx_install.pl $(XSS_AD)
	mkdir -p $(PROTODIR)$(X11_DIR)/share/man/man6
	-case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac;
	(cd $(SOURCE_DIR) ; \
	 for m in $(SUNTOUCHED_MANPAGES) ; do \
	    mv $(PROTODIR)$(X11_DIR)/share/man/man1/`basename $$m` \
	       $(PROTODIR)$(X11_DIR)/share/man/man6/`basename $$m .1`.6 ; \
	 done )




