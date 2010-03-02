###############################################################################
#
# xf86-input-vmmouse Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
MODULE_NAME=xf86-input-vmmouse

# Version number (used in path names)
MODULE_VERSION=12.6.5

# Checksums for upstream tarball
TARBALL_MD5  = ce78e0135b5e34338a1ea98afb933120
TARBALL_SHA1 = bf2f1031ccc0096f388272ce93b75572d24548be

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = xf86LoaderReqSymLists.patch,-p1

# vmmouse_client code won't build with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-server
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - need to set install locations for HAL config file & probe util/script
MODULE_CONFIG_OPTS= \
	--with-hal-bin-dir=/usr/sbin \
	--with-hal-callouts-dir=/usr/lib/hal$(ARCHLIBSUBDIR) \
	--with-hal-fdi-dir=/etc/hal/fdi/policy/10osvendor$(ARCHLIBSUBDIR) \
	--bindir=$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Libraries/modules to link with
MODULE_LD_OPTIONS= mouse_drv.so
MODULE_ADD_SOURCE_TARGETS= source_mouse_drv_links

include ../Makefile.inc

source_mouse_drv_links: default_source
	ln -s $(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)/input/mouse_drv.so $(SOURCE_DIR)/src/mouse_drv.so
	ln -s $(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)/input/mouse_drv.so $(SOURCE_DIR)/tools/mouse_drv.so

