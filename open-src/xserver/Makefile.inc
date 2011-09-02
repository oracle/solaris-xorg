# -*- Makefile -*- rules common to all xserver build trees
#
# Copyright (c) 2007, 2011, Oracle and/or its affiliates. All rights reserved.
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

PWD:sh=pwd
TOP=$(PWD)/../../..

MODTYPE=xserver

# Defaults for pkg names in this area
MODTYPE_PKGHIER=x11/server
MODTYPE_PKGHIER_SET=yes
MODULE_PKGTAIL_DEFAULT		= $(MODULE_PKGTAIL_SET:yes=$(POUND_SIGN))
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL:sh=basename $(pwd)
MODULE_PKGTAIL_SET	      	= yes

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

# Patches shared between Xorg & Xvnc builds
XORG_SOURCE_PATCHES:sh=sed '/^\#/ d' ../xorg/patch-list

SERVER_MODULES_DIR='$(X11_SERVERMODS_DIR)'
DRI_MODULES_DIR='$(X11_SERVERMODS_DIR)/dri'

# Changes to apply to man pages
MODTYPE_SUNTOUCH_MAN_FLAGS = -p $(MODULE_PREFIX)/bin/

# Command line options to GNU autoconf configure script for xorg-server
XSERVER_CONFIG_OPTS_COMMON = \
	--docdir='$(MODULE_DOC_DIR)' \
        --enable-xdm-auth-1 --enable-xdmcp --enable-xcsecurity \
	--enable-shared --disable-static \
	--enable-glx --enable-dri2 \
	--disable-xsdl --disable-xwin --disable-xnest \
	--disable-install-setuid \
	--with-vendor-name='$(VENDOR_NAME)' \
	--with-vendor-web='$(VENDOR_SUPPORT_URL)' \
	--with-builderstring='Solaris ABI: $(BUILD_TYPE)-bit' \
	--with-os-name="`uname -srm`" \
	--with-log-dir=/var/log --with-xkb-output=/var/run/xkb \
	--with-module-dir=$(SERVER_MODULES_DIR) \
	--with-dri-driver-path=$(DRI_MODULES_DIR) \
	--with-default-font-path='catalogue:$(X11_FONT_CATALOGUE)' \
	--with-xkb-bin-directory='$${exec_prefix}/bin' \
	--with-default-xkb-model=pc105 \
	$(ADDITIONAL_CONFIG_OPTS)

XSERVER_CONFIG_OPTS_sparc=$(XSERVER_CONFIG_OPTS_COMMON)
XSERVER_CONFIG_OPTS_i386=$(XSERVER_CONFIG_OPTS_COMMON)
XSERVER_CONFIG_OPTS=$(XSERVER_CONFIG_OPTS_$(MACH))

MODTYPE_CPPFLAGS_suncc= # No extra flags needed
MODTYPE_CPPFLAGS_gcc= # No extra flags needed

MODTYPE_CPPFLAGS=  \
	-I$(PROTODIR)/usr/include \
	-I$(PROTODIR)/usr/include/pixman-1 \
	-I$(PROTODIR)$(X11_INCLUDES_DIR) \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/drm \
	-I$(PROTODIR)$(X11_INCLUDES_DIR)/X11/dri \
	$(DPMS_DEFINES) -DSUNSOFT -DTSOL \
	$(MODTYPE_CPPFLAGS_$(MODULE_COMPILER))

# Environment variable options to GNU autoconf configure script
#  - XSERVERCFLAGS_CFLAGS - force CFLAGS to avoid -I /usr/X11/include breaking
#    use of headers from proto area
#  - PATH - include /usr/sbin for dtrace command (needed for building probes)
MODTYPE_CONFIG_ENV = \
	XSERVERCFLAGS_CFLAGS="-DHAS_FCHOWN -DHAS_STICKY_DIR_BIT -D_REENTRANT -D_POSIX_PTHREAD_SEMANTICS -DXTHREADS -DXUSE_MTSAFE_API" \
	PATH=$(PATH):/usr/sbin

# 64-bit X servers on SPARC crash on startup when built with Sun Studio 
# with -g unless -xmemalign=8i is also used (Sun bug #6545113)
MODTYPE_DEBUG_FLAGS_sparc_suncc = -xmemalign=8i
MODTYPE_DEBUG_FLAGS=$(MODTYPE_DEBUG_FLAGS_$(MACH)_$(MODULE_COMPILER))

MODTYPE_CFLAGS=$(PROG_CFLAGS)
MODTYPE_CPPFLAGS += $(PROG_CPPFLAGS)
MODTYPE_CXXFLAGS=$(PROG_CXXFLAGS)
MODTYPE_LDFLAGS=$(PROG_LDFLAGS)

# configure scripts add libraries like -lsocket & -lnsl to the global $(LIBS)
# when only some objects need them, so ignore them in the rest.
MODTYPE_LD_OPTIONS = -z ignore

# Environment variable options to use when building
#  - PATH - include /usr/sbin for dtrace command (needed for building probes)
MODTYPE_BUILD_ENV= PATH=$(PATH):/usr/sbin \
	LD_LIBRARY_PATH=$(PROTODIR)$(X11_SERVERLIBS_DIR)$(ARCHLIBSUBDIR):$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR)

# Mark packages as only compatible with the current server minor release series,
# since Xorg tends to break ABI's in each minor release train (1.8, 1.9, etc.)
MODTYPE_ATTRDATA_COMMANDS = @$(START_CMD_ECHO) ; \
	print "depend fmri=pkg:/x11/server/xserver-common@$(MIN_XSERVER_VERS) type=require" >> $@ ; \
	print "depend fmri=pkg:/x11/server/xorg@$(NEXT_XSERVER_VERS) type=exclude" >> $@

### Include common rulesets
include $(TOP)/open-src/common/Makefile.inc

# Fix paths in proto area to match paths in packages / installed systems
install_fix_paths: install_fix_paths_$(MACH)
install_fix_paths_sparc: # not needed
install_fix_paths_i386: install_fix_paths_amd64_links

# We used to deliver both 32-bit & 64-bit binaries on x86 and use isaexec to
# choose between them.   Now we just deliver 64-bit and make symlinks from
# the amd64 paths to the default paths for compatibility

ISAEXEC_DIR = $(PROTODIR)$(MODULE_PREFIX)/bin/$(SUBDIR64)
ISAEXEC_LINKS = $(LINKED_TO_ISAEXEC:%=$(ISAEXEC_DIR)/%)

install_fix_paths_amd64_links: $(ISAEXEC_LINKS)

$(ISAEXEC_DIR):
	mkdir -p $@

$(ISAEXEC_LINKS): $(ISAEXEC_DIR)
	@rm -f $@
	ln -s ../$(@F) $@
