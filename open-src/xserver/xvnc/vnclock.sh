#!/usr/sbin/sh
#
# Copyright (c) 2016, Oracle and/or its affiliates. All rights reserved.
#

XAUTHORITY=/var/xauth/$1
export XAUTHORITY
xscreensaver-command -display :$1 -lock
