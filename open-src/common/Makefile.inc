# -*- Makefile -*- rules commonly shared among X consolidation open source dirs
#
# Copyright (c) 2002, 2016, Oracle and/or its affiliates. All rights reserved.
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

TOP:sh=hg root

### For compatibility, include Makefile.defs
include $(TOP)/open-src/common/Makefile.defs

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
.PARALLEL: $(CLEAN_RULES) clobber

# Remove directories with both styles of naming
$(CLEAN_RULES):
	-rm -rf $(BUILD_DIR_$(@:clean_%=%)) $(@:clean_%=build_%) $(@:clean_%=build-%)

clobber: clean
	-rm -rf $(SOURCE_DIR)

# Tell dmake not to remove and create source in parallel
.NOPARALLEL: clobber source

# Rules for steps repeated for each build type

CONFIGURE_RULES = $(BUILD_TYPES:%=configure_%)
configure: $(CONFIGURE_RULES)
.PARALLEL: $(CONFIGURE_RULES)

configure-gen: source $(CONFIGURE_TARGETS)

$(CONFIGURE_RULES): $(SOURCE_TARGETS)
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
	    if [[ "$(SOURCE_TARBALL_NAME)" == "NONE" ]] ; then \
		touch "$(SOURCE_TARBALL)" ; \
	    else \
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
	    if [[ -n "${TARBALL_SHA256}" ]] ; then \
		ACTUAL_SHA256="$$(digest -a sha256 ${SOURCE_TARBALL})" ; \
	    	if [[ "$${ACTUAL_SHA256}" != "${TARBALL_SHA256}" ]] ; then \
			print -u2 \
			  "ERROR: SHA256 sum mismatch on ${SOURCE_TARBALL}\n" \
			  "$${ACTUAL_SHA256} !=\n ${TARBALL_SHA256}" ; \
			exit 1; \
	 	fi ; \
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
	$(MAKE) SOURCE_DIR="$(GIT_CLONE_DIR)" init_source
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
		$(MAKE) SOURCE_PATCHES="$$f" SOURCE_DIR="$(GIT_CLONE_DIR)" \
			patched_source ; \
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
	mkdir -p $(SOURCE_DIR)
	@ if [[ "$(SOURCE_TARBALL_NAME)" != "NONE" ]] ; then \
	  if [[ "$(MODULE_VERSION)" = "git" ]] ; then \
	    SOURCE_CMD='gtar --exclude=.git* -cf - $(GIT_CLONE_DIR)' ; \
	    SOURCE_FILE='-' ; \
	  else \
	    SOURCE_CMD=':' ; SOURCE_FILE='$(SOURCE_TARBALL)' ; \
	  fi ; \
	  $(START_CMD_ECHO) ; \
	  $${SOURCE_CMD} | \
	    gtar --directory="$(SOURCE_DIR)" --strip-components=1 \
		$(ADD_TAR_FLAGS) -xf $${SOURCE_FILE} ; \
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
	    if [[ -x /usr/bin/lndir ]] ; then \
	      lndir=/usr/bin/lndir ; \
	    else \
	      (cd $(TOP)/open-src/util/lndir && $(MAKE) $(MAKEFLAGS) install) ; \
	    fi ; \
	  fi ; \
	  $(START_CMD_ECHO) ; \
	  cd $(SOURCE_DIR) && $${lndir:-$(LNDIR)} ../$(ADDITIONAL_SOURCE_DIR) ; \
	fi
	touch $(SOURCE_ADD_TARGET)

# Step 3: Apply any local patches (must come after add_source is finished)
#
# SOURCE_PATCHES is a list in the format: filename[,flags]
# $${p/*,} is ksh93 syntax to remove the comma and everything before it in $p
# $${p%,*} is ksh93 syntax to remove the comma and everything after it in $p
PATCHED_TARGET_FILE=.patches.done
PATCHED_TARGET=$(SOURCE_DIR)/$(PATCHED_TARGET_FILE)
patched_source: $(PATCHED_TARGET)

# Include Xorg autoconf macros when rebuilding configure scripts
# and use specific version of automake.
ACLOCAL_PATH=/usr/bin/aclocal-$(AUTOMAKE_VERSION)
ACLOCAL_INCLUDES = -I $(PROTODIR)$(X11_ACLOCAL_DIR) \
	-I /usr/share/aclocal -I /usr/share/aclocal-$(AUTOMAKE_VERSION)
