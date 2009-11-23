# Makefile for X Consolidation Open Source font modules
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
# ident	"@(#)Makefile	1.12	09/11/23 SMI"
#
###############################################################################

# Need to install to proto area by default for inter-package dependencies to
# work right.
default_target: install

# encodings & util must be first, since they install files used by the rest
# sun-aliases must be last, since it builds on the results of the rest

FONT_SUBDIRS= \
	bitstream-vera \
	deja-vu \
	google-droid \
	liberation \
	adobe-75dpi \
	adobe-100dpi \
	adobe-utopia-75dpi \
	adobe-utopia-100dpi \
	adobe-utopia-type1 \
	alias \
	arabic-misc \
	bh-75dpi \
	bh-100dpi \
	bh-lucidatypewriter-75dpi \
	bh-lucidatypewriter-100dpi \
	bh-ttf \
	bh-type1 \
	bitstream-75dpi \
	bitstream-100dpi \
	bitstream-type1 \
	cronyx-cyrillic \
	cursor-misc \
	daewoo-misc \
	dec-misc \
	ibm-type1 \
	isas-misc \
	jis-misc \
	micro-misc \
	misc-cyrillic \
	misc-ethiopic \
	misc-meltho \
	misc-misc \
	mutt-misc \
	schumacher-misc \
	screen-cyrillic \
	sony-misc \
	sun-misc \
	winitzki-cyrillic \
	xfree86-type1 \

OS_SUBDIRS_common = \
	encodings \
	util \
	$(FONT_SUBDIRS) \
	sun-aliases

$(FONT_SUBDIRS): encodings util
sun-aliases: $(FONT_SUBDIRS)

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)
OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME="font/"
include $(TOP)/common/Makefile.subdirs

