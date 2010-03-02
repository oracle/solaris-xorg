###############################################################################
#
# Xp 1.x Makefile
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
MODULE_NAME=libXp

# Version number (used in path names)
MODULE_VERSION=1.0.0

# Checksums for upstream tarball
TARBALL_MD5  = 0f4ac39108c1ae8c443cdfac259b58fa
TARBALL_SHA1 = fac67954e247fc805af6e495ff8695b5724cd1e2

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = solaris-abi.patch 6724993.patch

# Since we're patching src/Makefile.am in solaris-abi.patch,
# we need to autoreconf
AUTORECONF=yes


# Library name
LIBNAME=Xp

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3Xp



# pkg-config files that need to have -R added
FIX_PC_FILES=xp.pc.in

include ../Makefile.inc
