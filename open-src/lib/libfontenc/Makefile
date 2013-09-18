###############################################################################
#
# libfontenc - font encoding table parser & mapping library
#
# Copyright (c) 2006, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=libfontenc

# Package metadata
MODULE_DESC = the libfontenc font encoding table parser & mapping library

# Version number (used in path names)
MODULE_VERSION=1.1.2

# Checksums for upstream tarball
TARBALL_MD5   = ad2919764933e075bb0361ad5caa3d19
TARBALL_SHA1  = 2beffa0e9260c479b49f90f789318c7f9db2d41d
TARBALL_SHA256= a9a4efed3359b2e80161bb66b65038fac145137fa134e71335264cbc23b02f62

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Library name
LIBNAME=fontenc

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Project Private
LIB_MAN_SUFFIX=3fontenc

# pkg-config files that need to have -R added
FIX_PC_FILES=fontenc.pc.in

# Install in private subdir
MODULE_ADD_LIB_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

include ../Makefile.inc
