###############################################################################
#
# xrdb 1.x Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
MODULE_NAME=xrdb

# Version number (used in path names)
MODULE_VERSION=1.0.6

# Checksums for upstream tarball
TARBALL_MD5  = 8dd881cd5a7dcb6e456549817f0fb475
TARBALL_SHA1 = e4faff5e02b3027298dc589e318a6dc7eb30a6d5

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed

# Specify which cpp to use instead of depending on builder's $PATH
MODULE_CONFIG_OPTS = --with-cpp=/usr/lib/cpp

MODULE_CPPFLAGS=-DPATHETICCPP

include ../Makefile.inc
