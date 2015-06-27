###############################################################################
#
# xf86-video-cirrus 1.x Makefile
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
MODULE_NAME=xf86-video-cirrus

# Version number (used in path names)
MODULE_VERSION=1.5.2

# Checksums for upstream tarball
TARBALL_MD5   = 91fd6b677d62027cd3001debb587a6a6
TARBALL_SHA1  = 722800029e816f453031a851bf4a816c796d7bde
TARBALL_SHA256= 3361e1a65d9b84c464752fd612bdf6087622c6dd204121715366a170e5c3ccd7

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = mapfiles.patch solaris-port.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Paths to find libraries/modules to link with - libraries themselves listed
# in src/Makefile.am since cirrus has submodules with different needs
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR)

AUTORECONF=yes

include ../Makefile.inc


