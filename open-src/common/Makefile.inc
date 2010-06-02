# -*- Makefile -*- rules commonly shared among X consolidation open source dirs
#
# Copyright (c) 2002, 2010, Oracle and/or its affiliates. All rights reserved.
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

# Where to install pkgconfig files
MODTYPE_PKGCONFIG_DIR_DEFAULT	= $(MODTYPE_PKGCONFIG_DIR_SET:yes=$(POUND_SIGN))
$(MODTYPE_PKGCONFIG_DIR_DEFAULT) MODTYPE_PKGCONFIG_DIR=$(PKGCONFIG_DIR)
MODULE_PKGCONFIG_DIR_DEFAULT	= $(MODULE_PKGCONFIG_DIR_SET:yes=$(POUND_SIGN))
$(MODULE_PKGCONFIG_DIR_DEFAULT)	MODULE_PKGCONFIG_DIR=$(MODTYPE_PKGCONFIG_DIR)

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
$(SOURCE_URL_DEFAULT)		SOURCE_URL=$(XORG_RELEASES_URL)/$(SOURCE_TARBALL_DIR)/$(SOURCE_TARBALL_NAME)

# Set default git repo location if not set in module makefile
GIT_URL_DEFAULT		= $(GIT_URL_SET:yes=$(POUND_SIGN))
$(GIT_URL_DEFAULT)	GIT_URL=$(XORG_GIT_URL)/$(SOURCE_TARBALL_DIR)/$(MODULE_NAME).git

# Set default build type to 32-bit if not set in module makefile
BUILD_TYPES_DEFAULT	= $(BUILD_TYPES_SET:yes=$(POUND_SIGN))
$(BUILD_TYPES_DEFAULT)	BUILD_TYPES=32

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

CLEAN_RULES = $(BUILD_TYPES:%=clean_%)
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

install-gen: build-gen $(INSTALL_TARGETS) install_license install_pkgconfig_uninstalled

$(INSTALL_RULES):
	$(MAKE) $(MAKEFLAGS) BUILD_TYPE="$(@:install_%=%)" \
	 $(BUILD_$(@:install_%=%)_FLAGS) install-gen

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
TARBALL_ARCHIVE_sun.com		= http://xserver.sfbay.sun.com/tarballs
TARBALL_ARCHIVE_DEFAULT		= $(TARBALL_ARCHIVE_SET:yes=$(POUND_SIGN))
$(TARBALL_ARCHIVE_DEFAULT)	ARCHIVE_DOMAIN = domainname | awk -F. '{if (NF > 1) {printf "%s.%s", $$(NF-1), $$NF}}'
$(TARBALL_ARCHIVE_DEFAULT)	TARBALL_ARCHIVE = $(TARBALL_ARCHIVE_$(ARCHIVE_DOMAIN:sh))
ARCHIVE_URL=$(TARBALL_ARCHIVE)/$(SOURCE_TARBALL_DIR)/$(SOURCE_TARBALL_NAME)

GIT_CLONE_DIR=$(MODULE_NAME)-$(MODULE_VERSION)

# Download if SOURCE_TARBALL_NAME is not set to NONE and the file either
# doesn't exist or is empty
# If MODULE_VERSION is git, checkout source from git repo instead of
# a tarball
download: $(SOURCE_TARBALL)

