###############################################################################
#
# Xpm Makefile
#
# Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.4	07/01/04
#

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32

# Package name used in tarballs
MODULE_NAME=libXpm

# Version number (used in path names)
MODULE_VERS=3.5.6

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = gettext.patch,-p1

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(XORG_RELEASES_URL)/lib/$(SOURCE_TARBALL_NAME)

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(MODULE_NAME)-$(MODULE_VERS)

# Where to install files
APP_prefix=$(X11_DIR)

# man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=cxpm/cxpm.man sxpm/sxpm.man
SUNTOUCH_MAN_FLAGS= -p $(APP_prefix)/bin/ \
	-a '{Availability, SUNWxwplt} {Interface Stability, Uncommitted}'

include $(TOP)/common/Makefile.inc

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --prefix=$(APP_prefix) --mandir='$${prefix}/share/man' \
	$(APP_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
CONFIG_ENV = CC=$(CC) CFLAGS="$(PROG_CFLAGS)" LD="$(LD) $(LDFLAGS)" \
	CPPFLAGS="-I $(PROTODIR)$(X11_INCLUDES_DIR)" \
	LDFLAGS="$(PROG_LDFLAGS) -L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR) 

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; cat acinclude.m4 >> aclocal.m4 ; \
	 automake && autoconf  && \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )

# Binary built in tree
APP_BIN=$(SOURCE_DIR)/cxpm/cxpm $(SOURCE_DIR)/sxpm/sxpm 

build_gen: $(APP_BIN)

$(APP_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(MAKE) $(MFLAGS))

install_gen: $(APP_BIN)
	(cd $(SOURCE_DIR)/cxpm ; $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)
	(cd $(SOURCE_DIR)/sxpm ; $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)
	mkdir -p $(PROTODIR)$(X11_LC_MESSAGES_DIR)
	(cd $(SOURCE_DIR)/cxpm ; cp -p cxpm.po $(PROTODIR)$(X11_LC_MESSAGES_DIR))
	(cd $(SOURCE_DIR)/sxpm ; cp -p sxpm.po $(PROTODIR)$(X11_LC_MESSAGES_DIR))
