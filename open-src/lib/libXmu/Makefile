###############################################################################
#
# Xmu 1.x Makefile
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
MODULE_NAME=libXmu

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	bitmap-path.patch \
	solaris-abi.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xmu

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES= no man pages in upstream package
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3Xmu

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# pkg-config files that need to have -R added
FIX_PC_FILES=xmu.pc.in xmuu.pc.in

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --with-lint --enable-lint-library

# Work around libtool brokenness that adds -R `pwd` to libXmu build
MODULE_BUILD_DEPS=delibtoolize

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Additional rules beyond what upstream provides
MODULE_ADD_INSTALL_TARGETS=install_solaris_exts 

include ../Makefile.inc

# Additional files we need to install
CP = cp -pf
MANDIR = $(PROTODIR)$(X11_MAN_DIR)/man3Xmu

install_solaris_exts: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(X11_INCLUDES_DIR)/X11/Xmu $(MANDIR) \
		 $(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR)
	$(CP) $(SOURCE_DIR)/include/X11/Xmu/XmuSolaris.h \
	       $(SOURCE_DIR)/include/X11/Xmu/WhitePoint.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/Xmu
	$(CP) $(ADDITIONAL_SOURCE_DIR)/*.3Xmu $(MANDIR)
