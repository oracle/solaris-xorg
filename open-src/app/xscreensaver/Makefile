###############################################################################
#
# Xscreensaver Makefile
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
# Use subject to license terms.
#
# Permission is hereby granted, free of charge, to any person obtaining a
# copy of this software and associated documentation files (the
# "Software"), to deal in the Software without restriction, including
# without limitation the rights to use, copy, modify, merge, publish,
# distribute, and/or sell copies of the Software, and to permit persons
# to whom the Software is furnished to do so, provided that the above
# copyright notice(s) and this permission notice appear in all copies of
# the Software and that both the above copyright notice(s) and this
# permission notice appear in supporting documentation.
# 
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
# OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
# OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
# HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
# INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
# FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
# NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
# WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
# 
# Except as contained in this notice, the name of a copyright holder
# shall not be used in advertising or otherwise to promote the sale, use
# or other dealings in this Software without prior written authorization
# of the copyright holder.
#
# @(#)Makefile	1.101	08/12/23
#

# Package name used in tarballs
MODULE_NAME=xscreensaver

# Version number (used in path names)
MODULE_VERSION=5.01

# Download site for source - master site no longer carries 5.01 so we use
# a mirror site until we upgrade to a newer version
#SOURCE_URL=http://www.jwz.org/xscreensaver/$(SOURCE_TARBALL_NAME)
SOURCE_URL=http://dlc.sun.com/osol/x/downloads/mirrors/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
SOURCE_UNCOMPRESS=bzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
	datarootdir.patch,-p1 \
	Sun.app-defaults.patch \
	GNOME-desktop.patch \
	solaris-paths.patch \
	dont-bug-jwz.patch \
	gtk-lock.patch \
	allow-root.patch \
	passwdTimeout-pref.patch \
	xinput.patch \
	i18n.patch \
	debug-msgs.patch \
	dpms.patch \
	gl-error-capture.patch \
	accessibility.patch \
	trusted.patch \
	pam_audit.patch \
	bug-6450019.patch \
	Sun-colors.patch \
	barcode-hack.patch \
	glsnake.patch \
        allowRootByPassFlag.patch \
	bug-6478841.patch \
	bug-6461887.patch \
	bug1-6461887.patch \
	bug-6520014.patch  \
        NULLRootPasswd.patch \
	RobustKBGrab.patch \
	bug-6573182.patch \
	bug-6610282.patch \
	bug-6475285.patch \
        XScr.ad.lockTimeout.patch \
	bug-6583181.patch \
        notice_events.patch \
	bug-6670659.patch \
	bug-6583247.patch \
	xscreensaver-hush-misc.patch,-p1 \
	bug-6698996.patch \
	intltool.patch

# We patch configure.ac & Makefile.am's so need to autoreconf
AUTORECONF=yes

# Where to find OpenGL headers/libraries
OPENGL_DIR_sparc=/usr/openwin
OPENGL_DIR_i386=/usr/X11
OPENGL_DIR=$(OPENGL_DIR_$(MACH))

# Additional command line options to GNU autoconf configure script
MODULE_CONFIG_OPTS= --enable-maintainer-mode \
 --with-gnome --enable-gtk-doc --with-gtk2=/usr --with-pixbuf=/usr \
 --enable-locking --with-pam=/usr --without-shadow --without-kerberos \
 --with-dpms --enable-dpms --disable-screengrab --enable-root-passwd \
 --with-gl=$(OPENGL_DIR)  --without-motif --with-jpeg=/usr \
 --datadir='$${prefix}/share' \
 --libexecdir='$${prefix}/lib/xscreensaver/bin' \
 --with-hackdir='$${prefix}/lib/xscreensaver/hacks' \
 --with-configdir=/usr/X11/lib/xscreensaver/config/control-center-2.0 \
 --with-image-directory=/usr/share/pixmaps/backgrounds \
 --with-text-file=/etc/motd \
 --with-x-app-defaults=/usr/X11/lib/X11/app-defaults

MODULE_LDFLAGS="-R/usr/X11/lib"

# Messages for translation
XS_POT=$(SOURCE_DIR)/po/xscreensaver.pot
POT_DEST=$(PROTODIR)/usr/X11/share/locale/C/LC_MESSAGES

