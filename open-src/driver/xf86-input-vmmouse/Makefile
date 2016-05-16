###############################################################################
#
# xf86-input-vmmouse Makefile
#
# Copyright (c) 2007, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=13.1.0

# Checksums for upstream tarball
TARBALL_MD5   = 85e2e464b7219c495ad3a16465c226ed
TARBALL_SHA1  = c00fbf985b22a86901fa905a8037005bca96d805
TARBALL_SHA256= 0af558957ac1be1b2863712c2475de8f4d7f14921fd01ded2e2fde4921b19319

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# vmmouse_client code won't build with Studio compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Cos of 23185711 - disable parfait temporarily
USE_PARFAIT=no

# Need to use GNU Make to build due to Solaris make bug 15352407
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - need to set install locations for HAL config file & probe util/script
MODULE_CONFIG_OPTS= \
	--with-hal-bin-dir=/usr/sbin \
	--with-hal-callouts-dir=/usr/lib/hal \
	--with-hal-fdi-dir=/etc/hal/fdi/policy/10osvendor \
	--bindir=$(X11_SERVERLIBS_DIR)

include ../Makefile.inc
