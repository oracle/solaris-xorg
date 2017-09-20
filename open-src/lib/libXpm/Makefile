###############################################################################
#
# libXpm - library for handling XPM format images
#
# Copyright (c) 2006, 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libXpm

# Version number (used in path names)
MODULE_VERSION=3.5.12

# Checksums for upstream tarball
TARBALL_MD5   = 20f4627672edb2bd06a749f11aa97302
TARBALL_SHA1  = 4e22fefe61714209539b08051b5287bcd9ecfd04
TARBALL_SHA256= fd6a6de3da48de8d1bb738ab6be4ad67f7cb0986c39bd3f7d51dd24f7854bdec

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = setuid.patch

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2001/060

# Library name
LIBNAME=Xpm

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/cxpm.man man/sxpm.man man/libXpm.3lib
MODULE_STABILITY=Uncommitted
MODULE_MTLEVEL=Unsafe
MODULE_MTLEVEL_SET=yes
LIB_MAN_SUFFIX=3
# Don't want to add MT-Level & -l flags to cxpm & sxpm command man pages
OVERRIDE_LIB_SUNTOUCH_DEFAULT=yes
# But do want to add them to the libXpm man page
MODULE_SUNTOUCH_MAN_FLAGS= $(SUNTOUCH_MAN_FLAGS_$(@F:%.$(FIXED_MAN_SUFFIX)=%))
SUNTOUCH_MAN_FLAGS_libXpm.3lib = $(LIB_SUNTOUCH_DEFAULTS)

# Link shared library with version mapfile
MODULE_LD_SHARED_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xpm.pc.in

# Additional arguments to pass to make
# - in 32-bit builds, only install library, not apps or headers
# - install 64-bit apps in default directory, not 64-bit subdir
MODULE_INSTALL_MAKEFLAGS_32 = -e SUBDIRS="src"
MODULE_INSTALL_MAKEFLAGS_64 = bindir=$(MODULE_BIN_DIR)
MODULE_INSTALL_MAKEFLAGS = $(MODULE_INSTALL_MAKEFLAGS_$(BUILD_TYPE))

# Install docs & l10n files as well
MODULE_ADD_INSTALL_TARGETS_64 = install-docs install-po
MODULE_ADD_INSTALL_TARGETS = $(MODULE_ADD_INSTALL_TARGETS_$(BUILD_TYPE))

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = cxpm sxpm

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXpm.so libXpm.so.4

include ../Makefile.inc

3LIB_MAN_DIR = $(MODULE_MAN_DIR)/man3lib

install-docs: $(PROTODIR)$(MODULE_DOC_DIR)/xpm.ps
install-docs: $(PROTODIR)$(3LIB_MAN_DIR)/libXpm.3lib

$(PROTODIR)$(MODULE_DOC_DIR)/xpm.ps: $(INSTALL_DEPS) $(PROTODIR)$(MODULE_DOC_DIR)
	-rm -f $@
	gzcat $(SOURCE_DIR)/doc/xpm.PS.gz > $@

$(PROTODIR)$(3LIB_MAN_DIR)/libXpm.3lib: $(INSTALL_DEPS) $(PROTODIR)$(3LIB_MAN_DIR)
	ginstall -m 0444 $(SOURCE_DIR)/man/libXpm.3lib $@

PO_FILES = $(BUILD_DIR)/cxpm/cxpm.po $(BUILD_DIR)/sxpm/sxpm.po

install-po: $(INSTALL_DEPS) $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)
	ginstall -m 0444 $(PO_FILES) $(PROTODIR)$(MODULE_LC_MESSAGES_DIR)

$(PROTODIR)$(3LIB_MAN_DIR) $(PROTODIR)$(MODULE_LC_MESSAGES_DIR):
	mkdir -p $@
