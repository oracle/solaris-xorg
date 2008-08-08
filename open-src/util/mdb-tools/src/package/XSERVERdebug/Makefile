# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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

PKGDEFS=$(TOP)/../packages/
DATAFILES=copyright
TMPLFILES= pkginfo
COMMONDIR=../XSERVERdebug
COMMONFILES= depend pkginfo.tmpl
CHKINSTALLSRC=
CHKINSTALL=$(CHKINSTALLSRC:%=checkinstall)

FILES=$(CHKINSTALL) $(DATAFILES) $(TMPLFILES)

PACKAGE:sh= basename `pwd`

PKGARCHIVE=../proto-packages

CLOBBERFILES= $(FILES)

all: $(FILES) pkg

install: all pkg

PKGVERS=$(PKGDEFS)/pkgversion
MACH:sh=uname -p
DATE:sh=date +0.%Y.%m.%d
VERSIONCOMM=sed -n 's/VERSION=//p' $(PKGVERS)
VERSION=$(VERSIONCOMM:sh)
BUILDCOMM=sed -n 's/BUILD=//p' $(PKGVERS)
BUILD=$(BUILDCOMM:sh)

PKGMK_VARS=plat=$(MACH) Xserver=$(Xserver)

pkginfo: pkginfo.tmpl $(PKGVERS)
	$(RM) $@; cat $@.tmpl | sed -e '/ARCH/s/ISA/'$(MACH)'/' -e 's/SUNW_PRODVERS=.*$$/SUNW_PRODVERS='$(VERSION)/ -e 's/VERSION=.*$$/VERSION='$(VERSION).$(BUILD),REV=$(DATE)/ -e 's/XSERVER/$(XSERVER)/g' -e 's/Xserver/$(Xserver)/g' > $@

prototype: $(COMMONDIR)/prototype
	$(RM) $@; cat $(COMMONDIR)/prototype | sed -e 's/@plat@/$(PLATDIR_32)/' -e 's/@plat_64@/$(PLATDIR_64)/' -e 's/@XSERVER@/$(XSERVER)/g' -e 's/@Xserver@/$(Xserver)/g' > $@

pkg: $(PKGARCHIVE) prototype $(COMMONFILES) FRC
	pkgmk -f prototype -d $(PKGARCHIVE) -o $(PKGMK_VARS) $(PACKAGE)

$(PKGARCHIVE):
	[ -d $(PKGARCHIVE) ] || mkdir -p $(PKGARCHIVE)

$(CHKINSTALL): $(PKGDEFS)/common_files/$(CHKINSTALLSRC)
	$(RM) $(CHKINSTALL)
	cp $(PKGDEFS)/common_files/$(CHKINSTALLSRC) $(CHKINSTALL)

# If the file is under SCCS, then it should not be destroyed
# by an install rule here.  This is a mistake in package makefile.
$(DATAFILES): $(PKGDEFS)/$$@
	@[ ! -f SCCS/s.$@ ] || \
		( echo "DATAFILES list is incorrect for $@">&2 && false )
	$(RM) $@; cp $(PKGDEFS)/$@ $@

$(COMMONFILES): $(COMMONDIR)/$@
	$(RM) $@; cp $(COMMONDIR)/$@ $@

clobber clean:
	-$(RM) $(CLOBBERFILES) $(CLEANFILES)

FRC:
