# Makefile for X Consolidation Open Source bits
#
# Copyright (c) 2005, 2010, Oracle and/or its affiliates. All rights reserved.
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
proto: util/util-macros util/lndir
lib: proto util
util: proto
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
	util/lndir \
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
TOP=$(PWD)/..

### Include common definitions
DIRNAME=""
include $(TOP)/open-src/common/Makefile.subdirs

