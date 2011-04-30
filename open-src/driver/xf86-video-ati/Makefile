###############################################################################
#
# xf86-video-ati Makefile
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
MODULE_NAME=xf86-video-ati

# Version number (used in path names)
MODULE_VERSION=6.14.1

# Checksums for upstream tarball
TARBALL_MD5  = 395e23ad026e4246dd70bbd2143688b5
TARBALL_SHA1 = 89b8a64a23fc307646a11cd2c2636a8125fada8f

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	r6xx-r7xx-UMS.patch,-p1 \
	mapfiles.patch,-p1 \
	solaris-dri-drv-port.patch

# Need to regenerate Makefile.in after patching Makefile.am in mapfiles.patch
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# AtomBios code won't build with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Extra flags to pass to configure script
MODULE_CONFIG_OPTS=--disable-kms

# Paths to find libraries/modules to link with - libraries themselves listed
# in src/Makefile.am since ATI has multiple submodules with different needs
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-L$(PROTODIR)$(MESA_XSERVERMODS_DIR)/extensions \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)/extensions \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-M $(PWD)/mapfile-bt829_drv-external \
	-M $(PWD)/mapfile-fi1236_drv-external \
	-M $(PWD)/mapfile-msp3430_drv-external \
	-M $(PWD)/mapfile-tda9885_drv-external \
	-M $(PWD)/mapfile-uda1380_drv-external \
	-R$(X11_SERVERMODS_DIR) \
	-R$(MESA_XSERVERMODS_DIR)/extensions \
	-R$(X11_SERVERMODS_DIR)/extensions \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

include ../Makefile.inc
