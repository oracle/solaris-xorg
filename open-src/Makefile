# Makefile for X Consolidation Open Source bits
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
# ident	"@(#)Makefile	1.25	09/12/05 SMI"
#
###############################################################################
#

# Need to install to proto area by default for inter-package dependencies to
# work right.
default_target: install

# Modules have to be built in a specific order for dependencies to be
# handled properly

# Some specific notes:
# - app/rgb depends on the rgb.h header installed by the Xorg xserver, 
#   so has to be built after xserver/xorg
# - util/util-macros must be first, since any module that does autoreconf
#   requires it
# - data/xbitmaps must be before app, as some applications depend on the
#   bitmaps installed, but the rest of data depends on applications like
#   xcursorgen for processing, so must come later
# - font depends on utils like mkfontdir from app

# High level dependencies
proto: util/util-macros
lib: proto
app: lib data/xbitmaps
xserver/xorg xserver/xvnc: proto doc lib
driver: xserver/xorg
app/rgb: xserver/xorg
data: app
FONTAPPS = app/bdftopcf app/mkfontscale app/mkfontdir
font: $(FONTAPPS)
$(FONTAPPS): lib

OS_SUBDIRS_common = \
	util/util-macros \
	proto \
	data/xbitmaps \
	util \
	doc \
	lib \
	app \
	$(FONTAPPS) \
	xserver/xorg \
	xserver/xvnc \
	driver \
	app/rgb \
	data \
	font

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)
OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

PWD:sh=pwd
TOP=$(PWD)

### Include common definitions
DIRNAME=""
include $(TOP)/common/Makefile.subdirs

