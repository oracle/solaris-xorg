# Makefile for X Consolidation Open Source Application modules
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
# @(#)Makefile	1.77	08/09/16
#
###############################################################################

# Dependency ordering:
#  rss-glx needs to come after xscreensaver

OS_SUBDIRS_common = \
	appres \
	bdftopcf \
	bitmap \
	constype \
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
	mkfontdir \
	mkfontscale \
	oclock \
	proxymngr \
	rstart \
	sessreg \
	setxkbmap \
	showfont \
	smproxy \
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
	xconsole \
	xcursorgen \
	xditview \
	xdpyinfo \
	xdriinfo \
	xedit \
	xev \
	xeyes \
	xfd \
	xfindproxy \
	xfontsel \
	xfsinfo \
	xfwp \
	xgamma \
	xgc \
	xhost \
	xinit \
	xkbcomp \
	xkbprint \
	xkbutils \
	xkill \
	xload \
	xlogo \
	xlsatoms \
	xlsclients \
	xlsfonts \
	xlswins \
	xmag_multivis \
	xmag \
	xman \
	xmodmap \
	xpm \
	xpr \
	xprehashprinterlist \
        xplsprinters \
	xprop \
	xrandr \
	xrdb \
	xrefresh \
	xrx \
	xset \
	xsetroot \
	xstdcmap \
        xterm \
	xvidtune \
	xvinfo \
	xwd \
	xwininfo \
	xwud \
	xscreensaver \
	rss-glx

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME="app/"
include $(TOP)/common/Makefile.subdirs
