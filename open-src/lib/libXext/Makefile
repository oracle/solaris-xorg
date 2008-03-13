###############################################################################
#
# Xext 1.x Makefile
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
# @(#)Makefile	1.1	08/03/12
#

# Package name used in tarballs
LIB_MODULE=libXext

# Version number (used in path names)
LIB_VERS=1.0.4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	shm.patch \
	sun-exts.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xext

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
LIB_STABILITY=Committed
LIB_MAN_SUFFIX=3Xext

# Library built in tree
LIB_BUILT=$(SOURCE_DIR)/src/.libs/libXext.so.0

# pkg-config files that need to have -R added
FIX_PC_FILES=xext.pc.in

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = --with-lint --enable-lint-library

# Enable Solaris-specific code
LIB_ADD_CPPFLAGS = -DSUNSOFT

# Link with version mapfile and libraries that contain sources that used
# to be in libXext
LIB_ADD_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=true

### Rules specific to this directory:

# Merge in additional sources from sun-src directory
add-sun-src:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)

source_gen:: add-sun-src

DESTDIR=$(PROTODIR)
INSTDIR=$(DESTDIR)$(X11_INCLUDES_DIR)/X11/extensions

install_gen::
	mkdir -p $(INSTDIR)
	cp -pf sun-src/include/X11/extensions/*.h $(INSTDIR)/

include ../Makefile.inc