AUTOMAKE_PATH=/usr/bin/automake-$(AUTOMAKE_VERSION)
AUTORECONF_ENV= ACLOCAL="$(ACLOCAL_PATH) $(ACLOCAL_INCLUDES)" \
	AUTOMAKE="$(AUTOMAKE_PATH)"

# XORG_WITH_XMLTO check below makes sure we're using a version of that macro
# compatible with xmlto 0.0.27 & later, as provided in util-macros > 0.19.0
$(PATCHED_TARGET): $(SOURCE_ADD_TARGET)
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
	fi ; \
	autoreconf="$(AUTORECONF)" ; \
	if [[ -f '$(SOURCE_DIR)/configure.ac' ]] ; then \
	    if grep -q XORG_WITH_XMLTO $(SOURCE_DIR)/configure.ac ; then \
	      if ! grep -q 'xmlto 0\.0\.27' $(SOURCE_DIR)/configure ; then \
		    print '* Forcing autoreconf to apply xmlto macro fix' ; \
		    autoreconf="yes" ; \
	            fi ; \
	    fi ; \
	fi ; \
	if [[ "$${autoreconf}" = "yes" || "$(MODULE_VERSION)" = "git" ]] ; then \
	     ($(START_CMD_ECHO) ; cd $(SOURCE_DIR) && \
	      $(AUTORECONF_ENV) autoreconf -v --install --force) ; \
	fi ; \
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

# Perl operations to perform on pkg-config *.pc files we build/install
# First, make warnings about missing files into fatal errors
FIX_PC_OPS  = $(PERL_MISSING_FILES_ERROR)
# Remove -L flags for libraries in the default library path (/usr/lib{/64})
FIX_PC_OPS += -e 's|-L\$$\{libdir}|| if "$(MODULE_ADD_LIB_SUBDIR)" eq "";'
# Add -R flags for libraries in non-default library paths
FIX_PC_OPS += -e 's|-L\$$\{libdir}|-L\$$\{libdir} -R\$$\{libdir}|;'
# Remove duplicate -R flags if upstream already provided one
FIX_PC_OPS += -e 's|-R\$$\{libdir}( .*)? -R\$$\{libdir}|-R\$$\{libdir}$$1|g;'
# Allow modules to add their own operations
FIX_PC_OPS += $(MODTYPE_FIX_PC_FLAGS) $(MODULE_FIX_PC_FLAGS)

$(ORIG_PC_FILES): $(PATCHED_TARGET)
	@ print "FIX_PC: $(@:.orig=)"
	$(MAKE_QUIET)$(PERL) -i.orig -p $(FIX_PC_OPS) $(@:.orig=)

# Tie them all together
UNPACK_TARGETS = $(SOURCE_INIT_TARGET) $(PATCHED_TARGET) \
	$(SOURCE_ADD_TARGET) $(FIXED_MAN_TARGET) $(FIXED_PC_TARGET)

source: $(SOURCE_TARGETS)
default_source: $(SOURCE_TARGETS)

$(UNPACK_TARGET): $(SOURCE_DEPS) $(UNPACK_TARGETS)
	touch $(UNPACK_TARGET)

# Make sure build directory exists, and if CLONE_SRC=yes, then
# lndir the sources into it.
$(BUILD_DIR): $(SOURCE_TARGETS)
	mkdir -p $@
	@if [[ "$(CLONE_SRC)" = "yes" ]]; then \
		$(START_CMD_ECHO) ; \
		cd $(BUILD_DIR) && $(LNDIR) ../$(SOURCE_DIR) ; \
	fi

### Run configure scripts

# Dependencies for running configure to make a Makefile
CONFIGURE_DEPS = $(SOURCE_TARGETS) $(BUILD_DIR) \
	$(MODTYPE_CONFIGURE_DEPS) $(MODULE_CONFIGURE_DEPS)

# Command line options to GNU autoconf configure script
DEFAULT_CONFIG_OPTS = --prefix=$(MODULE_PREFIX) \
	--with-xmlto --with-asciidoc --without-fop --enable-specs \
	--mandir='$${prefix}/share/man' --localstatedir=/var --sysconfdir=/etc

