# Makefile for X Consolidation Open Source Library modules
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
# ident	"@(#)Makefile	1.35	09/12/05 SMI"
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
# libpthread-stubs before libdrm (and in the future, before xcb)
# libdrm before mesa

# Dependencies needed for parallel make:
DPS: libX11 libXt
fontconfig: freetype
libdga: libX11
libdrm: libpthread-stubs
libFS: libxtrans
libICE: libxtrans
libSM: libxtrans libICE
libX11: libxtrans libXau libXdmcp
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
libXt: libX11 libSM
libXtsol: libX11 libXext
libXtst: libX11 libXext
libXv: libX11 libXext
libXvMC: libX11 libXext
libXxf86misc: libX11 libXext
libXxf86vm: libX11 libXext
libxkbfile: libX11
mesa: libdrm libX11 libXext

# pixman doesn't need these itself, but its test framework uses gtk,
# which does depend on these
pixman: libXext libXrender libXinerama libXi libXrandr libXcursor 
pixman: libXcomposite libXdamage libXfixes libX11 freetype fontconfig 

OS_SUBDIRS_common = \
	DPS \
	freetype \
	fontconfig \
	libdga \
	libdrm \
	libowconfig \
	libpthread-stubs \
	libxtrans \
	libXau \
	libXdmcp \
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
	pixman \
	hwc

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

OBSOLETE_SUBDIRS = \
	libXlg3d \
	libXprintUtil \
	libXprintAppUtil

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME="lib/"
include $(TOP)/common/Makefile.subdirs

