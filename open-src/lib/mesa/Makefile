###############################################################################
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
# @(#)Makefile	1.23	09/07/17
#

# Package name used in tarballs
MODULE_NAME=mesa

# Version number (used in path names)
MODULE_VERSION=7.4.4

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
	solaris-port.patch

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