CONFIG_OPTS = $(DEFAULT_CONFIG_OPTS) \
	$(MODTYPE_CONFIG_OPTS) $(MODULE_CONFIG_OPTS)

# Environment variable options to GNU autoconf configure script
$(USE_DEFAULT_CONFIG_CPPFLAGS:no=$(POUND_SIGN)) DEFAULT_CONFIG_CPPFLAGS= \
	-I$(PROTODIR)/usr/include -I$(PROTODIR)/usr/include/freetype2
$(USE_DEFAULT_CONFIG_LDFLAGS:no=$(POUND_SIGN)) DEFAULT_CONFIG_LDFLAGS= \
	-L$(PROTODIR)/usr/lib$(ARCHLIBSUBDIR)

DEFAULT_PKG_CONFIG_PATH=$(PROTODIR)$(PKGCONFIG_DIR):$(PROTODIR)$(PKGCONFIG_DIR_SHARE):$(PKGCONFIG_DIR):$(PKGCONFIG_DIR_SHARE)

# Most upstream software is written by developers using bash as /bin/sh, so
# we run configure scripts with that to minimize problems and work around
# some ksh93 bugs we've hit
CONFIG_SHELL_DEFAULT		= $(CONFIG_SHELL_SET:yes=$(POUND_SIGN))
$(CONFIG_SHELL_DEFAULT)		CONFIG_SHELL = /usr/bin/bash

# Set USE_DEFAULT_CONFIG_ENV=no in a Makefile to not use this default
# autoconfig environment
DEFAULT_CONFIG_ENV  = CC="$(CC)" CXX="$(CXX)" AR="$(AR)"
DEFAULT_CONFIG_ENV += LD="$(CC) $(ARCH_FLAGS) $(LDFLAGS)"
DEFAULT_CONFIG_ENV += CFLAGS="$(MODTYPE_CFLAGS) $(MODULE_CFLAGS)" 
DEFAULT_CONFIG_ENV += CXXFLAGS="$(MODTYPE_CXXFLAGS) $(MODULE_CXXFLAGS)"
DEFAULT_CONFIG_ENV += CPPFLAGS="$(DEFAULT_CONFIG_CPPFLAGS) $(MODTYPE_CPPFLAGS) $(MODULE_CPPFLAGS)"
DEFAULT_CONFIG_ENV += LDFLAGS="$(DEFAULT_CONFIG_LDFLAGS) $(MODTYPE_CFLAGS) $(MODTYPE_LDFLAGS) $(MODULE_CFLAGS) $(MODULE_LDFLAGS)"
DEFAULT_CONFIG_ENV += PKG_CONFIG_PATH="$(DEFAULT_PKG_CONFIG_PATH)"
DEFAULT_CONFIG_ENV += PERL="$(PERL)"
DEFAULT_CONFIG_ENV += PYTHON="$(PYTHON)" PYTHONPATH="$(PYTHON_PATH)"
DEFAULT_CONFIG_ENV += am_cv_python_pythondir="$(PYTHON_VENDOR_DIR)"
DEFAULT_CONFIG_ENV += am_cv_python_pyexecdir="$(PYTHON_VENDOR_DIR)"
DEFAULT_CONFIG_ENV += INSTALL="$(INSTALL_SCRIPT)" CPPROG="cp -p"
DEFAULT_CONFIG_ENV += MAKE="$(MODULE_MAKE)" CONFIG_SHELL="$(CONFIG_SHELL)"
# gnu/nm added for working around 17596443 - nm -p output change breaks autoconf
DEFAULT_CONFIG_ENV += NM="/usr/gnu/bin/nm"
# Never use xmkmf to find X libraries from autoconf - may not be installed
# on all machines and imake config files are generally not kept up to date
DEFAULT_CONFIG_ENV += XMKMF="false"
# Override default man page sections until all X.Org modules are updated
# to use new xorg-macros with Solaris 12 man page section support
DEFAULT_CONFIG_ENV += DRIVER_MAN_SUFFIX=4 FILE_MAN_SUFFIX=5
DEFAULT_CONFIG_ENV += MISC_MAN_SUFFIX=7 ADMIN_MAN_SUFFIX=8
# When using /usr/bin/*-config programs to get build flags, be sure to use
# version matching build type
DEFAULT_CONFIG_ENV += PATH="/usr/bin/$(SUBDIR$(BUILD_TYPE)):$(PATH)"

