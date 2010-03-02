###############################################################################
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
#

# Package name used in tarballs
MODULE_NAME=mesa

# Version number (used in path names)
MODULE_VERSION=7.4.4

# Checksums for upstream tarball
TARBALL_MD5  = b66528d314c574dccbe0ed963cac5e93
TARBALL_SHA1 = 1cdcef24900489bc0a879013b2138c0d9aedffe3

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
	install-pc-dir.patch,-p1 \
	glu-no-mangled-exports.patch \
	solaris-port.patch \
	6872597.patch 6866668.patch

# File created by running configure script
AUTOCONF_TARGET=$(SOURCE_DIR)/configs/autoconf
AUTOCONF_TARGET_SET=yes
MODULE_ADD_CONFIGURE_TARGETS=set_pc_dir

DRI_DRIVER_LIST_sparc=
DRI_DRIVER_LIST_i386=i915 i965 r200 r300 radeon
DRI_DRIVER_LIST= $(DRI_DRIVER_LIST_$(MACH)) swrast

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS = --with-driver=dri \
	--with-dri-driverdir='$(X11_SERVERMODS_DIR)/dri$(SERVERMOD_subdir)' \
	--with-dri-drivers='$(DRI_DRIVER_LIST)'

# Override LDFLAGS passed to configure to not include CFLAGS, since that
# breaks building libGLU with Sun Studio C++ compiler
MODULE_CONFIG_ENV = LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(ARCH_FLAGS) $(MODTYPE_LDFLAGS) $(MODULE_CFLAGS) $(MODULE_LDFLAGS)"

# Need to use GNU Make to build
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# To build with gcc, uncomment these lines and comment out the MODULE_CFLAGS
# MODULE_COMPILER=gcc
# MODULE_COMPILER_SET=yes

# Tell Sun compiler to not apply optimizations that increase binary sizes
# Without this flag, Studio 12 binaries are almost 2x bigger (CR 6808868)
MODULE_CFLAGS = -xspace

# Add include paths and linker paths needed for DRM modules
MODULE_CPPFLAGS = -I$(PROTODIR)$(X11_INCLUDES_DIR)/drm

MODULE_LDFLAGS= -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
		-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# pkg-config files that need to have -R added
FIX_PC_FILES=src/*/*.pc.in src/mesa/drivers/dri/dri.pc.in


### Include common rulesets
include ../Makefile.inc

# Mesa makefiles use a different variable name/method for pkgconfig path
set_pc_dir: $(AUTOCONF_TARGET)
	echo 'INSTALL_PC_DIR=$(PKGCONFIG_DIR)' >> $(AUTOCONF_TARGET)
