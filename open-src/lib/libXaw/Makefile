###############################################################################
#
# Xaw Makefile
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.2	08/03/19
#

## For now, this libXaw is installed to a private directory for the use
## of xorgcfg only.   Further evaluation is needed of the differences 
## between the current Solaris libXaw & this one before it can become the
## default/public libXaw.

# Package name used in tarballs
LIB_MODULE=libXaw

# Version number (used in path names)
LIB_VERS=1.0.4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=

# Library name
LIBNAME=Xaw

# Library man pages to apply Sun footer to & attributes to list
# SUNTOUCHED_MANPAGES=
SUN_PACKAGE=SUNWxwplt
LIB_STABILITY=Uncommitted
LIB_MAN_SUFFIX=3

# Library built in tree
LIB_BUILT=$(SOURCE_DIR)/src/.libs/libXaw.so.8.0.0

# Link with version mapfile
# LIB_ADD_LD_OPTIONS = -M $(PWD)/mapfile-vers

# pkg-config files that need to have -R added
FIX_PC_FILES=xaw*.pc.in

# Additional arguments to pass to configure
LIB_ADD_CONFIG_OPTS= --disable-xaw6 --disable-xaw8

# Install in private subdir for xorgcfg
LIB_ADD_SUBDIR=/xorgcfg

include ../Makefile.inc

