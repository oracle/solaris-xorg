###############################################################################
#
# xinput - input device management utility
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
MODULE_NAME=xinput

# Version number (used in path names)
MODULE_VERSION=1.6.0

# Checksums for upstream tarball
TARBALL_MD5   = d2459d35b4e0b41ded26a1d1159b7ac6
TARBALL_SHA1  = 958b77a2acf52197b9a1e3e3d11e9bc57fbb1e6c
TARBALL_SHA256= 4ab007d952c76665603bcb82ceb15fd3929d10faf0580fc4873ac16f5f63847e

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# LSARC/2009/506  xinput program
MODULE_ARC_CASES = LSARC/2009/506

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xinput

# ignore some unneeded dependencies that upstream includes:
#   unreferenced object=libXext.so.0; unused dependency of /usr/bin/xinput
MODULE_LDFLAGS = -z ignore

include ../Makefile.inc