# Logo images
LOGO_DEST=$(PROTODIR)/usr/X11/lib/xscreensaver/config
LOGO_FILES=$(SOURCE_DIR)/utils/images/logo-180.gif \
	$(SOURCE_DIR)/driver/*logo.png

# Man pages
XS_MAN=$(SOURCE_DIR)/driver/xscreensaver.man.orig

# Additional optimization flags, to make the hacks show off the hardware
# better and we can get away with optimizations not allowed in the core X code
XS_ARCH_FLAGS_sparc=-xtarget=ultra2
XS_ARCH_FLAGS_i386=
XS_ARCH_FLAGS=$(XS_ARCH_FLAGS_$(MACH))
MODULE_CFLAGS=-fsimple=2 -nofstore $(XS_ARCH_FLAGS)

# Configure/Make variables to override
# Set PERL to /usr/perl5/bin so it uses that path to perl for scripts
# as required by Sun rules for using the bundled version of perl
# XGETTEXT must be set to make sure we get GNU xgettext
# since intltool-update doesn't work with Solaris xgettext (see bugs
# 4812320 & 4826523)
MODULE_CONFIG_ENV= \
	PATH=/usr/perl5/bin:$(PATH) \
	XGETTEXT=/usr/gnu/bin/xgettext \
	GNOME_DATADIR='$${prefix}/lib/xscreensaver/config' \
	GLADE_DATADIR='$${prefix}/lib/xscreensaver/config' \
	PERL=/usr/perl5/bin/perl

MODULE_BUILD_ENV= \
	XGETTEXT=/usr/gnu/bin/xgettext

MODULE_MAKEFLAGS=-e install_prefix=$(PROTODIR) 

MODULE_ADD_BUILD_TARGETS=$(XS_MAN) $(XS_POT) set-install-perms

INSTALL_TARGETS=set-install-perms default_install install_xs
INSTALL_TARGETS_SET=yes

include ../Makefile.inc

# Merge in additional sources from sun-src directory
source_gen:: $(LNDIR)
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; set -x ; \
	cd $(SOURCE_DIR) ; \
	$(LNDIR) ../../sun-src ; \
	if [ -d ../../closed-src ] ; then \
	    $(LNDIR) ../../closed-src ; \
	fi ; \
	chmod a+x install-sh ; \
	cd driver ; \
	for i in unlock-logo.png trusted-logo.png ; do \
	    if [ ! -f $$i ] ; then \
	        ln -s opensolaris-logo.png $$i ; \
	    fi ; \
	done

$(XS_POT): $(CONFIGURE_TARGET)
	(cd $(SOURCE_DIR)/po ; \
	 $(BUILD_ENV) $(MAKE) $(MAKEFLAGS) $(MODULE_MAKEFLAGS) \
		generate_potfiles_in POTFILES xscreensaver.pot )

# Add Sun attributes section to man pages
$(XS_MAN):
	/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
	    -a '{Availability, SUNWxscreensaver} {Interface Stability, Volatile}' \
		-p /usr/X11/bin \
		$(SOURCE_DIR)/driver/xscreensaver.man \
		$(SOURCE_DIR)/driver/xscreensaver-command.man \
		$(SOURCE_DIR)/driver/xscreensaver-demo.man
	/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
	    -a '{Availability, SUNWxscreensaver} {Interface Stability, Private}' \
		-p /usr/X11/lib/xscreensaver/bin \
		$(SOURCE_DIR)/driver/xscreensaver-get*.man \
		$(SOURCE_DIR)/driver/xscreensaver-text.man
	/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
	    -a '{Availability, SUNWxscreensaver-hacks} {Interface Stability, Private}' \
		-p /usr/X11/lib/xscreensaver/hacks/ $(SOURCE_DIR)/hacks/*.man
	/usr/perl5/bin/perl $(TOP)/common/suntouch-manpages.pl \
	    -a '{Availability, SUNWxscreensaver-hacks-gl} {Interface Stability, Private}' \
		-p /usr/X11/lib/xscreensaver/hacks/ $(SOURCE_DIR)/hacks/glx/*.man

set-install-perms:
	(cd $(SOURCE_DIR) ; \
	 chmod a+x install-sh intltool-*; \
	 chmod +w hacks/vidwhacker hacks/webcollage \
		driver/xscreensaver-getimage-file \
		driver/xscreensaver-getimage-video )

install_xs:: $(BUILD_TARGETS)
	mkdir -p $(POT_DEST)
	cp -pf $(XS_POT) $(POT_DEST)/xscreensaver.pot
	mkdir -p $(LOGO_DEST)
	@case '$(MAKEFLAGS)' in *[ik]*) set +e;; esac; set -x ; \
	for i in $(LOGO_FILES) ; do \
	    case $$i in \
		*\**) ;; \
		*) cp -pf $$i $(LOGO_DEST) ;; \
	    esac ; \
	done


