# Makefile for X Consolidation Open Source Protocol modules
#
# Copyright (c) 2006, 2009, Oracle and/or its affiliates. All rights reserved.
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

OS_SUBDIRS_common = 		\
	x11proto 		\
	bigreqsproto 		\
	compositeproto 		\
	damageproto 		\
	dri2proto		\
	evieproto 		\
	fixesproto 		\
	fontcacheproto 		\
	fontsproto 		\
	glproto 		\
	inputproto 		\
	kbproto 		\
	pmproto 		\
	printproto 		\
	randrproto 		\
	recordproto 		\
	renderproto 		\
	resourceproto 		\
	scrnsaverproto 		\
	videoproto 		\
	xcmiscproto 		\
	xextproto 		\
	xf86bigfontproto 	\
	xf86dgaproto 		\
	xf86driproto 		\
	xf86miscproto 		\
	xf86vidmodeproto 	\
	xineramaproto 		\
	sun-ext-protos

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

OBSOLETE_SUBDIRS = \
	lg3dproto \
	trapproto

# Need to install to proto area by default for inter-package dependencies to
# work right.
default_target: install

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME="proto/"
include $(TOP)/common/Makefile.subdirs
