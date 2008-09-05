###############################################################################
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
# @(#)Makefile	1.14	08/09/05
#

# Package name used in tarballs
MODULE_NAME=mesa

# Version number (used in path names)
MODULE_VERSION=7.0.3

# Source tarball
SOURCE_TARBALL_NAME=MesaLib-$(MODULE_VERSION).tar.bz2
SOURCE_TARBALL_NAME_SET=yes

# Download site for source
SOURCE_URL=$(SF_DOWNLOADS_URL)/mesa3d/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/mesa
GIT_URL_SET=yes

# Directory created by unpacking source
SOURCE_DIRNAME=Mesa-$(MODULE_VERSION)
SOURCE_DIRNAME_SET=yes
SOURCE_DIR = $(BUILD_DIR)/$(SOURCE_DIRNAME)

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	solaris-paths.patch	\
	mklib.patch		\
	solaris-dri-port.patch	\
	6742579.patch		\
	mesa965.patch		\
	6538497.patch		\
	6559710.patch

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Mesa doesn't use GNU autoconf until Mesa 7.1, but has it's own configuration
CONFIGURE_TARGETS=$(SOURCE_DIR)/configs/current
CONFIGURE_TARGETS_SET=yes

MESA_CFG_32_sparc=sunos5-v8
MESA_CFG_64_sparc=sunos5-v9

# Build with gcc on x86:
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes
MESA_CFG_32_i386=solaris-x86-gcc-dri
MESA_CFG_64_i386=solaris-x64-gcc-dri

# Build with Sun Studio cc on x86:
#MODULE_COMPILER=suncc
#MODULE_COMPILER_SET=yes
#MESA_CFG_32_i386=solaris-x86-cc-dri
#MESA_CFG_64_i386=solaris-x64-cc-dri

MESA_CFG_32 = $(MESA_CFG_32_$(MACH))
MESA_CFG_64 = $(MESA_CFG_64_$(MACH))

MODULE_BUILD_32_FLAGS= MESA_CFG=$(MESA_CFG_32)
MODULE_BUILD_64_FLAGS= MESA_CFG=$(MESA_CFG_64)

# pkg-config --cflags libdrm
LIBDRM_CFLAGS = -I$(PROTODIR)$(X11_INCLUDES_DIR) -I$(PROTODIR)$(X11_INCLUDES_DIR)/drm
# pkg-config --libs libdrm
LIBDRM_LIB = -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) -R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) -ldrm

# Additional flags to pass to make
MODULE_BUILD_MAKEFLAGS= \
	SHELL=/usr/bin/ksh93 \
	PASSED_CFLAGS="$(MODTYPE_CFLAGS)" \
	PASSED_INCLUDES="$(DEFAULT_CONFIG_CPPFLAGS) $(MODTYPE_CPPFLAGS)" \
	ARCH_FLAGS="" \
	LIBDRM_CFLAGS="$(LIBDRM_CFLAGS)" LIBDRM_LIB="$(LIBDRM_LIB)" \
	default

# Additional environment variables to pass to make
MODULE_BUILD_ENV= PKG_CONFIG_PATH=$(DEFAULT_PKG_CONFIG_PATH)

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

# Configure Mesa
$(SOURCE_DIR)/configs/current: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR)/configs && \
	 rm -f current && \
	 ln -s $(MESA_CFG) current )

# Additional headers to install that X server builds use
install_gen::
	mkdir -p $(PROTODIR)/$(X11_INCLUDES_DIR)/GL/internal/
	$(INSTALL_SCRIPT) -m 644 $(SOURCE_DIR)/include/GL/internal/*h \
		$(PROTODIR)/$(X11_INCLUDES_DIR)/GL/internal/
