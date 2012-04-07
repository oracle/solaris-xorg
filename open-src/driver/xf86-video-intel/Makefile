###############################################################################
#
# xf86-video-intel Makefile
#
# Copyright (c) 2006, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=2.18.0

# Checksums for upstream tarball
TARBALL_MD5  = 34f3987ffe86e30c57abc33b7f8030e9
TARBALL_SHA1 = 77fae98e73414140bf214dca5da32bcf079c4463

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= \
	build-dri.patch \
	libraries.patch batch.patch \
	bromolow-chipset.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Need to build 32-bit libXvMC libraries even when X server is 64-bit only
BUILD_TYPES=32 64
BUILD_TYPES_SET=yes

# but we only want those libraries built in the 32-bit build, not the whole
# driver (which would overwrite the 64-bit driver)
BUILD_TARGETS = build-in-subdir $(MODTYPE_ADD_BUILD_TARGETS)
BUILD_TARGETS_32 = build_legacy_xvmc
BUILD_TARGETS += $(BUILD_TARGETS_$(BUILD_TYPE))
BUILD_TARGETS_SET=yes
subdir_32 = $(SOURCE_DIR)/src/xvmc
subdir_64 = $(SOURCE_DIR)
subdir = $(subdir_$(BUILD_TYPE))
install_subdir_cmd = install
INSTALL_TARGETS = install-in-subdir $(MODTYPE_ADD_INSTALL_TARGETS)
INSTALL_TARGETS_32 = install_legacy_xvmc
INSTALL_TARGETS_64 = install_docs
INSTALL_TARGETS += $(INSTALL_TARGETS_$(BUILD_TYPE))
INSTALL_TARGETS_SET=yes
LIBXVMC_LD_OPTIONS = -L$(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR)
LIBXVMC_LD_OPTIONS += $(LIB_REQUIRED_LDFLAGS) -z ignore -lc
MODULE_BUILD_ENV_32 = LD_OPTIONS="$(LIBXVMC_LD_OPTIONS)"
MODULE_BUILD_ENV = $(MODULE_BUILD_ENV_$(BUILD_TYPE))

# Intel driver doesn't work on some chipsets if built with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Extra flags to pass to configure script
# --libdir is needed to set path for XvMC libraries
MODULE_CONFIG_OPTS=--enable-dri --libdir='$(X11_LIB_DIR)$(ARCHLIBSUBDIR)'

# echo is set to workaround configure/libtool strangeness in 2.15.0 that
# causes ../../libtool: line 791: X--mode=install: command not found errors
MODULE_CONFIG_ENV = echo=/usr/gnu/bin/echo
MODULE_BUILD_ENV += echo=/usr/gnu/bin/echo

# Library paths at build time
MODULE_LD_OPTIONS= -z ignore \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-L$(PROTODIR)$(MESA_XSERVERMODS_DIR)/extensions \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR) \
	-M $(PWD)/mapfile.externs

# Need to regenerate configure & Makefile.in's after patching
# configure.ac & Makefile.am's in libraries.patch & build-dri.patch
AUTORECONF=yes

include ../Makefile.inc

install_docs: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	cp -pf $(SOURCE_DIR)/README $(PROTODIR)$(X11_DOC_DIR)/README.intel

build-in-subdir: $(BUILD_DEPS)
install-in-subdir: $(INSTALL_DEPS)

LEGACY_XVMC_SUBDIR = $(SOURCE_DIR)/src/legacy/i810/xvmc

build_legacy_xvmc: $(BUILD_DEPS)
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(LEGACY_XVMC_SUBDIR)) $(subdir_cmd)

install_legacy_xvmc: $(INSTALL_DEPS)
	$(DEFAULT_INSTALL_COMMAND:@DIR@=$(LEGACY_XVMC_SUBDIR)) $(subdir_cmd) \
                $(install_subdir_cmd)
