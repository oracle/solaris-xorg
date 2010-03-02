###############################################################################
#
# xf86-video-openchrome Makefile
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
MODULE_NAME=xf86-video-openchrome

# Version number (used in path names)
MODULE_VERSION=0.2.904

# Checksums for upstream tarball
TARBALL_MD5  = f2481d98ef54febf5bffbb88a2a2426d
TARBALL_SHA1 = 23225057d5b75d6aac3e7a710cbc067b2eb29228

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-graphics-ddx
MODULE_STABILITY=Volatile

# Extra flags to pass to configure script
# - Disable DRI since the kernel module isn't ported
MODULE_CONFIG_OPTS=--disable-dri

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-lfb -lint10 -lpciaccess -lshadow -lvbe \
	-lexa -lxaa -lvgahw -lshadowfb -lm

include ../Makefile.inc
