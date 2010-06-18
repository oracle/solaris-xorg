###############################################################################
#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
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

# This makefile simply creates a number of compatibility links in the
# proto area that we include in our packages

# Package name used in tarballs
MODULE_NAME=compat-links

# No sources
MODULE_VERSION=NONE
SOURCE_TARBALL_NAME=NONE
SOURCE_TARBALL_NAME_SET=yes

# Don't need default targets for configure, build, or install, since there's
# nothing to build, and the install rule follows here
CONFIGURE_TARGETS = 
CONFIGURE_TARGETS_SET=yes
BUILD_TARGETS = 
BUILD_TARGETS_SET=yes
INSTALL_TARGETS = install_links
INSTALL_TARGETS_SET=yes

### Include common rulesets
include ../Makefile.inc

DESTDIR=$(PROTODIR)

# Even though we deliver all our files to /usr/{bin,lib} now, other 
# consolidations still deliver to /usr/X11, so we have to maintain
# /usr/X11, which means leaving symlinks there to our programs & libraries
# to avoid breaking existing scripts and binaries
$(DESTDIR)/usr/X11:
	mkdir -p $@

# The SVID ABI established the X->X11 link
# PSARC 2004/187 established the X11R6->X11 link for compatibility with Linux
# PSARC 2008/405 removed /usr/openwin and replaced it with a link to /usr/X11
INSTALL_LINKS = $(DESTDIR)/usr/X $(DESTDIR)/usr/X11R6 $(DESTDIR)/usr/openwin

$(INSTALL_LINKS): $(DESTDIR)/usr/X11
	@rm -f $@
	ln -s X11 $@

install_links: $(INSTALL_LINKS)
