###############################################################################
#
# Makefile for X Consolidation packages
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
###############################################################################
# Heavily inspired by / borrowed from the ON usr/src/pkg/Makefile
# Please read the comments in it and the accompanying README in ON for many
# details omitted here.
# Major differences in the X implementation:
# - X only supports building one repo, not redist + extra
# - X only supports building packages on the same platform as the binaries,
#	no cross-platform packaging
# - X has no bfu legacy to deal with
# - ON has all manifests in one directory, X splits into current & legacy
# - Integrates with X consolidation makefile/build system instead of ON's

PWD:sh=pwd
TOP=$(PWD)/..

### Include common definitions
include $(TOP)/open-src/common/Makefile.init

.SUFFIXES: .mf .mog .dep .res .pub

# We don't yet support building packages cross-architecture, but would
# need this if we ever did.   (See "Cross platform packaging notes" in
# the ON usr/src/pkg/Makefile for what we'd have to do.)
PKGMACH = $(MACH)

# Map X makefile names to the ones used in the rules copied from ON
# More to be updated if we ever decide to support building cross-platform
PKGROOT = $(PROTODIR)
PKGDEST = $(PROTOTOP)/pkg_$(PKGMACH)

# Map the V=1 style of enabling more verbose build messages used in the
# X.Org/automake builds to the macro used in the rules copied from ON
#
# You can enable verbose output with either V=1 or PKGDEBUG=""
PKGDEBUG_V_0 = @
PKGDEBUG_V_1 = 
PKGDEBUG_V_ = $(PKGDEBUG_V_0)
PKGDEBUG = $(PKGDEBUG_V_$(V))

# Commands used in rules copied from ON
CP=             /usr/bin/cp -f
LN=             /usr/bin/ln
MV=             /usr/bin/mv -f
RM=             /usr/bin/rm -f
TOUCH=          /usr/bin/touch

#
# To suppress package dependency generation on any system, regardless
# of how it was installed, set SUPPRESSPKGDEP=true in the build
# environment.
#
SUPPRESSPKGDEP= false


#
# The packages directory will contain the processed manifests as
# direct build targets and subdirectories for package metadata extracted
# incidentally during manifest processing.
#
# Nothing underneath $(PDIR) should ever be managed by SCM.
#
PDIR= build-$(PKGMACH)

$(PDIR):
	mkdir -p $@

#
# By default, PKGS will list all manifests.  To build and/or publish a
# subset of packages, override this on the command line or in the
# build environment and then reference (implicitly or explicitly) the all
# or install targets.
#
CURRENT_MANIFESTS :sh= (cd manifests; print *.mf)
LEGACY_MANIFESTS :sh= (cd legacy; print *.mf)
MANIFESTS = $(CURRENT_MANIFESTS) $(LEGACY_MANIFESTS)
PKGS= $(MANIFESTS:%.mf=%)

#
# Track the synthetic manifests separately so we can properly express
# build rules and dependencies.  The synthetic and real packages use
# different sets of transforms and macros for pkgmogrify.
#
SYNTH_PKGS= X-incorporation X-redistributable

# Any given package list may be empty, but we can only determine that 
# dynamically, so we always generate all lists.
#
# The meanings of each package status are as follows:
#
# 	PKGSTAT		meaning
# 	----------	----------------------------------------------------
# 	noincorp	Do not include in incorporation or group package
#	obsolete	Include in incorporation, but not group package
#	renamed		Include in incorporation, but not group package
#	current		Include in incorporation and group package
#
# Since the semantics of the "noincorp" package status dictate that
# such packages are not included in the incorporation or group packages,
# there is no need to build noincorp package lists.
#
# Since packages depend on their incorporation, noincorp necessarily
# implies nodepend, or it would inadvertently pull in other packages.
#
PKGLISTS= \
	$(PDIR)/packages.current \
	$(PDIR)/packages.renamed \
	$(PDIR)/packages.obsolete

# Manifests are in either manifests or legacy directory - merge in $(PDIR)
$(PDIR)/%.mf: manifests/%.mf
	$(PKGDEBUG)$(LN) -s ../$< $@

$(PDIR)/%.mf: legacy/%.mf
	$(PKGDEBUG)$(LN) -s ../$< $@

PMANIFESTS = $(MANIFESTS:%=$(PDIR)/%)
manifests: $(PMANIFESTS)
$(PMANIFESTS): $(PDIR)

### Additional metadata generation

# This should eventually move into pkg publication

FONT_MANIFESTS :sh= (cd manifests; print system-font-*.mf)
FONT_METADATA = $(FONT_MANIFESTS:%.mf=$(PDIR)/%.font)
FONT_ATTRS = $(FONT_MANIFESTS:%.mf=$(PDIR)/%.attr)

