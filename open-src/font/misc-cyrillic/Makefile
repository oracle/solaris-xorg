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
MODULE_NAME=font-misc-cyrillic

# Package metadata
MODULE_LICENSE          = Mixed
MODULE_LICENSE_SET      = yes

# pkg(5) name we deliver the files in (override default)
MODULE_PKGTAIL=xorg/cyrillic
MODULE_PKGTAIL_SET=yes

# Version number (used in path names)
MODULE_VERSION=1.0.1

# Checksums for upstream tarball
TARBALL_MD5  = c79d7921d95b2c4f10fad464bb121090
TARBALL_SHA1 = c178f8a8b6897a8382a0f4315a5b577760ba703c

# Link to make in font catalogue dir
FONTPATH_LINKS = cyrillic:pri=75
MODULE_FONTDIR = $(X11_FONT_DIR)/cyrillic

### Include common rulesets
include ../Makefile.inc
