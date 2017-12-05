###############################################################################
#
# xf86-video-vboxvideo Makefile
#
# Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
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
MODULE_NAME=xf86-video-vboxvideo

ORACLE_TPNO     = 39608
ORACLE_TPNO_SET = yes

# Version number (used in path names)
MODULE_VERSION=1.0.0

# Checksums for upstream tarball
TARBALL_MD5   = 42c840707d0e5e3459ab615c9814578f
TARBALL_SHA1  = 219e0efdcc5afbd048aed4b780833765f01624bf
TARBALL_SHA256 = 7fb6a3bfbcbe95438617f55a2f7ace4c0edec8ea8b7007777f389438b40cbfa4

# ARC cases that covered this module
# PSARC/2017/256 Solaris Xorg driver for the VirtualBox virtual video card
MODULE_ARC_CASES = PSARC/2017/256

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES =

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=man/*.man
MODULE_STABILITY=Volatile

MODULE_COMPILER=gcc
MODULE_COMPILER_SET=yes

# Libraries/modules to link with
MODULE_LD_OPTIONS= \
	-L$(PROTODIR)$(X11_SERVERMODS_DIR) \
	-R$(X11_SERVERMODS_DIR) \
	-lfb -lpciaccess -lshadow -lvbe -lvgahw -lshadowfb

include ../Makefile.inc
