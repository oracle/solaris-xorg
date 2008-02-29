# -*- Makefile -*- rules common to most X.Org library modules
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile.inc	1.10	08/02/27
#

PWD:sh=pwd
TOP=$(PWD)/../..

BUILD_TYPES=32 64
BUILD_TYPES_SET=yes

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/$(LIB_MODULE)-$(LIB_VERS)

SUNTOUCH_MAN_FLAGS= -l $(LIBNAME) \
-a '{Availability, $(SUN_PACKAGE)} {Interface Stability, $(LIB_STABILITY)} {MT-Level, See XInitThreads(3X11)}' $(LIB_SUNTOUCH_MAN_FLAGS)

include $(TOP)/common/Makefile.inc

# Source tarball
SOURCE_TARBALL_DEFAULT	= $(SOURCE_TARBALL_SET:yes=$(POUND_SIGN))
$(SOURCE_TARBALL_DEFAULT) SOURCE_TARBALL_NAME=$(LIB_MODULE)-$(LIB_VERS).tar.bz2
SOURCE_TARBALL_DIR=lib

# Where to install files - default is /usr/X11
LIB_PREFIX_DEFAULT	= $(LIB_PREFIX_SET:yes=$(POUND_SIGN))
$(LIB_PREFIX_DEFAULT)	LIB_PREFIX=$(X11_DIR)

# Command line options to GNU autoconf configure script
CONFIG_OPTS = --enable-shared=yes --enable-static=no \
	--prefix=$(LIB_PREFIX) 	--mandir='$${prefix}/share/man' \
	--libdir='$${exec_prefix}/lib$(LIB_ADD_SUBDIR)$(LIBSUBDIR)' \
	--bindir='$${exec_prefix}/bin$(LIBSUBDIR)' \
	$(LIB_ADD_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
THIS_LIB_CFLAGS=$(LIB_CFLAGS) -I$(PROTODIR)/usr/X11/include $(LIB_ADD_CFLAGS)

# Use proto area for building against current library versions
LIB_LD_OPTIONS = $(LIB_REQUIRED_LDFLAGS) \
	-L$(PROTODIR)$(X11_DIR)/lib$(LIBSUBDIR) $(LIB_ADD_LD_OPTIONS)

CONFIG_ENV = CC=$(CC) CFLAGS="$(THIS_LIB_CFLAGS)" LD="$(LD) $(LDFLAGS)" \
	CPPFLAGS="-I $(PROTODIR)$(X11_INCLUDES_DIR) $(LIB_ADD_CPPFLAGS)" \
	LDFLAGS="$(LIB_LDFLAGS) -L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR) -L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR):/usr/lib$(ARCHLIBSUBDIR)/pkgconfig \
	INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	LIB_MAN_SUFFIX=$(LIB_MAN_SUFFIX) $(LIB_ADD_CONFIG_ENV)

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET) $(XPROTO_DEP)
	@case '$(MFLAGS)' in *[ik]*) set +e;; esac; \
	if [ "x$(AUTORECONF)" = "xtrue" ] ; then \
	    (cd $(SOURCE_DIR) && \
	     ACLOCAL="$(ACLOCAL_PATH)" autoreconf -v --install --force ) ; \
	fi
	(cd $(SOURCE_DIR) ; chmod a+x configure ; \
	 $(CONFIG_ENV) ./configure $(CONFIG_OPTS) )

build_gen: $(LIB_BUILT)

$(LIB_BUILT): $(SOURCE_DIR)/Makefile $(LIB_ADD_BUILD_DEPS)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) LD_OPTIONS="$(LIB_LD_OPTIONS)" $(MAKE) $(MFLAGS) $(LIB_ADD_MAKEFLAGS) )

install_gen:: $(LIB_BUILT)
	(cd $(SOURCE_DIR) ; $(LIBPATHS) LD_OPTIONS="$(LIB_LD_OPTIONS)" $(MAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) libdir=$(LIB_PREFIX)/lib$(LIB_ADD_SUBDIR)$(ARCHLIBSUBDIR) install pkgconfigdir=/usr/lib$(ARCHLIBSUBDIR)/pkgconfig $(LIB_ADD_MAKEFLAGS))
