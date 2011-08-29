# Makefile for X Consolidation Open Source bits
#
# Copyright (c) 2005, 2011, Oracle and/or its affiliates. All rights reserved.
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
proto: util/util-macros util/lndir util/build-tools
util/lndir: proto/x11proto
lib: proto font/util util
util: proto font/util
app: lib data/xbitmaps font/util
xserver/xorg xserver/xvnc: proto doc/xorg-sgml-doctools lib font/util
driver: xserver/xorg
app/rgb: xserver/xorg
doc/xorg-docs: doc/xorg-sgml-doctools
data: app
FONTAPPS = app/bdftopcf app/mkfontscale app/mkfontdir
font: $(FONTAPPS) font/util
$(FONTAPPS): lib

OS_SUBDIRS_common = \
	util/util-macros \
	util/build-tools \
	proto/x11proto \
	util/lndir \
	proto \
	data/xbitmaps \
	font/util \
	util \
	doc/xorg-sgml-doctools \
	doc/xorg-docs \
	lib \
	app \
	$(FONTAPPS) \
	xserver/xorg \
	xserver/xvnc \
	xserver/xserver-common \
	driver \
	app/rgb \
	data \
	font

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)
OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

# Subdirs we used to build, but now just need to clean up
OBSOLETE_SUBDIRS = \
	doc/docbook-utils

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME=""
include $(TOP)/open-src/common/Makefile.subdirs

PROTOTOP=$(TOP)/proto
PROTODIR=$(PROTOTOP)/root_$(MACH)

check: check_rtime

# check_rtime script from onbld to check binaries are built with the right
# linker flags
check_rtime:
	check_rtime -e ../exception_lists/check_rtime -d $(PROTODIR) $(PROTODIR)
