###############################################################################
#
# pixman Makefile
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
# 
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#

# Package name used in tarballs
MODULE_NAME=pixman

# Version number (used in path names)
MODULE_VERSION=0.16.4

# Checksums for upstream tarball
TARBALL_MD5  = 70c249a575ed02b7a5daa1bc9f9c6a4e
TARBALL_SHA1 = 08fecbd065fa3792823818fcf715b97bc66d94cd

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Override default CFLAGS for x86 builds to avoid clashes between
# -xtarget=pentium and the MMX/SSE flags added by configure for specific
# files in pixman and to work around assertions in Studio 12 when the 
# -xspace flag is used (Studio bug #6825222)
include ../../common/Makefile.init
NO_TARGET_CFLAGS = $(LIB_CFLAGS:-xtarget%=-xchip%)
NO_XSPACE_CFLAGS = $(NO_TARGET_CFLAGS:-xspace=)
MODULE_CONFIG_ENV_i386 = CFLAGS="$(NO_XSPACE_CFLAGS)"
MODULE_CONFIG_ENV_sparc = # not needed
MODULE_CONFIG_ENV = $(MODULE_CONFIG_ENV_$(MACH))

# Library name
LIBNAME=pixman

# Where to install pixman files
MODULE_PREFIX=/usr
MODULE_PREFIX_SET=yes

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWpixman
MODULE_STABILITY=Volatile
LIB_MAN_SUFFIX=3pixman

# pkg-config files that need to have -R added
FIX_PC_FILES=pixman-1.pc.in

include ../Makefile.inc
