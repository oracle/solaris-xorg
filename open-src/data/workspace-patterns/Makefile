###############################################################################
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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

CONFIGURE_TARGETS =
CONFIGURE_TARGETS_SET=yes

BUILD_TARGETS =
BUILD_TARGETS_SET=yes

INSTALL_TARGETS = install_patterns
INSTALL_TARGETS_SET=yes

### Include common rulesets
include ../Makefile.inc

DESTDIR=$(PROTODIR)
INSTDIR=$(DESTDIR)/$(X11_DIR)/share/patterns/

install_patterns:
	mkdir -p $(INSTDIR)
	cp -pf attributes $(INSTDIR)
	cp -pf background.xbm $(INSTDIR)
	cp -pf bricks.xbm $(INSTDIR)
	cp -pf burl.xbm $(INSTDIR)
	cp -pf corn.xbm $(INSTDIR)
	cp -pf dmndpla.xbm $(INSTDIR)
	cp -pf dolphin.xbm $(INSTDIR)
	cp -pf dotwave2.xbm $(INSTDIR)
	cp -pf dune.xbm $(INSTDIR)
	cp -pf flowers.xbm $(INSTDIR)
	cp -pf flyseye.xbm $(INSTDIR)
	cp -pf foreground.xbm $(INSTDIR)
	cp -pf fuji.xbm $(INSTDIR)
	cp -pf geiger2.xbm $(INSTDIR)
	cp -pf gray25.xbm $(INSTDIR)
	cp -pf gray50.xbm $(INSTDIR)
	cp -pf gray75.xbm $(INSTDIR)
	cp -pf miniweave.xbm $(INSTDIR)
	cp -pf sbump.xbm $(INSTDIR)
	cp -pf sizmk2.xbm $(INSTDIR)
	cp -pf southwest.xbm $(INSTDIR)
	cp -pf squares.xbm $(INSTDIR)
	cp -pf sunmulti.xbm $(INSTDIR)
	cp -pf swirl2.xbm $(INSTDIR)
	cp -pf wave1.xbm $(INSTDIR)
	cp -pf wrnkle2.xbm $(INSTDIR)
