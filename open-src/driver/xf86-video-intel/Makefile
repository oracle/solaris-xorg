###############################################################################
#
# xf86-video-intel Makefile
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-intel

# Version number (used in path names)
MODULE_VERSION=2.6.3

# Checksums for upstream tarball
TARBALL_MD5  = 440c014bbd3072b5d379fe1bdb861918
TARBALL_SHA1 = 96fe7e8b804ab8833b1d462f99c08be29bb9e283

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= build-dri.patch libraries.patch \
		6538497_6554161.patch bugs.patch \
                gem.patch 6872597.patch \
		gem-hang.patch 6898754.patch \
		6907204.patch port.patch \
		mplayer-crash.patch 6907803.patch \
		6915882.patch disable-tiling-for-ironlake.patch \
		6950311.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-graphics-ddx
MODULE_STABILITY=Volatile

# Intel driver doesn't work on some chipsets if built with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Extra flags to pass to configure script
MODULE_CONFIG_OPTS=--enable-dri

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions$(SERVERMOD_subdir) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERMODS_DIR)/extensions$(SERVERMOD_subdir) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) 

# Need to regenerate configure & Makefile.in's after patching
# configure.ac & Makefile.am's in libraries.patch & build-dri.patch
AUTORECONF=yes

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

# Need to override install path for all the modules, not just intel_drv.so
MODULE_MAKEFLAGS = \
	ch7017_ladir='$(DRIVER_DIR)' \
	ch7xxx_ladir='$(DRIVER_DIR)' \
	ivch_ladir='$(DRIVER_DIR)' \
	sil164_ladir='$(DRIVER_DIR)' \
	tfp410_ladir='$(DRIVER_DIR)'

include ../Makefile.inc

install_docs: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	cp -p $(SOURCE_DIR)/README $(PROTODIR)$(X11_DOC_DIR)/README.intel
