# -*- Makefile -*- rules common to all X.Org drivers
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile.inc	1.9	07/01/31
#

PWD:sh=pwd
TOP=$(PWD)/../..

BUILD_TYPES_sun4=64
BUILD_TYPES_i86pc=32 64
BUILD_TYPES:sh=arch | sed 's/^\(.*\)$/\$\(BUILD_TYPES_\1\)/'

# Source tarball
SOURCE_TARBALL_NAME=$(DRIVER_NAME)-$(DRIVER_VERS).tar.bz2

# Download site for source
SOURCE_URL=$(XORG_RELEASES_URL)/driver/$(SOURCE_TARBALL_NAME)

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(DRIVER_NAME)-$(DRIVER_VERS)

include $(TOP)/common/Makefile.inc

# Where to install files
DRIVER_prefix=$(X11_DIR)

# Command line options to GNU autoconf configure script
CONFIG_OPTS = \
	--prefix=$(DRIVER_prefix) --mandir='$${prefix}/share/man' \
	--with-xorg-module-dir='$${libdir}/modules$(MODULE_subdir)' \
	$(DRIVER_CONFIG_OPTS)

BUILD_32_FLAGS=ARCH_GCC_FLAGS="$(ARCH32_GCC_FLAGS)" ARCHBINSUBDIR="$(SUBDIR32)"
BUILD_64_FLAGS=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)" ARCHBINSUBDIR="$(SUBDIR64)"

CONFIG_CPPFLAGS= \
	-I $(PROTODIR)/usr/X11/include/xorg	\
	-I $(PROTODIR)/usr/X11/include 		\
	-I $(PROTODIR)/usr/X11/include/drm	\
	-I $(PROTODIR)/usr/X11/include/X11/dri	\
	$(DRIVER_CPPFLAGS)

# Environment variable options to GNU autoconf configure script
CONFIG_ENV_common = \
	CPPFLAGS="$(CONFIG_CPPFLAGS)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR) \
	INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	$(DRIVER_CONFIG_ENV)

DRIVER_LDFLAGS_cc=$(LDFLAGS) -B direct
DRIVER_LDFLAGS_gcc=$(GCC_LDFLAGS) -Wl,-B,direct -R/usr/sfw/lib$(ARCHLIBSUBDIR)

CONFIG_ENV_cc = CC=$(CC) CFLAGS="$(PROG_CFLAGS)" \
	LD="$(CC) $(DRIVER_LDFLAGS_cc)" LDFLAGS="$(DRIVER_LDFLAGS_cc)"
CONFIG_ENV_gcc = CC=$(GCC) CFLAGS="$(DEFAULT_GCC_CFLAGS) $(ARCH_GCC_FLAGS)" \
	LD="$(GCC) $(DRIVER_LDFLAGS_gcc)" LDFLAGS="$(DRIVER_LDFLAGS_gcc)"

CONFIG_ENV_sun4= $(CONFIG_ENV_cc) $(CONFIG_ENV_common)
CONFIG_ENV_i86pc= $(CONFIG_ENV_gcc) $(CONFIG_ENV_common)
CONFIG_ENV:sh=arch | sed 's/^\(.*\)$/\$\(CONFIG_ENV_\1\)/'


# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; chmod a+x configure ; \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )
	$(TOP)/common/delibtoolize.pl $(SOURCE_DIR)

# Binary built in tree
DRIVER_BIN=$(SOURCE_DIR)/src/$(DRIVER_NAME)

BUILD_ENV=LD_OPTIONS="-z defs $(DRIVER_ADD_LIBS) -M $(TOP)/xserver/xorg/$(BUILD_DIR)/mapfile-Xorg-externs -lc" \
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

build_gen: $(DRIVER_BIN)

$(DRIVER_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; $(BUILD_ENV) $(MAKE) $(MFLAGS) )

install_gen:: $(DRIVER_BIN)
	(cd $(SOURCE_DIR) ; $(BUILD_ENV) $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) install)
