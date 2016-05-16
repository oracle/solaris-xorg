###############################################################################
#
# xvinfo - utility to display X-Video extension adaptor information
#
# Copyright (c) 2006, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xvinfo

# Version number (used in path names)
MODULE_VERSION=1.1.3

# Checksums for upstream tarball
TARBALL_MD5  = 558360176b718dee3c39bc0648c0d10c 
TARBALL_SHA1 = 08ff744f7735317fd2fe1b0d2feba8c34cc2e5eb
TARBALL_SHA256 = 9fba8b68daf53863e66d5004fa9c703fcecf69db4d151ea2d3d885d621e6e5eb

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Package metadata
MODULE_DESC = the xvinfo utility to display X-Video extension adaptor information

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/diagnostic/x11-info-clients
MODULE_PKGNAME_SET=yes

# ARC cases that covered this module
# PSARC/2005/189 X client support for XFree86 extensions
MODULE_ARC_CASES = PSARC/2005/189

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xvinfo

include ../Makefile.inc
