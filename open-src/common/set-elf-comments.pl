#! /usr/perl5/bin/perl
#
# Copyright 2010 Sun Microsystems, Inc.  All rights reserved.
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
# ident	"@(#)set-elf-comments.pl	1.1	08/11/26 SMI"
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
    if (exists $ENV{'XBUILD_HG_ID'}) {
      $build_info = $ENV{'XBUILD_HG_ID'};
    } else {
      open my $VERS, '-|', $build_version_file
	or die "Can't run $build_version_file: $!\n";

      while ($_ = <$VERS>) {
	chomp($_);
	if ($_ =~ m/\S+/) {
	  $build_info = "hg: $_ - " . $build_info;
	}
      }
      close $VERS;
    }
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
