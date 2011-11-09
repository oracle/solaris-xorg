###############################################################################
#
# xterm Makefile
#
# Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xterm

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=terminal/xterm
MODULE_PKGNAME_SET=yes

# resize goes into a separate package so it can be installed in zones without
# X, but which you login to from a resizable GUI terminal window.
RESIZE_PKGNAME=terminal/resize

# Package classification (override default)
MODULE_PKGCLASS=Applications/System Utilities
MODULE_PKGCLASS_SET=yes

# Package metadata
UPSTREAM	= Thomas Dickey
UPSTREAM_URL	= http://invisible-island.net/xterm/
UPSTREAM_SET	= yes

# Version number (used in path names)
MODULE_VERSION=271

# Checksums for upstream tarball
TARBALL_MD5  = e07b05bd3d3a666a8cf086fcc8d227f3
TARBALL_SHA1 = 53586916147cb88e71b3d620a7d1b80f1696749b

# Source tarball
SOURCE_URL=ftp://invisible-island.net/xterm/$(SOURCE_TARBALL_NAME:.tar.gz=.tgz)
SOURCE_URL_SET=yes

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = resize.man.patch,-p1 desktop-icon.patch,-p1

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=koi8rxterm.man resize.man uxterm.man xterm.man
MODULE_STABILITY=Committed

# Fix man page footers to not say "Last changed: X Window System"
XORGRELSTRING = $(MODULE_NAME) $(MODULE_VERSION)
  XORGMANNAME = X Version 11
  XVENDORVERS = "$(XORGRELSTRING)" "$(XORGMANNAME)"
MODULE_SUNTOUCH_MAN_FLAGS = -r '{__vendorversion__, $(XVENDORVERS)}'

# Override specific attributes for specific man pages
MODULE_SUNTOUCH_MAN_FLAGS += \
	$(SUNTOUCH_MAN_FLAGS_$(@F:%.man.$(FIXED_MAN_SUFFIX)=%))
SUNTOUCH_MAN_FLAGS_resize = -o '{Availability, $(RESIZE_PKGNAME)}'

# Paths to find Xft/fontconfig/freetype headers libraries at build time
# and run time 
FT_CFLAGS= -I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/include/freetype2
FT_LIBS= \
  -L$(PROTODIR)/usr/lib$(LIBSUBDIR) -R/usr/lib$(LIBSUBDIR) -lXft -lfontconfig

# Override the flags xterm's configure script generates, since it incorrectly
# adds -R flags pointing to the build proto area, which are not wanted.
MODULE_BUILD_MAKEFLAGS = \
  LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(MODTYPE_CFLAGS) $(MODTYPE_LDFLAGS)"

# Additional flags to pass to configure script
MODULE_CONFIG_OPTS = --enable-256-color --enable-88-color --enable-wide-chars \
  --with-freetype-cflags="$(FT_CFLAGS)" --with-freetype-libs="$(FT_LIBS)" \
  --disable-setuid --disable-setgid  --enable-luit --disable-imake \
  --x-includes=$(PROTODIR)$(X11_INCLUDES_DIR) --x-libraries=$(X11_LIB_DIR) \
  --with-app-defaults=/usr/share/X11/app-defaults \
  --with-icondir=/usr/share/pixmaps --enable-logging

# Sort-of emulate automake silent rules - run make configure V=1 to see 
# full compiler flags
MODULE_CONFIG_OPTS_VERBOSE_0 = --disable-echo
MODULE_CONFIG_OPTS_VERBOSE_1 = --enable-echo
MODULE_CONFIG_OPTS_VERBOSE_ = $(MODULE_CONFIG_OPTS_VERBOSE_0)
MODULE_CONFIG_OPTS += $(MODULE_CONFIG_OPTS_VERBOSE_$(V))

# Tell configure where to find luit binary
MODULE_CONFIG_ENV = LUIT=$(X11_BIN_DIR)/luit

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xterm resize

MODULE_ADD_INSTALL_TARGETS=install_docs install_desktop

include ../Makefile.inc

# Install documentation of xterm control sequences for packaging
MODULE_DOCDIR=/usr/share/doc/xterm
PROTO_CTLSEQS=$(PROTODIR)$(MODULE_DOCDIR)/ctlseqs.txt

install_docs: $(PROTO_CTLSEQS)

# Install desktop file to allow starting xterm from GNOME menu
DESKTOP_DIR=/usr/share/applications
PROTO_DESKTOP=$(PROTODIR)$(DESKTOP_DIR)/xterm.desktop

install_desktop: $(PROTO_DESKTOP)

$(PROTO_CTLSEQS) $(PROTO_DESKTOP): $(UNPACK_TARGET)
	mkdir -p $(@D)
	cp -pf $(SOURCE_DIR)/$(@F) $@

# Metadata for separate package for resize
RESIZE_PKG_METADATA_DIR      = $(MODULE_PKG_METADATA_DIR:%xterm=%resize)
RESIZE_PKG_ATTRDATA_FILE     = $(RESIZE_PKG_METADATA_DIR)/$(ATTRDATA_FILE_NAME)
RESIZE_PKG_LICENSE_FILE      = $(RESIZE_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME)
RESIZE_METADATA_TARGETS = \
        $(RESIZE_PKG_METADATA_DIR) \
        $(RESIZE_PKG_ATTRDATA_FILE) \
        $(RESIZE_PKG_LICENSE_FILE)

install_metadata: $(RESIZE_METADATA_TARGETS)

$(RESIZE_PKG_METADATA_DIR):
	mkdir -p $@

$(RESIZE_PKG_ATTRDATA_FILE) $(RESIZE_PKG_LICENSE_FILE): $(RESIZE_PKG_METADATA_DIR)