$(USE_DEFAULT_CONFIG_ENV:no=$(POUND_SIGN)) CONFIG_ENV = $(DEFAULT_CONFIG_ENV) 
$(WITH_PARFAIT) CONFIG_ENV += $(PARFAIT_ENV)
CONFIG_ENV += $(MODTYPE_CONFIG_ENV) $(MODULE_CONFIG_ENV)

# File created by running configure - defaults to the top-level Makefile
# in a source directory unless AUTOCONF_TARGET_SET=yes is set
AUTOCONF_TARGET_DEFAULT = $(AUTOCONF_TARGET_SET:yes=$(POUND_SIGN))
$(AUTOCONF_TARGET_DEFAULT)	AUTOCONF_TARGET=$(SOURCE_DIR)/Makefile

CONFIG_SCRIPT = ../$(SOURCE_DIR)/configure

$(AUTOCONF_TARGET): $(CONFIGURE_DEPS)
	cd $(BUILD_DIR) ; \
	chmod a+x $(CONFIG_SCRIPT) ; \
	$(CONFIG_ENV) $(CONFIG_SHELL) $(CONFIG_SCRIPT) $(CONFIG_OPTS) ; \
	if [[ "$(DELIBTOOLIZE)" = "yes" ]] ; then \
	    $(PERL) $(TOP)/open-src/common/delibtoolize.pl $(DELIBTOOLIZE_FLAGS) . ; \
	fi
	-rm -f $(BUILD_DIR)/a.out.bc

default_configure: $(AUTOCONF_TARGET)

### Default rules for common build pattern

LD_OPTIONS = $(DEFAULT_LD_OPTIONS) $(MODTYPE_LD_OPTIONS) $(MODULE_LD_OPTIONS)

LD_EXEC_OPTIONS  = $(DEFAULT_LD_EXEC_OPTIONS)
LD_EXEC_OPTIONS += $(MODTYPE_LD_EXEC_OPTIONS) $(MODULE_LD_EXEC_OPTIONS)

LD_SHARED_OPTIONS  = $(DEFAULT_LD_SHARED_OPTIONS)
LD_SHARED_OPTIONS += $(MODTYPE_LD_SHARED_OPTIONS) $(MODULE_LD_SHARED_OPTIONS)

BUILD_ENV  = LD_OPTIONS='$(LD_OPTIONS)'
BUILD_ENV += LD_EXEC_OPTIONS='$(LD_EXEC_OPTIONS)'
BUILD_ENV += LD_SHARED_OPTIONS='$(LD_SHARED_OPTIONS)'
BUILD_ENV += PROTODIR='$(PROTODIR)'
BUILD_ENV += MAKE='$(MODULE_MAKE)'
# MKDIRPROG - Workaround parallel build race condition
#		in older upstream packaged install-sh
BUILD_ENV += MKDIRPROG='mkdir -p'
BUILD_ENV += PYTHONPATH="$(PYTHON_PATH)"
$(WITH_PARFAIT) BUILD_ENV += $(PARFAIT_ENV)
BUILD_ENV += $(MODTYPE_BUILD_ENV) $(MODULE_BUILD_ENV)

DEFAULT_BUILD_MAKEFLAGS=$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_BUILD_MAKEFLAGS) $(MODULE_BUILD_MAKEFLAGS)

# Dependencies for make build step
BUILD_DEPS = $(SOURCE_TARGETS) $(CONFIGURE_TARGETS) $(MODTYPE_BUILD_DEPS) $(MODULE_BUILD_DEPS)

DEFAULT_BUILD_COMMAND = cd @DIR@ ; \
	$(BUILD_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_BUILD_MAKEFLAGS)

default_build: $(BUILD_DEPS)
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(BUILD_DIR))

build-in-subdir:
	$(DEFAULT_BUILD_COMMAND:@DIR@=$(subdir)) $(subdir_cmd)

INSTALL_ENV = $(BUILD_ENV) \
	INSTALL="$(INSTALL_SCRIPT)" CPPROG="cp -p" \
	$(MODTYPE_INSTALL_ENV) $(MODULE_INSTALL_ENV)

