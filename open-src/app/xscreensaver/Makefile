###############################################################################
#
# Xscreensaver Makefile
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# @(#)Makefile	1.83	08/02/14
#

PWD:sh=pwd
TOP=$(PWD)/../..

# Version number (used in path names)
XSCREENSAVER_VERS=5.01

# Source tarball
SOURCE_TARBALL_NAME=xscreensaver-$(XSCREENSAVER_VERS).tar.gz

# Download site for source - master site no longer carries 5.01 so we use
# a mirror site until we upgrade to a newer version
#SOURCE_URL=http://www.jwz.org/xscreensaver/$(SOURCE_TARBALL_NAME)
SOURCE_URL=http://ftp.osuosl.org/pub/FreeBSD/distfiles/$(SOURCE_TARBALL_NAME)
SOURCE_URL_SET=yes
SOURCE_UNCOMPRESS=gzcat

# Patches to apply to source after unpacking, in order
SOURCE_PATCHES = \
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
        XScr.ad.lockTimeout.patch

# Directory created by unpacking source
SOURCE_DIR=$(BUILD_DIR)/xscreensaver-$(XSCREENSAVER_VERS)

include $(TOP)/common/Makefile.inc

# Merge in additional sources from sun-src directory
source_gen:: $(LNDIR)
	mkdir -p $(SOURCE_DIR)
	(cd $(SOURCE_DIR) && $(LNDIR) ../../sun-src)
	if [ -d closed-src ] ; then \
	    (cd $(SOURCE_DIR) && $(LNDIR) ../../closed-src) \
	fi

# Where to find OpenGL headers/libraries
OPENGL_DIR_sparc=/usr/openwin
OPENGL_DIR_i386=/usr/X11
OPENGL_DIR=$(OPENGL_DIR_$(MACH))

# Command line options to GNU autoconf configure script
XS_CFG= --prefix=/usr/X11 --enable-maintainer-mode \
 --with-gnome --enable-gtk-doc --with-gtk2=/usr --with-pixbuf=/usr \
 --enable-locking --with-pam=/usr --without-shadow --without-kerberos \
 --with-dpms --enable-dpms --disable-screengrab --enable-root-passwd \
 --with-gl=$(OPENGL_DIR)  --without-motif --with-jpeg=/usr \
 --mandir='$${prefix}/share/man' --datadir='$${prefix}/share' \
 --libexecdir='$${prefix}/lib/xscreensaver/bin' \
 --with-hackdir='$${prefix}/lib/xscreensaver/hacks' \
 --with-configdir=/usr/X11/lib/xscreensaver/config/control-center-2.0 \
 --with-image-directory=/usr/share/pixmaps/backgrounds \
 --with-text-file=/etc/motd \
 --with-x-app-defaults=/usr/X11/lib/X11/app-defaults

# BINARY built in tree
XS_BIN=$(SOURCE_DIR)/xscreensaver

# Messages for translation
XS_POT=$(SOURCE_DIR)/po/xscreensaver.pot
POT_DEST=$(PROTODIR)/usr/X11/share/locale/C/LC_MESSAGES

# Man pages
XS_MAN=$(SOURCE_DIR)/driver/xscreensaver.man.orig

# Include Paths
INCLUDES=-I/usr/X11/include

# Additional optimization flags, to make the hacks show off the hardware
# better and we can get away with optimizations not allowed in the core X code
XS_CFLAGS=$(CFLAGS) -fsimple=2 -nofstore -xprefetch $(XS_ARCH_FLAGS) -xstrconst
XS_ARCH_FLAGS=$(XS_ARCH_FLAGS_$(MACH))
XS_ARCH_FLAGS_sparc=-xtarget=ultra2
XS_ARCH_FLAGS_i386=

# Configure/Make variables to override
# Set PERL to /usr/perl5/bin so it uses that path to perl for scripts
# as required by Sun rules for using the bundled version of perl
# /opt/sfw/bin must be ahead of /usr/bin to make sure we get GNU xgettext
# since intltool-update doesn't work with Solaris xgettext (see bugs
# 4812320 & 4826523)
XS_CNFG_ENV=CC=$(CC) CFLAGS="$(XS_CFLAGS) $(INCLUDES)" CPPFLAGS="$(INCLUDES)" \
	PATH=/usr/perl5/bin:/usr/sfw/bin:/opt/sfw/bin:$(PATH) \
	GNOME_DATADIR='$${prefix}/lib/xscreensaver/config' \
	GLADE_DATADIR='$${prefix}/lib/xscreensaver/config' \
	LDFLAGS="$(PROG_LDFLAGS) -L/usr/X11/lib -R/usr/X11/lib" \
	PKG_CONFIG_PATH=$(PROTODIR)/usr/lib/$(ARCHLIBSUBDIR)/pkgconfig:$(TOP)/common/pc-files/$(BUILD_DIR) \
	PERL=/usr/perl5/bin/perl
XS_MAKE_ENV=install_prefix=$(PROTODIR) 

build_gen:   $(XS_BIN) $(XS_MAN) $(XS_POT)

# Run configure script
$(SOURCE_DIR)/Makefile: $(UNPACK_TARGET)
	(cd $(SOURCE_DIR) ; \
	 chmod a+x autogen.sh ; \
	 $(XS_CNFG_ENV) ./autogen.sh $(XS_CFG) )

$(XS_BIN): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR) ; chmod a+x install-sh ; \
	 $(MAKE) $(MFLAGS) -e $(XS_MAKE_ENV))

$(XS_POT): $(SOURCE_DIR)/Makefile
	(cd $(SOURCE_DIR)/po ; \
	 PATH=/opt/sfw/bin:$(PATH) $(MAKE) -e $(XS_MAKE_ENV) generate_potfiles_in ; \
	 PATH=/opt/sfw/bin:$(PATH) $(MAKE) -e $(XS_MAKE_ENV) POTFILES ; \
	 PATH=/opt/sfw/bin:$(PATH) $(MAKE) -e $(XS_MAKE_ENV) xscreensaver.pot )

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

install_gen: $(XS_BIN) $(XS_MAN) $(XS_POT)
	chmod +w $(SOURCE_DIR)/hacks/vidwhacker \
		$(SOURCE_DIR)/hacks/webcollage \
		$(SOURCE_DIR)/driver/xscreensaver-getimage-file \
		$(SOURCE_DIR)/driver/xscreensaver-getimage-video
	(cd $(SOURCE_DIR) ; chmod a+x install-sh intltool-*; \
	 $(MAKE) -e $(XS_MAKE_ENV) install )
	mkdir -p $(POT_DEST)
	cp -p $(XS_POT) $(POT_DEST)/xscreensaver.pot

