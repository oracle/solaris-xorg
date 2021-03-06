###############################################################################
#
# xcb - Lighter weight X11 client libraries
#
# Copyright (c) 2010, 2017, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=libxcb

# Version number (used in path names)
MODULE_VERSION=1.12

# Checksums for upstream tarball
TARBALL_MD5   = 28e552bd78bc1050b6b26ca1db0e5bb6
TARBALL_SHA1  = 2f03490d1c75c8a3f902f74b717af6501773926a
TARBALL_SHA256= 4adfb1b7c67e99bc9c2ccb110b2f175686576d2f792c8a71b9c8b19014057b5b

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
MODULE_DESC = the XCB family of X11 client libraries
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES  = Makefile.am.0.patch,-p1
SOURCE_PATCHES += manpage.patch
SOURCE_PATCHES += python-tabs.patch

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Library name (used for specfiles/mapfiles)
LIBNAME=xcb

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= man/xcb-examples.man man/xcb-requests.man
MODULE_STABILITY=Volatile
MODULE_MTLEVEL=Safe
MODULE_MTLEVEL_SET=yes
LIB_MAN_SUFFIX=3xcb

MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP) -lc
LD_LIBRARY_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = $(MODTYPE_CONFIG_OPTS) \
		     --disable-libtool-lock \
		     --enable-devel-docs=no \
		     --enable-composite \
		     --enable-damage \
		     --enable-dpms \
		     --enable-dri2 \
		     --enable-glx \
		     --enable-randr \
		     --enable-record \
		     --enable-render \
		     --enable-resource \
		     --enable-screensaver \
		     --enable-shape \
		     --enable-shm \
		     --enable-sync \
		     --enable-xevie \
		     --enable-xfixes \
		     --enable-xfree86-dri \
		     --enable-xinerama \
		     --enable-xinput \
		     --enable-xprint \
		     --disable-selinux \
		     --enable-xtest \
		     --enable-xv \
		     --enable-xvmc \
		     --with-queue-size=32768 \
		     --with-pic

# Work around libtool's interference
DELIBTOOLIZE=yes

# pkg-config files that need to have -R added
FIX_PC_FILES=*.pc.in

# Additional rules beyond what upstream provides
MODULE_ADD_INSTALL_TARGETS=install_solaris_exts

include ../Makefile.inc

# Additional files we need to install
MANDIR = $(PROTODIR)$(MODULE_MAN_DIR)/man3xcb

# Need to apply suntouch changes to man pages generated during build
install_solaris_exts: $(INSTALL_DEPS)
	$(PERL) $(TOP)/open-src/common/suntouch-manpages.pl \
		$(SUNTOUCH_MAN_FLAGS) $(BUILD_DIR)/src/man/*.3xcb
	mkdir -p $(MANDIR)
	ginstall -m 0444 $(SOURCE_DIR)/man/xcb-examples.man \
		$(MANDIR)/xcb-examples.3xcb
	ginstall -m 0444 $(SOURCE_DIR)/man/xcb-requests.man \
		$(MANDIR)/xcb-requests.3xcb
	ginstall -m 0444 $(BUILD_DIR)/src/man/*.3xcb $(MANDIR)/
