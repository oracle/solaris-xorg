###############################################################################
#
# Xfont Makefile
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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
# ident	"@(#)Makefile	1.20	09/12/05 SMI"
#

# Package name used in tarballs
MODULE_NAME=libXfont

# Version number (used in path names)
MODULE_VERSION=1.4.1

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	ps-fonts.patch \
	htons-inline.patch

# Library name
LIBNAME=Xfont

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-server
MODULE_STABILITY=Project Private
LIB_MAN_SUFFIX=3Xfont

# Additional configure script options to select the modules we use
MODULE_CONFIG_OPTS= --enable-freetype --disable-type1 --disable-cid \
	--disable-speedo --enable-fontcache --enable-fc --enable-builtins \
	--with-bzip2

# pkg-config files that need to have -R added
FIX_PC_FILES=xfont.pc.in

# Install in private subdir
MODULE_ADD_LIB_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

# Need to link with libfontenc in server private directory
MODULE_LDFLAGS= -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# Need to find libfontenc & libfreetype in matching directory when used
# in alternate root environments, like during OS install
# We use sfw/lib to find libfreetype (via the compatibility link) and not
# libc, which may be incompatible with the running system during Live Upgrade
MODULE_LD_OPTIONS= -R $$ORIGIN -R $$ORIGIN/$(FREETYPE_RELATIVE_PATH)
MODULE_BUILD_32_FLAGS= FREETYPE_RELATIVE_PATH=../../../../sfw/lib
MODULE_BUILD_64_FLAGS= FREETYPE_RELATIVE_PATH=../../../../../sfw/lib$(ARCHLIBSUBDIR)

# Link with version mapfile
MODULE_LD_OPTIONS += -M $(PWD)/mapfile-vers

include ../Makefile.inc

