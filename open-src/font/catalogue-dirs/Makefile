###############################################################################
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

MODULE_NAME=catalogue-dirs

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=NONE
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes

SOURCE_TARGETS = 
SOURCE_TARGETS_SET=yes

BUILD_TARGETS = 
BUILD_TARGETS_SET=yes

INSTALL_TARGETS = $(CHARSETS) core-pcf
INSTALL_TARGETS_SET=yes

CHARSETS= \
	ISO8859-2	\
	ISO8859-3	\
	ISO8859-4	\
	ISO8859-9	\
	ISO8859-10	\
	ISO8859-13	\
	ISO8859-14	\
	ISO8859-16

DESTDIR=$(PROTODIR)
INSTDIR=$(DESTDIR)$(X11_FONT_DIR)
CHARSET_FONT_DIRS=$(CHARSETS:%=$(INSTDIR)/%)

### Include common rulesets
include ../Makefile.inc

clean_gen::
	rm -rf $(CHARSET_FONT_DIRS)

subset_dir:
	-rm -rf $(INSTDIR)/$(TYPE)-$(LOCALE)
	mkdir -p $(INSTDIR)/$(TYPE)-$(LOCALE)
	(cd $(INSTDIR)/$(TYPE)-$(LOCALE) ; \
	 ln -s ../$(TYPE)/$(PATTERN).pcf* . ; \
	 $(MKFONTDIR) . ; \
	 $(INSTALLALIAS) ../$(TYPE)/fonts.alias )

$(CHARSETS):
	$(MAKE) $(MAKEFLAGS) subset_dir TYPE=75dpi  LOCALE=$@ PATTERN='*-$@'
	$(MAKE) $(MAKEFLAGS) subset_dir TYPE=100dpi LOCALE=$@ PATTERN='*-$@'
	$(MAKE) $(MAKEFLAGS) subset_dir TYPE=misc   LOCALE=$@ PATTERN='*-$@'

# All fonts ending in -ISO-8859-1 or -ISO-8859-15 or 1976-0
# or with a name not ending in a locale string.
CORE_PATTERN=PATTERN='*{-ISO8859-{1,15},1976-0,[a-z][0-9],[A-Za-z_][0-9][0-9],[A-Za-z]}'

core-pcf:
	$(MAKE) $(MAKEFLAGS) subset_dir TYPE=75dpi  LOCALE=core $(CORE_PATTERN)
	$(MAKE) $(MAKEFLAGS) subset_dir TYPE=100dpi LOCALE=core $(CORE_PATTERN)
	$(MAKE) $(MAKEFLAGS) subset_dir TYPE=misc   LOCALE=core $(CORE_PATTERN)

old_charsets:
	-rm -rf $(INSTDIR)/75dpi-$@ $(INSTDIR)/100dpi-$@
	mkdir -p $(INSTDIR)/75dpi-$@ $(INSTDIR)/100dpi-$@
	(cd $(INSTDIR)/75dpi-$@ ; \
	 ln -s ../75dpi/*-$@.pcf* . ; \
	 $(MKFONTDIR) . ; \
	 $(INSTALLALIAS) ../75dpi/fonts.alias )
	(cd $(INSTDIR)/100dpi-$@ ; \
	 ln -s ../100dpi/*-$@.pcf* . ; \
	 $(MKFONTDIR) . ; \
	 $(INSTALLALIAS) ../100dpi/fonts.alias )
	(cd $(INSTDIR)/75dpi-$@ ; \
	 ln -s ../75dpi/*-$@.pcf* . ; \
	 $(MKFONTDIR) . ; \
	 $(INSTALLALIAS) ../75dpi/fonts.alias )

old_core-pcf:
	-rm -rf $(INSTDIR)/75dpi-core $(INSTDIR)/100dpi-core
	mkdir -p $(INSTDIR)/75dpi-core $(INSTDIR)/100dpi-core
	(cd $(INSTDIR)/75dpi-core ; \
	 ln -s ../75dpi/*-ISO8859-{1,15}.pcf* . ; \
	 ln -s ../75dpi/*[A-Za-z_][0-9][0-9].pcf* . ;\
	 $(MKFONTDIR) . ; \
	 $(INSTALLALIAS) ../75dpi/fonts.alias )
	(cd $(INSTDIR)/100dpi-core ; \
	 ln -s ../100dpi/*-ISO8859-{1,15}.pcf* . ; \
	 ln -s ../100dpi/*[A-Za-z_][0-9][0-9].pcf* . ;\
	 $(MKFONTDIR) . ; \
	 $(INSTALLALIAS) ../100dpi/fonts.alias )
