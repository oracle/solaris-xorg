###############################################################################
#
# libXpm - library for handling XPM format images
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
MODULE_NAME=libXpm

# Version number (used in path names)
MODULE_VERSION=3.5.10

# Checksums for upstream tarball
TARBALL_MD5   = 7ae7eff7a14d411e84a67bd166bcec1a
TARBALL_SHA1  = eca9dbfa6253f7d6cc7d593dc83110c6c4876d7c
TARBALL_SHA256= a6db7e234750e7d60330017972e31d8e1f29f0a8c1391e4ac82f6102d919a735

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = setuid.patch

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2001/060

# Library name
LIBNAME=Xpm

# Library man pages to apply Sun footer to & attributes to list
# SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=3

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xpm.pc.in

# Additional arguments to pass to make
MODULE_MAKEFLAGS= -e SUBDIRS="src include"

# Install documentation as well
MODULE_ADD_INSTALL_TARGETS = install-docs

# Compatibility links from /usr/X11/lib to /usr/lib
MODULE_X11_LIBCOMPAT_LINKS = libXpm.so libXpm.so.4

include ../Makefile.inc

install-docs: $(PROTODIR)$(MODULE_DOC_DIR)/xpm.ps

$(PROTODIR)$(MODULE_DOC_DIR)/xpm.ps: $(INSTALL_DEPS) $(PROTODIR)$(MODULE_DOC_DIR)
	-rm -f $@
	gzcat $(SOURCE_DIR)/doc/xpm.PS.gz > $@
