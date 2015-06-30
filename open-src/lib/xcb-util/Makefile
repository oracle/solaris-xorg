###############################################################################
#
# xcb-util - Library of xcb utility functions
#
# Copyright (c) 2010, 2015, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xcb-util

# Version number (used in path names)
MODULE_VERSION=0.4.0

# Checksums for upstream tarball
TARBALL_MD5  = 2e97feed81919465a04ccc71e4073313
TARBALL_SHA1 = 7f2e9b7efcc2c34eb1d6ae312c3d73b075832e46
TARBALL_SHA256 = 46e49469cb3b594af1d33176cd7565def2be3fa8be4371d62271fabb5eae50e9

# ARC cases that covered this module
MODULE_ARC_CASES = PSARC/2010/109

# Package metadata
MODULE_DESC = the xcb-util library of xcb utility functions
UPSTREAM_PROJECT = xcb
UPSTREAM_PROJECT_SET = yes
ORACLE_TPNO = 22888
ORACLE_TPNO_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name (used for specfiles/mapfiles)
LIBNAME=xcbutil

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted

# pkg-config files to update
FIX_PC_FILES=xcb-atom.pc.in xcb-aux.pc.in xcb-event.pc.in xcb-util.pc.in 

# Need to use GNU Make to build, due to the use of $< in explicit rules
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

include ../Makefile.inc