$(SOURCE_TARBALL): $(TARBALLDIR)
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; $(START_CMD_ECHO) ; \
	if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    if [[ ! -d $(GIT_CLONE_DIR) ]] ; then \
		$(GIT) clone $(GIT_URL) $(GIT_CLONE_DIR) ; \
		if [[ ! -z "$(GIT_BRANCH)" ]] ; then \
		    cd $(GIT_CLONE_DIR) && $(GIT) checkout $(GIT_BRANCH) ; \
		fi ; \
	    fi ; \
	else \
	    if [[ "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
		if [[ ! -z "$(TARBALL_ARCHIVE)" ]] ; then \
		    $(WGET) -O $(SOURCE_TARBALL) $(ARCHIVE_URL) ; \
		else \
		    $(WGET) -O $(SOURCE_TARBALL) $(SOURCE_URL) ; \
		fi ; \
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


### Unpack source and apply patches

# Step 1: Initialize source dir with upstream sources
# - if there's a tarball, unpack it.
# - if using a git master, copy it.
SOURCE_INIT_TARGET_FILE=.source.init.done
SOURCE_INIT_TARGET=$(SOURCE_DIR)/$(SOURCE_INIT_TARGET_FILE)
init_source: $(SOURCE_INIT_TARGET)

$(SOURCE_INIT_TARGET):
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
	    (cd $(TOP)/util/lndir && $(MAKE) $(MAKEFLAGS) install) ; \
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

COMMON_SUNTOUCH_MAN_FLAGS = \
	-a '{Availability, $(SUN_PACKAGE)}' \
	-a '{Interface Stability, $(MODULE_STABILITY)}'

DEFAULT_SUNTOUCH_MAN_FLAGS = \
	$(COMMON_SUNTOUCH_MAN_FLAGS) \
	$(MODTYPE_SUNTOUCH_MAN_FLAGS) \
	$(MODULE_SUNTOUCH_MAN_FLAGS)

SUNTOUCH_MAN_FLAGS_DEFAULT	= $(SUNTOUCH_MAN_FLAGS_SET:yes=$(POUND_SIGN))
$(SUNTOUCH_MAN_FLAGS_DEFAULT)	SUNTOUCH_MAN_FLAGS = $(DEFAULT_SUNTOUCH_MAN_FLAGS)

$(FIXED_MAN_TARGET): $(PATCHED_TARGET) $(SOURCE_ADD_TARGET)
	$(PERL) $(TOP)/common/suntouch-manpages.pl $(SUNTOUCH_MAN_FLAGS) \
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
	-e 's|Requires.private: .*$$||;' \
	$(MODTYPE_FIX_PC_FLAGS) \
	$(MODULE_FIX_PC_FLAGS)

$(ORIG_PC_FILES): $(PATCHED_TARGET)
	$(PERL) -i.orig -p $(FIX_PC_OPS) $(@:.orig=)

# Tie them all together
SOURCE_DEPS = $(SOURCE_TARBALL) $(MODTYPE_SOURCE_DEPS) $(MODULE_SOURCE_DEPS)

UNPACK_TARGETS = $(SOURCE_INIT_TARGET) $(PATCHED_TARGET) \
	$(SOURCE_ADD_TARGET) $(FIXED_MAN_TARGET) $(FIXED_PC_TARGET)

default_source: $(UNPACK_TARGET)

$(UNPACK_TARGET): $(SOURCE_DEPS) $(UNPACK_TARGETS)
	touch $(UNPACK_TARGET)

### Run configure scripts

# Include Xorg autoconf macros when rebuilding configure scripts
# and use automake 1.10 instead of the older 1.9.
# /opt/jdsbld/share/aclocal is needed to get the nls.m4 from CBEgettext
# due to: 6785384 SUNWgnu-gettext does not include autoconf m4 files
ACLOCAL_PATH=/usr/bin/aclocal-1.10
ACLOCAL_INCLUDES = -I $(PROTODIR)$(X11_ACLOCAL_DIR) \
	-I /usr/share/aclocal -I /usr/share/aclocal-1.10 \
	-I /opt/jdsbld/share/aclocal
AUTOMAKE_PATH=/usr/bin/automake-1.10
AUTORECONF_ENV= ACLOCAL="$(ACLOCAL_PATH) $(ACLOCAL_INCLUDES)" \
	AUTOMAKE="$(AUTOMAKE_PATH)"

# Dependencies for running configure to make a Makefile
CONFIGURE_DEPS = $(SOURCE_TARGETS) \
	$(MODTYPE_CONFIGURE_DEPS) $(MODULE_CONFIGURE_DEPS)

# Command line options to GNU autoconf configure script
DEFAULT_CONFIG_OPTS = --prefix=$(MODULE_PREFIX) \
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

DEFAULT_PKG_CONFIG_PATH=$(TOP)/common/pc-files/$(BUILD_DIR):$(PROTODIR)$(PKGCONFIG_DIR):$(PROTODIR)$(PKGCONFIG_DIR_SHARE):$(PKGCONFIG_DIR):$(PKGCONFIG_DIR_SHARE)

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
    MAKE="$(MODULE_MAKE)" CONFIG_SHELL="$(SHELL)"

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
	$(CONFIG_ENV) $(SHELL) ./configure $(CONFIG_OPTS) ; \
	if [[ "$(DELIBTOOLIZE)" = "yes" ]] ; then \
	    $(TOP)/common/delibtoolize.pl $(DELIBTOOLIZE_FLAGS) . ; \
	fi

default_configure: $(AUTOCONF_TARGET)


# Additional rule used in xserver/Makefile.inc & driver/Makefile.inc to
# edit the Makefiles generated by configure to override module subdirs 
# for 64-bit x86 modules to be modules/xxx/amd64, not modules/amd64/xxx
FIXEDMODS_TARGET = $(AUTOCONF_TARGET).fixedmods
CONFIGURE_FIXEDMODS_/amd64 = $(FIXEDMODS_TARGET)
CONFIGURE_FIXEDMODS_TARGET = $(CONFIGURE_FIXEDMODS_$(SERVERMOD_subdir))

fix-moduledirs: $(CONFIGURE_FIXEDMODS_TARGET)

FIXEDMODS_SUBSTS = \
-e 's{\$$\(moduledir\)/(\w+|\$$\(\w+\))}{$(X11_SERVERMODS_DIR)/$$1$(SERVERMOD_subdir)}g;' \
-e 's{$(X11_SERVERMODS_DIR)$(SERVERMOD_subdir)/([^\s/]+)}{$(X11_SERVERMODS_DIR)/$$1$(SERVERMOD_subdir)}g;'

$(FIXEDMODS_TARGET): $(AUTOCONF_TARGET)
	find $(SOURCE_DIR) -name Makefile | \
	 xargs $(PERL) -i.orig -p $(FIXEDMODS_SUBSTS)


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

default_build: $(BUILD_DEPS)
	(cd $(SOURCE_DIR) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS))

print_default_build_flags:
	@print "BUILD_ENV='$(BUILD_ENV)'" ; \
	print "MAKE='$(MODULE_MAKE)'" ; \
	print "MAKEFLAGS='$(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS)'"

build-in-subdir:
	(cd $(subdir) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS) \
	 $(subdir_cmd) )

