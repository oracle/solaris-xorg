###############################################################################
#
# xinput - input device management utility
#
# Copyright (c) 2009, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xinput

# Version number (used in path names)
MODULE_VERSION=1.6.2

# Checksums for upstream tarball
TARBALL_MD5   = 6a889412eff2e3c1c6bb19146f6fe84c
TARBALL_SHA1  = 68367ae6cb4753eeb5fd209afe033d7c4a817748
TARBALL_SHA256= 3694d29b4180952fbf13c6d4e59541310cbb11eef5bf888ff3d8b7f4e3aee5c4

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xinput input device management utility

# ARC cases that covered this module
# LSARC/2009/506  xinput program
MODULE_ARC_CASES = LSARC/2009/506

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xinput.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xinput

# ignore some unneeded dependencies that upstream includes:
#   unreferenced object=libXext.so.0; unused dependency of /usr/bin/xinput
MODULE_LDFLAGS = $(ZDISCARD_UNUSED_DEP)

include ../Makefile.inc
