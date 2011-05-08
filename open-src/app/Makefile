# Makefile for X Consolidation Open Source Application modules
#
# Copyright (c) 2006, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Several utilities are not listed here because they are called out in
# the makefile in the higher level directory due to dependencies between
# them and modules in other directories.   These include:
#	rgb, mkfontdir, mkfontscale, bdftopcf

OS_SUBDIRS_common = \
	accessx \
	appres \
	bitmap \
	cmap_alloc \
        cmap_compact \
	constype \
	dispswitch \
	editres \
	fbconsole \
	fslsfonts \
	fstobdf \
	iceauth \
	ico \
	installalias \
	lbxproxy \
	listres \
	luit \
	kbd_mode \
	mesa-demos \
	mkcookie \
	oclock \
	proxymngr \
	sessreg \
	setxkbmap \
	showfont \
	smproxy \
	synergy \
	winsysck \
	twm \
	viewres \
	x11perf \
	xauth \
	xbiff \
	xcalc \
	xclipboard \
	xclock \
	xcmsdb \
        xcolor \
        xcompmgr \
        transset \
	xconsole \
	xcursorgen \
	xditview \
	xdm \
	xdpyinfo \
	xdriinfo \
	xedit \
	xev \
	xeyes \
	xfd \
	xfindproxy \
	xfontsel \
	xfs \
	xfsinfo \
	xfwp \
	xgamma \
	xgc \
	xhost \
	xinit \
	xinput \
	xkbcomp \
	xkbprint \
	xkbutils \
	xkill \
	xload \
        xlock \
	xlogo \
	xlsatoms \
	xlsclients \
	xlsfonts \
	xlswins \
	xmag_multivis \
	xmag \
	xmakemap \
	xman \
	xmodmap \
	xpm \
	xpr \
	xprop \
	xrandr \
	xrdb \
	xrefresh \
	xscope \
	xset \
	xsetroot \
	xstdcmap \
        xterm \
	xvidtune \
	xvinfo \
	xwd \
	xwininfo \
	xwud \

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common) \
	gfx-utils

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

OBSOLETE_SUBDIRS = \
	rstart \
	xplsprinters \
	xprehashprinterlist \
	xrx \
	xscreensaver \
	rss-glx

PWD:sh=pwd
TOP=$(PWD)/../..

### Include common definitions
DIRNAME="app/"
include $(TOP)/open-src/common/Makefile.subdirs
