# Makefile for X Consolidation Open Source Protocol modules
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.5	07/01/31
#
###############################################################################

COMMON_SUBDIRS = X11 BigReqs Composite Damage EvIE Fixes Fontcache Fonts \
	Input KB PM Randr Record Render Resource ScrnSaver Trap Video \
	XCMisc XExt XF86BigFont XF86DGA XF86Misc XF86VidMode Xinerama \
	lg3dproto

sparc_SUBDIRS = $(COMMON_SUBDIRS)

i386_SUBDIRS  = $(COMMON_SUBDIRS)       \
	glproto xf86driproto

OS_SUBDIRS:sh=uname -p | sed 's/^\(.*\)$/\$\(\1_SUBDIRS\)/'

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

subdirs:
	@case '${MFLAGS}' in *[ik]*) set +e;; esac; \
	for i in $(OS_SUBDIRS) ;\
	do \
	(cd $$i ; echo "## making" $(OS_TARGET) "in open-src/proto/$$i..."; \
	$(MAKE) $(MFLAGS) CDEBUGFLAGS="$(CDEBUGFLAGS)" $(OS_TARGET)); \
	done
