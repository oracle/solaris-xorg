###############################################################################
#
# xf86-video-mach64 Makefile
#
# Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-mach64

# Version number (used in path names)
MODULE_VERSION=6.9.4

# Checksums for upstream tarball
TARBALL_MD5   = d645197cbf238ac0427c3904eafdce2f
TARBALL_SHA1  = 59ef55e9962d6ff2d9f4583d95fdffc37cbcde86
TARBALL_SHA256= 97f108b8d7c0a8426bef9562683323ab6369956f18b0e220ee0d1a66b740855e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 6205248.patch,-p1

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - Disable DRI since the kernel module isn't ported
MODULE_CONFIG_OPTS=--disable-dri

# Paths to find libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR) \
	-lexa -lint10 -lpciaccess -lfb -lpixman-1 -lshadowfb -lvbe

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

include ../Makefile.inc

install_docs: $(INSTALL_DEPS)
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	cp -pf $(SOURCE_DIR)/README $(PROTODIR)$(X11_DOC_DIR)/README.ati
