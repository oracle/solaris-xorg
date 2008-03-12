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
# ident	"@(#)fix-fonts.conf.pl	1.11	08/03/11 SMI"
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
my $appended = 0;

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
