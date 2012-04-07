###############################################################################
#
# xf86-video-r128 Makefile
#
# Copyright (c) 2009, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-r128

# Version number (used in path names)
MODULE_VERSION=6.8.2

# Checksums for upstream tarball
TARBALL_MD5   = 31e0358b5d1189e3f30f5b11e9b86865
TARBALL_SHA1  = bb01cbdc173dbb53070c4ac06f3eeb3ad2645164
TARBALL_SHA256= 7b41135c3d177e61644573e5c16e35fd9eff98b4d6685c9c6de9e33401e904b1

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - Disable DRI since the kernel module isn't ported
MODULE_CONFIG_OPTS=--disable-dri

# Paths to find libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR)/extensions \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-lint10 -lpciaccess -lfb -lpixman-1 -lvbe -lxaa -lfbdevhw -lvgahw

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

include ../Makefile.inc

install_docs: $(INSTALL_DEPS)
	$(INSTALL_SCRIPT) -m 0444 $(SOURCE_DIR)/README \
		$(PROTODIR)$(X11_DOC_DIR)/README.r128
