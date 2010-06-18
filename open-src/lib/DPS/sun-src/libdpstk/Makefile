###############################################################################
#
# libdpstk Makefile
#
# Copyright (c) 2009, 2010, Oracle and/or its affiliates. All rights reserved.
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

# Manual conversion of the X11R6.9 xc/lib/dpstk/Imakefile

LDPRELIB = -L . -L ../libdps -L ../libpsres
DPSTKLIBS = $(LDPRELIB) -ldps -lXt -lX11 -lm -lc
DPSTKXMLIBS = $(LDPRELIB) -ldpstk -lpsres -ldps -L/usr/dt/lib -lXm $(DPSTKLIBS)

       PSWRAP = ../pswrap/pswrap

INCLUDES = $(MOTIFINCLUDES)

MOTIFSRCS = FSBwraps.c FontSample.c FontSB.c FontCreato.c \
	CSBwraps.c ColorSB.c DSWwraps.c DPSScrollW.c
MOTIFOBJS = FSBwraps.o FontSample.o FontSB.o FontCreato.o \
	CSBwraps.o ColorSB.o DSWwraps.o DPSScrollW.o

      RESPATH = NULL
      DEFINES = -DDEFAULT_RESOURCE_PATH=\"${RESPATH}\"

DERIVED_FILES = FSBwraps.c FSBwraps.h XDPSpwraps.c XDPSpwraps.h \
	XDPSswraps.c XDPSswraps.h CSBwraps.c CSBwraps.h \
	DSWwraps.c DSWwraps.h

SRCS =  XDPSpwraps.c XDPSswraps.c XDPSshare.c XDPSpreview.c \
	XDPSuserpath.c

OBJS = 	XDPSpwraps.o XDPSswraps.o XDPSshare.o XDPSpreview.o \
	XDPSuserpath.o

             CC = cc
      CPPFLAGS += $(DEFINES) -I ../include $(INCLUDES)
        CFLAGS += -Kpic
   SHLIBLDFLAGS = -G -z defs $(LDFLAGS) -R $(libdir)

# Inspired by automake 1.11 silent rules - hides details by default
# Run make V=1 to show details
DEFAULT_V  = 0
V_CC       = $(V_CC_$(V))
V_CC_      = $(V_CC_$(DEFAULT_V))
V_CC_0     = @echo "  CC    " $@;
V_PSWRAP   = $(V_PSWRAP_$(V))
V_PSWRAP_  = $(V_PSWRAP_$(DEFAULT_V))
V_PSWRAP_0 = @echo "  PSWRAP" $@;

all:: $(SRCS) $(MOTIFSRCS)

.SUFFIXES: .psw

.psw.c:
	$(V_PSWRAP)$(PSWRAP) -o $*.c -a -f $*.h -h $*.h $<

.c.o:
	$(V_CC)$(CC) $(CFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<

all:: libdpstk.so.5 libdpstk.so

libdpstk.so.5: $(SRCS) $(OBJS) mapfile-vers.dpstk
	$(V_CC)$(CC) $(CDEBUGFLAGS) -o $@ -h $@ $(SHLIBLDFLAGS) -M mapfile-vers.dpstk $(OBJS) $(DPSTKLIBS)

libdpstk.so: libdpstk.so.5
	-${RM} libdpstk.so
	ln -s libdpstk.so.5 libdpstk.so

all:: libdpstkXm.so.5 libdpstkXm.so

libdpstkXm.so.5: libdpstk.so $(MOTIFSRCS) $(MOTIFOBJS) mapfile-vers.dpstkXm
	$(V_CC)$(CC) $(CDEBUGFLAGS) -o $@ -h $@ $(SHLIBLDFLAGS) -M mapfile-vers.dpstkXm $(MOTIFOBJS) $(DPSTKXMLIBS)

libdpstkXm.so: libdpstkXm.so.5
	-${RM} libdpstkXm.so
	ln -s libdpstkXm.so.5 libdpstkXm.so

clean ::
	${RM} $(DERIVED_FILES) $(MOTIFOBJS) $(OBJS)

install: all
	$(INSTALL) libdpstk.so.5 $(DESTDIR)$(libdir)/libdpstk.so.5
	$(INSTALL) libdpstkXm.so.5 $(DESTDIR)$(libdir)/libdpstkXm.so.5
	$(INSTALL) -m 0444 ColorSelect.ad \
		$(DESTDIR)$(datadir)/X11/app-defaults/ColorSelect
	$(INSTALL) -m 0444 FontSelect.ad \
		$(DESTDIR)$(datadir)/X11/app-defaults/FontSelect
