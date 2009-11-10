###############################################################################
#
# libdps Makefile
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
#ident	"@(#)Makefile	1.2	09/11/10 SMI"
#

    REQUIREDLIBS = -lX11 -lsocket -lnsl -lc

#  The Client Library uses definitions just like Xlib for network
#  capabilities, such as TCPCONN, UNIXCONN, and DNETCONN.  See
#  csconndi.c and csopendi.c

# The connection code should be rewritten to use xtrans.  The STREAMSCONN
# code here needs work.  For now force ConnectionFlags to be
#  -DTCPCONN -DUNIXCONN

             CC = cc
        DEFINES = -DXDPS -DCSDPS -DTCPCONN -DUNIXCONN -DSVR4
    DPSCPPFLAGS = $(DEFINES) -I ../include
      DPSCFLAGS = -Kpic
   SHLIBLDFLAGS = -G -z defs $(LDFLAGS)
            CAT = cat
            SED = sed
            AWK = awk
           SORT = sort
         PSWRAP = ../pswrap/pswrap

COMMONOBJECTFILES = \
	dpsclient.o\
	dpsprintf.o\
	dpsexcept.o\
	dpsdict.o\
	dpsabbrev.o

COMMONSOURCEFILES = \
	dpsclient.c\
	dpsprintf.c\
	dpsexcept.c\
	dpsdict.c\
	dpsabbrev.c

XOBJECTFILES = \
	dpsXclient.o\
	dpssysnames.o\
	dpsXpriv.o\
	XDPS.o\
	dpsXcmu.o

#	dpsXtdisp.o\	- wasn't included in Solaris libdps for some reason

XSOURCEFILES = \
	dpsXclient.c\
	dpssysnames.c\
	dpsXpriv.c\
	XDPS.c\
	dpsXcmu.c

#	dpsXtdisp.c\	- wasn't included in Solaris libdps for some reason

XCSDPSOBJECTFILES = \
	cslibext.o\
	csopendi.o\
	csconndi.o\
	csstartNX.o\
	csfindNX.o\
	cslibint.o

XCSDPSSOURCEFILES = \
	cslibext.c\
	csopendi.c\
	csconndi.c\
	csstartNX.c\
	csfindNX.c\
	cslibint.c

DPSOPSOBJECTS = \
    dpsclrops.o\
    dpsctrlops.o\
    dpsctxtops.o\
    dpsdataops.o\
    dpsfontops.o\
    dpsgsttops.o\
    dpsioops.o\
    dpsmathops.o\
    dpsmtrxops.o\
    dpsmiscops.o\
    dpspntops.o\
    dpspathops.o\
    dpssysops.o\
    dpswinops.o\
    dpsopstack.o\
    dpsXops.o\
    dpsl2ops.o

PSOPSOBJECTS = \
    psclrops.o\
    psctrlops.o\
    psctxtops.o\
    psdataops.o\
    psfontops.o\
    psgsttops.o\
    psioops.o\
    psmathops.o\
    psmtrxops.o\
    psmiscops.o\
    pspntops.o\
    pspathops.o\
    pssysops.o\
    pswinops.o\
    psopstack.o\
    psXops.o\
    psl2ops.o

DPSOPSHEADERFILES = \
    dpsclrops.h\
    dpsctrlops.h\
    dpsctxtops.h\
    dpsdataops.h\
    dpsfontops.h\
    dpsgsttops.h\
    dpsioops.h\
    dpsmathops.h\
    dpsmtrxops.h\
    dpsmiscops.h\
    dpsopstack.h\
    dpspntops.h\
    dpspathops.h\
    dpssysops.h\
    dpswinops.h\
    dpsXops.h\
    dpsl2ops.h

PSOPSHEADERFILES = \
    psclrops.h\
    psctrlops.h\
    psctxtops.h\
    psdataops.h\
    psfontops.h\
    psgsttops.h\
    psioops.h\
    psmathops.h\
    psmtrxops.h\
    psmiscops.h\
    pspntops.h\
    pspathops.h\
    pssysops.h\
    pswinops.h\
    psopstack.h\
    psXops.h\
    psl2ops.h

DPSOPSCFILES = \
    dpsclrops.c\
    dpsctrlops.c\
    dpsctxtops.c\
    dpsdataops.c\
    dpsfontops.c\
    dpsgsttops.c\
    dpsioops.c\
    dpsmathops.c\
    dpsmtrxops.c\
    dpsmiscops.c\
    dpsopstack.c\
    dpspntops.c\
    dpspathops.c\
    dpssysops.c\
    dpswinops.c\
    dpsXops.c\
    dpsl2ops.c