$(FONT_ATTRS): $(FONT_METADATA)

$(PDIR)/%.font: $(PDIR)/%.mf
	@print "## Generating font metadata in $(@F)"
	$(PKGDEBUG)$(RM) $(@)
	$(PKGDEBUG)$(PERL) generate_font_metadata.pl \
		-p $(PROTODIR) -m $< > $@ || ( rm $@ ; exit 1 )

### Manifest mogrification

PKGMOGRIFY = pkgmogrify

# RELEASE = uname -r for the target release
RELEASE = 5.11

# Generate pkgmogrify transform file with correct build version info in
# Build number is derived from last nv_XXX tag in hg log
$(PDIR)/versions: transforms/versions.tmpl $(PDIR)
	@print "## Substituting build info in versions transformation template"
	$(PKGDEBUG)XNV_BUILDNUM="$${XNV_BUILDNUM:-$$(hg log | \
	    $(PERL) -l -n -e \
		'if ($$_ =~ m/^tag:\s+nv_(\d+)/) { print $$1 ; exit}')}" ; \
	PKGVERS_BUILTON="$(RELEASE)" ; \
	PKGVERS_BRANCH="0.$${XNV_BUILDNUM}" ; \
	PKGVERS_BUILD="$${PKGVERS_BUILTON}-$${PKGVERS_BRANCH}" ; \
	HG_ID="$$(hg id)" ; \
	$(PERL) -p -e "s{_PKGVERS_BUILD_}{$${PKGVERS_BUILD}};" \
		-e "s{_HG_ID_}{$${HG_ID}}" transforms/versions.tmpl > $@

# Set variables used by pkgmogrify 
# Most upstream packages provide specific version numbers instead of using
# the default of $(OSPKGVERS)
# Some use $(X11PKGVERS) to refer to the X11 katamari version since they
# bundle together components from multiple upstream packages with individual
# version numbers
X11PKGVERS = 7.5
OSPKGVERS = 0.$(RELEASE)

# Base URL for ARC cases used in opensolaris.arc_url metadata
ARC_URL=http://arc.opensolaris.org/caselog/

# These are the macros that may be used in package manifests
PKGMOG_DEFINES= \
        i386_ONLY=$(POUND_SIGN) \
        sparc_ONLY=$(POUND_SIGN) \
        $(PKGMACH)_ONLY= \
        ARCH=$(PKGMACH) \
        ARCH32=$(SUBDIR32_$(PKGMACH)) \
        ARCH64=$(SUBDIR64_$(PKGMACH)) \
        OSPKGVERS=$(OSPKGVERS) \
        X11PKGVERS=$(X11PKGVERS) \
	ARC_URL='$(ARC_URL)'

# Default transformations to apply
PM_TRANSFORMS = common_actions publish restart_fmri facets \
		licenses $(PDIR)/versions extract_metadata
PM_INC = transforms

PKGDEP_INCORP = depend fmri=pkg:/consolidation/X/X-incorporation type=require

PROC_PKGS = $(PKGS:%=$(PDIR)/%.mog)
PROC_SYNTH_PKGS = $(SYNTH_PKGS:%=$(PDIR)/%.mog)
PROC_CURRENT_PKGS = $(CURRENT_MANIFESTS:%.mf=$(PDIR)/%.mog)

# Extra transforms for non-legacy packages
$(PROC_CURRENT_PKGS):= EXTRA_TRANSFORMS = defaults

mogrify: $(PROC_PKGS)
$(PROC_PKGS): $(PDIR)/versions

# Combine & uniq metadata & license attributes generated during build
$(PDIR)/%.attr:
	@print "## Gathering metadata from build for $(@F)"
	$(PKGDEBUG)case '$(@F)' in \
		system-font-*) ADDITIONAL_ATTRS="$(@:%.attr=%.font)" ;; \
	esac ; \
	$(PERL) merge-metadata.pl $${ADDITIONAL_ATTRS} \
		~(N)$(PROTOMETA)/$(@F:.attr=)/attributes_*.mf \
		~(N)$(PROTOMETA)/$(@F:.attr=)/license_*.mf /dev/null > $@

