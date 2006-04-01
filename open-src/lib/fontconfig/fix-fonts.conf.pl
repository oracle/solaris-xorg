#!/usr/perl5/bin/perl -w
#
# ident	"@(#)fix-fonts.conf.pl	1.5	04/11/16 SMI"
#
# Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# This script performs a number of customizations to the fonts.conf shipped
# with Solaris, including:
#   - adding all the locale specific dirs listed in fontdirs file to the font path
#   - append additional settings from fonts.conf.append file just before closing </fontdir>
#   - Japanese customization requested in bug 5028919 - replace Kochi fonts with Sun fonts


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
      $newline = $line;
      chomp($fontdir);
      # skip if line is blank or commented out
      next if ($fontdir =~ /^\s*$/);
      next if ($fontdir =~ /^\s*\#/);
      $newline =~ s|--font-dirs-go-here--|$fontdir|;
      print $newline;
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
    $newline = $line;
    $newline =~ s|Kochi Mincho|HG-PMinchoL-Sun|;
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Mincho|HG-MinchoL-Sun|;
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Mincho|FZSongTi|;
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Mincho|FZMingTi|;
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Mincho|KacstQurn|;
    print $newline;
    $line =~ s|Kochi Mincho|SunDotum|;
    $kochifontsreplaced++;
    # Fall through to print $line at end
  }
  # Replace Kochi Gothic fonts with Sun Gothic fonts
  # Add additional Sun CJK fonts
  if ($line =~ m|<family>Kochi Gothic</family>|) {
    $newline = $line;
    if ($aliasfamily ne "monospace") {
        $newline =~ s|Kochi Gothic|HG-PGothicB-Sun|;
    } else {
        $newline =~ s|Kochi Gothic|HG-GothicB-Sun|;
    }
    print $newline;
    $newline = $line;
    if ($aliasfamily ne "monospace") {
        $newline =~ s|Kochi Gothic|HG-GothicB-Sun|;
    } else {
        $newline =~ s|Kochi Gothic|HG-MinchoL-Sun|;
    }
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Gothic|FZSongTi|;
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Gothic|FZMingTi|;
    print $newline;
    $newline = $line;
    $newline =~ s|Kochi Gothic|KacstQurn|;
    print $newline;
    if ($aliasfamily ne "monospace") {
      $line =~ s|Kochi Gothic|SunDotum|;
    } else {
      $line =~ s|Kochi Gothic|SunDotumChe|;
    }
    $kochifontsreplaced++;
    # Fall through to print $line at end
  }
  # Add additional entries to monospace faces list
  if ($line =~ m|<default><family>monospace</family></default>| ) {
    $newline = $line;
    $newline =~ s|<default><family>monospace</family></default>|<family>KacstQurn</family>|;
    print $newline;
    $newline = $line;
    $newline =~ s|<default><family>monospace</family></default>|<family>SunDotumChe</family>|;
    print $newline;
  }
  # Add Arial before Bitstream Vera Sans
  if ($line =~ m|<family>Bitstream Vera Sans</family>| ) {
    print "		<family>Arial</family>\n";
  }
  # Add Lucida Bright before Bitstream Vera Serif
  if ($line =~ m|<family>Bitstream Vera Serif</family>| ) {
    print "		<family>Lucida Bright</family>\n";
  }
  # Add Lucida Sans Typewriter before Bitstream Vera Serif
  if ($line =~ m|<family>Bitstream Vera Sans Mono</family>| ) {
    print "		<family>Lucida Sans Typewriter</family>\n";
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
