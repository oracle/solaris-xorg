###############################################################################
#
# Bitstream Vera Fonts Makefile
#
# Copyright (c) 2005, 2010, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=ttf-bitstream-vera

# Package metadata
UPSTREAM	= GNOME Foundation
UPSTREAM_URL	= http://www.gnome.org/fonts/
UPSTREAM_SET	= yes

MODULE_LICENSE          = Bitstream Vera Fonts License
MODULE_LICENSE_SET      = yes

# Version number (used in path names)
MODULE_VERSION=1.10

# Checksums for upstream tarball
TARBALL_MD5  = bb22bd5b4675f5dbe17c6963d8c00ed6
TARBALL_SHA1 = c3e8505a58d52e198a02960a2bd295e4c7a14242

# Download site for source
SOURCE_URL=http://ftp.gnome.org/pub/GNOME/sources/ttf-bitstream-vera/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=		# None

# Directory to install to
TTF_SUBDIR=bitstream-vera
TTF_SUBDIR_SET=yes

# License file
LICENSE_FILE=COPYRIGHT.TXT
LICENSE_FILE_SET=yes

TTF_FONT_FILES= \
	COPYRIGHT.TXT	\
	Vera.ttf	\
	VeraBI.ttf	\
	VeraBd.ttf	\
	VeraIt.ttf	\
	VeraMoBI.ttf	\
	VeraMoBd.ttf	\
	VeraMoIt.ttf	\
	VeraMono.ttf	\
	VeraSe.ttf	\
	VeraSeBd.ttf

### Include common rulesets
include ../Makefile.ttf

