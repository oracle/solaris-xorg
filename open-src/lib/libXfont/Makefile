###############################################################################
#
# Xfont Makefile
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
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
# @(#)Makefile	1.10	07/09/06
#

# Package name used in tarballs
LIB_MODULE=libXfont

# Version number (used in path names)
LIB_VERS=1.2.8

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	6206898.patch


# Library name
LIBNAME=Xfont

# Man pages to apply Sun footer to & attributes to list
#SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-server
LIB_STABILITY=Project Private
LIB_MAN_SUFFIX=3Xfont

# Additional configure script options to select the modules we use
LIB_ADD_CONFIG_OPTS= --enable-freetype --disable-type1 --disable-cid \
	--disable-speedo --enable-fontcache --enable-fc --disable-builtins

# Library built in tree
LIB_BUILT=$(SOURCE_DIR)/src/.libs/libXfont.so.1.0.0

# pkg-config files that need to have -R added
FIX_PC_FILES=xfont.pc.in

# Install in private subdir
LIB_ADD_SUBDIR=$(X11_SERVERLIBS_SUBDIR)

# Find freetype & fontenc libraries in private subdir
LIB_ADD_LD_OPTIONS=-L $(PROTODIR)/usr/sfw/lib$(LIBSUBDIR) \
	-L $(PROTODIR)$(X11_DIR)/lib$(X11_SERVERLIBS_SUBDIR)$(LIBSUBDIR)

# Support font modules like libbitstream.so that libXfont doesn't already know
LIB_ADD_CFLAGS=-DLOADABLEFONTS

include ../Makefile.inc

