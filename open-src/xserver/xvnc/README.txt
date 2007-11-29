###############################################################################
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
# Excerpts from Technical Product Training and Sustaining TOI docs for 
#  future reference

Architecture and Implementation

   The open source RealVNC release is built using the Xorg server sources
   to provide the X server portion of the sources for Xvnc.   This provides
   an Xvnc that includes the same features as Sun's Xorg, including Sun
   enhancements like Trusted Extensions support & Xserver DTrace probes.

   Since the original RealVNC code from realvnc.com is designed to use an
   older X server release, we have based our code on the Xorg port done by
   the Fedora Linux project.

Source code access

   Our code:
	In X gates: open-src/xserver/xvnc

   Upstream:
	Original: http://www.realvnc.com/products/free/4.1/download.html
	Fedora patches: http://cvs.fedoraproject.org/viewcvs/rpms/vnc/

   1. Please provide a brief description of the feature and how it is used.

      VNC provides a remote desktop session viewing protocol (RFB protocol). 
      RFB clients, better known as VNC viewers, are available for most 
      platforms, in both open source and commercial flavors.   The Vino 
      project in JDS (LSARC 2006/371) already delivers in Solaris a VNC server 
      that can capture the display from a running session on a normal X server.

      This project delivers Xvnc, an X server that displays to a Remote Frame
      Buffer (RFB) protocol client over the network, without requiring an 
      existing X server session displayed on local video hardware.   It also 
      delivers the RealVNC vncviewer to connect to remote VNC servers, and 
      several associated programs for managing these.

   2. Does this feature EOL any other feature or does it offer an alternative
      to another existing feature?

      The vncviewer provided by this project replaces the vncviewer script
      delivered by the Vino project (LSARC 2007/391) in several Nevada builds,
      but which was not yet included in any Solaris or SXDE release.

   3. Please list any dependencies on other features.

      None.

   4. Is the feature on or off by default? What considerations need to be 
      taken before turning on/off?

      The Xvnc server is off by default.   Considerations before enabling are
      documented in the man page and the ARC case security questionairre.

   5. Is this a platform specific feature and if so which platform?
      Please provide any differences in behaviors or functionality of the 
      feature if it is supported on the x86 platform.

      No.

   6. Please list the minimum system requirements.

      System running Solaris 10U5 or Nevada.

   7. List the things (GUI, commands, packages, binaries, configuration files,
      etc.) that are introduced, removed, or have substantially changed by
      this feature or project.

      New package SUNWxvnc:
	  /usr/bin/vncpasswd
	  /usr/bin/vncserver
	  /usr/X11/bin/Xvnc	
      
      New package SUNWvncviewer:
	  /usr/bin/vncviewer

   8. What major data structures have changed or have been added?

       N/A

   9. Please list any header file changes.

       N/A

  10. Are there known issues or bugs with this feature? If so, please provide
      bug Ids and any known workarounds.

      See bugster for known bugs.

  11. How would one recognize if the new feature is working or not working?
      (Note: The engineer will provide as much information as possible at 
       this time.  For the latest information, please refer to the appropriate
       System Administration document.)

      Can a VNC client (such as vncviewer) connect to a VNC server (such as
      Xvnc) and see the desktop session in it?

  12. Are there any diagnostic tools for this feature and what are they?

      Xvnc contains the same Xserver DTrace probes as the other Solaris
      X servers.    No other diagnostic tools are known at this time.

  13. Are there any unique conditions or constraints to Licensing, 
      Localization, and Internationalization?

      None known.

  14. Please provide a pointer to the following:

          o ARC case:	        PSARC 2007/545 & LSARC 2007/625
          o Project Plan:	N/A
          o Documentation Plan:	Man pages from open source release
          o Project Web Site:	http://www.realvnc.com/

  15.  Please provide the Bugster Product/Category/Subcategory.

	      solaris/xserver/vnc

  16.  If available, please provide a pointer to the cookbook procedures 
       for installation and configuration.

	   See test case instructions in bugster bug id 6572087.




