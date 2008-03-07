###############################################################################
#
# Xmu 1.x Makefile
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
# @(#)Makefile	1.4	08/03/06
#

# Package name used in tarballs
LIB_MODULE=libXmu

# Version number (used in path names)
LIB_VERS=1.0.4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	bitmap-path.patch \
	solaris-abi.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xmu

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES= no man pages in upstream package
SUN_PACKAGE=SUNWxwplt
LIB_STABILITY=Committed
LIB_MAN_SUFFIX=3Xmu

# Library built in tree
LIB_BUILT=$(SOURCE_DIR)/src/libXmu.so.4

# pkg-config files that need to have -R added
FIX_PC_FILES=xmu.pc.in xmuu.pc.in

# Command line options to GNU autoconf configure script
LIB_ADD_CONFIG_OPTS = --with-lint --enable-lint-library

# Work around libtool brokenness that adds -R `pwd` to libXmu build
LIB_ADD_BUILD_DEPS=delibtoolize

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=true

include ../Makefile.inc

# Add-in Sun-added source files
source_gen::
	mkdir -p $(SOURCE_DIR)/src $(SOURCE_DIR)/include/X11/Xmu/
	(cd $(SOURCE_DIR)/src ; ln -s ../../../sun-src/*.c .)
	(cd $(SOURCE_DIR)/include/X11/Xmu ; ln -s ../../../../../sun-src/*.h .)

# Additional files we need to install
CP = cp -pf
MANDIR = $(PROTODIR)/usr/X11/share/man/man3Xmu

install_gen::
	mkdir -p $(PROTODIR)/usr/X11/include/X11/Xmu $(MANDIR) \
		 $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)
	$(CP) $(SOURCE_DIR)/include/X11/Xmu/XmuSolaris.h \
	       $(SOURCE_DIR)/include/X11/Xmu/WhitePoint.h \
		$(PROTODIR)/usr/X11/include/X11/Xmu
	$(CP) sun-src/*.3Xmu $(MANDIR)
	rm -f $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)/libXmu.so \
		$(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)/libXmuu.so
	ln -s libXmu.so.4 $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)/libXmu.so
	ln -s libXmuu.so.1 $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)/libXmuu.so
