# Makefile for X Consolidation Open Source bits
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
# @(#)Makefile	1.20	08/06/09
#
###############################################################################
#
# Modules in this makefile are divided into 2 sections - those that need to
# be built before the X11 monolithic tree (because it depends on them) and 
# those that need to be build after the X11 monolithic tree (because they 
# depend on portions of the monolithic tree).

BEFOREX = \
	util/util-macros \
	proto \
	data/xbitmaps \
	util/lndir \
	doc \
	lib \
	app/bdftopcf

# app/rgb depends on the rgb.h header installed by the xserver, so has to
# be built after xserver
AFTERX = \
	app \
	xserver \
	driver \
	app/rgb \
	data \
	font

ALL = $(BEFOREX) $(AFTERX)

# Need to install to proto area by default for inter-package dependencies to
# work right.
all: install

World: clean install

beforeX-all: 
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=all OS_SUBDIRS=BEFOREX

afterX-all: 
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=all OS_SUBDIRS=AFTERX

clean: beforeX-clean afterX-clean

beforeX-clean: 
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=clean OS_SUBDIRS=BEFOREX

afterX-clean: 
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=clean OS_SUBDIRS=AFTERX

install: beforeX-install afterX-install

beforeX-install:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=install OS_SUBDIRS=BEFOREX

afterX-install:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=install OS_SUBDIRS=AFTERX

download: beforeX-download afterX-download

beforeX-download:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=download OS_SUBDIRS=BEFOREX

afterX-download:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=download OS_SUBDIRS=AFTERX

source:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=source OS_SUBDIRS=ALL

subdirs:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $($(OS_SUBDIRS)) ;\
	do \
	(cd $$i ; echo "## making" $(OS_TARGET) "in open-src/$$i..."; \
	$(MAKE) $(MFLAGS) CDEBUGFLAGS="$(CDEBUGFLAGS)" $(OS_TARGET)); \
	done
