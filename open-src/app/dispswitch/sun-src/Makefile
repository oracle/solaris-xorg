# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.1	09/02/24

SRCS = dispswitch.c
OBJS = $(SRCS:.c=.o)

PKGS = 'xrandr >= 1.2.0' x11
PKG_CFLAGS = pkg-config --cflags $(PKGS)
PKG_LIBS = pkg-config --libs $(PKGS)
CFLAGS += $(PKG_CFLAGS:sh)
LIBS = $(PKG_LIBS:sh)

dispswitch: $(OBJS)
	$(LD) -o $@ $(CFLAGS) $(LDFLAGS) $(OBJS) $(LIBS)

install: dispswitch
	mkdir -p $(DESTDIR)$(PREFIX)/bin \
		 $(DESTDIR)$(PREFIX)/share/man/man1 \
		 $(DESTDIR)/etc/xdg/autostart
	$(INSTALL) dispswitch $(DESTDIR)$(PREFIX)/bin
	$(INSTALL) dispswitch.man $(DESTDIR)$(PREFIX)/share/man/man1/dispswitch.1
	$(INSTALL) gnome-dispswitch.desktop $(DESTDIR)/etc/xdg/autostart
