###############################################################################
#
# Copyright (c) 2009, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Manual conversion of X11R6.9 xc/config/pswrap/Imakefile

             CC = cc
     FRIENDSDEF = -DFRIENDSFILE='"<DPS/dpsfriends.h>"'
        DEFINES = -DXENVIRONMENT
      CPPFLAGS += $(DEFINES)
           YACC = /usr/ccs/bin/yacc
         YFLAGS = -d
EXTRA_LIBRARIES = # -ll

SRCS = main.c pswdict.c pswfile.c \
	systemnames.c psw.c pswstring.c pswsemantics.c

OBJS = main.o pswparser.o lexer.o pswdict.o pswfile.o \
	systemnames.o psw.o pswstring.o pswsemantics.o

# Inspired by automake 1.11 silent rules - hides details by default
# Run make V=1 to show details
DEFAULT_V  = 0
V_CC       = $(V_CC_$(V))
V_CC_      = $(V_CC_$(DEFAULT_V))
V_CC_0     = @echo "  CC    " $@;
V_YACC     = $(V_YACC_$(V))
V_YACC_    = $(V_YACC_$(DEFAULT_V))
V_YACC_0   = @echo "  YACC  " $@;

all: pswrap

pswrap: $(OBJS)
	$(V_CC)$(CC) -o $@ $(LDFLAGS) $(PROG_LDFLAGS) $(OBJS) $(EXTRA_LIBRARIES)

pswfile.o:
	$(V_CC)$(CC) -c $(CFLAGS) $(CPPFLAGS) $(FRIENDSDEF) $*.c

.c.o:
	$(V_CC)$(CC) $(CFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<

pswparser.h + pswparser.c : pswparser.y
	$(V_YACC)$(YACC) $(YFLAGS) pswparser.y ; \
	mv y.tab.c pswparser.c ; \
	mv y.tab.h pswparser.h

install: pswrap
	$(INSTALL) pswrap $(DESTDIR)$(bindir)/pswrap

clean:
	${RM} lexer.c y.tab.h pswparser.c *.o pswrap

lexer.o: pswparser.h
