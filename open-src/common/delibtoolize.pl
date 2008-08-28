#! /usr/perl5/bin/perl
#
# Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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
# ident	"@(#)delibtoolize.pl	1.12	08/08/28 SMI"
#

#
# Undo libtool damage to makefiles to allow us to control the linker
# settings that libtool tries to force on us.
#
# Usage: delibtoolize.pl [-P] <path>
# -P - Use large pic flags (-KPIC/-fPIC) instead of default/small (-Kpic/-fpic)
# -s - Only track libraries from a single file at a time, instead of across all
#	files in a project

use strict;
use warnings;
use integer;
use Getopt::Std;

use File::Find;

my %opts;
getopts('Ps', \%opts);

my $pic_size = "pic";
if (exists($opts{'P'})) {
  $pic_size = "PIC";
}

my $single_file;
if (exists($opts{'s'})) {
  $single_file = 1;
}

my %compiler_pic_flags = ( 'cc' => "-K$pic_size -DPIC",
			   'gcc' => "-f$pic_size -DPIC" );
my %compiler_sharedobj_flags = ( 'cc' => '-G -z allextract',
				 'gcc' => '-shared  -Wl,-z,allextract' );

my %so_versions = ();
my @Makefiles;

sub scan_file {
  if ($_ eq 'Makefile' && -f $_) {
    my $old_file = $_;

    open my $OLD, '<', $old_file
      or die "Can't open $old_file for reading: $!\n";

    # Read in original file and preprocess for data we'll need later
    my $l = "";
    while (my $n = <$OLD>) {
      $l .= $n;
      # handle line continuation
      next if ($n =~ m/\\$/);

      if ($l =~ m/^([^\#\s]*)_la_LDFLAGS\s*=(.*)/ms) {
	my $libname = $1;
	my $flags = $2;
	my $vers;
	
	if ($flags =~ m/[\b\s]-version-(number|info)\s+(\S+)/ms) {
	  my $vtype = $1;
	  my $v = $2;
	  if (($vtype eq "info") && ($v =~ m/^(\d+):\d+:(\d+)$/ms)) {
	    $vers = $1 - $2;
	  } elsif ($v =~ m/^(\d+)[:\d]*$/ms) {
	    $vers = $1;
	  } else {
	    $vers = $v;
	  }
	}
	elsif ($flags =~ m/-avoid-version\b/ms) {
	  $vers = 'none';
	}

	my $ln = $libname;
	if ($single_file) {
	  $ln = $File::Find::name . "::" . $libname;
	}
	if (defined($vers) && !defined($so_versions{$ln})) {
	  $so_versions{$ln} = $vers;
	  print "Set version to $so_versions{$ln} for $ln.\n";
	}
      }
      $l = "";
    }
    close($OLD) or die;

    push @Makefiles, $File::Find::name;
  }
}

sub modify_file {
  my ($filename) = @_;

  print "delibtoolizing $filename...\n";

  my $old_file = $filename . '~';
  my $new_file = $filename;
  rename($new_file, $old_file) or
    die "Can't rename $new_file to $old_file: $!\n";

  open my $OLD, '<', $old_file
    or die "Can't open $old_file for reading: $!\n";
  open my $NEW, '>', $new_file
    or die "Can't open $new_file for writing: $!\n";

  my $compiler;
  my @inlines = ();

  # Read in original file and preprocess for data we'll need later
  my $l = "";
  while (my $n = <$OLD>) {
    $l .= $n;
    # handle line continuation
    next if ($n =~ m/\\$/);

    if ($l =~ m/^\s*CC\s*=\s*(\S*)/) {
      $compiler = $1;
    }

    push @inlines, $l;
    $l = "";
  }
  close($OLD) or die;

  my $compiler_type = 'cc'; # default to Sun Studio
  if (defined($compiler) && ($compiler =~ m/gcc/)) {
    $compiler_type = 'gcc';
  }

  my $picflags = $compiler_pic_flags{$compiler_type};
  my $sharedobjflags = $compiler_sharedobj_flags{$compiler_type};

  my $curtarget = "";

  foreach $l (@inlines) {
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
      $l =~ s{(\s+)-avoid-version\b}{$1}ms;
      $l =~ s{(\s+)-module\b}{$1}ms;
      $l =~ s{(\s+)-version-(?:number|info)\s+\S+}{$1-h \$\@}ms;
      $l =~ s{(\s+)-no-undefined\b}{$1-z defs}ms;
    }

    # Change file names
    my @so_list = keys %so_versions;
    if ($single_file) {
      my $pat = $filename . "::";
      @so_list = grep(/^$pat/, @so_list);
    }
    foreach my $so (@so_list) {
      if ($so_versions{$so} eq 'none') {
	$l =~ s{$so\.la\b}{$so.so}msg;
      } else {
	$l =~ s{$so\.la\b}{$so.so.$so_versions{$so}}msg;
      }
    }
    $l =~ s{\.la\b}{.a}msg;
    $l =~ s{\.libs/\*\.o\b}{*.lo}msg;
    $l =~ s{\.lo\b}{.o}msg;

    my $newtarget = $curtarget;
    if ($l =~ m/^(\S+):/) {
      $newtarget = $1;
    } elsif ($l =~ m/^\s*$/) { 
      $newtarget = "";
    }

    if ($curtarget ne $newtarget) { # end of rules for a target
      # Need to add in .so links that libtool makes for .la installs
      if ($curtarget =~ m/^install-(.*)LTLIBRARIES$/ms) {
	my $dirname = $1;
	my $installrule = <<'END_RULE';
	list='$(<DIRNAME>_LTLIBRARIES)'; for p in $$list; do \
	  so=$$(expr $$p : '\(.*\.so\)') ; \
	  if [ $$p != $$so ] ; then \
		echo "rm -f $(DESTDIR)$(<DIRNAME>dir)/$$so" ; \
		rm -f $(DESTDIR)$(<DIRNAME>dir)/$$so ; \
		echo "ln -s $$p $(DESTDIR)$(<DIRNAME>dir)/$$so" ; \
		ln -s $$p $(DESTDIR)$(<DIRNAME>dir)/$$so ; \
	  fi; \
	done
END_RULE
	$installrule =~ s/\<DIRNAME\>/$dirname/msg;
	$l .= $installrule;

#	my $installdir = '$(DESTDIR)$(' . $dirname . 'dir)';
#	foreach my $so (keys %so_versions) {
#	  if ($so_versions{$so} ne 'none') {
#	    $l .= "\t-rm -f $installdir/$so.so\n";
#	    $l .= "\tln -s $so.so.$so_versions{$so} $installdir/$so.so\n";
#	  }
#	}
      }

      $curtarget = $newtarget;
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

find(\&scan_file, @ARGV);

foreach my $mf ( @Makefiles ) {
  modify_file($mf);
}
