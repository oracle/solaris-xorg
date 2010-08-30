# Makefile for X Consolidation
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
###############################################################################
#

PWD:sh=pwd
TOP=$(PWD)

OS_SUBDIRS_common = open-src pkg
OS_SUBDIRS_sparc = $(OS_SUBDIRS_common)
OS_SUBDIRS_i386 = $(OS_SUBDIRS_common)

all: setup install check

setup: open-src/common/Makefile.options

# Choose options for branding, download sites, etc.
# Makefile.options is created as a link to a file containing the desired
# options, chosen by the first of these found in open-src/common:
#	1) Makefile.options.$(X_BUILD_OPTIONS)
#	2) Makefile.options.<last two components of `domainname`, lowercased>
#	3) Makefile.options.opensolaris
MK_OPTS = open-src/common/Makefile.options

$(MK_OPTS):
	@ if [[ -n "${X_BUILD_OPTIONS}" ]] ; then \
	    X_BUILD_OPTIONS="${X_BUILD_OPTIONS}" ; \
	    if [[ ! -f "$(MK_OPTS).$${X_BUILD_OPTIONS}" ]] ; then \
		print -u2 "Invalid X_BUILD_OPTIONS: " \
			"$(MK_OPTS).$${X_BUILD_OPTIONS} not found" ; \
		exit 1 ; \
	    fi \
	else \
	    X_BUILD_OPTIONS="$$(domainname | \
		awk -F. '{if (NF > 1) {printf "%s.%s", $$(NF-1), $$NF}}')" ; \
	    typeset -l X_BUILD_OPTIONS ; \
	    if [[ ! -f "$(MK_OPTS).$${X_BUILD_OPTIONS}" ]] ; then \
		X_BUILD_OPTIONS='opensolaris' ; \
	    fi \
	fi ; \
	print "Choosing build options from $(MK_OPTS).$${X_BUILD_OPTIONS}" ; \
	rm -f $@ ; \
	ln -s "Makefile.options.$${X_BUILD_OPTIONS}" $@

# install & check are run in each subdir via Makefile.subdirs 

### Include common definitions
DIRNAME=""
include $(TOP)/open-src/common/Makefile.subdirs
