###############################################################################
#
# xrx 1.x Makefile
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
MODULE_NAME=xrx

# pkg(5) name we deliver the files in (override default)
MODULE_PKGNAME=web/browser/firefox/plugin/firefox-xrx
MODULE_PKGNAME_SET=yes

# Package classification (override default)
MODULE_PKGCLASS=Applications/Plug-ins and Run-times
MODULE_PKGCLASS_SET=yes

# Additional metadata to include in pkg
MODULE_ATTRDATA_COMMANDS = \
	print 'set name=info.upstream_url' \
		'value="http://www.broadwayinfo.com/"' >> $@

# Version number (used in path names)
MODULE_VERSION=1.0.4

# Checksums for upstream tarball
TARBALL_MD5  = c93be22d20658ac5ff60469207dec118
TARBALL_SHA1 = ff5b8c329ed255138eb78e6b9c034add2b02de94

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES= 

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=helper/xrx.man plugin/libxrx.man
MODULE_STABILITY=Committed

# Additional flags to pass to configure script:
# PLUGIN_CFLAGS - force use of bundled browser plugin headers instead
# of trying to keep up with the many incompatible changes from Firefox
MODULE_CONFIG_OPTS = PLUGIN_CFLAGS='-I$$(top_srcdir)/plugin/include'
# Disable build of useless lib*.a files for browser plugin loadable modules
MODULE_CONFIG_OPTS += --enable-shared=yes --enable-static=no 
# Install directly to firefox plugin directory
MODULE_CONFIG_OPTS += --libdir='/usr/lib/firefox/plugins'

# Compatibility links from /usr/X11/bin to /usr/bin
MODULE_X11_BINCOMPAT_LINKS = xrx

include ../Makefile.inc
