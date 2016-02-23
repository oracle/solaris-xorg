###############################################################################
#
# Copyright (c) 2006, 2016, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=mesa-demos

# Package metadata
UPSTREAM	= The Mesa 3-D Graphics Library
UPSTREAM_URL	= http://mesa3d.org/
UPSTREAM_SET	= yes
MODULE_DESC	= the Mesa 3-D Graphics demo applications
ORACLE_TPNO	= 22939

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/demo/mesa-demos
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=8.2.0

# Checksums for upstream tarball
TARBALL_MD5  = 72613a2c8c013716db02e3ff59d29061
TARBALL_SHA1 = e1a66ac63eebba4f950f95d830f34a6005167116

# Download site for source
SOURCE_URL=ftp://ftp.freedesktop.org/pub/mesa/demos/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
GIT_URL=$(FDO_GIT_URL)/mesa/demos
GIT_URL_SET=yes

# Binaries built in tree
DEMOS_DIR=$(BUILD_DIR)/src/xdemos
GLXGEARS_BIN=$(DEMOS_DIR)/glxgears
GLXINFO_BIN=$(DEMOS_DIR)/glxinfo

# Override some configure checks for libraries we don't need in the subset
# we build & ship - have to set them to be non-empty strings though or 
# configure will fail.
MODULE_CONFIG_ENV = \
	GLEW_CFLAGS="-I-have-no-GLEW-" GLEW_LIBS="-I-have-no-GLEW-"

# What to build
BUILD_TARGETS= $(GLXGEARS_BIN) $(GLXINFO_BIN) $(MODTYPE_ADD_BUILD_TARGETS)
BUILD_TARGETS_SET=yes
BUILD_TYPES_SET=yes
BUILD_TYPES=32 64

# What to install
INSTALL_TARGETS= install_demos install_man
INSTALL_TARGETS_SET=yes

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = glxinfo

### Include common rulesets
include ../Makefile.inc

### Rules specific to this directory:

# Only care about building X demos
DEMO_LDFLAGS = $(PROG_LDFLAGS) -L$(PROTODIR)$(X11_LIB_DIR)$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)
MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP)

# Flags passed on make command line when building
MODULE_BUILD_MAKEFLAGS = LDFLAGS="$(DEMO_LDFLAGS)"

DEMO_BUILD_COMMAND = $(DEFAULT_BUILD_COMMAND:@DIR@=$(DEMOS_DIR))

# Build Mesa demos
$(GLXGEARS_BIN): $(UNPACK_TARGET) $(CONFIGURE_TARGETS)
	$(DEMO_BUILD_COMMAND) glxgears

$(GLXINFO_BIN): $(UNPACK_TARGET) $(CONFIGURE_TARGETS)
	$(DEMO_BUILD_COMMAND) glxinfo

# For testing - we don't ship the rest of these
all-xdemos_gen: $(UNPACK_TARGET) $(CONFIGURE_TARGETS)
	$(DEMO_BUILD_COMMAND)

all-xdemos:
	$(MAKE) $(MAKEFLAGS) $(BUILD_64_FLAGS) all-xdemos_gen

DESTDIR=$(PROTODIR)

BINSUBDIR       = $(BINSUBDIR_$(BUILD_TYPE))
BINSUBDIR_32    = /$(SUBDIR32)
BINSUBDIR_64    = # None

install_demos: $(INSTALL_DEPS)
	mkdir -p $(DESTDIR)$(MODULE_BIN_DIR)$(BINSUBDIR)
	cp -pf $(GLXGEARS_BIN) $(GLXINFO_BIN) $(DESTDIR)$(MODULE_BIN_DIR)$(BINSUBDIR)

install_man: 
	mkdir -p $(DESTDIR)$(MODULE_MAN_DIR)/man1
	cp -pf glxgears.1 glxinfo.1 $(DESTDIR)$(MODULE_MAN_DIR)/man1


