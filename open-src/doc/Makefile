# Makefile for X Consolidation Open Source Documentation tools & modules
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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
###############################################################################

OS_SUBDIRS_common = \
	docbook-utils \
	xorg-sgml-doctools \
	xorg-docs

OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)
OS_SUBDIRS_i386  = $(OS_SUBDIRS_common)

# Need to install to proto area by default for inter-package dependencies to
# work right.
default_target: install

# Dependencies for parallel make
xorg-docs: docbook-utils xorg-sgml-doctools
docbook-utils: xorg-sgml-doctools

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
DIRNAME="docs/"
include $(TOP)/common/Makefile.subdirs


