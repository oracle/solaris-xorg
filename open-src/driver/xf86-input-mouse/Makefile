###############################################################################
#
# xf86-input-mouse 1.x Makefile
#
# Copyright (c) 2006, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-input-mouse

# Version number (used in path names)
MODULE_VERSION=1.9.1

# Checksums for upstream tarball
TARBALL_MD5    = 77085b649c5c0b333565ba562f573951
TARBALL_SHA1   = edde3e0201ec18fbbbcf3c157f912dbae3b5a3cd
TARBALL_SHA256 = 3485d375779c08406f0789feedde15933dc703158a086ddac638598f479fc5ce

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES  = \
	6892799.patch,-p1 \
	15798251.patch,-p1 \
	17429216.patch,-p1

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Libraries/modules to link with
MODULE_LD_OPTIONS= -ldevinfo -lm

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

# Install xorg-mouse.pc to /usr/share
MODULE_PKGCONFIG_DIR=$(PKGCONFIG_DIR_SHARE)
MODULE_PKGCONFIG_DIR_SET=yes

MODULE_INSTALL_MAKEFLAGS = sdkdir='$(XORG_SDK_INCLUDES_DIR)'

include ../Makefile.inc

install_docs: $(PROTODIR)$(X11_DOC_DIR)/README.mouse

$(SOURCE_DIR)/README: $(INSTALL_DEPS)

$(PROTODIR)$(X11_DOC_DIR):
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)

$(PROTODIR)$(X11_DOC_DIR)/README.mouse: $(PROTODIR)$(X11_DOC_DIR) $(SOURCE_DIR)/README
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/README $@
