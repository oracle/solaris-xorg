###############################################################################
#
# xf86-input-mouse 1.x Makefile
#
# Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.7.0

# Checksums for upstream tarball
TARBALL_MD5  = 7f31472689c15b6de62eff04d0fb57d7
TARBALL_SHA1 = 8b99054e5e4feafd7e08c87b936e64386b74ae8f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	Fix-OSMouse.patch,-p1 \
	6892799.patch,-p1

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Libraries/modules to link with
MODULE_LD_OPTIONS= -lm

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

include ../Makefile.inc

install_docs: $(PROTODIR)$(X11_DOC_DIR)/README.mouse

$(SOURCE_DIR)/README: $(INSTALL_DEPS)

$(PROTODIR)$(X11_DOC_DIR):
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)

$(PROTODIR)$(X11_DOC_DIR)/README.mouse: $(PROTODIR)$(X11_DOC_DIR) $(SOURCE_DIR)/README
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/README $@
