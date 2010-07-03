###############################################################################
#
# transset 1.x Makefile
#
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=transset

# Package metadata - set manually since we download from somewhere
# other than the normal X.Org site
UPSTREAM	= $(UPSTREAM_xorg)
UPSTREAM_URL	= $(UPSTREAM_URL_xorg)
UPSTREAM_SET	= yes

# Version number (used in path names)
MODULE_VERSION=0.9.1

# Checksums for upstream tarball
TARBALL_MD5  = d289dd300124c9680e9eeae83947b0cf
TARBALL_SHA1 = 6e59ffc4f7ccca4d88bf5ec1bb7700ad611720f9

# Source tarball
SOURCE_URL=http://dlc.sun.com/osol/x/downloads/mirrors/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=transset.1
MODULE_STABILITY=Volatile

include ../Makefile.inc
