#
# Copyright (c) 2015, 2017, Oracle and/or its affiliates. All rights reserved.
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

# If invoked with -v FUNCTION=name, then just print $1 (s|/|-|).
# Otherwise we expect to be invoked with TARGET=<whatever>, then if
# s|/|-| on $1 matches TARGET, we generate a manifest.
{
  if (substr($1, 1, 1) == "#") { # Skip comments.
    continue
  }
  fmri=$1
  split(fmri, a, "@")
  gsub("/", "-", a[1])
  target = sprintf("%s.p5m", a[1])
  if (FUNCTION == "name") {
    printf "%s\n", target
    continue
  }
  if (target != TARGET) {
    continue
  }
  printf "set name=pkg.fmri value=pkg:/%s\n", fmri
  if ($NF ~ /^arch=/) {
    arch=substr($NF,6);
    NF--;
  }
  if ($NF ~ /^incorporate=/) {
    incorporate=$NF;
    NF--;
  }
  if (NF == 2) {
    print "set name=pkg.renamed value=true"
    printf "depend type=require fmri=pkg:/%s\n", $2
  } else {
    print "set name=pkg.obsolete value=true"
  }
  if (incorporate) {
    printf "set name=org.opensolaris.consolidation %s value=default\n",
      incorporate
  }
  if (arch) {
    printf "set name=variant.arch value=%s\n", arch
  }
      
  exit 0 # We're done; no point continuing.
}
