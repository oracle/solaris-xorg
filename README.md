# This repo is no longer maintained

The software from the X consolidation in Solaris has been moved to the Userland consolidation, under `components/x11`, so you can now find it at <https://github.com/oracle/solaris-userland>.

----

# Getting started with the X Consolidation

## Getting Started
This README provides a very brief overview of the gate (i.e., source
code repository), how to retrieve a copy, and how to build it.  

## Overview
The X consolidation maintains a project at

     https://github.com/oracle/solaris-xorg

That repo contains Makefile, patches, IPS (i.e., pkg(7)) manifests,
and other files necessary to build, package and publish the Xorg bits
on Solaris.

## Getting the Bits
The canonical repository internal to Oracle is stored in Mercurial, and
is mirrored to an external Git repository on GitHub.  In order to build
or develop in the gate, you will need to clone it.  You can do so with
the following command.  

    $ git clone https://github.com/oracle/solaris-xorg your-clone

## Building the software
Please review BUILD_INSTRUCTIONS for details on how to build and generate
IPS packages.

# Copyright
Copyright (c) 2017, Oracle and/or its affiliates. All rights reserved.