DEFAULT_INSTALL_MAKEFLAGS= -e DESTDIR=$(PROTODIR) \
	pkgconfigdir=$(MODULE_PKGCONFIG_DIR) \
	$(MODTYPE_MAKEFLAGS) $(MODULE_MAKEFLAGS) \
	$(MODTYPE_INSTALL_MAKEFLAGS) $(MODULE_INSTALL_MAKEFLAGS)

# Dependencies for make install step
INSTALL_DEPS = $(BUILD_TARGETS) $(MODTYPE_INSTALL_DEPS) $(MODULE_INSTALL_DEPS)

DEFAULT_INSTALL_COMMAND = cd @DIR@ ; \
	$(INSTALL_ENV) $(MODULE_MAKE) $(MAKEFLAGS) $(DEFAULT_INSTALL_MAKEFLAGS)

default_install: $(INSTALL_DEPS)
	$(DEFAULT_INSTALL_COMMAND:@DIR@=$(BUILD_DIR)) install

install-in-subdir:
	$(DEFAULT_INSTALL_COMMAND:@DIR@=$(subdir)) $(subdir_cmd) \
		$(install_subdir_cmd)

# Adjust paths in pkg-config files for -uninstalled variants used during build
# First, make warnings about missing files into fatal errors
UNINSTALLED_PC_OPS  = $(PERL_MISSING_FILES_ERROR)
# Set perl $protodir to absolute path to $(PROTODIR)
UNINSTALLED_PC_OPS += -e 'BEGIN {use Cwd; $$protodir=Cwd::realpath("$(PROTODIR)");}'
# Prepend $protodir to paths for includes, libraries to link against, etc.
UNINSTALLED_PC_OPS += -e 's|(-[LI]\s*)\$$|$$1$$protodir\$$|g;'
UNINSTALLED_PC_OPS += -e 's|^sdkdir=|sdkdir=$$protodir|g;'
UNINSTALLED_PC_OPS += -e 's|$$protodir\$$\{sdkdir}|\$$\{sdkdir}|g;'
# Allow modules to add their own operations
UNINSTALLED_PC_OPS += $(MODTYPE_ADD_UNINSTALLED_PC_OPS)
UNINSTALLED_PC_OPS += $(MODULE_ADD_UNINSTALLED_PC_OPS)

INSTALL_PCU_DEPS_DEFAULT	= $(INSTALL_PCU_DEPS_SET:yes=$(POUND_SIGN))
$(INSTALL_PCU_DEPS_DEFAULT)	INSTALL_PCU_DEPS = $(INSTALL_TARGETS)

install_pkgconfig_uninstalled: $(INSTALL_PCU_DEPS)
	@ if [[ ! -z "$(FIX_PC_FILES)" ]] ; then \
	    cd $(BUILD_DIR) ; \
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
	$(PERL) $(TOP)/open-src/common/set-elf-comments.pl \
	 $(ELF_COMMENTS_FLAGS) $(BUILD_DIR)

MANIFEST_DEST=$(PWD)/tmp-proto-$(MACH)
MANIFEST_TMP=$(PWD)/tmp.p5m

MANIFEST_TARGETS_DEFAULT	= $(MANIFEST_TARGETS_SET:yes=$(POUND_SIGN))
$(MANIFEST_TARGETS_DEFAULT)	MANIFEST_TARGETS=$(MODULE_PKG_MANIFEST)

manifest: $(MANIFEST_TARGETS)

manifest-per-type: $(MODULE_PKG_MANIFEST)

MANIFEST_INSTALL_RULES = PROTOTOOLSDIR="$(PROTODIR)" \
	PROTODIR="$(MANIFEST_DEST)"

$(MANIFEST_DEST): install
	/usr/xpg4/bin/make $(MAKEFLAGS) install $(MANIFEST_INSTALL_RULES) \
		|| (rm -rf $(MANIFEST_DEST) ; exit 1)

$(MANIFEST_TMP): $(MANIFEST_DEST)
	$(PERL) $(TOP)/open-src/common/pkg-manifest-generate.pl \
		input_manifest='$(MODULE_PKG_MANIFEST)' \
		exception_manifest='$(PKG_SRC_DIR)/exceptions-validate_pkg.p5m' \
		manifest_license='$(PKG_LICENSE_DIR)/cr_Oracle.tmpl' \
		manifest_license='$(PKG_LICENSE_DIR)/lic_X11' \
		pkg_name='$(MODULE_PKGNAME)' \
		summary='$(MODULE_DESC)' \
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

