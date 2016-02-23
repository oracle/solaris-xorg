###############################################################################
#
# Copyright (c) 2007, 2016, Oracle and/or its affiliates. All rights reserved.
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

# Package metadata
MODULE_DESC = X.Org documentation for the X Window System

# Version number (used in path names)
MODULE_VERSION=1.7.1

# Checksums for upstream tarball
TARBALL_MD5   = ce5a04d87b330b9091576b3410dc26d3
TARBALL_SHA1  = 19be186705145ca7231ca646b61208c9eba48ca7
TARBALL_SHA256= 24b8677c3462c10465cf50d40576d76682acd5835526093a575865b2aa242c4b

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += upstream-manpages.patch,-p1
SOURCE_PATCHES += solaris-manpages.patch,-p1

# Man pages to apply distro customizations to
# Normal attributes table not added, since these aren't simple interfaces
SUNTOUCHED_MANPAGES=man/*.man
SUNTOUCH_MAN_FLAGS=-r '{__distroname__, $(DISTRO_NAME)}'
SUNTOUCH_MAN_FLAGS_SET=yes

# Additional arguments to configure script
MODULE_CONFIG_OPTS= --docdir='$(X11_DOC_DIR)' \
	XORG_SGML_PATH=$(PROTODIR)$(X11_SGML_DIR:/X11=)

# Additional rules to fix up unpacked source
MODULE_ADD_SOURCE_TARGETS = fix_entity_paths

# Additional install rules beyond what upstream installs
MODULE_ADD_INSTALL_TARGETS=install_docs

### Include common rulesets
include ../Makefile.inc

# Fix entity paths in DocBook files so that gnome-help can resolve them
fix_entity_paths: $(UNPACK_TARGET)
	find $(SOURCE_DIR) -name '*.xml' | xargs $(PERL) -i.orig -p -e \
	 's{SYSTEM ".*/X11/defs.ent"}{SYSTEM "$(X11_SGML_DIR)/defs.ent"}'

MAN_DIR=$(PROTODIR)$(MODULE_MAN_DIR)

install_docs: $(MAN_DIR)/man7/X11.7 $(MAN_DIR)/man7/XStandards.7

$(MAN_DIR)/man7/XStandards.7: default_install
	mv -f $(MAN_DIR)/man7/Standards.7 $@

$(MAN_DIR)/man7/X11.7:
	mkdir -p $(MAN_DIR)/man7
	$(INSTALL_SCRIPT) -m 0444 X11.7 $@

default_install: install_preformatted

install_preformatted:
	mkdir -p $(PROTODIR)$(X11_DOC_DIR)
	(cd preformatted && tar -cf - .) | \
	 (cd $(PROTODIR)$(X11_DOC_DIR) && tar -xf - )
