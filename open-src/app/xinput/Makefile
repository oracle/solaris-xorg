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
MODULE_VERSION=1.5.99.901

# Checksums for upstream tarball
TARBALL_MD5   = 3b57cb4cbac70eb0d7a553def7dd333e
TARBALL_SHA1  = bc99cddeac306a23ad96f35d8ff3314d7e45d460
TARBALL_SHA256= 4765ca49af591ad5fed1af6847a16b807317212d40d5d21e5f97aa834f3e815e

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
