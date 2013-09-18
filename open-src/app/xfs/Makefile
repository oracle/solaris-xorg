###############################################################################
#
# xfs - X11 font server
#
# Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xfs

# Version number (used in path names)
MODULE_VERSION=1.1.3

# Checksums for upstream tarball
TARBALL_MD5   = 38a5cb2d9103b91b4497310393a88d6d
TARBALL_SHA1  = 68b9c6032f4ac91334a9ad07b0c58a89841e83b1
TARBALL_SHA256= 7bf44a32ad04dbbcf3b5eef2cbfe879459378b312cb795e0d2d25e665f7196b7

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES += smf-manpage.patch
SOURCE_PATCHES += config-warning.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xfs.man
MODULE_STABILITY=Committed

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/xfs
MODULE_PKGNAME_SET=yes

# Package metadata
MODULE_DESC = the X Font Server (xfs)

# Dates to show in Oracle copyright notice in pkg license file
# Earliest & latest of the copyrights in the Oracle files in this pkg
ORACLE_COPYRIGHT_YEARS = 1994, 2010

# ARC cases that covered this module
# PSARC/2004/752 X Consolidation conversion to SMF
MODULE_ARC_CASES = PSARC/2004/752
# LSARC/2008/009 X11R7 font server and utilities
MODULE_ARC_CASES += LSARC/2008/009

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= --sysconfdir=$(X11_DIR)/lib \
	--enable-syslog --enable-inetd --disable-devel-docs \
	--with-default-font-path='catalogue:/etc/X11/fontpath.d' \
	--with-default-config-file='/etc/X11/fs/config,$(X11_X11LIB_DIR)/fs/config,/etc/openwin/fs/fontserver.cfg,/usr/openwin/lib/X11/fontserver.cfg'

# Need to link with libfontenc in server private directory
MODULE_LDFLAGS=$(X11_SERVERLIBS_LDFLAGS)

# ignore some unneeded dependencies that upstream includes:
# unreferenced object=/usr/lib/64/libFS.so.5; unused dependency of /usr/bin/xfs
MODULE_LDFLAGS += -z ignore

MODULE_ADD_INSTALL_TARGETS = install_smf install_fsadmin

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xfs

include ../Makefile.inc

# Make sure SMF manifest is valid before installing it
check_smf: xfs.xml
	/usr/sbin/svccfg validate xfs.xml

X11_SMF_MANIFEST_DIR=$(SMF_MANIFEST_DIR)/application/x11

install_smf: check_smf
	mkdir -p $(PROTODIR)$(X11_SMF_MANIFEST_DIR)
	$(INSTALL_SCRIPT) -m 0444 xfs.xml $(PROTODIR)$(X11_SMF_MANIFEST_DIR)

install_fsadmin:
	mkdir -p $(PROTODIR)$(X11_DIR)/sbin \
		$(PROTODIR)$(X11_MAN_DIR)/man1
	$(INSTALL_SCRIPT) -m 0755 fsadmin $(PROTODIR)$(X11_DIR)/sbin/fsadmin
	$(INSTALL_SCRIPT) -m 0444 fsadmin.1 $(PROTODIR)$(X11_MAN_DIR)/man1/

# Special compatibility link from /usr/X11/bin/fsadmin to /usr/sbin/fsadmin
install: $(X11_BINCOMPAT_DIR)/fsadmin

$(X11_BINCOMPAT_DIR)/fsadmin: $(X11_BINCOMPAT_DIR)
	@rm -f $@
	ln -s ../../sbin/fsadmin $@
