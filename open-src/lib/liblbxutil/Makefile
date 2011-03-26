###############################################################################
# lbxutil 1.x Makefile
#
# Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=liblbxutil

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/network/x11-network-proxies
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=Applications/Internet
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5  = 273329a78c2e9ea189ac416c7fde94a1
TARBALL_SHA1 = 99616bab0ca0afeaa9c0a19222091b1ff4f18680

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=lbxutil

# Man pages to apply Sun footer to & attributes to list
#LIB_STABILITY=Committed
#LIB_MAN_SUFFIX=3lbxutil

### Include common definitions
PWD:sh=pwd
TOP=$(PWD)/../../..
include $(TOP)/open-src/common/Makefile.init

# Build as a static library since only lbxproxy needs it anymore
# and install it in $(TOOLS_DIR) since we're not packaging it
MODULE_CONFIG_OPTS = --enable-shared=no --enable-static=yes
MODULE_CONFIG_OPTS += --libdir='$${exec_prefix}/lib$(ARCHLIBSUBDIR)'
MODULE_CFLAGS = $(PROG_CFLAGS)
MODULE_ADD_UNINSTALLED_PC_OPS = -e 's|$$protodir|$(TOOLS_DIR)|g;'
MODULE_INSTALL_MAKEFLAGS = DESTDIR=$(TOOLS_DIR)

# Only need 64-bit since we only ship lbxproxy as 64-bit
BUILD_TYPES=64
BUILD_TYPES_SET=yes

# pkg-config files that need to have -R added
FIX_PC_FILES=lbxutil.pc.in

# Don't include lib/Makefile.inc that sets options for shared libraries
# which we don't want in this special case
include ../../common/Makefile.inc

# Since liblbxutil doesn't depend on any other libraries we build, it may
# be the first library built.   Since it installs the normal .pc file to
# $(TOOLS_DIR), have to ensure the directory exists before installing the
# lbxutil-uninstalled.pc file.
install_pkgconfig_uninstalled: $(PROTODIR)$(MODULE_PKGCONFIG_DIR)

$(PROTODIR)$(MODULE_PKGCONFIG_DIR):
	mkdir -p $@
