#! /usr/perl5/bin/perl
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
#

# Merge fonts.scale files

use strict;
use warnings;
use integer;

use Getopt::Long;

my @infiles;
my $outfile;

my $options_ok = GetOptions(
	   "in=s" => \@infiles, 	# --in=<filename>, may be repeated
	   "out=s" => \$outfile);	# --out=<filename>, only once	

die "Invalid options" if !$options_ok;

die "No input specified" if (scalar(@infiles) < 1);

die "No output specified" if !defined($outfile);

my %fonts = ();

for my $f (@infiles) {
  open my $IN, '<', $f or die "Couldn't open $f for reading: $!\n";

  my $firstline = <$IN>;  # count of lines, ignore

  while (my $l = <$IN>) {
    chomp $l;
    $l =~ m/(\S*)\s+(.*)/;
    $fonts{$2} = $1;
  }
  close $IN;
}

open my $OUT, '>', $outfile or die "Couldn't open $outfile for writing: $!\n";

print $OUT scalar(keys %fonts), "\n";

for my $ft (sort {$fonts{$a} cmp $fonts{$b}} keys %fonts) {
  print $OUT $fonts{$ft}, ' ', $ft, "\n";
}

close $OUT or die "Couldn't finish writing $outfile: $!\n";
