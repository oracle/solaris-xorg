###############################################################################
#
# imake - Legacy X11 build configuration and makefile generation tool
#
# Copyright (c) 2008, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=imake

# Package metadata
MODULE_DESC = the imake makefile generation tool

# Version number (used in path names)
MODULE_VERSION=1.0.6

# Checksums for upstream tarball
TARBALL_MD5   = d5058b1317a85bedc1dc40c7e85c8d26
TARBALL_SHA1  = a54c025d7ac9894b6bc919d13454c6adb12ae140
TARBALL_SHA256= fa8983fdb0a72ba8482d1d2683967fb45c29edd7149b25f619f97ea5a599069d

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
MODULE_STABILITY=Uncommitted

# Build 32-bit so that the default flags it sets in generated Makefiles are 
# 32-bit, requiring users to request 64-bit flags when they want them.
BUILD_TYPES_SET=yes
BUILD_TYPES=32

# Additional flags to pass to configure script
MODULE_CONFIG_OPTS  = --enable-mkdirhier \
 --disable-ccmakedep --disable-cleanlinks --disable-makeg \
 --disable-mergelib --disable-mkhtmlindex --disable-revpath

# Prefer cpp from /usr/lib
MODULE_CONFIG_OPTS += RAWCPP=/usr/lib/cpp

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = imake xmkmf mkdirhier

# Workaround Parfait 0.5.0.1 bug 14139420
MODULE_BUILD_ENV += PARFAIT_CLANG=0

### Include common rulesets
include ../Makefile.inc

