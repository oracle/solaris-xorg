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
# @(#)Makefile	1.5	07/01/31
#

PWD:sh=pwd
TOP=$(PWD)/../..
BUILD_TYPES=32 64

# Package name used in tarballs
LIB_MODULE=libdrm

# Version number (used in path names)
LIB_VERS=2.3.0

# Library name (used for specfiles/mapfiles)
LIBNAME=drm

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=*.man
#SUN_PACKAGE=SUNWxorg-server
#LIB_STABILITY=Volatile
#LIB_MAN_SUFFIX=3drm

# Library built in tree
LIB_BUILT=$(BUILD_DIR)/.libs/libdrm.so.6.0.0

# pkg-config files that need to have -R added
FIX_PC_FILES=libdrm.pc.in

########################################################################

# Source tarball
SOURCE_TARBALL_NAME=$(LIB_MODULE)-$(LIB_VERS).tar.bz2

# Download site for source
SOURCE_URL=http://dri.freedesktop.org/libdrm/$(SOURCE_TARBALL_NAME)

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(LIB_MODULE)-$(LIB_VERS)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = solaris-drm-port.patch

SUNTOUCH_MAN_FLAGS= -l $(LIBNAME) \
-a '{Availability, $(SUN_PACKAGE)} {Interface Stability, $(LIB_STABILITY)} {MT-Level, See XInitThreads(3X11)}' $(LIB_SUNTOUCH_MAN_FLAGS)

include $(TOP)/common/Makefile.inc

# Where to install files
LIB_prefix=$(X11_DIR)
LIB_ADD_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --enable-shared=yes --enable-static=no \
	--prefix=$(LIB_prefix) 	--mandir='$${prefix}/share/man' \
	--libdir='$${exec_prefix}/lib$(LIB_ADD_SUBDIR)$(LIBSUBDIR)' \
	$(LIB_ADD_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
THIS_LIB_CFLAGS=$(LIB_CFLAGS) -I$(PROTODIR)/usr/X11/include $(LIB_ADD_CFLAGS)

BUILD_32_FLAGS=ARCH_GCC_FLAGS="$(ARCH32_GCC_FLAGS)"
BUILD_64_FLAGS=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)"

CONFIG_ENV = CC=gcc CFLAGS="$(DEFAULT_GCC_CFLAGS) $(ARCH_GCC_FLAGS)" \
	LDFLAGS="$(GCC_LIB_LDFLAGS)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR) \
	INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	LIB_MAN_SUFFIX=$(LIB_MAN_SUFFIX)

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET) $(XPROTO_DEP)
	(cd $(SOURCE_DIR) ; chmod a+x configure ; \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )

build_gen: $(LIB_BUILT)

$(LIB_BUILT): $(SOURCE_DIR)/Makefile $(LIB_ADD_BUILD_DEPS)
	(cd $(SOURCE_DIR) ; LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS) $(LIB_ADD_LD_OPTIONS)" $(MAKE) $(MFLAGS) )


install_gen: $(LIB_BUILT)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS) $(LIB_ADD_LD_OPTIONS)" $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) libdir=$(LIB_prefix)/lib$(LIB_ADD_SUBDIR)$(ARCHLIBSUBDIR) install pkgconfigdir=/usr/lib$(ARCHLIBSUBDIR)/pkgconfig)
