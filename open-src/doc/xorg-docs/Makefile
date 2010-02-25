###############################################################################
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xorg-docs

# Version number (used in path names)
MODULE_VERSION=1.5

# Checksums for upstream tarball
TARBALL_MD5  = 359ac83ad27eecd5588914ba8715301d
TARBALL_SHA1 = e4a0fdf848e9d5baa2de625ce74f204de37e0fb0

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	solaris-manpages.patch

# Makefiles require gmake -i to build all targets
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes
MODULE_MAKEFLAGS= MAKEFLAGS="-i"

# Additional arguments to configure script
MODULE_CONFIG_OPTS=--enable-txt --disable-pdf --disable-ps --disable-html \
	--docdir='$(X11_DOC_DIR)' \
	XORG_SGML_PATH=$(PROTODIR)$(X11_DIR)/share/sgml

# Module dependencies
MODULE_CONFIGURE_DEPS=$(DOCBOOK2TEXT) $(XORG_DEFS_ENT)

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

### Include common rulesets
include ../Makefile.inc

SGML_DIR=$(SOURCE_DIR)/sgml
DOC_DIR=$(PROTODIR)$(X11_DOC_DIR)
MAN_DIR=$(PROTODIR)$(X11_MAN_DIR)

install_docs: $(INSTALL_DEPS)
	mkdir -p $(DOC_DIR) $(MAN_DIR)/man5
	cp -p $(SGML_DIR)/LICENSE.txt		$(DOC_DIR)/LICENSE
	cp -p $(SGML_DIR)/RELNOTES.txt		$(DOC_DIR)/RELNOTES
	cp -p $(SGML_DIR)/input/XKB-Config.txt	$(DOC_DIR)/README.XKB-Config
	cp -p $(SGML_DIR)/input/XKB-Enhancing.txt $(DOC_DIR)/README.XKB-Enhancing
	cp -p $(SGML_DIR)/fonts/fonts.txt	$(DOC_DIR)/README.fonts
	cp -p X11.5                             $(MAN_DIR)/man5/X11.5