$(PDIR)/%.mog: $(PDIR)/%.mf $(PDIR)/%.attr
	@print "## Processing manifest $(@F:.mog=.mf)"
	$(PKGDEBUG)$(RM) $(@) $(@:%.mog=%) $(@:%.mog=%.nodepend) \
		$(@:%.mog=%.metadata.*) $(@).vars
	$(PKGDEBUG)$(PKGMOGRIFY) $(PKGMOG_VERBOSE) $(PM_INC:%= -I %) \
		$(PKGMOG_DEFINES:%=-D %) -P $(@).vars -O $(@) \
		$(@:.mog=.mf) $(@:.mog=.attr) \
		$(PM_TRANSFORMS) $(EXTRA_TRANSFORMS) 
	$(PKGDEBUG)if [[ -f $(@) ]]; then \
		eval PKGSTAT=current NODEPEND="$(SUPPRESSPKGDEP)" \
			$$(cat -s $(@).vars) ; \
		if [[ "$$NODEPEND" != "false" || "$$PKGSTAT" = "noincorp" ]]; \
		then \
			$(TOUCH) $(@:%.mog=%.nodepend); \
		fi; \
		$(LN) -s $(@F) \
			$(PDIR)/$(@F:%.mog=%).metadata.$$PKGSTAT ; \
		if [[ "$$PKGSTAT" = "current" || "$$PKGSTAT" = "renamed"  ]]; \
		then \
			print $(PKGDEP_INCORP) >> $(@); \
		fi; \
	else \
		$(TOUCH) $(@); \
	fi
	$(PKGDEBUG)$(RM) $(@).vars

