###############################################################################
#
# Copyright (c) 2008, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=font-util

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/font-utilities
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=System/X11
MODULE_PKGCLASS_SET=yes

# Version number (used in path names)
MODULE_VERSION = 1.3.0

# Checksums for upstream tarball
TARBALL_MD5   = ddfc8a89d597651408369d940d03d06b
TARBALL_SHA1  = a015f4c5a35d98af72835797293697e0eb9cd506
TARBALL_SHA256= dfa9e55625a4e0250f32fabab1fd5c8ffcd2d1ff2720d6fcf0f74bc8a5929195

# Set encoding directory path used during build in fontutil.pc
FIX_PC_FILES = fontutil.pc.in
MODULE_ADD_UNINSTALLED_PC_OPS = -e 's|mapdir=|mapdir=$$protodir|g;'

# Install fontutil.pc to /usr/share since it defines the architecture
# independent font paths in /usr/share/fonts and not any library flags
MODULE_PKGCONFIG_DIR=$(PKGCONFIG_DIR_SHARE)
MODULE_PKGCONFIG_DIR_SET=yes

# Apply program flags, like app/*
MODULE_CFLAGS=$(PROG_CFLAGS)
MODULE_LDFLAGS=$(PROG_LDFLAGS)
MODULE_ADD_BUILD_TARGETS=set-elf-comments

### Include common rulesets
include ../Makefile.inc
