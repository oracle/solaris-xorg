###############################################################################
#
# Xfixes 1.x Makefile
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
MODULE_NAME=libXfixes

# Version number (used in path names)
MODULE_VERSION=4.0.4

# Checksums for upstream tarball
TARBALL_MD5  = 7f2c40852eb337b237ad944ca5c30d49
TARBALL_SHA1 = 3f2d1c473855ba0cf13137a80d585df7fe37111c

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = soversion.patch

# Need to autoreconf since soversion.patch changes Makefile.am's
AUTORECONF=yes

# Library name
LIBNAME=Xfixes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3

# Link with version mapfile
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xfixes.pc.in

include ../Makefile.inc
