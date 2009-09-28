Xdm Default Configuration
-------------------------

The /usr/lib/X11/xdm directory contains a collection of scripts run by 
the default configuration of xdm, and a collection of sample configuration
files.

All of these files will be overwritten by upgrades and should not be
edited in place in /usr/lib/X11/xdm.

The xdm-config file (/etc/X11/xdm/xdm-config unless overridden by the -config
command line option to xdm) specifies the path in which to find the other
configuration files and scripts.

To change a setting in the configuration files (xdm-config, Xaccess, 
Xresources, Xservers), edit the copies in /etc/X11/xdm or create a new
directory for your configuration and copy the appropriate files there.

To change the actions performed by one of the scripts, copy it to your
configuration directory, edit the copy, and then edit the path to it in
the xdm-config file for your configuration.

For more information about the contents of these files, see the xdm(1) 
manual page.


###############################################################################
#
# Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
###############################################################################
