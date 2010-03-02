###############################################################################
#
# xf86-video-vesa Makefile
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
MODULE_NAME=xf86-video-vesa

# Version number (used in path names)
MODULE_VERSION=2.2.1

# Checksums for upstream tarball
TARBALL_MD5  = 61a1dc9a22991bd04d0ff98f800775c1
TARBALL_SHA1 = 086adbf06e9043ed2c18d887fb32ce605324876e

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = vesa_xorg72.patch

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
SUN_PACKAGE=SUNWxorg-server
MODULE_STABILITY=Volatile

MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-R$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir) \
	-R$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR) \
	-lfb -lint10 -lpciaccess -lshadow -lvbe

include ../Makefile.inc
