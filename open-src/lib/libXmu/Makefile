###############################################################################
#
# Xmu 1.x Makefile
#
# Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5  = 6836883a0120e8346cf7f58dc42e465a
TARBALL_SHA1 = 81876a0848070bfc4476a2538fc16898eb6d96cd

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	bitmap-path.patch \
	solaris-abi.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xmu

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES= no man pages in upstream package
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3xmu

# ARC cases that covered this module
# PSARC/2005/192 Update libXmu to X11R6.8 version
MODULE_ARC_CASES = PSARC/2005/192
# PSARC/2008/036 libXmuu
MODULE_ARC_CASES += PSARC/2008/036

# Additional pkg(5) package name used for libxmuu package
XMUU_PKGNAME=$(MODULE_PKGNAME:libxmu=libxmuu)

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# pkg-config files that need to have -R added
FIX_PC_FILES=xmu.pc.in xmuu.pc.in

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --with-lint --enable-lint-library

# configure adds -lsocket -lnsl to both libraries, but libXmu only needs
# -lnsl and libXmuu doesn't need either one.
MODULE_LD_OPTIONS = -z ignore

# Work around libtool brokenness that adds -R `pwd` to libXmu build
DELIBTOOLIZE=yes

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Additional rules beyond what upstream provides
MODULE_ADD_INSTALL_TARGETS=install_solaris_exts 

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXmu.so libXmu.so.4 libXmuu.so libXmuu.so.1

include ../Makefile.inc

# Additional files we need to install
CP = cp -pf
MANDIR = $(PROTODIR)$(X11_MAN_DIR)/man3xmu

install_solaris_exts: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(X11_INCLUDES_DIR)/X11/Xmu $(MANDIR) \
		 $(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR)
	$(CP) $(SOURCE_DIR)/include/X11/Xmu/XmuSolaris.h \
	       $(SOURCE_DIR)/include/X11/Xmu/WhitePoint.h \
		$(PROTODIR)$(X11_INCLUDES_DIR)/X11/Xmu
	$(CP) $(ADDITIONAL_SOURCE_DIR)/*.3xmu $(MANDIR)

# Metadata for libxmuu package
LIBXMUU_PKG_METADATA_DIR      = $(PROTOMETA)/x11-library-libxmuu
LIBXMUU_PKG_ATTRDATA_FILE     = $(LIBXMUU_PKG_METADATA_DIR)/$(ATTRDATA_FILE_NAME)
LIBXMUU_PKG_LICENSE_FILE      = $(LIBXMUU_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME)
LIBXMUU_METADATA_TARGETS = \
	$(LIBXMUU_PKG_METADATA_DIR) \
	$(LIBXMUU_PKG_ATTRDATA_FILE) \
	$(LIBXMUU_PKG_LICENSE_FILE)

install_metadata: $(LIBXMUU_METADATA_TARGETS)

$(LIBXMUU_PKG_METADATA_DIR):
	mkdir -p $@

$(LIBXMUU_PKG_ATTRDATA_FILE) $(LIBXMUU_PKG_LICENSE_FILE): $(LIBXMUU_PKG_METADATA_DIR)
