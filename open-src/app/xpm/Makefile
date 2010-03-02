###############################################################################
#
# Xpm apps Makefile
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
#

# Package name used in tarballs
# Source is really in X.Org's libXpm module
MODULE_NAME=libXpm
SOURCE_TARBALL_DIR=lib
SOURCE_TARBALL_DIR_SET=yes

# Version number (used in path names)
MODULE_VERSION=3.5.8

# Checksums for upstream tarball
TARBALL_MD5  = 38e58e72d476a74298a59052fde185a3
TARBALL_SHA1 = 3bfc833ed4527f74b1c66c386da62271d0313413

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=cxpm/cxpm.man sxpm/sxpm.man
SUN_PACKAGE=SUNWxwplt
MODULE_STABILITY=Uncommitted

INSTALL_TARGETS=install_xpm_apps
INSTALL_TARGETS_SET=yes

# Additional arguments to pass to make
#MODULE_MAKEFLAGS= -e SUBDIRS="cxpm sxpm"

# Undo libtool use of libXpm.la to link with
#MODULE_BUILD_DEPS=delibtoolize

include ../Makefile.inc

install_xpm_apps: $(BUILD_TARGETS)
	(cd $(SOURCE_DIR)/cxpm ; $(MAKE) $(MAKEFLAGS) -e DESTDIR=$(PROTODIR) install)
	(cd $(SOURCE_DIR)/sxpm ; $(MAKE) $(MAKEFLAGS) -e DESTDIR=$(PROTODIR) install)
	mkdir -p $(PROTODIR)$(X11_LC_MESSAGES_DIR)
	cp -pf $(SOURCE_DIR)/cxpm/cxpm.po $(PROTODIR)$(X11_LC_MESSAGES_DIR)
	cp -pf $(SOURCE_DIR)/sxpm/sxpm.po $(PROTODIR)$(X11_LC_MESSAGES_DIR)
