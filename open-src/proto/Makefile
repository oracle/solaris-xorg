# Makefile for X Consolidation Open Source Protocol modules
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
# @(#)Makefile	1.9	08/03/25
#
###############################################################################

OS_SUBDIRS_common = \
	x11proto bigreqsproto compositeproto damageproto evieproto \
	fixesproto fontcacheproto fontsproto inputproto kbproto pmproto \
	randrproto recordproto renderproto resourceproto scrnsaverproto \
	trapproto videoproto xcmiscproto xextproto xf86bigfontproto \
	xf86dgaproto xf86miscproto xf86vidmodeproto xineramaproto \
	lg3dproto sun-ext-protos

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)       \
	glproto xf86driproto

MACH:sh=uname -p
OS_SUBDIRS=$(OS_SUBDIRS_$(MACH))

# Need to install to proto area by default for inter-package dependencies to
# work right.
all: install

World: clean install

clean: 
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=clean

install:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=install

download:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=download

source:
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=source

subdirs:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(OS_SUBDIRS) ;\
	do \
	(cd $$i ; echo "## making" $(OS_TARGET) "in open-src/proto/$$i..."; \
	$(MAKE) $(MFLAGS) CDEBUGFLAGS="$(CDEBUGFLAGS)" $(OS_TARGET)); \
	done
