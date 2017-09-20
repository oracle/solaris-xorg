###############################################################################
#
# xf86-video-vmware driver for guests inside VMWare hypervisors
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
MODULE_NAME=xf86-video-vmware

# Version number (used in path names)
MODULE_VERSION=13.2.1

# Checksums for upstream tarball
TARBALL_MD5   = 4c3912e4d8947f6c2fc1ee9e2f211d74
TARBALL_SHA1  = eb4909a61bb45e1c88ea66f968bc79e55b1239c4
TARBALL_SHA256= e2f7f7101fba7f53b268e7a25908babbf155b3984fb5268b3d244eb6c11bf62b

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = solaris-port.patch

AUTORECONF = yes

# Cos of 23185711 - disable parfait temporarily
USE_PARFAIT=no

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# vmwarevideo code won't build with Sun compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR) \
	-lfb -lpixman-1 -lshadowfb -lvgahw -lpciaccess

include ../Makefile.inc
