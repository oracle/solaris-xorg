#! /usr/perl5/5.10.0/bin/perl
#
# Copyright (c) 2008, 2011, Oracle and/or its affiliates. All rights reserved.
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
# Usage: set-elf-comments.pl [-b] [-B <pkgversion>] -M "module version" \
#	 [-X <exclude regexp>] <path>
#
#  <pkgversion> may either be a path to a file containing BUILD="<version>"
#   or the string "hg id" to get the version from the hg id output.
#
# If the XBUILD_HG_ID environment variable is set, it is used for the hg id
# instead of forking a hg id process for every component in a full tree build
#
# If -X is specified, it gives a regular expresion for filenames to skip
# for this module.

require 5.10.0;		# needed for Time::Hires::stat

use strict;
use warnings;
use Getopt::Std;
use POSIX qw(strftime);
use File::Find;
use Time::HiRes qw();

$Getopt::Std::STANDARD_HELP_VERSION = 1;

my %opts;
getopts('B:M:X:b', \%opts);

my $module_version_info = '@(#)';
if (exists($opts{'M'})) {
  $module_version_info .= $opts{'M'};
} else {
  die qq(Must specify -M "module version");
}

my $exclude_regexp;
if (exists($opts{'X'})) {
    $exclude_regexp = $opts{'X'};
}

if (exists($opts{'b'})) {
    # Add build info, including date & anything specified by -B
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
}

$module_version_info =~ s/\s+$//ms;

my %elf_files;

sub scan_file {
  # skip sources & intermediate build files that we don't ship
  return if $_ =~ m/\.[acho]$/ims;

  # skip files matching specified regexp
  if (defined $exclude_regexp) {
      if ($_ =~ m{$exclude_regexp}so) {
	  print "Excluding $_\n";
	  return;
      }
  }

  # If the file is not a symlink, is a regular file, and is at least 256 bytes
  if ((! -l $_) && (-f _) && (-s _ > 256)) {
    open my $IN, '<', $_
      or die "Can't open $_ for reading: $!\n";

    my $magic_number;
    sysread($IN, $magic_number, 4)
      or die "Can't read from $_: $!\n";

    close $IN;

    if ($magic_number eq "\177ELF") {
      my @sb = Time::HiRes::stat($_);
      $elf_files{$File::Find::name} = $sb[9]; # mtime
    }
  }
}

find(\&scan_file, @ARGV);

if (scalar(keys %elf_files) > 0) {
    my @filelist = sort { $elf_files{$a} <=> $elf_files{$b} } keys %elf_files;

    my @cmd = ('/usr/bin/mcs', '-a', $module_version_info, '-c', @filelist);

    print join(' ', map { if ($_ =~ /\s+/) { qq("$_") } else { $_ } } @cmd),
	       "\n";
    system(@cmd) == 0
	or die "*** mcs failed: $?\n";
}
