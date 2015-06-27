###############################################################################
#
# xgamma - monitor gamma correction setting application
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
MODULE_NAME=xgamma

# Version number (used in path names)
MODULE_VERSION=1.0.6

# Checksums for upstream tarball
TARBALL_MD5   = 90b4305157c2b966d5180e2ee61262be
TARBALL_SHA1  = af1484d0d70bc71dc9d3b7b95645881b7165c41b
TARBALL_SHA256= 0ef1c35b5c18b1b22317f455c8df13c0a471a8efad63c89c98ae3ce8c2b222d3

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/x11-server-utilities
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the xgamma monitor gamma correction setting application

# ARC cases that covered this module
# PSARC/2005/189 X client support for XFree86 extensions
MODULE_ARC_CASES = PSARC/2005/189
# PSARC/2015/221 X11 application updates
MODULE_ARC_CASES += PSARC/2015/221

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES = man/xgamma.man
MODULE_STABILITY=Volatile

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xgamma

include ../Makefile.inc
