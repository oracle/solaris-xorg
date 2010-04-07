###############################################################################
#
# Copyright (c) 1995, 2010, Oracle and/or its affiliates. All rights reserved.
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

MODULE_NAME=workspace-patterns

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

INSTALL_TARGETS = install_patterns
INSTALL_TARGETS_SET=yes

### Include common rulesets
include ../Makefile.inc

DESTDIR=$(PROTODIR)
INSTDIR=$(DESTDIR)$(X11_DIR)/share/patterns/

PATTERN_FILES = \
	attributes      \
	background.xbm  \
	bricks.xbm      \
	burl.xbm	\
	corn.xbm	\
	dmndpla.xbm     \
	dolphin.xbm     \
	dotwave2.xbm    \
	dune.xbm	\
	flowers.xbm     \
	flyseye.xbm     \
	foreground.xbm  \
	fuji.xbm	\
	geiger2.xbm     \
	gray25.xbm      \
	gray50.xbm      \
	gray75.xbm      \
	miniweave.xbm   \
	sbump.xbm       \
	sizmk2.xbm      \
	southwest.xbm   \
	squares.xbm     \
	sunmulti.xbm    \
	swirl2.xbm      \
	wave1.xbm	\
	wrnkle2.xbm

install_patterns:
	mkdir -p $(INSTDIR)
	cp -pf $(PATTERN_FILES) $(INSTDIR)
