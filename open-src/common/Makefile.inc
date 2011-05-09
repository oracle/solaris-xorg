# -*- Makefile -*- rules commonly shared among X consolidation open source dirs
#
# Copyright (c) 2002, 2011, Oracle and/or its affiliates. All rights reserved.
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

### Default rules

# Which compiler to use to build module
MODTYPE_COMPILER_DEFAULT	= $(MODTYPE_COMPILER_SET:yes=$(POUND_SIGN))
$(MODTYPE_COMPILER_DEFAULT)	MODTYPE_COMPILER = $(DEFAULT_COMPILER)
MODULE_COMPILER_DEFAULT		= $(MODULE_COMPILER_SET:yes=$(POUND_SIGN))
$(MODULE_COMPILER_DEFAULT)	MODULE_COMPILER = $(MODTYPE_COMPILER)

# Which make to use to build module
MODTYPE_MAKE_DEFAULT		= $(MODTYPE_MAKE_SET:yes=$(POUND_SIGN))
$(MODTYPE_MAKE_DEFAULT)		MODTYPE_MAKE = $(MAKE)
MODULE_MAKE_DEFAULT		= $(MODULE_MAKE_SET:yes=$(POUND_SIGN))
$(MODULE_MAKE_DEFAULT)		MODULE_MAKE = $(MODTYPE_MAKE)

# Where to install files
MODTYPE_PREFIX_DEFAULT		= $(MODTYPE_PREFIX_SET:yes=$(POUND_SIGN))
$(MODTYPE_PREFIX_DEFAULT)	MODTYPE_PREFIX=$(X11_DIR)
MODULE_PREFIX_DEFAULT		= $(MODULE_PREFIX_SET:yes=$(POUND_SIGN))
$(MODULE_PREFIX_DEFAULT)	MODULE_PREFIX=$(MODTYPE_PREFIX)

# Where to install documentation files
MODTYPE_DOC_DIR_DEFAULT		= $(MODTYPE_DOC_DIR_SET:yes=$(POUND_SIGN))
$(MODTYPE_DOC_DIR_DEFAULT)	MODTYPE_DOC_DIR=$(MODULE_PREFIX)/share/doc/$(MODULE_NAME)
MODULE_DOC_DIR_DEFAULT		= $(MODULE_DOC_DIR_SET:yes=$(POUND_SIGN))
$(MODULE_DOC_DIR_DEFAULT)	MODULE_DOC_DIR=$(MODTYPE_DOC_DIR)

# Where to install pkgconfig files
MODTYPE_PKGCONFIG_DIR_DEFAULT	= $(MODTYPE_PKGCONFIG_DIR_SET:yes=$(POUND_SIGN))
$(MODTYPE_PKGCONFIG_DIR_DEFAULT) MODTYPE_PKGCONFIG_DIR=$(PKGCONFIG_DIR)
MODULE_PKGCONFIG_DIR_DEFAULT	= $(MODULE_PKGCONFIG_DIR_SET:yes=$(POUND_SIGN))
$(MODULE_PKGCONFIG_DIR_DEFAULT)	MODULE_PKGCONFIG_DIR=$(MODTYPE_PKGCONFIG_DIR)

# Package files will be delivered in
MODTYPE_PKGHIER_DEFAULT		= $(MODTYPE_PKGHIER_SET:yes=$(POUND_SIGN))
$(MODTYPE_PKGHIER_DEFAULT)	MODTYPE_PKGHIER=x11/$(MODTYPE)
MODULE_PKGTAIL_DEFAULT		= $(MODULE_PKGTAIL_SET:yes=$(POUND_SIGN))
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL_CMD=print $(MODULE_NAME) | \
						   tr '[:upper:]' '[:lower:]'
$(MODULE_PKGTAIL_DEFAULT)	MODULE_PKGTAIL=$(MODULE_PKGTAIL_CMD:sh)
MODULE_PKGNAME_DEFAULT		= $(MODULE_PKGNAME_SET:yes=$(POUND_SIGN))
$(MODULE_PKGNAME_DEFAULT)	MODULE_PKGNAME=$(MODTYPE_PKGHIER)/$(MODULE_PKGTAIL)

# Package manifest file
PKG_SRC_DIR			= $(TOP)/pkg
PKG_MANIFEST_DIR		= $(PKG_SRC_DIR)/manifests
PKG_LICENSE_DIR			= $(PKG_SRC_DIR)/license_files
MODULE_PKG_MANIFEST_NAME_CMD	= print $(MODULE_PKGNAME) | tr '/' '-'
MODULE_PKG_MANIFEST_NAME	= $(MODULE_PKG_MANIFEST_NAME_CMD:sh)
MODULE_PKG_MANIFEST = $(PKG_MANIFEST_DIR)/$(MODULE_PKG_MANIFEST_NAME).p5m

# Package classification - must be a valid value recognized by pkg gui
MODULE_PKGCLASS_DEFAULT		= $(MODULE_PKGCLASS_SET:yes=$(POUND_SIGN))
$(MODULE_PKGCLASS_DEFAULT)	MODULE_PKGCLASS=System/X11

# Directory created by unpacking source, if not set in module makefile
SOURCE_DIRNAME_DEFAULT		= $(SOURCE_DIRNAME_SET:yes=$(POUND_SIGN))
$(SOURCE_DIRNAME_DEFAULT)	SOURCE_DIRNAME=$(MODULE_NAME)-$(MODULE_VERSION)

BUILD_DIR_$(BUILD_TYPE) = build-$(BUILD_TYPE)
# Special cases for 32-bit & 64-bit directory names to allow building
# both architectures in the same tree (serially - building both in
# parallel is likely to have race conditions)
BUILD_DIR_32 = build-$(MACH32)
BUILD_DIR_64 = build-$(MACH64)
BUILD_DIR = $(BUILD_DIR_$(BUILD_TYPE))

SOURCE_DIR = $(BUILD_DIR)/$(SOURCE_DIRNAME)
SOURCE_DIR_32 = $(BUILD_DIR_32)/$(SOURCE_DIRNAME)
SOURCE_DIR_64 = $(BUILD_DIR_64)/$(SOURCE_DIRNAME)

# Set default tarball if not set in module makefile
SOURCE_TARBALL_NAME_DEFAULT	= $(SOURCE_TARBALL_NAME_SET:yes=$(POUND_SIGN))
$(SOURCE_TARBALL_NAME_DEFAULT)	SOURCE_TARBALL_NAME=$(SOURCE_DIRNAME).tar.bz2

