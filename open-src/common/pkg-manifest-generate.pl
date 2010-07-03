#! /usr/perl5/bin/perl
#
# Copyright (c) 2010, Oracle and/or its affiliates. All rights reserved.
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
# Script to generate a package manifest for use with pkg(5) from a
# temporary proto area
#
# Usage: pkg-manifest-generate.pl [<attribute>=<value>...]
#
#  If <manifest input> exists, it is merged into the output.
#  If <manifest input> does not exist, <license file> is included in the output
#  The result is written to <manifest output>, or stdout if not specified

use strict;
use warnings;

my %options;

foreach my $arg (@ARGV) {
  my ($attr, $val) = split /=/, $arg, 2;

  push @{$options{$attr}}, $val;
}

sub required_option_list {
  my ($opt, $message) = @_;

  if (!exists $options{$opt}) {
    die ("Must specify $opt $message\n");
  }
  return $options{$opt};
}

sub required_option {
  my ($opt, $message) = @_;

  my $opt_list_ref = required_option_list(@_);

  if (scalar(@{$opt_list_ref}) != 1) {
    die ("Must specify only one value for $opt $message\n");
  }
  return $opt_list_ref->[0];
}

my %actions_seen = ();
my @manifest_header;

# Check if there is an existing manifest to merge with
if (exists $options{'input_manifest'}) {
  foreach my $mf (@{$options{'input_manifest'}}) {
    next if (! -f $mf);
    open my $INPUT_MF, '<', $mf or die "Cannot open input_manifest $mf: $!\n";
    my $action = "";
    while (my $im = <$INPUT_MF>) {
      chomp($im);
      if ($im =~ m{^(.*)\\$}) {  # Line continues
	$action .= $1 . " ";
      } else {
	$action .= $im;
	push @manifest_header, $action;

	$action =~ s{\s+}{ }g;
	if ($action =~ m{ path=(\S+)}) {
	  $actions_seen{$1} = $action;
	} else {
	  $actions_seen{$action} = $action;
	}

	$action = "";
      }
    }
    close $INPUT_MF;
  }
}

# Generate a manifest header if not merging into an existing file
if (!@manifest_header) {

  my $manifest_license_listref = required_option_list
    ('manifest_license', 'when not merging with existing manifest.');

  foreach my $lf (@{$options{'manifest_license'}}) {
    open my $LICENSE, '<', $lf or die "Cannot open manifest_license $lf: $!\n";
    while (my $ll = <$LICENSE>) {
      chomp($ll);
      if ($ll !~ m{^\#}) {
	$ll = '# ' . $ll;
      }
      push @manifest_header, $ll;
    }
    close $LICENSE;
  }

  my $pkg_name = required_option ('pkg_name',
				  'when not merging with existing manifest.');

  push @manifest_header, join('', 'set name=pkg.fmri value=pkg:/',
			      $pkg_name, '@$(PKGVERS)');

  push @manifest_header, 'set name=pkg.description ' .
    'value="XXX: Please provide a descriptive paragraph for the package."';

  my $pkg_summary = '';

SDIR:  foreach my $sdir (@{$options{'source_dir'}}) {
    foreach my $bdir (glob("build-*/$sdir")) {
      # First try looking in a README file for a short summary
      my $rf = join('/', $bdir, 'README');
      if (open my $README, '<', $rf) {
	while (my $readme = <$README>) {
	  chomp($readme);
	  last if $readme =~ m{^[\-=\s]*$};
	  $readme =~ s{[\t\n]+}{ }g;
	  if ($pkg_summary =~ m{\S$}) {
	    $pkg_summary .= ' ';
	  }
	  $pkg_summary .= $readme;
	}
	close $README;
	last SDIR if $pkg_summary ne '';
      }

      # Then try looking in man pages
      my @manpage = glob("$bdir/man/*.man");
      push @manpage, glob("$bdir/*.man");

      foreach my $manpage (@manpage) {
	if ($manpage && (-f $manpage)) {
	  my $desc;

	  open(my $MANPAGE, '<', $manpage) or warn "Cannot read $manpage\n";

	  while (my $l = <$MANPAGE>) {
	    if ($l =~ /^.SH NAME/) {
	      $desc = <$MANPAGE>;
	      last;
	    }
	  }
	  close($MANPAGE);

	  chomp($desc);

	  # Remove backslashes, such as \- instead of -
	  $desc =~ s/\\//g;

	  if ($sdir =~ /^xf86-(input|video)-(.*)-.*$/) {
	    $desc =~ s/driver$/driver for the Xorg X server/;
	  }

	  if ($desc !~ m{^\s*$}) {
	    $pkg_summary = $desc;
	    last SDIR;
	  }
	}
      }
    }
  }

  if ($pkg_summary eq '') {
    $pkg_summary =
      'XXX: Please provide a short name for the package';
  } else {
    $pkg_summary =~ s{^\s+}{};
    $pkg_summary =~ s{\s+$}{};
  }

  push @manifest_header, join('', 'set name=pkg.summary ',
			      'value="', $pkg_summary, '"');

  if (exists $options{'classification'}) {
    my $pkg_class = $options{'classification'};

    push @manifest_header, join('', 'set name=info.classification ',
				'value="org.opensolaris.category.2008:',
				$pkg_class, '"');
  }
}

## Contents

my @manifest_contents;

my $subdir64 = required_option('subdir_64',
   'with the name used for the subdirectory containing 64-bit objects');

my $proto_area_ref = required_option_list('proto_area',
					  'for files to include in manifest');

foreach my $proto_area (@{$proto_area_ref}) {
  my $pkgsend_cmd = join(' ', 'pkgsend', 'generate', $proto_area);
  open my $PKGSEND, '-|', $pkgsend_cmd or die "Cannot run $pkgsend_cmd: $!\n";

  while (my $ps = <$PKGSEND>) {
    chomp($ps);

    # Skip -uninstalled.pc files, since those are only used during build
    next if $ps =~ m{^file\b.*\bpath=.*-uninstalled.pc\b};

    # Convert 64-bit subdirectories to platform-independent form
    $ps =~ s{([=/])${subdir64}\b}{$1\$(ARCH64)}g;

    # Don't add duplicates of actions we've already got
    my $action = $ps;
    if ($ps =~ m{ path=(\S+)}) {
      $action = $1;
    } else {
      $action =~ s{\s+}{ }g;
    }
    next if exists $actions_seen{$action};
    $actions_seen{$action} = $ps;

    # Drop file path from file actions, so we always use the path attribute
    $ps =~ s{^file (\S+) (.* path=\1)}{file $2};

    # Drop attributes that will be generated during package build
    push @manifest_contents,
      join(" ", grep (! /^(mode|group|owner|pkg.size|timestamp)=/,
		      split /\s+/, $ps));
  }
  close $PKGSEND;
}

my $pkgfmt_cmd = 'pkgfmt';
if (exists $options{'pkgfmt'}) {
  $pkgfmt_cmd = join(' ', @{$options{'pkgfmt'}});
}
open my $PKGFMT, '|-', $pkgfmt_cmd or die "Cannot run $pkgfmt_cmd: $!\n";
foreach my $line (@manifest_header, @manifest_contents) {
  print $PKGFMT $line, "\n";
}
close $PKGFMT;
