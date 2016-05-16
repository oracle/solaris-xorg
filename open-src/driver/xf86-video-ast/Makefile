###############################################################################
#
# xf86-video-ast - Xorg driver for ASpeed Technologies graphics devices 
#  (such as found in some Sun/Oracle server ILOMs)
#
# Copyright (c) 2006, 2016, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-ast

# Version number (used in path names)
MODULE_VERSION=1.1.5

# Checksums for upstream tarball
TARBALL_MD5   = 4f85febe48d51e53624550a96fc9e9ee
TARBALL_SHA1  = 34a610e506079f32b240335520200457aa72d4c2
TARBALL_SHA256= 1edbbc55d47d3fd71dec99b15c2483e22738c642623a0fb86ef4a81a9067a2de

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += sparc-ast.patch,-p1 \
		16027951.patch,-p1 \
		17907916.patch,-p1 \
		20816358-disable-DP501.patch,-p1 \
		22087975.patch,-p1

# Link in additional source files from sun-src directory
ADDITIONAL_SOURCE_DIR=sun-src

# Regenerate Makefile.in's from Makefile.am's after patching them
AUTORECONF=yes

# Dates to show in Oracle copyright notice in pkg license file
# Earliest & latest of the copyrights in the Oracle files in this pkg
ORACLE_COPYRIGHT_YEARS = 2009, 2012

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

# Libraries/modules to link with
AST_LIBRARIES_sparc = # nothing extra
AST_LIBRARIES_i386 = -lint10 -lpciaccess -lvbe -lvgahw
AST_LIBRARIES = -lfb -lshadow $(AST_LIBRARIES_$(MACH))
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR) \
	$(AST_LIBRARIES)

include ../Makefile.inc