# Tarballs for most modules built here are found in the X.Org archives/repos
# in a directory with the same name as what we use in our tree
SOURCE_TARBALL_DIR_DEFAULT	= $(SOURCE_TARBALL_DIR_SET:yes=$(POUND_SIGN))
$(SOURCE_TARBALL_DIR_DEFAULT)	SOURCE_TARBALL_DIR=$(MODTYPE)

# Set default download location if not set in module makefile
SOURCE_URL_DEFAULT		= $(SOURCE_URL_SET:yes=$(POUND_SIGN))
$(SOURCE_URL_DEFAULT)		SOURCE_URL=$(SOURCE_URL_$(UPSTREAM_PROJECT))

# Set default git repo location if not set in module makefile
GIT_URL_DEFAULT		= $(GIT_URL_SET:yes=$(POUND_SIGN))
$(GIT_URL_DEFAULT)	GIT_URL=$(XORG_GIT_URL)/$(SOURCE_TARBALL_DIR)/$(MODULE_NAME).git

# Set default build type to 64-bit if not set in module makefile
BUILD_TYPES_DEFAULT	= $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES=64

# Set default rules if not set in module/modtype Makefile
SOURCE_TARGETS_DEFAULT		= $(SOURCE_TARGETS_SET:yes=$(POUND_SIGN))
$(SOURCE_TARGETS_DEFAULT)	SOURCE_TARGETS=$(UNPACK_TARGET) \
					$(MODTYPE_ADD_SOURCE_TARGETS) \
					$(MODULE_ADD_SOURCE_TARGETS)

CONFIGURE_TARGETS_DEFAULT	= $(CONFIGURE_TARGETS_SET:yes=$(POUND_SIGN))
$(CONFIGURE_TARGETS_DEFAULT)	CONFIGURE_TARGETS=default_configure \
					$(MODTYPE_ADD_CONFIGURE_TARGETS) \
					$(MODULE_ADD_CONFIGURE_TARGETS)

BUILD_TARGETS_DEFAULT		= $(BUILD_TARGETS_SET:yes=$(POUND_SIGN))
$(BUILD_TARGETS_DEFAULT)	BUILD_TARGETS=default_build \
					$(MODTYPE_ADD_BUILD_TARGETS) \
					$(MODULE_ADD_BUILD_TARGETS)

INSTALL_TARGETS_DEFAULT		= $(INSTALL_TARGETS_SET:yes=$(POUND_SIGN))
$(INSTALL_TARGETS_DEFAULT)	INSTALL_TARGETS=default_install \
					$(MODTYPE_ADD_INSTALL_TARGETS) \
					$(MODULE_ADD_INSTALL_TARGETS)

# Additional flags to pass to make in 32-bit & 64-bit builds
BUILD_32_FLAGS= ARCH_FLAGS="$(ARCH32_FLAGS)" \
	$(MODTYPE_BUILD_32_FLAGS) $(MODULE_BUILD_32_FLAGS)

BUILD_64_FLAGS= ARCH_FLAGS="$(ARCH64_FLAGS)" \
	$(MODTYPE_BUILD_64_FLAGS) $(MODULE_BUILD_64_FLAGS)

all: $(BUILD_TYPES:%=build_%)
all_%: build_%

# We always want to clean 32 & 64 bit subdirs, plus any special ones
# But if we just add clean_32 & clean_64, then make complains when they
# end up having multiple targets, so we remove them if they're in the list
# generated from $(BUILD_TYPES)
CLEAN_RULES_1 = $(BUILD_TYPES:%=clean_%)
CLEAN_RULES_2 = $(CLEAN_RULES_1:clean_32=)
CLEAN_RULES_3 = $(CLEAN_RULES_2:clean_64=)
CLEAN_RULES =  clean_32 clean_64 $(CLEAN_RULES_3)
clean: $(CLEAN_RULES)
.PARALLEL: $(CLEAN_RULES)

# Remove directories with both styles of naming
$(CLEAN_RULES):
	-rm -rf $(BUILD_DIR_$(@:clean_%=%)) $(@:clean_%=build_%) $(@:clean_%=build-%)

# Tell dmake not to remove and create source in parallel
.NOPARALLEL: clean source
.NOPARALLEL: clean_$(BUILD_TYPE) source_$(BUILD_TYPE)

SOURCE_RULES = $(BUILD_TYPES:%=source_%)
source: $(SOURCE_RULES)
.PARALLEL: $(SOURCE_RULES)

source-gen: $(SOURCE_TARGETS)

$(SOURCE_RULES):
	$(MAKE) $(MAKEFLAGS) BUILD_TYPE="$(@:source_%=%)" source-gen

CONFIGURE_RULES = $(BUILD_TYPES:%=configure_%)
configure: $(CONFIGURE_RULES)
.PARALLEL: $(CONFIGURE_RULES)

configure-gen: source-gen $(CONFIGURE_TARGETS)

$(CONFIGURE_RULES):
	$(MAKE) $(MAKEFLAGS) BUILD_TYPE="$(@:configure_%=%)" \
	 $(BUILD_$(@:configure_%=%)_FLAGS) configure-gen

BUILD_RULES = $(BUILD_TYPES:%=build_%)
build: $(BUILD_RULES)
.PARALLEL: $(BUILD_RULES)

build-gen: configure-gen $(BUILD_TARGETS)

$(BUILD_RULES):
	$(MAKE) $(MAKEFLAGS) BUILD_TYPE="$(@:build_%=%)" \
	 $(BUILD_$(@:build_%=%)_FLAGS) build-gen

INSTALL_RULES = $(BUILD_TYPES:%=install_%)
install: $(INSTALL_RULES)

install-gen: build-gen $(INSTALL_TARGETS) install_metadata 
install-gen: install_pkgconfig_uninstalled install_X11_compat_links

$(INSTALL_RULES):
	$(MAKE) $(MAKEFLAGS) BUILD_TYPE="$(@:install_%=%)" \
	 $(BUILD_$(@:install_%=%)_FLAGS) install-gen

$(PROTODIR)$(MODULE_DOC_DIR):
	mkdir -p $@

DEBUG_BUILD_FLAGS = BUILD_DEBUG=yes

# Allow adding "debug-" prefix to rules like build_32, install, etc.
debug-%:
	$(MAKE) $(MAKEFLAGS) $(@:debug-%=%) $(DEBUG_BUILD_FLAGS)