DEFAULT_INSTALL_MAKEFLAGS= -e DESTDIR=$(PROTODIR) \
	pkgconfigdir=$(MODULE_PKGCONFIG_DIR) \
	$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_INSTALL_MAKEFLAGS) $(MODULE_INSTALL_MAKEFLAGS) \

# Dependencies for make install step
INSTALL_DEPS = $(BUILD_TARGETS) $(MODTYPE_INSTALL_DEPS) $(MODULE_INSTALL_DEPS)

default_install: $(INSTALL_DEPS)
	(cd $(SOURCE_DIR) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	 install)

print_default_install_flags:
	@print "BUILD_ENV='$(BUILD_ENV)'" ; \
	print "MAKE='$(MODULE_MAKE)'" ; \
	print "MAKE_FLAGS='$(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS)'"

install-in-subdir:
	(cd $(subdir) ; \
	 $(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS) \
	 $(subdir_cmd))

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
		pc=$${pcin%%+(.pc|.in)} ; \
		($(START_CMD_ECHO) ; \
		 $(PERL) -p $(UNINSTALLED_PC_OPS) $$pc.pc > \
		    $(PROTODIR)$(MODULE_PKGCONFIG_DIR)/$$(basename $${pc})-uninstalled.pc ) ; \
	    done ; \
	fi

# Set comments in ELF binaries to help identify versions/builds
ELF_COMMENTS = $(MODULE_NAME) $(MODULE_VERSION)
ELF_COMMENTS += $(MODTYPE_ELF_COMMENTS)$(MODULE_ELF_COMMENTS)

$(BUILD_DEBUG:yes=)			DEBUG_ELF_COMMENTS=(DEBUG)
$(BUILD_DEBUG:yes=$(POUND_SIGN))	DEBUG_ELF_COMMENTS=
ELF_COMMENTS += $(DEBUG_ELF_COMMENTS)

