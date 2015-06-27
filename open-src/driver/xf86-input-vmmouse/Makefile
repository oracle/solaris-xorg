###############################################################################
#
# xf86-input-vmmouse Makefile
#
# Copyright (c) 2007, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=13.0.99

# Checksums for upstream tarball
TARBALL_MD5   = 8cd44ea15241422edbcaa7597f9c0e24
TARBALL_SHA1  = 2ff9a9d92a2ed59b3ce48197f186bcc39e29f303
TARBALL_SHA256= efd6235e4a2d325f2510fa6d90e355cf75625467270c30974f29d4fb8084f564

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# vmmouse_client code won't build with Studio compiler
MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

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
