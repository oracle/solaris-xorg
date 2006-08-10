#!/usr/perl5/bin/perl -w

#
# Copyright 2006 Sun Microsystems, Inc.  All Rights Reserved.
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
# @(#)suntouch-manpages.pl	1.2	06/08/09
#

# Updates manual pages to include standard Sun man page sections
#
# Arguments: 
#	-a '{attribute, value}, ...' - entries for Attributes section table
#	-l libname		     - add library line to synopsis
#	-p path			     - add path to command in synopsis

use Getopt::Std;
use integer;
use strict;

my %opts;
getopts('a:l:p:', \%opts);

my $add_attributes = 0;
my $attributes;

if (exists($opts{"a"})) {
  $add_attributes = 1;
  $attributes = $opts{"a"};
}

my $add_library_to_synopsis = 0;
my $library;

if (exists($opts{"l"})) {
  $add_library_to_synopsis = 1;
  $library = $opts{"l"};
}

my $add_path_to_synopsis = 0;
my $synpath;

if (exists($opts{"p"})) {
  $add_path_to_synopsis = 1;
  $synpath = $opts{"p"};
}

my $filename;

while ($filename = shift) {
  rename($filename, "$filename.orig") 
    || die "Cannot rename $filename to $filename.orig";
  open(IN, "<$filename.orig") 
    || die "Cannot read $filename.orig";
  open(OUT, ">$filename")
    || die "Cannot write to $filename";

  my $firstline = <IN>;

  if ($add_attributes) {
    # Check for man page preprocessor list - if found, make sure t is in it for
    # table processing, if not found, add one;

    if ($firstline =~ m/\'\\\"/) {
      # Found preprocessor list
      if ($firstline =~ m/t/) {
	# Do nothing - tbl preprocessing already selected
      } else {
	chomp($firstline);
	$firstline .= "t\n";
      }
    } else {
      # No preprocessor list found
      print OUT q('\" t), "\n";
    }
  }

  print OUT $firstline;

  my $nextline;
  while ($nextline = <IN>) {
    print OUT $nextline;

    if ($nextline =~ m/.SH[\s "]*SYNOPSIS/) {
      if ($add_library_to_synopsis) {
	print OUT ".nf\n",
	  q(\fBcc\fR [ \fIflag\fR\&.\&.\&. ] \fIfile\fR\&.\&.\&. \fB\-l),
	    $library, q(\fR [ \fIlibrary\fR\&.\&.\&. ]), "\n.fi\n";
      }
      elsif ($add_path_to_synopsis) {
	$nextline = <IN>;
	$nextline =~ s/^(\.B[IR]*\s+\"?)/$1$synpath/;
	print OUT $nextline;
      }
    }
  }

  if ($add_attributes) {
    print OUT &get_attributes_table($attributes);
  }

  close(IN);
  close(OUT);
}


sub get_attributes_table {
  my $attribute_list = $_[0];

  my $attributes_table = q{
.\\" Begin Sun update
.SH "ATTRIBUTES"
See \fBattributes\fR(5) for descriptions of the following attributes:
.sp
.TS
allbox;
cw(2.750000i)| cw(2.750000i)
lw(2.750000i)| lw(2.750000i).
ATTRIBUTE TYPE	ATTRIBUTE VALUE
<attributes>
.TE 
.sp
.\\" End Sun update
};

  # Parse input list of attributes
  $attribute_list =~ s/^\s*{//;
  $attribute_list =~ s/}\s*$//;
  my @attribs = split /}\s*{/, $attribute_list;

  my $a;
  my $attribute_entries = "";

  foreach $a (@attribs) {
    my ($name, $value) = split /,\s*/, $a, 2;
    
    $attribute_entries .= $name . "\t" . $value . "\n";
  }
  
  $attributes_table =~ s/<attributes>\n/$attribute_entries/;

  return $attributes_table;
}
