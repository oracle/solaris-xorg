###############################################################################
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=font-ibm-type1

# Package metadata
MODULE_LICENSE          = IBM/MIT X Consortium Courier Typefont agreement
MODULE_LICENSE_SET      = yes


# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=system/font/xorg/xorg-core
MODULE_PKGNAME_SET=yes

# Version number (used in path names)
MODULE_VERSION = 1.0.3

# Checksums for upstream tarball
TARBALL_MD5  = bfb2593d2102585f45daa960f43cb3c4
TARBALL_SHA1 = bfc7d0a3aa0f96bf61ea26d6b3f5afbbdd0f35f6

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=

# Extra configure flags
MODULE_CONFIG_OPTS=

# Link to make in font catalogue dir
FONTPATH_LINKS = Type1:pri=45
MODULE_FONTDIR = $(X11_FONT_DIR)/Type1

### Include common rulesets
include ../Makefile.inc
