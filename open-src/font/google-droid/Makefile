###############################################################################
#
# Droid Fonts Makefile
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
MODULE_NAME=google-droid-fonts

# Version number (used in path names)
MODULE_VERSION=2010.02.24

# Checksums for upstream tarball
TARBALL_MD5  = 6ea3273ac2868f72499276a562838289
TARBALL_SHA1 = 4eef8d0fc11231350cb3fe59cdd19716f192e311

# Source tarball
SOURCE_TARBALL_NAME=$(MODULE_NAME)-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Directory created when unpacking tarball
SOURCE_DIRNAME=base
SOURCE_DIRNAME_SET=yes

# Download site for source
# Upstream doesn't have a stable download tarball, so we use our mirror
# to get consistent checksums - updates will come from snapshot link on
# http://android.git.kernel.org/?p=platform/frameworks/base.git;a=tree;f=data/fonts;hb=HEAD
# Current version was downloaded on 2/24/2010 from 
# http://android.git.kernel.org/?p=platform/frameworks/base.git;a=snapshot;h=54e235b011be9e1e38a2b9be787d0a196493d89e;sf=tgz
SOURCE_URL=http://dlc.sun.com/osol/x/downloads/mirrors/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES=		# None

# License file
LICENSE_FILE=NOTICE
LICENSE_FILE_SET=yes

TTF_FONT_FILES = \
    $(LICENSE_FILE)		\
    DroidSans.ttf		\
    DroidSans-Bold.ttf		\
    DroidSerif-Regular.ttf	\
    DroidSerif-Bold.ttf		\
    DroidSerif-Italic.ttf	\
    DroidSerif-BoldItalic.ttf	\
    DroidSansMono.ttf		\
    DroidSansFallback.ttf

TTF_SUBDIR=google-droid
TTF_SUBDIR_SET=yes

### Include common rulesets
include ../Makefile.ttf