# make the package lists from the real manifests for the synthetic manifests
$(PKGLISTS): $(PROC_PKGS)
	@print "## Generating $(@F:packages.%=%) package list"
	$(PKGDEBUG)$(PERL) -nl -w -Mdiagnostics \
	 -e 'if ($$_ =~ m{name=pkg.fmri value=(\S+)}) {' \
	 -e '   printf "depend fmri=$$1 type=\$$(PKGDEP_TYPE)\n";' \
	 -e '   close ARGV; ' \
	 -e '}' $(PDIR)/*.metadata.$(@F:packages.%=%) > $(@).new
	$(PKGDEBUG) mv -f $(@).new $(@)

pkglists: $(PKGLISTS)

# The package lists are generated with $(PKGDEP_TYPE) as their
# dependency types, so that they can be included by either an
# incorporation or a group package.
#
$(PDIR)/X-redistributable.mog:= PKGDEP_TYPE= require
$(PDIR)/X-incorporation.mog:= PKGDEP_TYPE= incorporate

# rule to build the synthetic manifests
$(PROC_SYNTH_PKGS): $(PKGLISTS) $$(@F:%.mog=%.mf)
	@print "## Processing synthetic manifest $(@F:%.mog=%.mf)"
	$(PKGDEBUG)$(RM) $(@) $(@:%.mog=%).metadata.*
	$(PKGDEBUG)$(PKGMOGRIFY) $(PKGMOG_VERBOSE) -I transforms -I $(PDIR) \
		$(PKGMOG_DEFINES:%=-D %) -D PKGDEP_TYPE=$(PKGDEP_TYPE) \
		-O $(@) $(@F:%.mog=%.mf) \
		$(PM_TRANSFORMS) defaults synthetic

### Dependency detection
DEP_PKGS= $(PKGS:%=$(PDIR)/%.dep)
DEP_SYNTH_PKGS= $(SYNTH_PKGS:%=$(PDIR)/%.dep)

PKGDEP_TOKENS_i386= \
	'PLATFORM=i86hvm' \
	'PLATFORM=i86pc' \
	'PLATFORM=i86xpv' \
	'ISALIST=amd64' \
	'ISALIST=i386'
PKGDEP_TOKENS_sparc= \
	'PLATFORM=sun4u' \
	'PLATFORM=sun4v' \
	'ISALIST=sparcv9' \
	'ISALIST=sparc'
PKGDEP_TOKENS= $(PKGDEP_TOKENS_$(PKGMACH))

$(PDIR)/%.dep: $(PDIR)/%.mog
	@print "## Generating dependencies for $(<F)"
	$(PKGDEBUG)$(RM) $(@)
	$(PKGDEBUG)if [[ ! -f $(@:%.dep=%.nodepend) ]]; then \
		pkgdepend generate -m $(PKGDEP_TOKENS:%=-D %) $(<) \
			$(PKGROOT) > $(@); \
	else \
		$(CP) $(<) $(@); \
	fi

$(DEP_SYNTH_PKGS): $$(@:%.dep=%.mog)
	@print "## Skipping dependency generation for $(@F:%.dep=%)"
	$(PKGDEBUG)$(CP) $(@:%.dep=%.mog) $(@)

### Dependency resolution

#
# This rule resolves dependencies across all published manifests.
# We should be able to do this with
#
#    pkgdepend resolve -m $(PUB_PKGS:%.pub=%.dep)
#
# but until 14113 is fixed, the incorporations confuse pkgdepend, so we
# just create the .res file for DEP_SYNTH_PKGS directly.
#

PKGDEP_VERBOSE_FLAG_0 =
PKGDEP_VERBOSE_FLAG_1 = -v
PKGDEP_VERBOSE_FLAG = $(PKGDEP_VERBOSE_FLAG_$(V))

$(PDIR)/gendeps: $(DEP_SYNTH_PKGS) $(DEP_PKGS)
	-$(PKGDEBUG)if [[ "$(SUPPRESSPKGDEP)" = "true" ]]; then \
		print "## Suppressing dependency resolution"; \
		for p in $(DEP_PKGS:%.dep=%); do \
			$(CP) $$p.dep $$p.res; \
		done; \
	else \
		print "## Resolving dependencies"; \
		pkgdepend resolve $(PKGDEP_VERBOSE_FLAG) -m $(DEP_PKGS); \
		for p in $(DEP_PKGS:%.dep=%); do \
			$(MV) $$p.dep.res $$p.res; \
		done; \
	fi
	$(PKGDEBUG)for p in $(DEP_SYNTH_PKGS:%.dep=%); \
	do \
		$(CP) $$p.dep $$p.res; \
	done
	$(PKGDEBUG)$(TOUCH) $(@)

gendeps: $(PDIR)/gendeps

### Repo setup

# Default in developer builds is "xnv-devel"
# X RE cronjob build scripts override to "xnv-nightly" or "xnv-biweekly"
PKGPUBLISHER = xnv-devel

# Initialize the empty on-disk repositories
$(PKGDEST):
	@print "## Initializing $(@F)"
	$(PKGDEBUG)mkdir -p $@
	$(PKGDEBUG)pkgsend -s file://$(@) create-repository \
		--set-property publisher.prefix=$(PKGPUBLISHER)

clean-repo:
	$(RM) -r $(PKGDEST)

### Package publication to repo
PUB_PKGS= $(SYNTH_PKGS:%=$(PDIR)/%.pub) $(PKGS:%=$(PDIR)/%.pub)
publish_pkgs: $(PKGDEST) $(PDIR)/gendeps $(PUB_PKGS)

$(PUB_PKGS): $(PKGDEST) $(PDIR)/gendeps

$(PDIR)/%.pub:  $(PKGDEST) $(PDIR)/gendeps $(PDIR)/%.res
	$(PKGDEBUG) if [ -s $(@:%.pub=%.res) ]; then \
		print "## Publishing $(@F:%.pub=%) to proto repository"; \
		pkgsend -s file://$(PKGDEST) publish -d $(PKGROOT) \
		    -d license_files -d $(PROTOMETA)/$(@F:%.pub=%) \
		    --fmri-in-manifest --no-index --no-catalog \
		    $(@:%.pub=%.res) > /dev/null; \
	fi; \
	$(TOUCH) $(@)

### Repo finalization

install: repository-metadata
default: install

repository-metadata: $(PUB_PKGS)
	@print "## Creating repository metadata"
	$(PKGDEBUG)/usr/lib/pkg.depotd -d $(PKGDEST) \
		--add-content --exit-ready
	$(PKGDEBUG)chmod a+r $(PKGDEST)/cfg_cache

### Overall rules

all: $(PROC_PKGS) $(PROC_SYNTH_PKGS) $(DEP_PKGS) $(DEP_SYNTH_PKGS) \
	$(PDIR)/gendeps

clean:
	-$(RM) -r $(PDIR)

.PARALLEL: $(PKGS) $(PROC_PKGS) $(DEP_PKGS) \
	$(PROC_SYNTH_PKGS) $(DEP_SYNTH_PKGS) $(PUB_PKGS)

#
# rules to validate proto area against manifests, and check for safe
# file permission modes
#

EXCEPTIONS= $(PWD)/../exception_lists/packaging $(PDIR)/generated-exceptions

# Automatically exclude all pkg-config uninstalled.pc files, since those
# should never be packaged
$(PDIR)/generated-exceptions:
	$(PKGDEBUG)(cd $(PKGROOT) ; find . -name '*-uninstalled.pc') > $@

protocmp: $(PROC_PKGS) $(EXCEPTIONS) fix_perms
	$(PKGDEBUG)validate_pkg -a $(PKGMACH) -v $(EXCEPTIONS:%=-e %) \
		-m $(PDIR) -p $(PKGROOT)

pmodes: $(PROC_PKGS) fix_perms
	$(PKGDEBUG)validate_pkg -a $(PKGMACH) -M -m $(PDIR)

check: protocmp pmodes

# Instead of fixing all the upstream packages to follow our permission rules
# just fix the proto area to match our rules

fix_perms: 
	@print "## Fixing proto area permissions"
	$(PKGDEBUG)find $(PKGROOT) -type f -exec chmod a-w \{\} \+
