###############################################################################
#
# SPARC graphics utilities modules (fbconfig & VTS) Makefile
#
# Copyright (c) 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=fbconfig

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=system/graphics/fbconfig
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=Drivers/Display
MODULE_PKGCLASS_SET=yes

# No upstream sources, only local sources
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=sun-src

# Haven't made sure it's 64-bit clean yet, so continue building 32-bit for now
BUILD_TYPES_SET=yes
BUILD_TYPES=32

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= # None
MODULE_STABILITY=Committed

# No configure script to run
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

# Since we don't have a configure script, pass configure flags to make
MODULE_BUILD_MAKEFLAGS=$(CONFIG_ENV)
MODULE_INSTALL_MAKEFLAGS=$(CONFIG_ENV)

include ../Makefile.inc

# Install metadata for fb-specific packages too
FB_MODULES = ast efb

EXTRA_METADATA_DIRS = $(FB_MODULES:%=$(MODULE_PKG_METADATA_DIR)-fbconfig-%)

EXTRA_ATTRDATA_FILES	= $(EXTRA_METADATA_DIRS:%=%/$(ATTRDATA_FILE_NAME))
EXTRA_LICENSE_FILES	= $(EXTRA_METADATA_DIRS:%=%/$(LICENSE_FILE_NAME))
EXTRA_METADATA_TARGETS	= $(EXTRA_METADATA_DIRS) \
                          $(EXTRA_ATTRDATA_FILES) \
                          $(EXTRA_LICENSE_FILES)

install_metadata: $(EXTRA_METADATA_TARGETS)

$(EXTRA_ATTRDATA_FILES) $(EXTRA_LICENSE_FILES): $(EXTRA_METADATA_DIRS)

$(EXTRA_METADATA_DIRS):
	mkdir -p $@

# Modules for specific frame buffers are covered only by Oracle copyright
AST_LICENSE_FILE = $(MODULE_PKG_METADATA_DIR)-fbconfig-ast/$(LICENSE_FILE_NAME)
EFB_LICENSE_FILE = $(MODULE_PKG_METADATA_DIR)-fbconfig-efb/$(LICENSE_FILE_NAME)

$(AST_LICENSE_FILE):=		LICENSE_FILE	= COPYING.ast
$(EFB_LICENSE_FILE):=		LICENSE_FILE	= COPYING.efb

$(EXTRA_LICENSE_FILES):=      	LICENSE_NAME    = $(ORACLE_LICENSE_NAME)
$(EXTRA_LICENSE_FILES):= 	LICENSE_APPEND  = $(ORACLE_LICENSE_APPEND)
