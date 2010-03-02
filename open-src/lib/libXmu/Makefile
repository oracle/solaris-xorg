###############################################################################
#
# Xmu 1.x Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the "Software"),
# to deal in the Software without restriction, including without limitation
# the rights to use, copy, modify, merge, publish, distribute, sublicense,
# and/or sell copies of the Software, and to permit persons to whom the
# Software is furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice (including the next
# paragraph) shall be included in all copies or substantial portions of the
# Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
# THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
# DEALINGS IN THE SOFTWARE.
#
#

# Package name used in tarballs
MODULE_NAME=libXmu

# Version number (used in path names)
MODULE_VERSION=1.0.5

# Checksums for upstream tarball
TARBALL_MD5  = fc4d66be7a1a1eb474954728415e46d6
TARBALL_SHA1 = e7ff5960f2374852b941f909cdbdeafe7d29322f

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
