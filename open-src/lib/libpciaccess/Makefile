###############################################################################
#
# libpciaccess Makefile
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=libpciaccess

# Version number (used in path names)
MODULE_VERSION=0.10.6

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	scanpci.man.patch 

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
source_scanpci_man:: default_source
	(cd $(SOURCE_DIR) && \
	 /usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
	        -a '{Availability, $(SUN_PACKAGE)}' \
		-a '{Interface Stability, $(MODULE_STABILITY)}' \
		-p $(MODULE_PREFIX)/bin/ \
		src/scanpci.man && \
	 sed -e 's/__xorgversion__/"$(MODULE_NAME) $(MODULE_VERSION)"/' \
		src/scanpci.man > src/scanpci.1 )

# We want to install scanpci, even though the upstream Makefile doesn't
install_scanpci:: default_install
	mkdir -p $(PROTODIR)$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR) \
		 $(PROTODIR)$(MODULE_PREFIX)/share/man/man1
	$(INSTALL_SCRIPT) -m 755 $(SOURCE_DIR)/src/scanpci \
		$(PROTODIR)$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)/scanpci
	$(INSTALL_SCRIPT) -m 444 $(SOURCE_DIR)/src/scanpci.1 \
		$(PROTODIR)$(MODULE_PREFIX)/share/man/man1/scanpci.1

