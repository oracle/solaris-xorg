###############################################################################
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.3	07/01/31
#

# Package name used in tarballs
DOC_NAME=xorg-docs

# Version number (used in path names)
VERSION=1.4

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	docbook-errors.patch,-p1

# Makefiles require gmake to build all targets
build_gen:= MAKE=$(GNUMAKE) -i

DOC_CONFIG_OPTS=--enable-txt --disable-pdf --disable-ps --disable-html

### Include common rulesets
include ../Makefile.inc

SGML_DIR=$(SOURCE_DIR)/sgml
DOC_DIR=$(PROTODIR)$(X11_DOC_DIR)

source_gen::
	perl -i~ -p -e 's/&#65533;/&nbsp;/g' \
		$(SGML_DIR)/*.sgml $(SGML_DIR)/*/*.sgml

install_gen::
	mkdir -p $(DOC_DIR)
	cp -p $(SGML_DIR)/LICENSE.txt		$(DOC_DIR)/LICENSE
	cp -p $(SGML_DIR)/RELNOTES.txt		$(DOC_DIR)/RELNOTES
	cp -p $(SGML_DIR)/input/XKB-Config.txt	$(DOC_DIR)/README.XKB-Config
	cp -p $(SGML_DIR)/input/XKB-Enhancing.txt $(DOC_DIR)/README.XKB-Enhancing
	cp -p $(SGML_DIR)/fonts/fonts.txt	$(DOC_DIR)/README.fonts
