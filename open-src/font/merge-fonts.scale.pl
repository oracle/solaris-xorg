#! /usr/perl5/bin/perl
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
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
