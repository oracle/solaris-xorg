###############################################################################
#
# Xext 1.x Makefile
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
MODULE_NAME=libXext

# Version number (used in path names)
MODULE_VERSION=1.1.1

# Checksums for upstream tarball
TARBALL_MD5  = c417c0e8df39a067f90a2a2e7133637d
TARBALL_SHA1 = c2eb8d8d760c6881e51c938812764d1d6cefd51c

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	server-compat.patch,-p1 \
	lint-flags.patch,-p1 \
	shm.patch \
	sun-exts.patch

# Library name (used for specfiles/mapfiles)
LIBNAME=Xext

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Committed
LIB_MAN_SUFFIX=3Xext

# pkg-config files that need to have -R added
FIX_PC_FILES=xext.pc.in

# Enable Solaris-specific code
MODULE_CPPFLAGS = -DSUNSOFT

# Link with version mapfile and libraries that contain sources that used
# to be in libXext
MODULE_LD_OPTIONS = -M $(PWD)/mapfile-vers

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Merge in additional sources from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

include ../Makefile.inc


