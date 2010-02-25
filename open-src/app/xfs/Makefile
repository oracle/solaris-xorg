###############################################################################
#
# xfs Makefile
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
MODULE_NAME=xfs

# Version number (used in path names)
MODULE_VERSION=1.1.0

# Checksums for upstream tarball
TARBALL_MD5  = 5cfc06a320f74f07a0aa73da62307e9a
TARBALL_SHA1 = 040dee6ff7628c192874447fb178a965c744eea9

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	smf-manpage.patch \
	config-warning.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=*.man
SUN_PACKAGE=SUNWxwfs
MODULE_STABILITY=Committed

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= --sysconfdir=$(X11_DIR)/lib \
	--enable-syslog --enable-inetd --disable-devel-docs \
	--with-default-font-path='catalogue:/etc/X11/fontpath.d' \
	--with-default-config-file='/etc/X11/fs/config,$(X11_X11LIB_DIR)/fs/config,/etc/openwin/fs/fontserver.cfg,/usr/openwin/lib/X11/fontserver.cfg'

# Need to link with libfontenc in server private directory
MODULE_LDFLAGS=$(X11_SERVERLIBS_LDFLAGS)

MODULE_ADD_INSTALL_TARGETS = install_smf install_fsadmin

include ../Makefile.inc

# Make sure SMF manifest is valid before installing it
check_smf: xfs.xml
	/usr/sbin/svccfg validate xfs.xml

SMF_MANIFEST_DIR=/var/svc/manifest/application/x11

install_smf: check_smf
	mkdir -p $(PROTODIR)$(SMF_MANIFEST_DIR)
	$(INSTALL_SCRIPT) xfs.xml $(PROTODIR)$(SMF_MANIFEST_DIR)

install_fsadmin:
	mkdir -p $(PROTODIR)$(X11_DIR)/sbin \
		$(PROTODIR)$(X11_MAN_DIR)/man/man1
	$(INSTALL_SCRIPT) -m 0755 fsadmin $(PROTODIR)$(X11_DIR)/sbin
	$(INSTALL_SCRIPT) fsadmin.1 $(PROTODIR)$(X11_MAN_DIR)/man1