set-elf-comments: $(BUILD_TARGETS:set-elf-comments=)
	$(TOP)/common/set-elf-comments.pl -B 'hg id' \
	 -M '$(ELF_COMMENTS)' $(SOURCE_DIR)


### Other tools/dependencies needed to build

# Some modules (such as FreeType 2) require GNU make
$(GNUMAKE):
	@print -n "Building with GNU make from $$(whence gmake): "
	@$(GNUMAKE) --version

$(MAKE):
	@print "Building with make from $$(whence $(MAKE))"

# Some modules use lndir to merge in sources from Sun specific directories
$(LNDIR):
	(cd $(TOP)/util/lndir && $(MAKE) $(MAKEFLAGS) install)

# Some modules use these tools to convert DocBook docs to text or html
$(DOCBOOK2HTML) + $(DOCBOOK2TEXT):
	cd $(TOP)/doc/docbook-utils && $(MAKE) $(MAKEFLAGS) install

$(XORG_DEFS_ENT):
	cd $(TOP)/doc/xorg-sgml-doctools && $(MAKE) $(MAKEFLAGS) install

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
	    gawk '/^--- /	{exit} \
		  /^diff -u/	{exit} \
			 	{print}' $$p > new/$$p ; \
	    gdiff -urp -x '*~' -x '*.orig' ./prev ./tmp-regen \
		| sed -e 's% \./prev/$(SOURCE_DIRNAME)/% %' -e 's% \./tmp-regen/$(SOURCE_DIRNAME)/% %' \
		| grep -v '^Common subdirectories:' \
		| grep -v '^Only in ' \
		>> new/$$p ; \
	    CUR_LIST="$${CUR_LIST} new/$$p" ; \
	done
	rm -rf tmp-regen

# Derive directory name relative to open-src
THIS_MODULE:sh=echo ${PWD} | sed 's|.*/open-src/||'

# Allows running "make list-patches" to get a list of all patches we
# currently apply in our builds
list-patches:
	@if [[ "$(SOURCE_PATCHES)" != "" ]] ; then \
	    for p in $(SOURCE_PATCHES) x ; do \
		if [[ "$$p" != "x" ]] ; then \
	            print "$(THIS_MODULE)/$${p%,*}" ;  \
		fi ; \
	    done ; \
	fi

### Licensing files

# Default license file name is COPYING (X.Org & GNU autoconf default)
# For other license file names, set LICENSE_FILE to the file name
# and set LICENSE_FILE_SET=yes
# A file in the same directory as the Makefile takes precedence
# over one that is delivered in the package.
LICENSE_FILE_DEFAULT	= $(LICENSE_FILE_SET:yes=$(POUND_SIGN))
$(LICENSE_FILE_DEFAULT)	LICENSE_FILE=COPYING

MODULE_LICENSE_DESTDIR	= $(LICENSE_DEST)/$(THIS_MODULE)

install_license: $(MODULE_LICENSE_DESTDIR)/COPYING

$(MODULE_LICENSE_DESTDIR):
	mkdir -p $@

$(MODULE_LICENSE_DESTDIR)/COPYING: $(MODULE_LICENSE_DESTDIR) $(SOURCE_TARGETS)
	@ $(START_CMD_ECHO) ; \
	if [[ -f $(LICENSE_FILE) ]] ; then \
	    LICENSE_TO_COPY="$(LICENSE_FILE)" ; \
	elif [[ -f $(SOURCE_DIR)/$(LICENSE_FILE) ]] ; then \
	    LICENSE_TO_COPY="$(SOURCE_DIR)/$(LICENSE_FILE)" ; \
	fi ; \
	if [[ "$${LICENSE_TO_COPY}" != "" ]] ; then \
	    rm -f $@ ; \
	    cp -pf $${LICENSE_TO_COPY} $@ ; \
	fi

# For use in debugging or calling from scripts like the code review helper
# Use such as: make print_make_vars VARS="MODTYPE MODULE_NAME"
# or: make print_make_var BUILD_TYPE=32 VARS=SOURCE_DIR
print_make_var-%:
	@print -n ' $(@:print_make_var-%=%)="$($(@:print_make_var-%=%))"'

print_make_vars: $(VARS:%=print_make_var-%)
	@print '\n'
