###############################################################################
#
# Xpm apps Makefile
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
# Source is really in X.Org's libXpm module
MODULE_NAME=libXpm
SOURCE_TARBALL_DIR=lib
SOURCE_TARBALL_DIR_SET=yes

# Version number (used in path names)
MODULE_VERSION=3.5.8

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
