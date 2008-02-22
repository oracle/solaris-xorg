#!/usr/perl5/bin/perl -w
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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
# ident	"@(#)fix-fonts.conf.pl	1.9	08/02/21 SMI"
#
# This script performs a number of customizations to the fonts.conf shipped
# with Solaris, including:
#   - adding all the locale specific dirs listed in fontdirs file to the font path
#   - append additional settings from fonts.conf.append file just before closing </fontdir>
#   - Japanese customization requested in bugs 5028919 & 6665751


require 5.005;                          # minimal Perl version required
use strict;                             # 
use diagnostics;                        #

my $line;
my $newline;
my $inalias = 0;
my $aliasfamily = "";

my $fontdirsreplaced = 0;
my $kochifontsreplaced = 0;
my $eudcfontsadded = 0;
my $appended = 0;
my $preuserconf = 0;

# Prints a line with text substituted
# Arguments: input line, text to replace, new text
sub print_substitute_line {
  my ($line, $oldtext, $newtext) = @_;
  $line =~ s|$oldtext|$newtext|;
  print $line;
}

while ($line = <>) {
  # keep track of if we're in an <alias>...</alias> block and if so which one
  if ($inalias == 0) {
    if ($line =~ m|<alias>|) {
      $inalias = 1;
    }
  } else {
    if ($line =~ m|</alias>|) {
      $inalias = 0;
      $aliasfamily = "";
    } elsif (($aliasfamily eq "") && ($line =~ m|<family>([^>]*)</family>|)) {
      $aliasfamily = $1;
    }
  }

  # replace --font-dirs-go-here-- with all the extra font dirs from the fontdirs file
  if ($line =~ m|--font-dirs-go-here--|) {
    my $fontdir;

    open(FONTDIRLIST, "<fontdirs") || die "Cannot open file fontdirs";
    while ($fontdir = <FONTDIRLIST>) {
      chomp($fontdir);
      # skip if line is blank or commented out
      next if ($fontdir =~ /^\s*$/);
      next if ($fontdir =~ /^\s*\#/);
      print_substitute_line($line, '--font-dirs-go-here--', $fontdir);
    }
    close(FONTDIRLIST);
    $fontdirsreplaced++;
    next;
  }
  # Append contents of fonts.conf.preuser just before loading user configuration
  if ($line =~ m|Load per-user customization file|) {
    print " Additional Solaris-specific configuration settings\n -->\n\n";
    open(INSERT, "<fonts.conf.preuser") || die "Cannot open file fonts.conf.preuser";
    while ($newline = <INSERT>) {
      print $newline;
    }
    close(INSERT);
    $preuserconf++;
    print "\n<!-- \n";
    # Fall through to print $line at end
  }
  # Append contents of fonts.conf.append just before closing </fontconfig>
  if ($line =~ m|</fontconfig>|) {
    open(APPEND, "<fonts.conf.append") || die "Cannot open file fonts.conf.append";
    while ($newline = <APPEND>) {
      print $newline;
    }
    close(APPEND);
    $appended++;
    # Fall through to print $line at end
  }
  # Replace Kochi Mincho fonts with Sun Mincho fonts
  # Add additional Sun CJK fonts
  if ($line =~ m|<family>Kochi Mincho</family>|) {
    print_substitute_line($line, 'Kochi Mincho', 'HG-PMinchoL-Sun');
    print_substitute_line($line, 'Kochi Mincho', 'HG-MinchoL-Sun');
    print_substitute_line($line, 'Kochi Mincho', 'IPAPMincho');
    print_substitute_line($line, 'Kochi Mincho', 'IPAMincho');
    print_substitute_line($line, 'Kochi Mincho', 'IPAGothic');
    print_substitute_line($line, 'Kochi Mincho', 'FZSongTi');
    print_substitute_line($line, 'Kochi Mincho', 'FZMingTi');
    print_substitute_line($line, 'Kochi Mincho', 'KacstQurn');
    print_substitute_line($line, 'Kochi Mincho', 'SunDotum');
    $kochifontsreplaced++;
    next;
  }
  # Replace Kochi Gothic fonts with Sun Gothic fonts
  # Add additional Sun CJK fonts
  if ($line =~ m|<family>Kochi Gothic</family>|) {
    if ($aliasfamily ne "monospace") {
      print_substitute_line($line, 'Kochi Gothic', 'HG-PGothicB-Sun');
      print_substitute_line($line, 'Kochi Gothic', 'HG-GothicB-Sun');
      print_substitute_line($line, 'Kochi Gothic', 'IPAPGothic');
      print_substitute_line($line, 'Kochi Gothic', 'IPAGothic');
    } else {
      print_substitute_line($line, 'Kochi Gothic', 'HG-GothicB-Sun');
      print_substitute_line($line, 'Kochi Gothic', 'HG-MinchoL-Sun');
      print_substitute_line($line, 'Kochi Gothic', 'IPAGothic');
      print_substitute_line($line, 'Kochi Gothic', 'IPAMincho');
    }
    print_substitute_line($line, 'Kochi Gothic', 'FZSongTi');
    print_substitute_line($line, 'Kochi Gothic', 'FZMingTi');
    print_substitute_line($line, 'Kochi Gothic', 'KacstQurn');
    if ($aliasfamily ne "monospace") {
      print_substitute_line($line, 'Kochi Gothic', 'SunDotum');
    } else {
      print_substitute_line($line, 'Kochi Gothic', 'SunDotumChe');
    }
    $kochifontsreplaced++;
    next;
  }
  # Add additional entries to monospace faces list
  if ($line =~ m|<default><family>monospace</family></default>| ) {
    print_substitute_line($line, 
			  '<default><family>monospace</family></default>',
			  '<family>KacstQurn</family>');
    print_substitute_line($line, 
			  '<default><family>monospace</family></default>',
			  '<family>SunDotumChe</family>');
  }
  # Add Arial before Bitstream Vera Sans
  if ($line =~ m|<family>Bitstream Vera Sans</family>| ) {
    print_substitute_line($line, 'Bitstream Vera Sans', 'Arial');
  }
  # Add Lucida Bright before Bitstream Vera Serif
  if ($line =~ m|<family>Bitstream Vera Serif</family>| ) {
    print_substitute_line($line, 'Bitstream Vera Serif', 'Lucida Bright');
  }
  # Add Lucida Sans Typewriter before Bitstream Vera Sans Mono
  if ($line =~ m|<family>Bitstream Vera Sans Mono</family>| ) {
    print_substitute_line($line, 'Bitstream Vera Sans Mono', 
			  'Lucida Sans Typewriter');
  }
  # Add DejaVu entries before Bitstream Vera
  if ($line =~ m|<family>Bitstream Vera.*</family>|) {
    print_substitute_line($line, 'Bitstream Vera', 'DejaVu');
  }
  # Add EUDC fonts to preferred fonts lists (bug 6195182)
  if (($line =~ m|<family>Bitstream Vera|) && 
      (($aliasfamily eq "serif") || ($aliasfamily eq "sans-serif") ||
       ($aliasfamily eq "monospace"))) {
    print $line;
    $line =~ s|<family>[^>]+</family>|<family>EUDC</family>|;
    $eudcfontsadded++;
    # Fall through to print $line at end
  }
  print $line;
}


# Check to make sure we did everything
if ($fontdirsreplaced == 0) {
  die "Did not find --font-dirs-go-here-- tag to replace with list of font dirs!";
}

if ($appended == 0) {
  die "Did not find </fontconfig> tag to add fonts.conf.append contents!";
} elsif ($appended > 1) {
  die "Found too many </fontconfig> tags!";
}

if ($preuserconf == 0) {
  die "Did not find per-user customization comment tag to add fonts.conf.preuse contents!";
} elsif ($appended > 1) {
  die "Found too many per-user customization comment tags!";
}

if ($kochifontsreplaced != 5) {
  die "Did not find expected number of Kochi font entries to edit! (Found $kochifontsreplaced instead of 5)";
}

if ($eudcfontsadded != 3) {
  die "Did not find expected number of preferred font alias entries to insert EUDC into! (Found $eudcfontsadded instead of 3)";
}
