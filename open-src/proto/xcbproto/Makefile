###############################################################################
#
# xcbproto Makefile
#
# Copyright (c) 2010, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xcb-proto

# Version number (used in path names)
MODULE_VERSION=1.6

# Checksums for upstream tarball
TARBALL_MD5  = 04313e1d914b44d0e457f6c494fc178b
TARBALL_SHA1 = e82418557c7f59f29da9ec18e0906d6d78e3a164

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = configure.0.patch \
		 Makefile.in.1.patch

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# We need Python
PYTHON_VERSION=2.6
PYTHON=/usr/bin/python$(PYTHON_VERSION)
PYTHON_PATH=$(PROTODIR)/usr/lib/python$(PYTHON_VERSION)/vendor-packages:$(PROTODIR)/usr/lib/python$(PYTHON_VERSION)/site-packages:/usr/lib/python$(PYTHON_VERSION)/site-packages:/usr/lib/python$(PYTHON_VERSION)/vendor-packages

# We want to use the PYTHON set above in our environment
MODULE_BUILD_ENV = PYTHON="$(PYTHON)" \
		   PYTHONPATH="$(PYTHON_PATH)"

### Include common rulesets
include ../Makefile.inc

