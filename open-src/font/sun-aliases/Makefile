###############################################################################
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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

MODULE_NAME=sun-aliases

# No upstream sources for these files, since they're Solaris-specific
MODULE_VERSION=NONE
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes

SOURCE_TARGETS = 
SOURCE_TARGETS_SET=yes

CONFIGURE_TARGETS = 
CONFIGURE_TARGETS_SET=yes

BUILD_TARGETS = 
BUILD_TARGETS_SET=yes

INSTALL_TARGETS = install_aliases
INSTALL_TARGETS_SET=yes

BUILD_TYPES = core ISO8859-1
BUILD_TYPES_SET = yes

### Include common rulesets
include ../Makefile.inc

DESTDIR=$(PROTODIR)
INSTDIR=$(DESTDIR)$(X11_FONT_DIR)

ALIAS_DIRS_core = Type1 misc 75dpi 100dpi
ALIAS_DIRS_ISO8859-1 = misc-ISO8859-1 75dpi-ISO8859-1 100dpi-ISO8859-1
ALIAS_DIRS = $(ALIAS_DIRS_$(BUILD_TYPE))
ALIAS_FILES = $(ALIAS_DIRS:%=$(INSTDIR)/%/fonts.alias)

install_aliases: $(ALIAS_FILES)

$(INSTDIR)/%-ISO8859-1/fonts.alias.all: $(INSTDIR)/%/fonts.alias.all
	-rm -f $@
	mkdir -p $(@D)
	cp -pf $< $@

$(INSTDIR)/%/fonts.alias.all: fonts.alias.%
	-rm -f $@
	mkdir -p $(@:fonts.alias.all=)
	@if [[ -f $(@:.all=) ]] ; then \
		$(START_CMD_ECHO) ; \
		cat $(@:.all=) > $@ ; \
	fi
	cat $< >> $@

$(INSTDIR)/%/fonts.alias: $(INSTDIR)/%/fonts.alias.all
	(cd $(@:fonts.alias=) ; \
	 LD_LIBRARY_PATH="$(FONT_LIBS_PATH)" $(INSTALLALIAS) fonts.alias.all .)
