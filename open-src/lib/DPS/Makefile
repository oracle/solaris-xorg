###############################################################################
#
# DPS Makefile
#
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
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

BUILD_TYPES_SET=yes
BUILD_TYPES=32

MODULE_NAME=DPS

# Sources were extracted from X11R6.9 source tree, but we don't want
# to download & unpack all that, and upstream isn't maintaining, so
# we just have a local copy here.
MODULE_VERSION=X11R6.9
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=sun-src

# Library name (used for specfiles/mapfiles)
LIBNAME=dps

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES= # None
MODULE_STABILITY=Obsolete

# No configure script to run
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

# Since we don't have a configure script, pass configure flags to make
MODULE_MAKEFLAGS=$(CONFIG_ENV) PREFIX=$(MODULE_PREFIX) \
	datadir=$(MODULE_PREFIX)/share bindir=$(X11_DIR)/bin \
	libdir=$(MODULE_PREFIX)/lib$(MODULE_ADD_LIB_SUBDIR)$(ARCHLIBSUBDIR)

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = makepsres pswrap

include ../Makefile.inc

# Install metadata for dps-motif package too
MOTIF_PKG_METADATA_DIR	= $(MODULE_PKG_METADATA_DIR:%dps/=%dps-motif/)
MOTIF_PKG_ATTRDATA_FILE	= $(MOTIF_PKG_METADATA_DIR)/$(ATTRDATA_FILE_NAME)
MOTIF_PKG_LICENSE_FILE	= $(MOTIF_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME)
EXTRA_METADATA_TARGETS = \
	$(MOTIF_PKG_METADATA_DIR) \
	$(MOTIF_PKG_ATTRDATA_FILE) \
	$(MOTIF_PKG_LICENSE_FILE)

install_metadata: $(EXTRA_METADATA_TARGETS)

$(MOTIF_PKG_METADATA_DIR):
	mkdir -p $@

$(MOTIF_PKG_ATTRDATA_FILE) $(MOTIF_PKG_LICENSE_FILE): $(MOTIF_PKG_METADATA_DIR)
