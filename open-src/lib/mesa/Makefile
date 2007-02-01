###############################################################################
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

# Build 32-bit, 64-bit, or both?
BUILD_TYPES=32 64

# Version number (used in path names)
MESA_VERS=6.5.2

# Source tarball
SOURCE_TARBALL_NAME=MesaLib-$(MESA_VERS).tar.bz2

# Download site for source
SOURCE_URL=http://easynews.dl.sourceforge.net/sourceforge/mesa3d/$(SOURCE_TARBALL_NAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	solaris-paths.patch	\
	mklib.patch		\
	solaris-dri-port.patch	\
	glext.h.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/Mesa-$(MESA_VERS)

# Library built in tree
MESA_LIB=$(SOURCE_DIR)/lib/libGL.so.1

# Where to install most files
MESA_prefix=$(PROTODIR)$(X11_DIR)

# Where to install libraries
MESA_libdir=$(MESA_prefix)/lib

# What to build
BUILD_TARGETS=$(GNUMAKE) $(MESA_LIB)

### Include common rulesets
include $(TOP)/common/Makefile.inc

### Rules specific to this directory:

# Merge in additional sources from sun-src directory
source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)

MESA_CFG_32_sun4=sunos5-v8
MESA_CFG_64_sun4=sunos5-v9

# Build with gcc on x86:
MESA_CFLAGS=$(DEFAULT_GCC_CFLAGS) $(ARCH_GCC_FLAGS) -D_REENTRANT
MESA_CFG_32_i86pc=solaris-x86-gcc-dri
MESA_CFG_64_i86pc=solaris-x64-gcc-dri

# Build with Sun Studio cc on x86:
#MESA_CFLAGS=$(LIB_CFLAGS)
#MESA_CFG_32_i86pc=solaris-x86-cc-dri
#MESA_CFG_64_i86pc=solaris-x64-cc-dri

MESA_CFG_32:sh=arch | sed 's/^\(.*\)$/\$\(MESA_CFG_32_\1\)/'
MESA_CFG_64:sh=arch | sed 's/^\(.*\)$/\$\(MESA_CFG_64_\1\)/'

BUILD_32_FLAGS=ARCH_GCC_FLAGS="$(ARCH32_GCC_FLAGS)" MESA_CFG=$(MESA_CFG_32)
BUILD_64_FLAGS=ARCH_GCC_FLAGS="$(ARCH64_GCC_FLAGS)" MESA_CFG=$(MESA_CFG_64)

# pkg-config --cflags libdrm
LIBDRM_CFLAGS = -I$(PROTODIR)$(X11_INCLUDES_DIR) -I$(PROTODIR)$(X11_INCLUDES_DIR)/drm
# pkg-config --libs libdrm
LIBDRM_LIB = -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) -R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) -ldrm

MESA_INCLUDES= -I$(PROTODIR)$(X11_INCLUDES_DIR)

MESA_MAKEVARS= SHELL=/bin/ksh PASSED_CFLAGS="$(MESA_CFLAGS)" PASSED_INCLUDES="$(MESA_INCLUDES)" ARCH_FLAGS="" LIBDRM_CFLAGS="$(LIBDRM_CFLAGS)" LIBDRM_LIB="$(LIBDRM_LIB)"

MESA_MAKEENV= PKG_CONFIG_PATH=$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR):/usr/lib$(ARCHLIBSUBDIR)/pkgconfig
# LD_OPTIONS="$(LIB_REQUIRED_LDFLAGS)"

# Build Mesa
$(MESA_LIB):  $(GNUMAKE) $(UNPACK_TARGET)
	(cd $(SOURCE_DIR)/configs && rm -f current && ln -s $(MESA_CFG) current)
	(cd $(SOURCE_DIR) && $(MESA_MAKEENV) $(GNUMAKE) $(MFLAGS) -e $(MESA_MAKEVARS) default)

install_gen: $(MESA_LIB)
	(cd $(SOURCE_DIR) && $(GNUMAKE) $(MFLAGS) -e DESTDIR=$(PROTODIR) ARCHLIBSUBDIR=$(ARCHLIBSUBDIR) install )


