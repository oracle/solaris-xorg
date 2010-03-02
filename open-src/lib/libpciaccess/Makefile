###############################################################################
#
# libpciaccess Makefile
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

# Package name used in tarballs
MODULE_NAME=libpciaccess

# Version number (used in path names)
MODULE_VERSION=0.10.9

# Checksums for upstream tarball
TARBALL_MD5  = 685cb20e7a6165bc010972f1183addbd
TARBALL_SHA1 = 491e137f34a22f7208a6b0f68cbe51d929923591

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	pciaccess_private.h.patch \
	scanpci.man.patch \
	solx_devfs.c.patch

# Library name
LIBNAME=libpciaccess

# Where to install libpciaccess files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWpciaccess
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3pciaccess

# pkg-config files that need to have -R added
FIX_PC_FILES=pciaccess.pc.in

# Activate additional debugging code in debug builds
MODULE_DEBUG_FLAGS=-DDEBUG

# Additional targets for scanpci
MODULE_ADD_SOURCE_TARGETS = source_scanpci_man
MODULE_ADD_INSTALL_TARGETS = install_scanpci

include ../Makefile.inc

# Update scanpci.man with program man page flags instead of library ones
source_scanpci_man: default_source
	(cd $(SOURCE_DIR) && \
	 /usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
	        -a '{Availability, $(SUN_PACKAGE)}' \
		-a '{Interface Stability, $(MODULE_STABILITY)}' \
		-p $(MODULE_PREFIX)/bin/ \
		src/scanpci.man && \
	 sed -e 's/__xorgversion__/"$(MODULE_NAME) $(MODULE_VERSION)"/' \
		src/scanpci.man > src/scanpci.1 )

# We want to install scanpci, even though the upstream Makefile doesn't
install_scanpci: default_install
	mkdir -p $(PROTODIR)$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR) \
		 $(PROTODIR)$(MODULE_PREFIX)/share/man/man1
	$(INSTALL_SCRIPT) -m 755 $(SOURCE_DIR)/src/scanpci \
		$(PROTODIR)$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)/scanpci
	$(INSTALL_SCRIPT) -m 444 $(SOURCE_DIR)/src/scanpci.1 \
		$(PROTODIR)$(MODULE_PREFIX)/share/man/man1/scanpci.1

