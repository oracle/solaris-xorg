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

MODULE_NAME=xmakemap

# No upstream sources for these files, since they're Sun created
MODULE_VERSION=src
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes
ADDITIONAL_SOURCE_DIR=sun-src

# Don't need default targets for configure, build, or install, since there's
# nothing to build, and the install rule follows here
CONFIGURE_TARGETS = 
CONFIGURE_TARGETS_SET=yes
BUILD_TARGETS = 
BUILD_TARGETS_SET=yes
INSTALL_TARGETS = install_xmakemap
INSTALL_TARGETS_SET=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
SUN_PACKAGE=SUNWxwman
MODULE_STABILITY=Committed

### Include common rulesets
include ../Makefile.inc

DESTDIR=$(PROTODIR)

install_xmakemap: $(SOURCE_TARGETS) $(INSTALL_DEPS)
	mkdir -p $(DESTDIR)$(X11_BIN_DIR) \
		$(DESTDIR)$(X11_MAN_DIR)/man1
	$(INSTALL_SCRIPT) -m 0755 $(SOURCE_DIR)/xmakemap \
		$(DESTDIR)$(X11_BIN_DIR)
	$(INSTALL_SCRIPT) $(SOURCE_DIR)/xmakemap.man \
		$(DESTDIR)$(X11_MAN_DIR)/man1/xmakemap.1

