###############################################################################
#
# xf86-input-keyboard 1.x Makefile
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-input-keyboard

# Version number (used in path names)
MODULE_VERSION=1.4.0

# Checksums for upstream tarball
TARBALL_MD5  = fd17158ffeacecc8cc670604460cb98b
TARBALL_SHA1 = 5061fdaff8b7ddcbcca82f065c4abbfff255a1da

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	6886424.patch \
	6897086.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-server
MODULE_STABILITY=Volatile

# Keyboard driver is kbd_drv, not keyboard_drv as the package name implies
MODULE_MAKEFLAGS = kbd_drv_ladir='$(DRIVER_DIR)'

include ../Makefile.inc
