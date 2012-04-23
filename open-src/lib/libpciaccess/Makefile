###############################################################################
#
# libpciaccess Makefile
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libpciaccess

# Version number (used in path names)
MODULE_VERSION=0.13.1

# Checksums for upstream tarball
TARBALL_MD5   = 399a419ac6a54f0fc07c69c9bdf452dc
TARBALL_SHA1  = ae4dcf27a1b52c6a1fd90b21165fbaecae34e8ac
TARBALL_SHA256= 27ab68f645691a4ca258dcc9d131040586e134f9233ce079794496f6264da084

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	legacy-io.patch,-p1 \
	scanpci.man.patch \
	primary-vga.patch,-p1 \
	scanpci_64bit.patch,-p1

# Library name
LIBNAME=libpciaccess

# Where to install libpciaccess files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=diagnostic/scanpci
MODULE_PKGNAME_SET=yes
# Package classification (override default)
MODULE_PKGCLASS=System/Hardware
MODULE_PKGCLASS_SET=yes

# ARC cases that covered this module
# PSARC/2008/638  libpciaccess & scanpci 
MODULE_ARC_CASES = PSARC/2008/638

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/scanpci.man
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3pciaccess

# Update scanpci.man with program man page flags instead of library ones
SUNTOUCH_MAN_FLAGS  = $(COMMON_SUNTOUCH_MAN_FLAGS)
SUNTOUCH_MAN_FLAGS +=-p $(MODULE_PREFIX)/bin/
SUNTOUCH_MAN_FLAGS_SET=yes

# pkg-config files that need to have -R added
FIX_PC_FILES=pciaccess.pc.in

# Activate additional debugging code in debug builds
MODULE_DEBUG_FLAGS=-DDEBUG

# Override linker flags for scanpci program
PCIACCESS_PROG_LDFLAGS = $(MAPFILES_FOR_PROGS) ../src/libpciaccess.so.0
PROGRAM_VARS = scanpci
PROGRAM_MAKEFLAGS = $(PROGRAM_VARS:%=%_LDADD="$(PCIACCESS_PROG_LDFLAGS)")
MODULE_BUILD_MAKEFLAGS = AM_MAKEFLAGS='$(PROGRAM_MAKEFLAGS)'

# Additional targets for scanpci
MODULE_ADD_INSTALL_TARGETS_64 = install_scanpci
MODULE_ADD_INSTALL_TARGETS = $(MODULE_ADD_INSTALL_TARGETS_$(BUILD_TYPE))

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = scanpci

include ../Makefile.inc

# We want to install scanpci, even though the upstream Makefile doesn't
install_scanpci: default_install
	mkdir -p $(PROTODIR)$(MODULE_PREFIX)/bin \
		 $(PROTODIR)/etc/security/exec_attr.d/ \
		 $(PROTODIR)$(MODULE_PREFIX)/share/man/man1
	$(INSTALL_SCRIPT) -m 755 $(SOURCE_DIR)/scanpci/scanpci \
		$(PROTODIR)$(MODULE_PREFIX)/bin/scanpci
	$(INSTALL_SCRIPT) -m 444 $(SOURCE_DIR)/man/scanpci.1 \
		$(PROTODIR)$(MODULE_PREFIX)/share/man/man1/scanpci.1
	$(INSTALL_SCRIPT) -m 444 exec_attr \
		$(PROTODIR)/etc/security/exec_attr.d/scanpci
