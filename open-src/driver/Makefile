# Makefile for X Consolidation Open Source Xorg driver modules
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
# ident	"@(#)Makefile	1.16	09/12/05 SMI"
#
###############################################################################

OS_SUBDIRS_common = \
	xf86-input-keyboard		\
	xf86-input-mouse		\
	xf86-input-void			\
	xf86-video-ast			\
	xf86-video-dummy		

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)	\
	xf86-video-wsfb

OS_SUBDIRS_i386  = $(OS_SUBDIRS_common)	\
	xf86-input-acecad		\
	xf86-input-synaptics		\
	xf86-input-vmmouse		\
	xf86-input-hotkey		\
	xf86-video-apm			\
	xf86-video-ark			\
	xf86-video-ati			\
	xf86-video-chips		\
	xf86-video-cirrus		\
	xf86-video-glint		\
	xf86-video-i128			\
	xf86-video-i740			\
	xf86-video-intel		\
	xf86-video-mach64		\
	xf86-video-mga			\
	xf86-video-neomagic		\
	xf86-video-nv			\
	xf86-video-openchrome		\
	xf86-video-radeonhd		\
	xf86-video-rendition		\
	xf86-video-r128			\
	xf86-video-s3			\
	xf86-video-s3virge		\
	xf86-video-savage		\
	xf86-video-siliconmotion	\
	xf86-video-sis			\
	xf86-video-tdfx			\
	xf86-video-tga			\
	xf86-video-trident		\
	xf86-video-tseng		\
	xf86-video-vesa			\
	xf86-video-vmware

# Subdirs we used to build, but now just need to clean up
OBSOLETE_SUBDIRS = \
	xf86-input-calcomp		\
	xf86-input-citron		\
	xf86-input-digitaledge		\
	xf86-input-dmc			\
	xf86-input-dynapro		\
	xf86-input-elographics		\
	xf86-input-fpit			\
	xf86-input-hyperpen		\
	xf86-input-jamstudio		\
	xf86-input-magellan		\
	xf86-input-microtouch		\
	xf86-input-mutouch		\
	xf86-input-palmax		\
	xf86-input-penmount		\
	xf86-input-spaceorb		\
	xf86-input-summa		\
	xf86-input-tek4957		\
	xf86-video-cyrix		\
	xf86-video-i810			\
	xf86-video-nsc			\
	xf86-video-vga			\
	xf86-video-via

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME="driver/"
include $(TOP)/common/Makefile.subdirs

# Dependencies between modules
xf86-input-vmmouse: xf86-input-mouse

