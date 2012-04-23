###############################################################################
#
# listres - Xt toolkit resource list utility
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=listres

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/xt-resource-utilities
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.3

# Checksums for upstream tarball
TARBALL_MD5   = 4e8edcc03001203c101e79d880d55762
TARBALL_SHA1  = ff226b0e565c337ac1ae60027487131920b8d86b
TARBALL_SHA256= c3d71e9a01650e3fd470189a749cd0202ef03d9e399350a7a7443d7595ed478e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/listres.man
MODULE_STABILITY=Committed

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = listres

# pkg-config dependencies cause a link to libX11 to be brought in that we
# can ignore
MODULE_LDFLAGS = -z ignore

include ../Makefile.inc
