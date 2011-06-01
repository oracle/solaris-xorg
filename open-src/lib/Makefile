# Makefile for X Consolidation Open Source Library modules
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Need to install to proto area by default for inter-package dependencies to
# work right.
default_target: install

# Order copied from X.Org's util/modular/build.sh:
# the libraries have a dependency order:
# xtrans, Xau, Xdmcp before anything else
# fontenc before Xfont
# ICE before SM
# X11 before Xext
# (X11 and SM) before Xt
# Xt before Xmu and Xpm and Xtst
# Xext before any other extension library, including mesa
# Xfixes before Xcomposite, Xcursor or Xdamage
# libpthread-stubs before libdrm and xcb
# libdrm before mesa

# Dependencies needed for parallel make:
DPS: libX11 libXt
fontconfig: freetype
libdga: libX11
libdmx: libX11 libXext
libdrm: libpthread-stubs
libxcb: libpthread-stubs
xcb-util: libxcb
xcb-util-image: libxcb xcb-util
xcb-util-keysyms: libxcb
xcb-util-renderutil: libxcb
xcb-util-wm: libxcb
libFS: libxtrans
libICE: libxtrans
libSM: libxtrans libICE
libX11: libxtrans libXau libXdmcp libxcb
libXaw: libX11 libXt libXmu libXpm
libXaw4: libX11 libXt libXmu
libXaw5: libX11 libXt libXmu
libXcomposite: libX11 libXext libXfixes
libXcursor: libX11 libXfixes libXrender
libXdamage: libXfixes
libXevie: libX11 libXext
libXext: libX11 libXau
libXfixes: libX11 libXext
libXfont: libxtrans freetype libfontenc
libXft: freetype fontconfig libXrender libX11
libXi: libX11 libXext
libXinerama: libX11 libXext
libXmu: libX11 libXt libXext
libXp: libX11 libXext
libXpm: libX11 libXt
libXrandr: libX11 libXext libXrender
libXrender: libX11 libXext
libXres: libX11 libXext
libXScrnSaver: libX11 libXext
libXt: libX11 libSM libICE
libXtsol: libX11 libXext
libXtst: libX11 libXext
libXv: libX11 libXext
libXvMC: libX11 libXext
libXxf86misc: libX11 libXext
libXxf86vm: libX11 libXext
libxkbfile: libX11
mesa: libdrm libX11 libXext
freeglut: mesa

# pixman doesn't need these itself, but its test framework uses gtk,
# which does depend on these
pixman: libXext libXrender libXinerama libXi libXrandr libXcursor 
pixman: libXcomposite libXdamage libXfixes libX11 freetype fontconfig 

OS_SUBDIRS_common = \
	DPS \
	freetype \
	fontconfig \
	libdga \
	libdmx \
	libdrm \
	libowconfig \
	libpthread-stubs \
	libxtrans \
	libXau \
	libXdmcp \
	libxcb \
	xcb-util \
	xcb-util-image \
	xcb-util-keysyms \
	xcb-util-renderutil \
	xcb-util-wm \
	libX11 \
	libXext \
	libfontenc \
	libFS \
	libICE \
	liblbxutil \
	libpciaccess \
	libSM \
	libXt \
	libXmu \
	libXpm \
	libXp \
	libXaw \
	libXaw4 \
	libXaw5 \
	libXfixes \
	libXcomposite \
	libXrender \
	libXdamage \
	libXcursor \
	libXevie \
	libXfont \
	libXft \
	libXi \
	libXinerama \
	libxkbfile \
	libXrandr \
	libXres \
	libXScrnSaver \
	libXtsol \
	libXtst \
	libXv \
	libXvMC \
	libXxf86misc \
	libXxf86vm \
	mesa \
	freeglut \
	pixman

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

OBSOLETE_SUBDIRS = \
	hwc \
	libXlg3d \
	libXprintUtil \
	libXprintAppUtil

PWD:sh=pwd
TOP=$(PWD)/../..

### Include common definitions
DIRNAME="lib/"
include $(TOP)/open-src/common/Makefile.subdirs

