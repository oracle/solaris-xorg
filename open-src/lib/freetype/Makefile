###############################################################################
#
# Freetype 2.x Makefile
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
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Package name used in tarballs
MODULE_NAME=freetype

# Library name used in man pages & spec files
LIBNAME=freetype

# Version number (used in path names)
MODULE_VERSION=2.3.9

# Download site for source
SOURCE_URL=http://download.savannah.gnu.org/releases/freetype/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	freetype-2.3.9-CVE-2009-0946.patch,-p1 \
	fedora_aliasing_patch90.patch,-p1 \
	freetype-config.patch,-p1 \
	ftoption.h.0.patch,-p1 \
	ftconfig.h.1.patch,-p1 \
	ttobjs.h.3.patch,-p1 \
	ttobjs.c.4.patch,-p1 \
	ttinterp.h.5.patch,-p1 \
	ttinterp.c.6.patch,-p1 \
	ttgload.c.7.patch,-p1 \
	ftconfig.in.8.patch,-p1 \
	unix-cc.in.9.patch,-p1 \
	freetype2.in.10.patch,-p1 \
	configure.11.patch,-p1 \
	configure.12.patch,-p1

# Where to install freetype files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= \
	--with-zlib \
	--with-pic

# pkg-config files that need to have -R added
FIX_PC_FILES=builds/unix/freetype2.in

# License file
LICENSE_FILE=docs/FTL.TXT
LICENSE_FILE_SET=yes

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Don't run normal configure/install rules - freetype is different
CONFIGURE_TARGETS=$(SOURCE_DIR)/config.mk
CONFIGURE_TARGETS_SET=yes
INSTALL_TARGETS=install_freetype
INSTALL_TARGETS_SET=yes

# Environment variables to pass to configure & build steps
MODULE_ENV = \
    SHELL="/bin/bash" \
    MAKESHELL="/bin/bash" \
    CONFIG_SHELL="/bin/bash" \
    CPP="$(CC) -E $(LIB_CFLAGS)" \
    CXXCPP="$(CXX) -E $(CXXFLAGS)" \
    MAKEFLAGS= \
    MAKE=$(GNUMAKE) GNUMAKE=$(GNUMAKE) CFG="$(CONFIG_OPTS)"

MODULE_CONFIG_ENV = $(MODULE_ENV)
MODULE_BUILD_ENV = $(CONFIG_ENV)

MODULE_INSTALL_MAKEFLAGS=bindir=$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

# Run configure script (Freetype helpfully does this the first time you call
# gmake - this isn't normal for autoconf programs)
$(SOURCE_DIR)/config.mk: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	chmod a+x builds/unix/configure builds/unix/mkinstalldirs builds/unix/install-sh ; \
	$(CONFIG_ENV) $(GNUMAKE) )

FT_bindir=$(MODULE_PREFIX)/bin$(ARCHLIBSUBDIR)
FT_libdir=$(MODULE_PREFIX)/lib$(ARCHLIBSUBDIR)

# Have to override default_install since the -e causes our BUILD_DIR to
# clash with the one in freetype's Makefiles
install_freetype:: $(BUILD_TARGETS)
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
install_32::
	-/bin/rm -f $(PROTODIR)/usr/sfw
	ln -s . $(PROTODIR)/usr/sfw

# Make 64-bit link when installing 64-bit binaries

BIN_INST_DIR=$(PROTODIR)$(MODULE_PREFIX)/bin

install_64::
	-/bin/rm -f $(BIN_INST_DIR)/64
	mkdir -p $(BIN_INST_DIR)
	ln -s $(SUBDIR64) $(BIN_INST_DIR)/64
