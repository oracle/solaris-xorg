#! /usr/perl5/bin/perl
#
# Copyright (c) 2008, 2010, Oracle and/or its affiliates. All rights reserved.
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

#
# Set version information in ELF files to give hints to help in troubleshooting
#
# Usage: set-elf-comments.pl -B <pkgversion> -M "module version" <path>
#  <pkgversion> may either be a path to a file containing BUILD="<version>"
#   or the string "hg id" to get the version from the hg id output.
#
# If the XBUILD_HG_ID environment variable is set, it is used for the hg id
# instead of forking a hg id process for every component in a full tree build
#

use strict;
use warnings;
use Getopt::Std;
use POSIX qw(strftime);
use File::Find;

my %opts;
getopts('B:M:', \%opts);

my $module_version_info = '@(#)';
if (exists($opts{'M'})) {
  $module_version_info .= $opts{'M'};
} else {
  die qq(Must specify -M "module version");
}

my $build_info = strftime("%e %b %Y", localtime);

if (exists($opts{'B'})) {
  my $build_version_file = $opts{'B'};

  if ($build_version_file eq 'hg id') {
    my $hg_id = 'revision unavailable';
    if (exists $ENV{'XBUILD_HG_ID'}) {
      $hg_id = $ENV{'XBUILD_HG_ID'};
    } else {
      open my $VERS, '-|', $build_version_file
	or die "Can't run $build_version_file: $!\n";

      while ($_ = <$VERS>) {
	chomp($_);
	if ($_ =~ m/\S+/) {
	  my ($rev, $tag) = split(' ', $_, 2);
	  if ($tag eq 'tip') {
	    $hg_id = $rev;
	  } else {
	    $hg_id = $_;
	  }
	}
      }
      close $VERS;
    }
    $build_info = "hg: $hg_id - $build_info";
  } else {
    open my $VERS, '<', $build_version_file
      or die "Can't open $build_version_file for reading: $!\n";

    while ($_ = <$VERS>) {
      if ($_ =~ m/^BUILD="(.*)"/) {
	my $v = $1 / 100.0;
	if ($v >= 1.0) {
	  $build_info = "build $v - " . $build_info;
	}
      }
    }
    close $VERS;
  }
}

$module_version_info .= " ($build_info)";
$module_version_info =~ s/\s+$//ms;

sub scan_file {
  # skip sources & intermediate build files that we don't ship
  return if $_ =~ m/\.[acho]$/ims;

  # skip the *.uc files used by xf86-video-rendition for firmware
  return if $_ =~ m/\.uc$/ms;

  # If the file is not a symlink, is a regular file, and is at least 256 bytes
  if ((! -l $_) && (-f _) && (-s _ > 256)) {
    open my $IN, '<', $_ 
      or die "Can't open $_ for reading: $!\n";

    my $magic_number;
    sysread($IN, $magic_number, 4)
      or die "Can't read from $_: $!\n";

    close $IN;

    if ($magic_number eq "\177ELF") {
      my @cmd = ('/usr/bin/mcs', '-a', $module_version_info, '-c', $_);

      print join(' ', @cmd), "\n";
      system(@cmd) == 0
	or die "*** mcs $File::Find::name failed: $?\n";
    }
  }
}


find(\&scan_file, @ARGV);
