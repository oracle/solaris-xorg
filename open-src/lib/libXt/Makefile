###############################################################################
#
# Xt 1.x Makefile
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
MODULE_NAME=libXt

# Version number (used in path names)
MODULE_VERSION=1.0.8

# Checksums for upstream tarball
TARBALL_MD5  = fb7d2aa5b24cd5fe9b238a26d88030e7
TARBALL_SHA1 = d5e3dfba90a12169771399b3e2ccae07243489c9

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 6671721.patch

# Regenerate configure script & Makefile.in's after applying patches
AUTORECONF=yes

# Work around libtool's interference
DELIBTOOLIZE=yes

# Library name (used for specfiles/mapfiles)
LIBNAME=Xt

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3Xt



# pkg-config files that need to have -R added
FIX_PC_FILES=xt.pc.in

# the xinline option is here to fix bug 6748586, No Hands lib
# needs to interpose on these functions, and if they are inline
# then the loader can't find them.  It complains silently.
MODULE_CFLAGS = -DGETVALUES_BUG -xinline=no%_XtAppCreateShell,no%XtAppNextEvent,no%XtAppProcessEvent

include ../Makefile.inc
