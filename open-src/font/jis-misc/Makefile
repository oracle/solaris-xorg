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
MODULE_NAME=font-jis-misc

# Package metadata
MODULE_LICENSE          = Japanese Industrial Standard
MODULE_LICENSE_SET      = yes

# Version number (used in path names)
MODULE_VERSION=1.0.1

# Checksums for upstream tarball
TARBALL_MD5  = 5835c99d056f4a29d7d6a5ae873cf00e
TARBALL_SHA1 = adb3275d19e71e9553aa8a5fdc3b2c647277d8d4

# Install to separate directory for easier packaging
MODULE_FONTDIR = $(X11_FONT_DIR)/jiskan
FONTPATH_LINKS = jiskan:unscaled:pri=14 jiskan:pri=84

### Include common rulesets
include ../Makefile.inc
