###############################################################################
#
# libdga Makefile
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
# ident	"@(#)Makefile	1.1	09/11/09 SMI"
#

SHLIBLDFLAGS = -G -z defs $(LDFLAGS)
REQUIREDLIBS = -lX11 -lsocket -lc
DEFINES = -D_TS_ERRNO -DMT -DXTHREADS -DPANORAMIX -DIPv6 -DSVR4

HEADERS = dga_Xrequests.h win_grab.h rtn_grab.h cmap_grab.h pix_grab.h \
	  mbufsetstr.h vmbuf_grab.h dga_incls.h
SRCS =  dga_Xrequests.c win_grab.c dga_db.c rtn_grab.c cmap_grab.c pix_grab.c \
	mbsmemb_update.c mbsmemb_inquiry.c win_update.c mbuf_update.c \
	mbufset.c draw_inquiry.c draw_db.c pix_update.c pix_inquiry.c dga_ab.c
OBJS = $(SRCS:.c=.o)

# Inspired by automake 1.11 silent rules - hides details by default
# Run make V=1 to show details
DEFAULT_V  = 0
V_CC       = $(V_CC_$(V))
V_CC_      = $(V_CC_$(DEFAULT_V))
V_CC_0     = @echo "  CC    " $@;

all: libdga.so.1

libdga.so.1: $(OBJS)
	$(V_CC)$(LD) $(CFLAGS) -o $@ $(SHLIBLDFLAGS) -h libdga.so.1 \
		$(OBJS) $(REQUIREDLIBS)

.c.o:
	$(V_CC)$(CC) -Kpic $(CFLAGS) $(CPPFLAGS) $(DEFINES) -c $<

install: libdga.so.1
	mkdir -p $(DESTDIR)$(libdir) $(DESTDIR)$(INCDIR)
	$(INSTALL) libdga.so.1 $(DESTDIR)$(libdir)/libdga.so.1
