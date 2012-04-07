###############################################################################
#
# xfs Makefile
#
# Copyright (c) 2009, 2012, Oracle and/or its affiliates. All rights reserved.
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
MODULE_VERSION=1.1.2

# Checksums for upstream tarball
TARBALL_MD5   = 39e507a7a1b656885c4828a915e5805b
TARBALL_SHA1  = d4a64e12f8a773bd0120977191586ca12baf1ace
TARBALL_SHA256= 0dea61cce3897b4d9d844bbd2c7850cd4e80e12c12b38158b4f92574b027e39d

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	smf-manpage.patch \
	config-warning.patch

# Need to rebuild config/Makefile.in after applying solaris-make.patch
AUTORECONF=yes

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/xfs.man
MODULE_STABILITY=Committed

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=x11/xfs
MODULE_PKGNAME_SET=yes

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
