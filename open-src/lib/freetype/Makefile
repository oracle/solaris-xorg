###############################################################################
#
# Freetype 2.x Makefile
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
# @(#)Makefile	1.77	08/10/06
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Package name used in tarballs
MODULE_NAME=freetype

# Library name used in man pages & spec files
LIBNAME=freetype

# Version number (used in path names)
MODULE_VERSION=2.3.7

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-config.patch \
	ftoption.h.0.patch \
	ftconfig.h.1.patch \
	freetype.h.2.patch \
	ttobjs.h.3.patch \
	ttobjs.c.4.patch \
	ttinterp.h.5.patch \
	ttinterp.c.6.patch \
	ttgload.c.7.patch \
	ftconfig.in.8.patch \
	unix-cc.in.9.patch \
	freetype2.in.10.patch \
	configure.11.patch \
	configure.12.patch

# Where to install freetype files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= \
	--with-zlib \
	--with-pic


FT_LIB=$(FT_DIR)/objs/.libs/libfreetype.so.6.3.17

# What to build
BUILD_TARGETS=$(GNUMAKE) $(FT_LIB)

# pkg-config files that need to have -R added
FIX_PC_FILES=builds/unix/freetype2.in

# License file
LICENSE_FILE=docs/FTL.TXT
LICENSE_FILE_SET=yes

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Don't run normal configure/build/install rules - freetype is different
CONFIGURE_TARGETS=$(SOURCE_DIR)/config.mk
CONFIGURE_TARGETS_SET=yes
BUILD_TARGETS=$(FT_LIB)
BUILD_TARGETS_SET=yes
INSTALL_TARGETS=
INSTALL_TARGETS_SET=yes

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

# Run configure script (Freetype helpfully does this the first time you call
# gmake - this isn't normal for autoconf programs)
$(SOURCE_DIR)/config.mk: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	chmod a+x builds/unix/configure builds/unix/mkinstalldirs builds/unix/install-sh ; \
	    SHELL="/bin/bash" \
	    MAKESHELL="/bin/bash" \
	    CONFIG_SHELL="/bin/bash" \
	    CC="$(CC)" \
	    CXX="$(CXX)" \
	    CPP="$(CC) -E $(LIB_CFLAGS)" \
	    CXXCPP="$(CXX) -E $(CXXFLAGS)" \
	    CFLAGS="$(LIB_CFLAGS)" \
	    CXXFLAGS="$(LIB_CXXFLAGS)" \
	    LDFLAGS="$(LIB_LDFLAGS)" \
	    CFG="$(FT_CFG)" \
	    LD="$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS)" \
	    MAKE=$(GNUMAKE) GNUMAKE=$(GNUMAKE) CFG="$(CONFIG_OPTS)" \
	    INSTALL="$(TOP)/common/install-sh -c" CPPROG="cp -p" \
	    $(GNUMAKE) )

# Build Freetype
$(FT_LIB): $(CONFIGURE_TARGETS)
	(cd $(SOURCE_DIR) ; \
	    SHELL="/bin/bash" \
	    MAKESHELL="/bin/bash" \
	    CONFIG_SHELL="/bin/bash" \
	    CC="$(CC)" \
	    CXX="$(CXX)" \
	    CPP="$(CC) -E $(LIB_CFLAGS)" \
	    CXXCPP="$(CXX) -E $(CXXFLAGS)" \
	    CFLAGS="$(LIB_CFLAGS)" \
	    CXXFLAGS="$(LIB_CXXFLAGS)" \
	    LDFLAGS="$(LIB_LDFLAGS)" \
	    MAKE=$(GNUMAKE) GNUMAKE=$(GNUMAKE) \
	    LDFLAGS="$(LIB_LDFLAGS)" \
	    CFG="$(FT_CFG)" \
	    LD="$(CC) $(LIB_CFLAGS) $(LIB_LDFLAGS)" \
	    INSTALL="$(TOP)/common/install-sh -c" \
	    $(GNUMAKE) )

FT_bindir=$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)
FT_libdir=$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)

install_gen:: $(FT_LIB)
	mkdir -p $(PROTODIR)/usr/bin$(ARCHLIBSUBDIR)
	( cd $(SOURCE_DIR)/builds/unix ; \
	    ./config.status --file ftconfig ; \
	    rm -f ftconfig.h ; \
	    mv ftconfig ftconfig.h ; \
	    touch -acm ftconfig.h )
	( cd $(SOURCE_DIR) ; \
	    cp -fp ./builds/unix/ftconfig.h ./include/freetype/config/ftconfig.h )
	( cd $(SOURCE_DIR) ; \
	  $(GNUMAKE) DESTDIR=$(PROTODIR) prefix=$(MODULE_PREFIX) libdir=$(FT_libdir) bindir=$(FT_bindir) install )

# $(PROTODIR)/usr/sfw is made so that the RUNPATH in mkfontscale picks up
# libfreetype from the $(PROTODIR) when running in the build
install_gen::
	-/bin/rm -f $(PROTODIR)/usr/sfw
	ln -s . $(PROTODIR)/usr/sfw

# Make 64-bit link when installing 64-bit binaries

BIN_INST_DIR=$(PROTODIR)$(MODULE_PREFIX)/bin

install_64::
	-/bin/rm -f $(BIN_INST_DIR)/64
	ln -s $(SUBDIR64) $(BIN_INST_DIR)/64


