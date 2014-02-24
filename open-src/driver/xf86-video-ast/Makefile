###############################################################################
#
# xf86-video-ast - Xorg driver for ASpeed Technologies graphics devices 
#  (such as found in some Sun/Oracle server ILOMs)
#
# Copyright (c) 2006, 2014, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=0.97.0

# Checksums for upstream tarball
TARBALL_MD5   = 2b630ed7d7864045b324b39cd58da367
TARBALL_SHA1  = 11cb9a687cec6f0943ad6b9a47434815334d2a20
TARBALL_SHA256= 28fcd4781676485293f6dcd46e0797866f6219e22e1851c9796b037589998e76

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += sparc-ast.patch,-p1 \
		16027951.patch,-p1 \
		ast.patch,-p1 \
		ast_vgatool.patch,-p1 \
		bstore.patch \
		17907916.patch

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
AST_LIBRARIES = -lfb $(AST_LIBRARIES_$(MACH))
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR) \
	$(AST_LIBRARIES)

include ../Makefile.inc
