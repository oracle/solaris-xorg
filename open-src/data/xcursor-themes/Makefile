###############################################################################
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xcursor-themes

# Version number (used in path names)
MODULE_VERSION=1.0.3

# Checksums for upstream tarball
TARBALL_MD5  = ba21aad0b353f1881f5069e423a44587
TARBALL_SHA1 = d65dd5cb931bf4a9722e64b5a265eb14dcc6ec09

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Additional environment variable options to GNU autoconf configure script
MODULE_CONFIG_ENV= PATH="$(PROTODIR)$(X11_DIR)/bin:$(PATH)"

# Additional environment variable options needed when building
MODULE_BUILD_ENV = LD_LIBRARY_PATH="$(PROTODIR)$(X11_DIR)/lib" \
	PATH="$(PROTODIR)$(X11_DIR)/bin:$(PATH)"

# ARC cases that covered this module
# LSARC/2007/491 libXcursor
MODULE_ARC_CASES = LSARC/2007/491

### Include common rulesets
include ../Makefile.inc