# Some modules use lndir to merge in sources from Sun specific directories
$(LNDIR):
	(cd $(TOP)/open-src/util/lndir && $(MAKE) $(MAKEFLAGS) install)

### Useful rules for maintaining patch sets

# regenerate patches against new baseline to reduce messages about hunks
# having fuzz or offsets when applying patches - new patches will be in new/

regen-patches:
	CUR_LIST=""
	rm -rf tmp-regen
	$(MAKE) SOURCE_PATCHES="$(CUR_LIST)" SOURCE_DIR=tmp-regen \
	    AUTORECONF="no" patched_source
	mkdir -p new
	for f in $(SOURCE_PATCHES) ; do \
	    p=$${f%,*} ; \
	    print '=================' $$p ; \
	    rm -rf prev ; \
	    mv tmp-regen prev ; \
	    $(MAKE) SOURCE_PATCHES="$${CUR_LIST} $$f" SOURCE_DIR=tmp-regen \
		AUTORECONF="no" patched_source ; \
	    find tmp-regen -name '*.orig' -exec rm {} \+ ; \
	    gawk '/^--- /	{exit} \
		  /^diff -/	{exit} \
			 	{print}' $$p > new/$$p ; \
	    LC_COLLATE=C git diff --no-color ./prev ./tmp-regen \
		| sed -e 's% a/\./prev/% a/%' -e 's% b/\./tmp-regen/% b/%' \
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
GIT_URL_xorg		= $(XORG_GIT_URL)/$(SOURCE_TARBALL_DIR)/$(MODULE_NAME:font-%=%)
# TPNO's for X.Org software & font batches
ORACLE_TPNO_xorg	= 28154
ORACLE_TPNO_xorg_fonts	= 11716

UPSTREAM_xcb		= The XCB Project
UPSTREAM_URL_xcb	= http://xcb.freedesktop.org/
SOURCE_URL_xcb		= http://xcb.freedesktop.org/dist/$(SOURCE_TARBALL_NAME)
GIT_URL_xcb		= $(FDO_GIT_URL)/xcb/$(MODULE_NAME:xcb-%=%)
ORACLE_TPNO_xcb		= 18006

UPSTREAM_PROJECT_DEFAULT	= $(UPSTREAM_PROJECT_SET:yes=$(POUND_SIGN))

