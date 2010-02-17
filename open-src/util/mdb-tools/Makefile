###############################################################################
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

MODULE_NAME=mdb-tools

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=src

# We build per-Xserver type, not per-bitsize
BUILD_TYPES=Xorg
BUILD_TYPES_SET=yes

# No configure script
CONFIGURE_TARGETS=
CONFIGURE_TARGETS_SET=yes

# Override default build target, since set-elf-comments changes binaries
# after packaging and causes checksum errors
BUILD_TARGETS=default_build
BUILD_TARGETS_SET=yes

# No install steps, since the package is built in the tree
INSTALL_TARGETS=
INSTALL_TARGETS_SET=yes

# Without configure, we pass default flags via make instead
MODULE_MAKEFLAGS= \
	CC="$(CC)" \
	CFLAGS="$(PROG_CFLAGS)" \
	CPPFLAGS="$(DEFAULT_CONFIG_CPPFLAGS)" \
	ARCH32_FLAGS="$(ARCH32_FLAGS)" ARCH64_FLAGS="$(ARCH64_FLAGS)" \
	PLATDIR_32="$(MACH32)" PLATDIR_64="$(MACH64)" \
	PROTODIR="$(PROTODIR)" \
	TOP="$(TOP)" \
	XSERVER="$(XSERVER)" Xserver="$(Xserver)"

### Include common rulesets
include ../Makefile.inc

XORG_MAKEFLAGS = XSERVER=XORG Xserver=Xorg BUILD_DIR=build_Xorg

source_Xorg::
	$(MAKE) $(MAKEFLAGS) $(XORG_MAKEFLAGS) source_gen

build_Xorg:: source_Xorg
	$(MAKE) $(MAKEFLAGS) $(XORG_MAKEFLAGS) build_gen

clean_Xorg:
	-rm -rf build_Xorg
