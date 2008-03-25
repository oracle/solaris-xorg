# Makefile for X Consolidation Open Source Library modules
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
# @(#)Makefile	1.18	08/03/25
#
###############################################################################

# Order copied from X.Org's util/modular/build.sh:
# the libraries have a dependency order:
# xtrans, Xau, Xdmcp before anything else
# fontenc before Xfont
# ICE before SM
# X11 before Xext
# (X11 and SM) before Xt
# Xt before Xmu and Xpm and Xtst
# Xext before any other extension library
# Xfixes before Xcomposite
# Xp before XprintUtil before XprintAppUtil

OS_SUBDIRS_common = \
	freetype \
	fontconfig \
	libxtrans \
	libXau \
	libXdmcp \
	libX11 \
	libXext \
	libfontenc \
	libICE \
	libSM \
	libXt \
	libXmu \
	libXpm \
	libXaw \
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
	libxkbui \
	libXrandr \
	libXres \
	libXScrnSaver \
	libXtst \
	libXv \
	libXvMC \
	libXxf86misc \
	libXxf86vm \
	libXlg3d

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)

OS_SUBDIRS_i386 = $(OS_SUBDIRS_common) \
	libdrm \
	mesa

MACH:sh=uname -p
OS_SUBDIRS=$(OS_SUBDIRS_$(MACH))

all: 
	$(MAKE) $(MFLAGS) subdirs OS_TARGET=all

World: clean all

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
	(cd $$i ; echo "## making" $(OS_TARGET) "in open-src/lib/$$i..."; \
	$(MAKE) $(MFLAGS) CDEBUGFLAGS="$(CDEBUGFLAGS)" $(OS_TARGET)); \
	done
