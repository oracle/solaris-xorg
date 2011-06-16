###############################################################################
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=xorg-docs

# Version number (used in path names)
MODULE_VERSION=1.6

# Checksums for upstream tarball
TARBALL_MD5  = 86101433834b485ab243f235757d7079
TARBALL_SHA1 = be4a0259d52a5f39acbe5aa34d213cceb90f59d8

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=	\
	X.man-decnet.patch,-p1 \
	solaris-manpages.patch

# Man pages to apply distro customizations to
# Normal attributes table not added, since these aren't simple interfaces
SUNTOUCHED_MANPAGES=man/*.man
SUNTOUCH_MAN_FLAGS=-r '{__distroname__, $(DISTRO_NAME)}'
SUNTOUCH_MAN_FLAGS_SET=yes

# Additional arguments to configure script
MODULE_CONFIG_OPTS= --docdir='$(X11_DOC_DIR)' \
	XORG_SGML_PATH=$(PROTODIR)$(X11_DIR)/share/sgml

# Additional rules to fix up unpacked source
MODULE_ADD_SOURCE_TARGETS = fix_entity_paths

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

### Include common rulesets
include ../Makefile.inc

# Fix entity paths in DocBook files so that gnome-help can resolve them
fix_entity_paths: $(UNPACK_TARGET)
	find $(SOURCE_DIR) -name '*.xml' | xargs $(PERL) -i.orig -p -e \
	 's{SYSTEM ".*/X11/defs.ent"}{SYSTEM "$(X11_DIR)/share/sgml/X11/defs.ent"}'

MAN_DIR=$(PROTODIR)$(X11_MAN_DIR)

install_docs: $(MAN_DIR)/man5/X11.5 $(MAN_DIR)/man5/XStandards.5

$(MAN_DIR)/man5/XStandards.5: default_install
	mv -f $(MAN_DIR)/man5/Standards.5 $@

$(MAN_DIR)/man5/X11.5:
	mkdir -p $(MAN_DIR)/man5
	$(INSTALL_SCRIPT) -m 0444 X11.5 $@

default_install: install_preformatted

install_preformatted:
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	(cd preformatted && tar -cf - .) | \
	 (cd $(PROTODIR)$(X11_DOC_DIR) && tar -xf - )