# Allow debug_XX to run "make all_XX" with debug flags (including without _XX)
debug%:
	$(MAKE) $(MAKEFLAGS) $(@:debug%=all%) $(DEBUG_BUILD_FLAGS)


### Source tarballs & downloading if necessary

SOURCE_TARBALL=$(TARBALLDIR)/$(SOURCE_TARBALL_NAME)

# Allow downloading from local archive/mirror
# TARBALL_ARCHIVE is now set in Makefile.options
ARCHIVE_URL=$(TARBALL_ARCHIVE)/$(SOURCE_TARBALL_DIR)/$(SOURCE_TARBALL_NAME)

GIT_CLONE_DIR=$(MODULE_NAME)-git

# Download if SOURCE_TARBALL_NAME is not set to NONE and the file either
# doesn't exist or is empty
# If MODULE_VERSION is git, checkout source from git repo instead of
# a tarball
download: $(SOURCE_TARBALL)

$(SOURCE_TARBALL): $(TARBALLDIR)
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; \
	if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    if [[ ! -d $(GIT_CLONE_DIR) ]] ; then \
		$(START_CMD_ECHO) ; \
		$(GIT) clone $(GIT_URL) $(GIT_CLONE_DIR) ; \
		if [[ ! -z "$(GIT_BRANCH)" ]] ; then \
		    cd $(GIT_CLONE_DIR) && $(GIT) checkout $(GIT_BRANCH) ; \
		fi ; \
	    fi ; \
	else \
	    if [[ "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
		if [[ ! -z "$(TARBALL_ARCHIVE)" ]] ; then \
		    URL='$(ARCHIVE_URL)' ; \
		else \
		    URL='$(SOURCE_URL)' ; \
		fi ; \
		$(START_CMD_ECHO) ; \
		$(WGET) -O '$(SOURCE_TARBALL)' "$${URL}" || \
		    ( status=$$? ; rm -f '$(SOURCE_TARBALL)' ; \
		      exit $${status} ) ; \
	    fi ; \
	fi

CHECKSUM_TARGET = .checksum_done.$(MODULE_VERSION)
check-checksums: $(CHECKSUM_TARGET)

$(CHECKSUM_TARGET): $(SOURCE_TARBALL)
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; \
	if [[ "$(MODULE_VERSION)" != "git" && \
	      "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
	    ACTUAL_MD5="$$(digest -a md5 $(SOURCE_TARBALL))" ; \
	    if [[ "$${ACTUAL_MD5}" != "${TARBALL_MD5}" ]] ; then \
		print -u2 "ERROR: MD5 sum mismatch on ${SOURCE_TARBALL}\n" \
			"$${ACTUAL_MD5} != ${TARBALL_MD5}" ; \
		exit 1; \
	    fi ; \
	    ACTUAL_SHA1="$$(digest -a sha1 ${SOURCE_TARBALL})" ; \
	    if [[ "$${ACTUAL_SHA1}" != "${TARBALL_SHA1}" ]] ; then \
		print -u2 "ERROR: SHA1 sum mismatch on ${SOURCE_TARBALL}\n" \
			"$${ACTUAL_SHA1} != ${TARBALL_SHA1}" ; \
		exit 1; \
	    fi ; \
	fi
	touch $(CHECKSUM_TARGET)

clean: clean-checksums

clean-checksums:
	-rm -f .checksum_done.*


# Submit a tarball to the archive
ARCHIVE_SUBMIT=/ws/x11/tarballs/bin/archive-tarballs

archive-tarball:
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; \
	if [[ "$(MODULE_VERSION)" != "git" && \
	      "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
		$(ARCHIVE_SUBMIT) \
		 --modtype='$(SOURCE_TARBALL_DIR)' \
		 --tarball='$(SOURCE_TARBALL_NAME)' \
		 --url='$(SOURCE_URL)' \
		 --md5='$(TARBALL_MD5)' \
		 --sha1='$(TARBALL_SHA1)' ; \
	fi

$(TARBALLDIR):
	mkdir -p $(TARBALLDIR)

# Update a git cloned repo
git-update:
	if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    if [[ -d $(GIT_CLONE_DIR) ]] ; then \
		cd $(GIT_CLONE_DIR) ; \
		if [[ ! -z "$(GIT_BRANCH)" ]] ; then \
		    $(GIT) checkout master && \
		    $(GIT) fetch && \
		    $(GIT) checkout $(GIT_BRANCH) ; \
		else \
		    $(GIT) fetch ; \
		fi ; \
	    fi ; \
	fi

# Create a local git repo from tarball and patches, to ease generating
# patches for submission to upstream git or rebasing to new tarballs
git-import:
	mkdir $(GIT_CLONE_DIR)
	cd $(GIT_CLONE_DIR) && git init
	$(MAKE) BUILD_DIR="$(GIT_CLONE_DIR)" SOURCE_DIR="$(GIT_CLONE_DIR)" \
		ADD_TAR_FLAGS="--strip-components=1" init_source
	cd $(GIT_CLONE_DIR) && git add . && \
		git commit -a -m 'Initialized from $(SOURCE_TARBALL_NAME)' \
		&& git tag '$(MODULE_NAME)-$(MODULE_VERSION)'
	@for f in $(SOURCE_PATCHES) ; do \
	    p=$${f%,*} ; \
	    print '=================' $$p ; \
	    if head -n 1 $$p | \
	       ggrep -Eq '^From .* Mon Sep 17 00:00:00 2001' ; then \
		(cd $(GIT_CLONE_DIR) ; $(START_CMD_ECHO) ; git am $(PWD)/$$p) ; \
	    else \
		rm -f $(GIT_CLONE_DIR)/$(PATCHED_TARGET_FILE) ; \
		$(MAKE) SOURCE_PATCHES="$$f" BUILD_DIR="$(GIT_CLONE_DIR)" \
			SOURCE_DIR="$(GIT_CLONE_DIR)" patched_source ; \
		printf '%s\n\n' "$${p%.patch}" > git.comments ; \
	    	gawk '/^--- /		{exit} \
		      /^diff -/		{exit} \
		      {print}' $$p >> git.comments ; \
		(cd $(GIT_CLONE_DIR) ; $(START_CMD_ECHO) ; \
		 git commit -a --file=$(PWD)/git.comments ) ; \
		rm git.comments ; \
	   fi ; \
	done


### Unpack source and apply patches

SOURCE_DEPS = $(SOURCE_TARBALL) $(CHECKSUM_TARGET) \
	$(MODTYPE_SOURCE_DEPS) $(MODULE_SOURCE_DEPS)

# Step 1: Initialize source dir with upstream sources
# - if there's a tarball, unpack it.
# - if using a git master, copy it.
SOURCE_INIT_TARGET_FILE=.source.init.done
SOURCE_INIT_TARGET=$(SOURCE_DIR)/$(SOURCE_INIT_TARGET_FILE)
init_source: $(SOURCE_INIT_TARGET)

$(SOURCE_INIT_TARGET): $(SOURCE_DEPS)
	mkdir -p $(BUILD_DIR)
	@ if [[ "$(SOURCE_TARBALL_NAME)" == "NONE" ]] ; then \
	  $(START_CMD_ECHO); mkdir -p $(SOURCE_DIR) ; \
	else \
	  if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    SOURCE_CMD='gtar --exclude=.git* -cf - $(GIT_CLONE_DIR)' ; \
	  else \
	    UNCOMP_CMD="$(SOURCE_UNCOMPRESS)" ; \
	    SOURCE_CMD="$${UNCOMP_CMD:-bzcat} $(SOURCE_TARBALL)" ; \
	  fi ; \
	  $(START_CMD_ECHO) ; \
	  $${SOURCE_CMD} | \
		gtar --directory="$(BUILD_DIR)" $(ADD_TAR_FLAGS) -xf -  ; \
	fi
	touch $(SOURCE_INIT_TARGET)

# Step 2: Add in any additional local sources from $(ADDITIONAL_SOURCE_DIR)
# (done after init source to avoid races in directory/file creation)
SOURCE_ADD_TARGET_FILE=.source.add.done
SOURCE_ADD_TARGET=$(SOURCE_DIR)/$(SOURCE_ADD_TARGET_FILE)
add_source: $(SOURCE_ADD_TARGET)

$(SOURCE_ADD_TARGET): $(SOURCE_INIT_TARGET)
	@if [[ "$(ADDITIONAL_SOURCE_DIR)" != "" ]] ; then \
	  if [[ ! -x $(LNDIR) ]] ; then \
	    (cd $(TOP)/open-src/util/lndir && $(MAKE) $(MAKEFLAGS) install) ; \
	  fi ; \
	  $(START_CMD_ECHO) ; \
	  cd $(SOURCE_DIR) && $(LNDIR) ../../$(ADDITIONAL_SOURCE_DIR) ; \
	fi
	touch $(SOURCE_ADD_TARGET)

# Step 3: Apply any local patches (may be done in parallel to add_source,
# but must come after init_source is finished)
#
# SOURCE_PATCHES is a list in the format: filename[,flags]
# $${p/*,} is ksh93 syntax to remove the comma and everything before it in $p
# $${p%,*} is ksh93 syntax to remove the comma and everything after it in $p
PATCHED_TARGET_FILE=.patches.done
PATCHED_TARGET=$(SOURCE_DIR)/$(PATCHED_TARGET_FILE)
patched_source: $(PATCHED_TARGET)

$(PATCHED_TARGET): $(SOURCE_INIT_TARGET)
	@ if [[ "$(SOURCE_PATCHES)" != "" ]] ; then \
	    for p in $(SOURCE_PATCHES) x ; do \
	        if [[ "$$p" != "x" ]] ; then \
		    patch_file="$${p%,*}" ; \
		    if [[ "$$p" =~ "," ]] ; then \
			patch_flags="$${p/*,}" ; \
		    else \
			patch_flags="-p0" ; \
		    fi ; \
		    ($(START_CMD_ECHO) ; \
		     gpatch -d $(SOURCE_DIR) $${patch_flags} < "$${patch_file}" ) ; \
		fi ; \
	    done ; \
	fi
	touch $(PATCHED_TARGET)

# Step 4: Apply common Solaris changes to man pages in $(SUNTOUCHED_MANPAGES)
FIXED_MAN_SUFFIX=mantouch.done
FIXED_MAN_TARGET=$(SUNTOUCHED_MANPAGES:%=$(SOURCE_DIR)/%.$(FIXED_MAN_SUFFIX))
fixed_man: $(FIXED_MAN_TARGET)

COMMON_SUNTOUCH_MAN_FLAGS  = -a '{Availability, $(MODULE_PKGNAME)}'
COMMON_SUNTOUCH_MAN_FLAGS += -a '{Interface Stability, $(MODULE_STABILITY)}' 
COMMON_SUNTOUCH_MAN_FLAGS += -r '{__distroname__, $(DISTRO_NAME)}'

DEFAULT_SUNTOUCH_MAN_FLAGS = \
	$(COMMON_SUNTOUCH_MAN_FLAGS) \
	$(MODTYPE_SUNTOUCH_MAN_FLAGS) \
	$(MODULE_SUNTOUCH_MAN_FLAGS)

SUNTOUCH_MAN_FLAGS_DEFAULT	= $(SUNTOUCH_MAN_FLAGS_SET:yes=$(POUND_SIGN))
$(SUNTOUCH_MAN_FLAGS_DEFAULT)	SUNTOUCH_MAN_FLAGS = $(DEFAULT_SUNTOUCH_MAN_FLAGS)

$(FIXED_MAN_TARGET): $(PATCHED_TARGET) $(SOURCE_ADD_TARGET)
	$(PERL) $(TOP)/open-src/common/suntouch-manpages.pl $(SUNTOUCH_MAN_FLAGS) \
		$(@:%.$(FIXED_MAN_SUFFIX)=%)
	touch $@

# Step 5: Apply common Solaris changes to pkg-config *.pc files listed in
# $(FIX_PC_FILES)
SOURCE_PC_FILES = $(FIX_PC_FILES:%=$(SOURCE_DIR)/%)
ORIG_PC_FILES = $(SOURCE_PC_FILES:%=%.orig)
FIXED_PC_TARGET = $(ORIG_PC_FILES)
fixed_pc: $(FIXED_PC_TARGET)

FIX_PC_OPS= \
	-e 's|-L\$${libdir}|-L\$${libdir} -R\$${libdir}|;' \
	-e 's|-R\$${libdir}( .*)? -R\$${libdir}|-R\$${libdir}$$1|g;' \
	$(MODTYPE_FIX_PC_FLAGS) \
	$(MODULE_FIX_PC_FLAGS)

$(ORIG_PC_FILES): $(PATCHED_TARGET)
	$(PERL) -i.orig -p $(FIX_PC_OPS) $(@:.orig=)

# Tie them all together
UNPACK_TARGETS = $(SOURCE_INIT_TARGET) $(PATCHED_TARGET) \
	$(SOURCE_ADD_TARGET) $(FIXED_MAN_TARGET) $(FIXED_PC_TARGET)

default_source: $(UNPACK_TARGET)

$(UNPACK_TARGET): $(SOURCE_DEPS) $(UNPACK_TARGETS)
	touch $(UNPACK_TARGET)

### Run configure scripts

# Include Xorg autoconf macros when rebuilding configure scripts
# and use automake 1.10 instead of the older 1.9.
ACLOCAL_PATH=/usr/bin/aclocal-1.10
ACLOCAL_INCLUDES = -I $(PROTODIR)$(X11_ACLOCAL_DIR) \
	-I $(TOOLS_ACLOCAL_DIR) \
	-I /usr/share/aclocal -I /usr/share/aclocal-1.10
AUTOMAKE_PATH=/usr/bin/automake-1.10
AUTORECONF_ENV= ACLOCAL="$(ACLOCAL_PATH) $(ACLOCAL_INCLUDES)" \
	AUTOMAKE="$(AUTOMAKE_PATH)"

# Dependencies for running configure to make a Makefile
CONFIGURE_DEPS = $(SOURCE_TARGETS) \
	$(MODTYPE_CONFIGURE_DEPS) $(MODULE_CONFIGURE_DEPS)

# Command line options to GNU autoconf configure script
DEFAULT_CONFIG_OPTS = --prefix=$(MODULE_PREFIX) \
	--without-xmlto --without-asciidoc --without-fop \
	--mandir='$${prefix}/share/man' --localstatedir=/var

CONFIG_OPTS = $(DEFAULT_CONFIG_OPTS) \
	$(MODTYPE_CONFIG_OPTS) $(MODULE_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
$(USE_DEFAULT_CONFIG_CPPFLAGS:no=$(POUND_SIGN)) DEFAULT_CONFIG_CPPFLAGS= \
	-I$(PROTODIR)$(X11_INCLUDES_DIR) \
	-I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/include/freetype2
$(USE_DEFAULT_CONFIG_LDFLAGS:no=$(POUND_SIGN)) DEFAULT_CONFIG_LDFLAGS= \
	-L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR) \
	-L$(PROTODIR)$(X11_DIR)/lib$(ARCHLIBSUBDIR)

DEFAULT_PKG_CONFIG_PATH=$(TOP)/open-src/common/pc-files/$(BUILD_DIR):$(PROTODIR)$(PKGCONFIG_DIR):$(PROTODIR)$(PKGCONFIG_DIR_SHARE):$(PKGCONFIG_DIR):$(PKGCONFIG_DIR_SHARE)

# Most upstream software is written by developers using bash as /bin/sh, so
# we run configure scripts with that to minimize problems and work around
# some ksh93 bugs we've hit
CONFIG_SHELL_DEFAULT		= $(CONFIG_SHELL_SET:yes=$(POUND_SIGN))
$(CONFIG_SHELL_DEFAULT)		CONFIG_SHELL = /usr/bin/bash

# Set USE_DEFAULT_CONFIG_ENV=no in a Makefile to not use this default
# autoconfig environment
$(USE_DEFAULT_CONFIG_ENV:no=$(POUND_SIGN))	DEFAULT_CONFIG_ENV = \
    CC="$(CC)" CXX="$(CXX)" \
    LD="$(CC) $(LDFLAGS)" \
    CFLAGS="$(MODTYPE_CFLAGS) $(MODULE_CFLAGS)" \
    CXXFLAGS="$(MODTYPE_CXXFLAGS) $(MODULE_CXXFLAGS)" \
    CPPFLAGS="$(DEFAULT_CONFIG_CPPFLAGS) $(MODTYPE_CPPFLAGS) $(MODULE_CPPFLAGS)" \
    LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(MODTYPE_CFLAGS) $(MODTYPE_LDFLAGS) $(MODULE_CFLAGS) $(MODULE_LDFLAGS)" \
    PKG_CONFIG_PATH="$(DEFAULT_PKG_CONFIG_PATH)" \
    INSTALL="$(INSTALL_SCRIPT)" CPPROG="cp -p" \
    MAKE="$(MODULE_MAKE)" CONFIG_SHELL="$(CONFIG_SHELL)"

CONFIG_ENV = $(DEFAULT_CONFIG_ENV) $(MODTYPE_CONFIG_ENV) $(MODULE_CONFIG_ENV)

# File created by running configure - defaults to the top-level Makefile
# in a source directory unless AUTOCONF_TARGET_SET=yes is set
AUTOCONF_TARGET_DEFAULT = $(AUTOCONF_TARGET_SET:yes=$(POUND_SIGN))
$(AUTOCONF_TARGET_DEFAULT)	AUTOCONF_TARGET=$(SOURCE_DIR)/Makefile

$(AUTOCONF_TARGET): $(CONFIGURE_DEPS)
	cd $(SOURCE_DIR) ; \
	if [[ "$(AUTORECONF)" = "yes" || "$(MODULE_VERSION)" = "git" ]] ; then \
	     $(AUTORECONF_ENV) autoreconf -v --install --force ; \
	fi ; \
	chmod a+x configure ; \
	$(CONFIG_ENV) $(CONFIG_SHELL) ./configure $(CONFIG_OPTS) ; \
	if [[ "$(DELIBTOOLIZE)" = "yes" ]] ; then \
	    $(TOP)/open-src/common/delibtoolize.pl $(DELIBTOOLIZE_FLAGS) . ; \
	fi

default_configure: $(AUTOCONF_TARGET)

### Default rules for common build pattern

LD_OPTIONS = -L$(PROTODIR)$(X11_DIR)/lib$(LIBSUBDIR) \
	$(MODTYPE_LD_OPTIONS) $(MODULE_LD_OPTIONS)

# MKDIRPROG - Workaround parallel build race condition
#		in older upstream packaged install-sh

BUILD_ENV = LD_OPTIONS='$(LD_OPTIONS)' \
	PROTODIR='$(PROTODIR)' \
	MAKE='$(MODULE_MAKE)' \
	MKDIRPROG='mkdir -p' \
	$(MODTYPE_BUILD_ENV) $(MODULE_BUILD_ENV)

DEFAULT_BUILD_MAKEFLAGS=$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_BUILD_MAKEFLAGS) $(MODULE_BUILD_MAKEFLAGS)

# Dependencies for make build step
BUILD_DEPS = $(SOURCE_TARGETS) $(CONFIGURE_TARGETS) $(MODTYPE_BUILD_DEPS) $(MODULE_BUILD_DEPS) $(MODULE_MAKE)

DEFAULT_BUILD_COMMAND = cd @DIR@ ; \
	$(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS)

default_build: $(BUILD_DEPS)
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(SOURCE_DIR))

build-in-subdir:
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(subdir)) $(subdir_cmd)

DEFAULT_INSTALL_MAKEFLAGS= -e DESTDIR=$(PROTODIR) \
	pkgconfigdir=$(MODULE_PKGCONFIG_DIR) \
	$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_INSTALL_MAKEFLAGS) $(MODULE_INSTALL_MAKEFLAGS) \

# Dependencies for make install step
INSTALL_DEPS = $(BUILD_TARGETS) $(MODTYPE_INSTALL_DEPS) $(MODULE_INSTALL_DEPS)

DEFAULT_INSTALL_COMMAND = cd @DIR@ ; \
	$(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS)

default_install: $(INSTALL_DEPS)
	$(DEFAULT_INSTALL_COMMAND:@DIR@=$(SOURCE_DIR)) install

install-in-subdir:
	$(DEFAULT_INSTALL_COMMAND:@DIR@=$(subdir)) $(subdir_cmd) \
		$(install_subdir_cmd)

# Adjust paths in pkg-config files for -uninstalled variants used during build
UNINSTALLED_PC_OPS= \
	-e 'BEGIN {use Cwd; $$protodir=Cwd::realpath("$(PROTODIR)");}' \
	-e 's|(-[LI]\s*)\$$|$$1$$protodir\$$|g;' \
	-e 's|^sdkdir=|sdkdir=$$protodir|g;' \
	-e 's|$$protodir\$${sdkdir}|\$${sdkdir}|g;' \
	$(MODTYPE_ADD_UNINSTALLED_PC_OPS) $(MODULE_ADD_UNINSTALLED_PC_OPS)

INSTALL_PCU_DEPS_DEFAULT	= $(INSTALL_PCU_DEPS_SET:yes=$(POUND_SIGN))
$(INSTALL_PCU_DEPS_DEFAULT)	INSTALL_PCU_DEPS = $(INSTALL_TARGETS)

install_pkgconfig_uninstalled: $(INSTALL_PCU_DEPS)
	@ if [[ ! -z "$(FIX_PC_FILES)" ]] ; then \
	    cd $(SOURCE_DIR) ; \
	    for pcin in $(FIX_PC_FILES:.in=) ; do \
		pc="$${pcin%%+(.pc|.in)}" ; \
		pcu="$${pc##*/}-uninstalled.pc" ; \
		($(START_CMD_ECHO) ; \
		 rm -f $(PROTODIR)$(MODULE_PKGCONFIG_DIR)/$${pcu} ; \
		 $(PERL) -p $(UNINSTALLED_PC_OPS) $$pc.pc > \
		    $(PROTODIR)$(MODULE_PKGCONFIG_DIR)/$${pcu} ) ; \
	    done ; \
	fi

# Add links if needed from /usr/X11/* to new locations in /usr/*
X11_BINCOMPAT_DIR = $(PROTODIR)/usr/X11/bin
X11_BINCOMPAT_LINKS = $(MODULE_X11_BINCOMPAT_LINKS:%=$(X11_BINCOMPAT_DIR)/%)
X11_LIBCOMPAT_DIR = $(PROTODIR)/usr/X11/lib$(ARCHLIBSUBDIR)
X11_LIBCOMPAT_LINKS = $(MODULE_X11_LIBCOMPAT_LINKS:%=$(X11_LIBCOMPAT_DIR)/%)

$(X11_BINCOMPAT_DIR) $(X11_LIBCOMPAT_DIR):
	mkdir -p $@

$(X11_BINCOMPAT_LINKS): $(X11_BINCOMPAT_DIR)
	@rm -f $@
	ln -s ../../bin/$(@F) $@

$(X11_LIBCOMPAT_LINKS): $(X11_LIBCOMPAT_DIR)
	@rm -f $@
	@if [[ -z "$(ARCHLIBSUBDIR)" ]] ; then \
	    TARGETDIR=../../lib ; \
	else \
	    TARGETDIR=../../../lib$(ARCHLIBSUBDIR) ; \
	fi ; $(START_CMD_ECHO) ; \
	ln -s $${TARGETDIR}/$(@F) $@

install_X11_compat_links: $(X11_BINCOMPAT_LINKS) $(X11_LIBCOMPAT_LINKS)

# Set comments in ELF binaries to help identify versions/builds
$(BUILD_DEBUG:yes=)			DEBUG_ELF_COMMENTS=(DEBUG)
$(BUILD_DEBUG:yes=)			DEBUG_ELF_COMMENTS_FLAGS=-b -B 'hg id'
$(BUILD_DEBUG:yes=$(POUND_SIGN))	DEBUG_ELF_COMMENTS=
$(BUILD_DEBUG:yes=$(POUND_SIGN))	DEBUG_ELF_COMMENTS_FLAGS=

ELF_COMMENTS = $(MODULE_NAME) $(MODULE_VERSION)
ELF_COMMENTS += $(DEBUG_ELF_COMMENTS)
ELF_COMMENTS += $(MODTYPE_ELF_COMMENTS)$(MODULE_ELF_COMMENTS)

ELF_COMMENTS_FLAGS = -M '$(ELF_COMMENTS)'
ELF_COMMENTS_FLAGS += $(DEBUG_ELF_COMMENTS_FLAGS)
ELF_COMMENTS_FLAGS += $(MODTYPE_ELF_COMMENTS_FLAGS) $(MODULE_ELF_COMMENTS_FLAGS)

set-elf-comments: $(BUILD_TARGETS:set-elf-comments=)
	$(TOP)/open-src/common/set-elf-comments.pl $(ELF_COMMENTS_FLAGS) \
	 $(SOURCE_DIR)

# Help generate templates for package manifests
DEFAULT_LICENSE=$(PKG_LICENSE_DIR)/cr_Oracle $(PKG_LICENSE_DIR)/lic_X11

MANIFEST_DEST=$(PWD)/tmp-proto-$(MACH)
MANIFEST_TMP=$(PWD)/tmp.p5m

PKGFMT=/usr/bin/pkgfmt

MANIFEST_TARGETS_DEFAULT	= $(MANIFEST_TARGETS_SET:yes=$(POUND_SIGN))
$(MANIFEST_TARGETS_DEFAULT)	MANIFEST_TARGETS=$(MODULE_PKG_MANIFEST)

manifest: $(MANIFEST_TARGETS)

manifest-per-type: $(MODULE_PKG_MANIFEST)

MANIFEST_INSTALL_RULES = PROTOTOOLSDIR="$(PROTODIR)" \
	PROTODIR="$(MANIFEST_DEST)"

$(MANIFEST_DEST): install
	/usr/xpg4/bin/make $(MAKEFLAGS) install $(MANIFEST_INSTALL_RULES) \
		|| (rm -rf $(MANIFEST_DEST) ; exit 1)

$(MANIFEST_TMP): $(MANIFEST_DEST) $(PKGFMT)
	$(PERL) $(TOP)/open-src/common/pkg-manifest-generate.pl \
		input_manifest='$(MODULE_PKG_MANIFEST)' \
		manifest_license='$(PKG_LICENSE_DIR)/cr_Oracle' \
		manifest_license='$(PKG_LICENSE_DIR)/lic_X11' \
		pkg_name='$(MODULE_PKGNAME)' \
		source_dir='$(SOURCE_DIRNAME)' \
		arch_64='$(ARCH64)' subdir_64='$(SUBDIR64)' \
		proto_area='$(MANIFEST_DEST)' \
		pkgfmt='$(PKGFMT)' \
		version='__version:$(MODULE_NAME)__' \
		> $(MANIFEST_TMP)
	-rm -rf $(MANIFEST_DEST)

$(MODULE_PKG_MANIFEST): $(MANIFEST_TMP) $(PKG_MANIFEST_DIR)
	mv -f $(MANIFEST_TMP) $@

$(PKG_MANIFEST_DIR):
	mkdir -p $(PKG_MANIFEST_DIR)

### Other tools/dependencies needed to build

# Some modules (such as FreeType 2) require GNU make
$(GNUMAKE):
	@print -n "Building with GNU make from $$(whence gmake): "
	@$(GNUMAKE) --version

$(MAKE):
	@print "Building with make from $$(whence $(MAKE))"

# Some modules use lndir to merge in sources from Sun specific directories
$(LNDIR):
	(cd $(TOP)/open-src/util/lndir && $(MAKE) $(MAKEFLAGS) install)

### Useful rules for maintaining patch sets

# regenerate patches against new baseline to reduce messages about hunks
# having fuzz or offsets when applying patches - new patches will be in new/

regen-patches:
	CUR_LIST=""
	rm -rf tmp-regen
	$(MAKE) SOURCE_PATCHES="$(CUR_LIST)" BUILD_DIR=tmp-regen patched_source
	mkdir -p new
	for f in $(SOURCE_PATCHES) ; do \
	    p=$${f%,*} ; \
	    print '=================' $$p ; \
	    rm -rf prev ; \
	    mv tmp-regen prev ; \
	    $(MAKE) SOURCE_PATCHES="$${CUR_LIST} $$f" BUILD_DIR=tmp-regen patched_source ; \
	    find tmp-regen -name '*.orig' -exec rm {} \+ ; \
	    gawk '/^--- /	{exit} \
		  /^diff -/	{exit} \
			 	{print}' $$p > new/$$p ; \
	    LC_COLLATE=C git diff --no-color ./prev ./tmp-regen \
		| sed -e 's% a/\./prev/$(SOURCE_DIRNAME)/% a/%' -e 's% b/\./tmp-regen/$(SOURCE_DIRNAME)/% b/%' \
		| grep -v '^Common subdirectories:' \
		| grep -v '^Only in ' \
		>> new/$$p ; \
	    CUR_LIST="$${CUR_LIST} new/$$p,-p1" ; \
	done
	rm -rf tmp-regen

# Allows running "make list-patches" to get a list of all patches we
# currently apply in our builds
list-patches:
	@if [[ "$(SOURCE_PATCHES)" != "" ]] ; then \
	    THIS_MODULE="$${PWD##*/open-src/}" ; \
	    for p in $(SOURCE_PATCHES) x ; do \
		if [[ "$$p" != "x" ]] ; then \
	            print "$${THIS_MODULE}/$${p%,*}" ;  \
		fi ; \
	    done ; \
	fi

### Metadata used in packaging

MODULE_PKG_METADATA_DIR	= $(PROTOMETA)/$(MODULE_PKG_MANIFEST_NAME)

$(MODULE_PKG_METADATA_DIR):
	mkdir -p $@

METADATA_TARGETS = $(MODULE_PKG_METADATA_DIR)

# Base URL for ARC cases used in opensolaris.arc_url metadata
# Set to '$(ARC_URL)' here, resolved by pkgmogrify to the URL
# set in pkg/Makefile during the package building process
ARC_URL_BASE = $$(ARC_URL)

# Metadata about upstream
UPSTREAM_NONE		= NONE
UPSTREAM_NONE_SET	= yes

UPSTREAM_xorg		= X.Org Foundation
UPSTREAM_URL_xorg	= http://www.x.org/
SOURCE_URL_xorg		= $(XORG_RELEASES_URL)/$(SOURCE_TARBALL_DIR)/$(SOURCE_TARBALL_NAME)

UPSTREAM_xcb		= The XCB Project
UPSTREAM_URL_xcb	= http://xcb.freedesktop.org/
SOURCE_URL_xcb		= http://xcb.freedesktop.org/dist/$(SOURCE_TARBALL_NAME)

UPSTREAM_PROJECT_DEFAULT	= $(UPSTREAM_PROJECT_SET:yes=$(POUND_SIGN))
$(UPSTREAM_PROJECT_DEFAULT)	UPSTREAM_PROJECT = xorg

UPSTREAM_DEFAULT	= $(UPSTREAM_SET:yes=$(POUND_SIGN))
$(UPSTREAM_DEFAULT) UPSTREAM = $(UPSTREAM_$(SOURCE_TARBALL_NAME))
$(UPSTREAM_DEFAULT) UPSTREAM_SET_YET = $(UPSTREAM_$(SOURCE_TARBALL_NAME)_SET)
$(UPSTREAM_DEFAULT) UPSTREAM_NOTFOUND = $(UPSTREAM_SET_YET:yes=$(POUND_SIGN))
$(UPSTREAM_NOTFOUND) $(SOURCE_URL_DEFAULT) UPSTREAM = $(UPSTREAM_$(UPSTREAM_PROJECT))
$(UPSTREAM_NOTFOUND) $(SOURCE_URL_DEFAULT) UPSTREAM_URL = $(UPSTREAM_URL_$(UPSTREAM_PROJECT))

## Attributes files

ATTRDATA_FILE_NAME	= attributes_$(MODULE_NAME).p5m
ATTRDATA_FILE		= $(MODULE_PKG_METADATA_DIR)/$(ATTRDATA_FILE_NAME)

METADATA_TARGETS += $(ATTRDATA_FILE)

# $${MODVERS//\.+(0)(\d)/.\2} - ksh93 pattern substitution to remove leading
# 0's from package version number components, to meet pkg(5) fmri requirements
%/$(ATTRDATA_FILE_NAME): %
	-rm -f $@
	@if [[ "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=info.source_url value="$(SOURCE_URL)"' >> $@ ; \
	fi
	@if [[ "$(UPSTREAM)" != "NONE" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=info.upstream value="$(UPSTREAM)"' >> $@ ; \
	    print 'set name=info.upstream_url value="$(UPSTREAM_URL)"' >> $@ ; \
	fi
	@if [[ ! -z "$(MODULE_ARC_CASES)" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=opensolaris.arc_url' \
		'$(MODULE_ARC_CASES:%=value="$(ARC_URL_BASE)%/")' >> $@ ;  \
	fi
	@if [[ ! -z "$(MODULE_PKGCLASS)" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=info.classification' \
		'value="org.opensolaris.category.2008:$(MODULE_PKGCLASS)"' \
		>> $@ ;  \
	fi
	@ MODVERS="$(MODULE_VERSION)" ; $(START_CMD_ECHO) ; \
	printf '<transform set -> edit value __version:$(MODULE_NAME)__ %s>\n' \
	       "$${MODVERS//\.+(0)(\d)/.\2}" >> $@
	$(MODTYPE_ATTRDATA_COMMANDS)
	$(MODULE_ATTRDATA_COMMANDS)

## License files

# Default license file name is COPYING (X.Org & GNU autoconf default)
# For other license file names, set LICENSE_FILE to the file name
# and set LICENSE_FILE_SET=yes
# A file in the same directory as the Makefile takes precedence
# over one that is delivered in the package.
LICENSE_FILE_DEFAULT	= $(LICENSE_FILE_SET:yes=$(POUND_SIGN))
$(LICENSE_FILE_DEFAULT)	LICENSE_FILE=COPYING

# Default is the MIT/X11 license
MIT_LICENSE_NAME = MIT License ($(MODULE_NAME))
MIT_LICENSE_FILE = $(PKG_SRC_DIR)/license_files/lic_X11

MODULE_LICENSE_DEFAULT		= $(MODULE_LICENSE_SET:yes=$(POUND_SIGN))
$(MODULE_LICENSE_DEFAULT)	MODULE_LICENSE = $(MIT_LICENSE_NAME)

# Some packages contain only code that Oracle owns all the rights to.
# For these, Makefile.options.oracle.com chooses to omit the license
# notice when Oracle is building its distros.   Everyone else gets the
# notice required by open source license Oracle released the code under.
LICENSE_CHOICE_DEFAULT		= $(LICENSE_CHOICE_SET:yes=$(POUND_SIGN))
$(LICENSE_CHOICE_DEFAULT)	LICENSE_CHOICE=open

ORACLE_LICENSE_NAME_open	= $(MIT_LICENSE_NAME)
ORACLE_LICENSE_APPEND_open	= $(MIT_LICENSE_FILE)

ORACLE_LICENSE_NAME_Oracle	= Oracle Copyright Notice ($(MODULE_NAME))
ORACLE_LICENSE_APPEND_Oracle 	= # Nothing to append to copyright notice

ORACLE_LICENSE_NAME 		= $(ORACLE_LICENSE_NAME_$(LICENSE_CHOICE))
ORACLE_LICENSE_PREPEND	 	= # Nothing to prepend to copyright notice
ORACLE_LICENSE_APPEND		= $(ORACLE_LICENSE_APPEND_$(LICENSE_CHOICE))

# Use default license info unless MODULE_LICENSE = "Oracle"
LICENSE_NAME			= $(MODULE_LICENSE)
LICENSE_PREPEND 		= $(MODULE_LICENSE_PREPEND)
LICENSE_APPEND 			= $(MODULE_LICENSE_APPEND)

LICENSE_OVERRIDE_1		= $(POUND_SIGN)$(MODULE_LICENSE)
LICENSE_OVERRIDE		= $(LICENSE_OVERRIDE_1:$(POUND_SIGN)Oracle%=)
$(LICENSE_OVERRIDE)		LICENSE_NAME = $(ORACLE_LICENSE_NAME)
$(LICENSE_OVERRIDE)		LICENSE_PREPEND = $(ORACLE_LICENSE_PREPEND)
$(LICENSE_OVERRIDE)		LICENSE_APPEND = $(ORACLE_LICENSE_APPEND)

LICENSE_FILE_NAME = license_$(MODULE_NAME)
LICENSE_FILE_DEST = $(MODULE_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME)
LICENSE_ATTR_FILE = $(MODULE_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME).p5m

install_license: $(LICENSE_FILE_DEST)
METADATA_TARGETS += $(LICENSE_FILE_DEST)

%/$(LICENSE_FILE_NAME): % $(SOURCE_TARGETS)
	@ $(START_CMD_ECHO) ; \
	if [[ -f "$(LICENSE_FILE)" ]] ; then \
	    license_to_copy="$(LICENSE_FILE)" ; \
	elif [[ -f "$(SOURCE_DIR)/$(LICENSE_FILE)" ]] ; then \
	    license_to_copy="$(SOURCE_DIR)/$(LICENSE_FILE)" ; \
	fi ; \
	if [[ -n "$${license_to_copy}" ]] ; then \
	    cat $(LICENSE_PREPEND) "$${license_to_copy}" $(LICENSE_APPEND) > "$@" ; \
	    print 'license $(LICENSE_FILE_NAME) license="$(LICENSE_NAME)"' > "$@.p5m" ; \
	fi

install_metadata: $(METADATA_TARGETS)
