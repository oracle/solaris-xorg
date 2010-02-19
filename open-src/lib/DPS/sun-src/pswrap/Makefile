###############################################################################
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
#

# Manual conversion of X11R6.9 xc/config/pswrap/Imakefile

             CC = cc
     FRIENDSDEF = -DFRIENDSFILE='"<DPS/dpsfriends.h>"'
        DEFINES = -DXENVIRONMENT
      CPPFLAGS += $(DEFINES)
           YACC = /usr/ccs/bin/yacc
         YFLAGS = -d
EXTRA_LIBRARIES = -ll

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
	$(V_CC)$(CC) -o $@ $(LDFLAGS) $(OBJS) $(EXTRA_LIBRARIES)

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
