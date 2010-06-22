# Makefile for X Consolidation Open Source font modules
#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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
TOP=$(PWD)/../..

### Include common definitions
DIRNAME="font/"
include $(TOP)/open-src/common/Makefile.subdirs