PSOPSCFILES = \
    psclrops.c\
    psctrlops.c\
    psctxtops.c\
    psdataops.c\
    psfontops.c\
    psgsttops.c\
    psioops.c\
    psmathops.c\
    psmtrxops.c\
    psmiscops.c\
    pspntops.c\
    pspathops.c\
    pssysops.c\
    pswinops.c\
    psopstack.c\
    psXops.c\
    psl2ops.c

HEADERS = psops.h dpsops.h

all:: $(HEADERS)

includes:: $(DPSOPSCFILES) $(PSOPSCFILES) $(HEADERS)

$(DPSOPSCFILES) $(PSOPSCFILES): $(PSWRAP)

SRCS = \
	${COMMONSOURCEFILES} \
	${XSOURCEFILES} \
	${XCSDPSSOURCEFILES} \
	${DPSOPSCFILES} \
	${PSOPSCFILES}

OBJS = \
	${COMMONOBJECTFILES} \
	${XOBJECTFILES} \
	${XCSDPSOBJECTFILES} \
	${DPSOPSOBJECTS} \
	${PSOPSOBJECTS}

DERIVED_FILES = $(DPSOPSHEADERFILES) $(PSOPSHEADERFILES) \
		$(DPSOPSCFILES) $(PSOPSCFILES)

# Inspired by automake 1.11 silent rules - hides details by default
# Run make V=1 to show details
DEFAULT_V  = 0
V_CC       = $(V_CC_$(V))
V_CC_      = $(V_CC_$(DEFAULT_V))
V_CC_0     = @echo "  CC    " $@;
V_PSWRAP   = $(V_PSWRAP_$(V))
V_PSWRAP_  = $(V_PSWRAP_$(DEFAULT_V))
V_PSWRAP_0 = @echo "  PSWRAP" $@;
V_GEN      = $(V_GEN_$(V))
V_GEN_     = $(V_GEN_$(DEFAULT_V))
V_GEN_0    = @echo "  GEN   " $@;

.SUFFIXES: .psw

.psw.c : 
	$(V_PSWRAP)$(PSWRAP) -a -o $*.c -h $*.h $<

.psw.h : 
	$(V_PSWRAP)$(PSWRAP) -a -h $*.h $< > /dev/null

.c.o:
	$(V_CC)$(CC) $(DPSCFLAGS) $(CFLAGS) $(DPSCPPFLAGS) $(CPPFLAGS) -c $(OUTPUT_OPTION) $<

includes :: $(HEADERS)
depend :: $(DPSOPSCFILES) $(PSOPSCFILES)

psops.h : $(PSOPSHEADERFILES)
	$(V_GEN) ${RM} $@ ; \
	${CAT} $(PSOPSHEADERFILES) |\
	    ${SED} -e '/^$$/D' -e '/#/D' -e '/^\//D'\
		-e '/^   gener/D' -e '/^.\//D' |\
	    ${SORT} > .sort ; \
	${AWK} '/;/ {print; printf("\n");}' .sort > .ttt ; \
	${CAT} psname.txt header.txt psifdef.txt .ttt psendif.txt > psops.h ; \
	${RM} .sort .ttt .attt

dpsops.h : psops.h $(DPSOPSHEADERFILES)
	$(V_GEN) ${RM} $@ ; \
	${CAT} $(DPSOPSHEADERFILES) |\
	    ${SED} -e '/^$$/D' -e '/#/D' -e '/^\//D'\
		-e '/^   gener/D' -e '/^.\//D' |\
	    ${SORT} > .sort ; \
	${AWK} '/;/ {print; printf("\n");}' .sort > .ttt ; \
	${CAT} dpsname.txt header.txt dpsifdef.txt .ttt dpsendif.txt > dpsops.h ; \
	${RM} .sort .ttt .attt

all:: ../include/DPS/psops.h ../include/DPS/dpsops.h
$(OBJS): $(HEADERS) ../include/DPS/psops.h ../include/DPS/dpsops.h

../include/DPS/psops.h: psops.h
	-${RM} $@
	ln -s ../../libdps/psops.h $@

../include/DPS/dpsops.h: dpsops.h
	-${RM} $@
	ln -s ../../libdps/dpsops.h $@

all:: $(OBJS) libdps.so.5 libdps.so

MAPFILE = mapfile

libdps.so.5: $(OBJS) $(EXTRASHAREDOBJS) $(MAPFILE)
	$(V_CC)$(CC) $(CDEBUGFLAGS) -o $@ $(SHLIBLDFLAGS) -M $(MAPFILE) $(OBJS) $(EXTRASHAREDOBJS) $(REQUIREDLIBS)

libdps.so: libdps.so.5
	-${RM} libdps.so
	ln -s libdps.so.5 libdps.so

install: all
	$(INSTALL) libdps.so.5 $(DESTDIR)$(libdir)/libdps.so.5

clean ::
	${RM} $(DERIVED_FILES) .sort .ttt .att psops.h dpsops.h *%
