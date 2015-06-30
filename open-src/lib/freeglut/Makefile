###############################################################################
#
# FreeGLUT 2.8.1 Makefile
#
# Copyright (c) 2010, 2015, Oracle and/or its affiliates. All rights reserved.
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

# Package name used in tarballs
MODULE_NAME=freeglut

# Version number (used in path names)
MODULE_VERSION=2.8.1

# Checksums for upstream tarball
TARBALL_MD5  = 918ffbddcffbac83c218bc52355b6d5a
TARBALL_SHA1 = 7330b622481e2226c0c9f6d2e72febe96b03f9c4
TARBALL_SHA256 = dde46626a62a1cd9cf48a11951cdd592e7067c345cffe193a149dfd47aef999a

# ARC cases that covered this module
MODULE_ARC_CASES = LSARC/2010/016
# PSARC/2015/247 FreeGLUT 2.8.1
MODULE_ARC_CASES += PSARC/2015/247

# Package metadata
UPSTREAM = The FreeGLUT Project
UPSTREAM_URL = http://freeglut.sourceforge.net/
UPSTREAM_SET = yes
ORACLE_TPNO = 22885

# Download site for source
SOURCE_URL = $(SF_DOWNLOADS_URL)/freeglut/freeglut/$(MODULE_VERSION)/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET = yes

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = 

# Source tarball
SOURCE_TARBALL_NAME = freeglut-$(MODULE_VERSION).tar.gz
SOURCE_TARBALL_NAME_SET=yes

# Library name (used for specfiles/mapfiles)
LIBNAME=glut

# Man pages to apply Sun footer to & attributes to list
SUNTOUCHED_MANPAGES=
MODULE_STABILITY=Uncommitted
LIB_MAN_SUFFIX=

# We must build with GNU make
MODULE_MAKE=$(GNUMAKE)
MODULE_MAKE_SET=yes

# Command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS  = --x-includes=/usr/include
MODULE_CONFIG_OPTS += --x-libraries=/usr/lib$(ARCHLIBSUBDIR)

#  FreeGLUT really wants to know that we are POSIX friendly
MODULE_CPPFLAGS = -D__posix__ -D__unix__

# Need to find libdrm in protodir at build time, since libGL links to it
MODULE_LDFLAGS= -L$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR)

# ignore some unneeded dependencies that upstream includes:
#   unreferenced object=libXext.so.0; unused dependency of libglut.so.3.9.0
#   unreferenced object=libXxf86vm.so.1; unused dependency of libglut.so.3.9.0
#   unreferenced object=libXi.so.5; unused dependency of libglut.so.3.9.0
MODULE_LD_OPTIONS = $(ZDISCARD_UNUSED_DEP)

# Additional targets to install
MODULE_ADD_INSTALL_TARGETS = install_doc

include ../Makefile.inc

# FreeGLUT's $(VERSION_INFO) breaks delibtoolize.pl
DELIBTOOLIZE=no

install: install_extra_symlinks

install_extra_symlinks:
	( cd $(PROTODIR)/usr/lib/GL ; \
	    ln -sf ../libglut.so.3.9.0 libglut.so.3 ; \
	    ln -sf ../libglut.so.3.9.0 libglut.so ; \
	    cd - )
	( cd $(PROTODIR)/usr/lib/GL/$(SUBDIR64) ; \
	    ln -sf ../../$(SUBDIR64)/libglut.so.3.9.0 libglut.so.3 ; \
	    ln -sf ../../$(SUBDIR64)/libglut.so.3.9.0 libglut.so ; \
	    cd - )

FREEGLUT_DOC=$(PROTODIR)$(MODULE_DOC_DIR)/freeglut_user_interface.html	
install_doc: $(FREEGLUT_DOC)

$(FREEGLUT_DOC): $(INSTALL_DEPS) 
	$(INSTALL_SCRIPT) -c -m 0444 \
	    $(SOURCE_DIR)/doc/freeglut_user_interface.html $@