UPSTREAM_DEFAULT	= $(UPSTREAM_SET:yes=$(POUND_SIGN))
$(UPSTREAM_DEFAULT) UPSTREAM = $(UPSTREAM_$(SOURCE_TARBALL_NAME))
$(UPSTREAM_DEFAULT) UPSTREAM_SET_YET = $(UPSTREAM_$(SOURCE_TARBALL_NAME)_SET)
$(UPSTREAM_DEFAULT) UPSTREAM_NOTFOUND = $(UPSTREAM_SET_YET:yes=$(POUND_SIGN))
$(UPSTREAM_DEFAULT) $(UPSTREAM_NOTFOUND) $(UPSTREAM_PROJECT_DEFAULT) UPSTREAM_PROJECT = xorg
$(UPSTREAM_NOTFOUND) $(SOURCE_URL_DEFAULT) UPSTREAM = $(UPSTREAM_$(UPSTREAM_PROJECT))
$(UPSTREAM_NOTFOUND) $(SOURCE_URL_DEFAULT) UPSTREAM_URL = $(UPSTREAM_URL_$(UPSTREAM_PROJECT))
ORACLE_TPNO_DEFAULT	= $(ORACLE_TPNO_SET:yes=$(POUND_SIGN))
$(UPSTREAM_NOTFOUND) $(SOURCE_URL_DEFAULT) $(ORACLE_TPNO_DEFAULT) ORACLE_TPNO = $(ORACLE_TPNO_$(UPSTREAM_PROJECT))

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
	    print 'set name=info.source-url value="$(SOURCE_URL)"' >> $@ ; \
	fi
	@if [[ "$(UPSTREAM)" != "NONE" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=info.upstream value="$(UPSTREAM)"' >> $@ ; \
	    print 'set name=info.upstream-url value="$(UPSTREAM_URL)"' >> $@ ; \
	fi
	@if [[ ! -z "$(GIT_URL)" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=info.repository-url value="$(GIT_URL)"' >> $@ ; \
	fi
	@if [[ ! -z "$(MODULE_ARC_CASES)" ]] ; then \
	    $(START_CMD_ECHO) ; \
	    print 'set name=org.opensolaris.arc-caseid' \
		'$(MODULE_ARC_CASES:%=value="%")' >> $@ ;  \
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

LICENSE_FILE_NAME = license_$(MODULE_NAME)
LICENSE_FILE_DEST = $(MODULE_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME)
LICENSE_ATTR_FILE = $(MODULE_PKG_METADATA_DIR)/$(LICENSE_FILE_NAME).p5m

# Default license file name is COPYING (X.Org & GNU autoconf default)
# For other license file names, set LICENSE_FILE to the file name
# and set LICENSE_FILE_SET=yes
# A file in the same directory as the Makefile takes precedence
# over one that is delivered in the package.
LICENSE_FILE_DEFAULT	= $(LICENSE_FILE_SET:yes=$(POUND_SIGN))
$(LICENSE_FILE_DEFAULT)	LICENSE_FILE=COPYING

# Oracle copyright notice template
ORACLE_COPYRIGHT_TEMPLATE=$(PKG_LICENSE_DIR)/cr_Oracle.tmpl

# Default is the MIT/X11 license
MIT_LICENSE_NAME = MIT License
MIT_LICENSE_FILE = $(PKG_LICENSE_DIR)/lic_X11

MODULE_LICENSE_DEFAULT		= $(MODULE_LICENSE_SET:yes=$(POUND_SIGN))
$(MODULE_LICENSE_DEFAULT)	MODULE_LICENSE = $(MIT_LICENSE_NAME)

# Each license file should have a header showing which upstream module it's 
# from for easier viewing/cross-referencing.  The format of this header is
# common to all Solaris consolidations, so should not be changed without
# coordination.
PRINT_IN_BOX = $(PERL) -n -e '$$border = "-" x length($$_);' \
	-e 'printf "--%s--\n- %s -\n--%s--\n\n", $$border, $$_, $$border'

# Some packages contain only code that Oracle owns all the rights to.
# For these, Makefile.options.oracle.com chooses to omit the license
# notice when Oracle is building its distros.   Everyone else gets the
# notice required by open source license Oracle released the code under.
LICENSE_CHOICE_DEFAULT		= $(LICENSE_CHOICE_SET:yes=$(POUND_SIGN))
$(LICENSE_CHOICE_DEFAULT)	LICENSE_CHOICE=open

ORACLE_LICENSE_NAME_open	= $(MIT_LICENSE_NAME)
ORACLE_LICENSE_INSERT_open	= $(MIT_LICENSE_FILE)

ORACLE_LICENSE_NAME_Oracle	= Oracle Copyright Notice
ORACLE_LICENSE_INSERT_Oracle 	= # Nothing to append to copyright notice

ORACLE_LICENSE_NAME 		= $(ORACLE_LICENSE_NAME_$(LICENSE_CHOICE))
ORACLE_LICENSE_INSERT		= $(ORACLE_LICENSE_INSERT_$(LICENSE_CHOICE))

# Use default license info unless MODULE_LICENSE = "Oracle"
LICENSE_NAME			= $(MODULE_LICENSE)
LICENSE_PREPEND			= $(MODULE_LICENSE_PREPEND)
LICENSE_INSERT 			= $(MODULE_LICENSE_INSERT)

LICENSE_OVERRIDE_1		= $(POUND_SIGN)$(MODULE_LICENSE)
LICENSE_OVERRIDE		= $(LICENSE_OVERRIDE_1:$(POUND_SIGN)Oracle%=)
$(LICENSE_OVERRIDE)		LICENSE_NAME = $(ORACLE_LICENSE_NAME)
$(LICENSE_OVERRIDE)		LICENSE_INSERT = $(ORACLE_LICENSE_INSERT)

# Name to use in TPNO metadata & license file headers
MODULE_TPNO_NAME_DEFAULT	= $(MODULE_TPNO_NAME_SET:yes=$(POUND_SIGN))
$(MODULE_TPNO_NAME_DEFAULT)	MODULE_TPNO_NAME = $(MODULE_NAME)

install_license: $(LICENSE_FILE_DEST)
METADATA_TARGETS += $(LICENSE_FILE_DEST)

%/$(LICENSE_FILE_NAME): % $(SOURCE_TARGETS) $(LICENSE_PREPEND) $(LICENSE_INSERT)
	@ $(START_CMD_ECHO) ; \
	if [[ -f '$(LICENSE_FILE)' ]] ; then \
	    license_to_copy='$(LICENSE_FILE)' ; \
	elif [[ -f '$(SOURCE_DIR)/$(LICENSE_FILE)' ]] ; then \
	    license_to_copy='$(SOURCE_DIR)/$(LICENSE_FILE)' ; \
	else \
	    license_to_copy='/dev/null' ; \
	fi ; \
	if [[ -s "$${license_to_copy}" || -n '$(ORACLE_COPYRIGHT_YEARS)' ]] ; then \
	    m='$(MODULE_TPNO_NAME)' ; \
	    if [[ '$(MODULE_VERSION)' != 'src' ]] ; then \
		m+=' $(MODULE_VERSION)' ; \
	    fi ; \
	    print -n $$m | $(PRINT_IN_BOX) > $@ ; \
	fi ; \
	if [[ -n '$(ORACLE_COPYRIGHT_YEARS)' ]] ; then \
	    sed 's/@YEARS@/$(ORACLE_COPYRIGHT_YEARS)/' \
		$(ORACLE_COPYRIGHT_TEMPLATE) >> $@ ; \
	    if [[ -n '$(LICENSE_INSERT)' ]] ; then \
		cat $(LICENSE_INSERT) >> $@ ; \
	    fi ; \
	fi ; \
	if [[ -n '$(MODULE_LICENSE_DESC)' ]] ; then \
	    license_desc='$(MODULE_LICENSE_DESC)' ; \
	elif [[ -n '$(MODULE_DESC)' ]] ; then \
	    license_desc='$(MODULE_DESC)' ; \
	else \
	    license_desc='$(MODTYPE_DESC)' ; \
	fi ; \
	if [[ -s "$${license_to_copy}" ]] ; then \
	    if [[ -n "$${license_desc}" ]] ; then \
		contents="$${license_desc}," ; \
	    else \
		contents="technology" ; \
	    fi ; \
	    print "This package may include $${contents} which may be subject to one or more of the following notices:\n" | fmt >> $@ ; \
	    if [[ -n '$(LICENSE_PREPEND)' ]] ; then \
		cat $(LICENSE_PREPEND) >> $@ ; \
	    fi ; \
	    cat "$${license_to_copy}" >> $@ ; \
	fi ; \
	if [[ -n '$(LICENSE_APPEND)' ]] ; then \
	  cat $(LICENSE_APPEND) >> $@ ; \
	fi ; \
	if [[ -s $@ ]] ; then \
	    license_attributes='license="$(LICENSE_NAME) ($(MODULE_TPNO_NAME))"' ; \
	    license_attributes+=' com.oracle.info.name="$(MODULE_TPNO_NAME)"' ; \
	    if [[ '$(MODULE_VERSION)' != 'src' ]] ; then \
		license_attributes+=' com.oracle.info.version="$(MODULE_VERSION)"' ; \
	    fi ; \
	    license_attributes+=" com.oracle.info.description=\"$${license_desc}\"" ; \
	    if [[ -n '$(ORACLE_TPNO)' && '$(LICENSE_CHOICE)' == 'Oracle' ]] ; then \
		license_attributes+=' com.oracle.info.tpno="$(ORACLE_TPNO)"' ; \
	    fi ; \
	    print 'license $(LICENSE_FILE_NAME)' "$${license_attributes}" > $@.p5m ; \
	else \
	    print '*** $@ is empty' ; \
	fi

install_metadata: $(METADATA_TARGETS)

### Parfait scans

parfait:
	parfait -e all -c "$(TOP)/open-src/common/X-parfait.conf" -z "$(TOP)" \
		-j "$${DMAKE_MAX_JOBS:-100%}" . || true
