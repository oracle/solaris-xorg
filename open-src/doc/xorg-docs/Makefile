###############################################################################
#
# Copyright (c) 2007, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xorg-docs

# Version number (used in path names)
MODULE_VERSION=1.5

# Checksums for upstream tarball
TARBALL_MD5  = 359ac83ad27eecd5588914ba8715301d
TARBALL_SHA1 = e4a0fdf848e9d5baa2de625ce74f204de37e0fb0

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	solaris-manpages.patch

# Makefiles require gmake -i to build all targets
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
MODULE_MAKEFLAGS= MAKEFLAGS="-i"

# Additional arguments to configure script
MODULE_CONFIG_OPTS=--enable-txt --disable-pdf --disable-ps --disable-html \
	--docdir='$(X11_DOC_DIR)' --with-x11docdir='$(X11_DOC_DIR)' \
	XORG_SGML_PATH=$(PROTODIR)$(X11_DIR)/share/sgml

# Module dependencies
MODULE_CONFIGURE_DEPS=$(DOCBOOK2TEXT) $(XORG_DEFS_ENT)

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

### Include common rulesets
include ../Makefile.inc

MAN_DIR=$(PROTODIR)$(X11_MAN_DIR)

install_docs:
	mkdir -p $(MAN_DIR)/man5
	cp -pf X11.5 $(MAN_DIR)/man5/X11.5
