#!/usr/perl5/bin/perl -w

#
# Copyright (c) 2006, 2010, Oracle and/or its affiliates. All rights reserved.
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

# Updates manual pages to include standard Sun man page sections
#
# Arguments:
#	-a '{attribute, value}, ...' - add entries to Attributes section table
#	-o '{attribute, value}, ...' - override previous entries in
#					Attributes section table
#	-l libname		     - add library line to synopsis
#	-p path			     - add path to command in synopsis
#	-r '{text, replacement}'     - replace text with replacement

use Getopt::Long;
use integer;
use strict;

my @attributes;
my @overrides;
my @replacements;
my $library;
my $synpath;

my $result = GetOptions('a|attribute=s' => \@attributes,
			'o|override=s'  => \@overrides,
			'r|replace=s'	=> \@replacements,
			'l|library=s'	=> \$library,
			'p|path=s'	=> \$synpath);

my $add_attributes = 0;
my $attributes_table;

if (scalar(@attributes) + scalar(@overrides) > 0) {
  $add_attributes = 1;
  $attributes_table = &get_attributes_table(\@attributes, \@overrides);
}

# Reference to generated function to substitute text replacements
my $text_subref;

if (scalar(@replacements) > 0) {
  $text_subref = &get_text_substitutions(\@replacements);
}

my $add_library_to_synopsis = 0;

if (defined($library)) {
  $add_library_to_synopsis = 1;
}

my $add_path_to_synopsis = 0;

if (defined($synpath)) {
  $add_path_to_synopsis = 1;
}

my $filename;

while ($filename = shift) {
  rename($filename, "$filename.orig")
    || die "Cannot rename $filename to $filename.orig: $!";
  open(IN, '<', "$filename.orig")
    || die "Cannot read $filename.orig: $!";
  open(OUT, '>', $filename)
    || die "Cannot write to $filename: $!";

  my $firstline = <IN>;

  if ($add_attributes > 0) {
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
    if ($text_subref) {
      $nextline = &$text_subref($nextline);
    }
    print OUT $nextline;

    if ($nextline =~ m/.SH[\s "]*(SYNOPSIS|SYNTAX)/) {
      if ($add_library_to_synopsis) {
	print OUT ".nf\n",
	  q(\fBcc\fR [ \fIflag\fR\&.\&.\&. ] \fIfile\fR\&.\&.\&. \fB\-l),
	    $library, q(\fR [ \fIlibrary\fR\&.\&.\&. ]), "\n.fi\n";
      }
      elsif ($add_path_to_synopsis) {
	$nextline = <IN>;
	$nextline =~ s/^(\.B[IR]*\s+\"?)/$1$synpath/;
	$nextline =~ s/^(\\fB)/$1$synpath/;
	print OUT $nextline;
      }
    }
  }

  if ($add_attributes) {
    print OUT $attributes_table;
  }

  close(IN);
  close(OUT);
}


sub get_attributes_table {
  my ($attributes_ref, $overrides_ref) = @_;

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

  my $attribute_entries = "";

  my @attribute_pairs = parse_attributes_list($attributes_ref);
  my @overrides_pairs = parse_attributes_list($overrides_ref);

  foreach my $o (@overrides_pairs) {
    my ($oname, $ovalue) = @{$o};
    my $found_match = 0;

    foreach my $a (@attribute_pairs) {
      if ($a->[0] eq $oname) {
	$a->[1] = $ovalue;
	$found_match++;
      }
    }

    if ($found_match == 0) {
      push @attribute_pairs, $o;
    }
  }

  foreach my $a (@attribute_pairs) {
    my ($name, $value) = @{$a};
    $attribute_entries .= $name . "\t" . $value . "\n";
  }

  $attributes_table =~ s/<attributes>\n/$attribute_entries/;

  return $attributes_table;
}

sub parse_attributes_list {
  my ($list_ref) = @_;

  my $list_string = join(" ", @{$list_ref});
  $list_string =~ s/^\s*{//;
  $list_string =~ s/}\s*$//;

  my @attribs = split /}\s*{/, $list_string;
  my @attrib_pairs = ();

  foreach my $a (@attribs) {
    my @pair = split /,\s*/, $a, 2;  # pair = name, value
    push @attrib_pairs, \@pair;
  }
  return @attrib_pairs;
}

sub get_text_substitutions {
  my ($replacements_ref) = @_;

  my @replacement_pairs = parse_attributes_list($replacements_ref);

  my @subst_pattern_list = ();

  foreach my $r (@replacement_pairs) {
    my ($text_in, $text_out) = @{$r};
    push @subst_pattern_list, " s{$text_in}{$text_out}go;";
  }

  my $subst_function = join("\n",
			    'sub {',
			    ' $_ = $_[0]; ',
			    @subst_pattern_list, 
			    ' return $_;',
			    '}');

  return eval $subst_function;
}
