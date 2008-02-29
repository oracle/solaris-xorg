#! /usr/perl5/bin/perl
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
# ident	"@(#)delibtoolize.pl	1.9	08/02/27 SMI"
#

#
# Undo libtool damage to makefiles to allow us to control the linker
# settings that libtool tries to force on us.
#
# Usage: delibtoolize.pl [-P] <path>
# -P - Use large pic flags (-KPIC/-fPIC) instead of default/small (-Kpic/-fpic)

use strict;
use warnings;
use integer;
use Getopt::Std;

use File::Find;

my %opts;
getopts('P', \%opts);

my $pic_size = "pic";
if (exists($opts{'P'})) {
  $pic_size = "PIC";
}

sub process_file {
  if ($_ eq 'Makefile' && -f $_) {
    print "delibtoolizing $File::Find::name...\n";
    my $old_file = $_ . '~';
    my $new_file = $_;
    rename($new_file, $old_file) or 
      die "Can't rename $new_file to $old_file: $!\n";

    open my $OLD, '<', $old_file
      or die "Can't open $old_file for reading: $!\n";
    open my $NEW, '>', $new_file
      or die "Can't open $new_file for writing: $!\n";
    
    my $compiler;
    my @inlines = ();
    my %so_versions = ();

    # Read in original file and preprocess for data we'll need later
    while (my $l = <$OLD>) {
      if ($l =~ m/^\s*CC\s*=\s*(\S*)/) {
	$compiler = $1;
      }

      # TODO: handle line continuation
      if ($l =~ m/^(\S*)_la_LDFLAGS\s=.*-version-number (\d+)[:\d]*/ms) {
	$so_versions{$1} = $2;
      }
      if ($l =~ m/^(\S*)_la_LDFLAGS\s=.*\s*-avoid-version\b/ms) {
	$so_versions{$1} = 'none';
	$l =~ s{-avoid-version\b}{}ms;
      }

      push @inlines, $l;
    }
    close($OLD) or die;

    my $picflags = "-K$pic_size -DPIC";
    my $sharedobjflags = '-G';

    if (defined($compiler) && ($compiler =~ m/gcc/)) {
      $picflags = "-f$pic_size -DPIC";
      $sharedobjflags = '-shared';
    }

    my $curtarget = "";

    my $l = "";

    foreach my $curline (@inlines) {
      $l .= $curline;
      next if ($curline =~ m/\\$/);
      chomp $l;

      # Remove libtool script from compile steps &
      # add PIC flags that libtool normally provides
      $l =~ s{\$\(LIBTOOL\)
	      (?:[\\\s]+ \$\(LT_QUIET\))?
	      (?:[\\\s]+ --tag=(?:CC|CXX))?
	      (?:[\\\s]+ \$\(AM_LIBTOOLFLAGS\) [\\\s]+ \$\(LIBTOOLFLAGS\))?
	      [\\\s]+ --mode=compile
	      [\\\s]+ (\$\(CC\)|\$\(CCAS\)|\$\(CXX\))
	    }{$1 $picflags}xs;

      # Remove libtool script from link step
      $l =~ s{\$\(LIBTOOL\)
	      (?:[\\\s]+ \$\(LT_QUIET\))?
	      (?:[\\\s]+ --tag=(?:CC|CXX))?
	      (?:[\\\s]+ \$\(AM_LIBTOOLFLAGS\) [\\\s]+ \$\(LIBTOOLFLAGS\))?
	      [\\\s]+ --mode=link
	    }{}xs;

      # Change -rpath to -R in link arguments
      $l =~ s{(\s*)-rpath(\s*)}{$1-R$2}msg;

      # Change flags for building shared object from arguments to libtool
      # script into arguments to linker
      if ($l =~ m/_la_LDFLAGS\s*=/) {
	$l =~ s{(\s*$sharedobjflags)+\b}{}msg;
	$l =~ s{(_la_LDFLAGS\s*=\s*)}{$1 $sharedobjflags }ms;
	$l =~ s{\s+-module\b}{}ms;
	$l =~ s{\s+-version-number\s+\d+[:\d]*}{}ms;
	$l =~ s{\s+-no-undefined\b}{ -z defs}ms;
      }

      # Change file names
      foreach my $so (keys %so_versions) {
	if ($so_versions{$so} eq 'none') {
	  $l =~ s{$so\.la\b}{$so.so}msg;
	} else {
	  $l =~ s{$so\.la\b}{$so.so.$so_versions{$so}}msg;
	}
      }
      $l =~ s{\.la\b}{.a}msg;
      $l =~ s{\.libs/\*\.o\b}{*.lo}msg;
      $l =~ s{\.lo\b}{.o}msg;

      if ($l =~ m/^(\S+):/) {
	$curtarget = $1;
      } elsif ($l =~ m/^\s*$/) {
	$curtarget = "";
      }

      # Static libraries
      if ($curtarget =~ m/^.*\.a$/) {
	$l =~ s{\$\(\w*LINK\)}{\$(AR) cru $curtarget}ms;
	$l =~ s{\$\(\w*(?:LIBS|LIBADD)\)}{}msg;
	$l =~ s{(\$\((?:\w*_)?AR\).*\s+)-R\s*\$\(libdir\)}{$1}msg;
      }

      print $NEW $l, "\n";
      $l = "";
    }
    close($NEW) or die;
  }
}

find(\&process_file, @ARGV);
