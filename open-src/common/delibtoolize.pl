#! /usr/perl5/bin/perl
#
# Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
# Use is subject to license terms.
#
# ident	"@(#)delibtoolize.pl	1.6	07/07/25 SMI"
#

#
# Undo libtool damage to makefiles to allow us to control the linker
# settings that libtool tries to force on us.
#

use strict;
use warnings;
use integer;

use File::Find;

sub process_file {
  if ($_ eq "Makefile" && -f $_) {
    print "delibtoolizing $File::Find::name...\n";
    my $old_file = $_ . "~";
    my $new_file = $_;
    rename($new_file, $old_file) or 
      die "Can't rename $new_file to $old_file: $!\n";

    open(OLD, "<$old_file") or die "Can't open $old_file for reading: $!\n";
    open(NEW, ">$new_file") or die "Can't open $new_file for writing: $!\n";
    
    my $compiler;
    my $l;
    my @inlines = ();
    my %so_versions = ();

    # Read in original file and preprocess for data we'll need later
    while ($l = <OLD>) {
      if ($l =~ m%^\s*CC\s*=\s*(\S*)%) {
	$compiler = $1;
      }

      # TODO: handle line continuation
      if ($l =~ m%^(\S*)_la_LDFLAGS\s=.*-version-number (\d+)[:\d]+%) {
	$so_versions{$1} = $2;
      }
      if ($l =~ m%^(\S*)_la_LDFLAGS\s=.*\s*-avoid-version\b%) {
	$so_versions{$1} = "none";
	$l =~ s%-avoid-version\b%%;
      }

      push @inlines, $l;
    }
    close(OLD) or die;

    my $picflags = "-Kpic -DPIC";
    my $sharedobjflags = "-G";

    if ($compiler =~ m/gcc/) {
      $picflags = "-fpic -DPIC";
      $sharedobjflags = "-shared";
    }

    my $curtarget = "";

    my $l = "";

    foreach my $curline (@inlines) {
      $l .= $curline;
      next if ($curline =~ m/\\$/);

      # Remove libtool script from compile steps &
      # add PIC flags that libtool normally provides
      $l =~ s{\$\(LIBTOOL\)
	      (?:[\\\s]+ --tag=CC)?
	      (?:[\\\s]+ \$\(AM_LIBTOOLFLAGS\) [\\\s]+ \$\(LIBTOOLFLAGS\))?
	      [\\\s]+ --mode=compile
	      [\\\s]+ (\$\(CC\)|\$\(CCAS\))
	    }{$1 $picflags}xs;

      # Remove libtool script from link step
      $l =~ s{\$\(LIBTOOL\)
	      (?:[\\\s]+ --tag=CC)?
	      (?:[\\\s]+ \$\(AM_LIBTOOLFLAGS\) [\\\s]+ \$\(LIBTOOLFLAGS\))?
	      [\\\s]+ --mode=link
	    }{}xs;

      # Change -rpath to -R in link arguments
      $l =~ s{(\s*)-rpath(\s*)}{$1-R$2}g;

      # Change flags for building shared object from arguments to libtool
      # script into arguments to linker
      $l =~ s{(_la_LDFLAGS\s*=.*) -module}{$1 $sharedobjflags};
      $l =~ s{(_la_LDFLAGS\s*=.*) -version-number (\d+)[:\d]+}
	     {$1 $sharedobjflags};
      $l =~ s{(_la_LDFLAGS\s*=.*) -no-undefined}{$1 -z defs};

      # Change file names
      my $so;
      foreach $so (keys %so_versions) {
	if ($so_versions{$so} eq "none") {
	  $l =~ s%$so\.la\b%$so.so%g;
	} else {
	  $l =~ s%$so\.la\b%$so.so.$so_versions{$so}%g;
	}
      }
      $l =~ s%\.la\b%.a%g;

      if ($l =~ m%^(\S+):%) {
	$curtarget = $1;
      } elsif ($l =~ m%^\s*$%) {
	$curtarget = "";
      }

      if ($curtarget =~ m%^.*\.a$%) {
	$l =~ s%\$\(LINK\)%\$(AR) cru $curtarget%;
      }

      print NEW $l;
      $l = "";
    }
    close(NEW) or die;
  }
}

find(\&process_file, @ARGV);
