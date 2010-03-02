#
# Copyright © 2003 Sun Microsystems, Inc.  All rights reserved.
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

# AuDispose.c
Function	XauDisposeAuth
Include		<X11/Xauth.h>
Declaration	void XauDisposeAuth (Xauth *auth)
Version		SUNW_1.1
Arch		all
End

# AuFileName.c
Function	XauFileName
Include		<X11/Xauth.h>
Declaration	char *XauFileName (void)
Exception	$return == NULL
Version		SUNW_1.1
Arch		all
End

# AuGetAddr.c
Function	XauGetAuthByAddr
Include		<X11/Xauth.h>
Declaration	Xauth *XauGetAuthByAddr(unsigned int family, \
			unsigned int address_length, const char* address, \
			unsigned int number_length, const char* number, \
			unsigned int name_length, const char* name)
Exception	$return == NULL
Version		SUNW_1.1
Arch		all
End

# AuGetBest.c
Function	XauGetBestAuthByAddr
Include		<X11/Xauth.h>
Declaration	Xauth *XauGetBestAuthByAddr (unsigned int family, \
			unsigned int address_length, const char* address, \
			unsigned int number_length, const char* number, \
		       int types_length, char** types, const int* type_lengths)
Exception	$return == NULL
Version		SUNW_1.1
Arch		all
End

# AuLock.c
Function	XauLockAuth
Include		<X11/Xauth.h>
Declaration	int XauLockAuth (const char *file_name, int retries, \
			int timeout, long dead)
Exception	$return == LOCK_ERROR
Version		SUNW_1.1
Arch		all
End

# AuRead.c
Function	XauReadAuth
Include		<X11/Xauth.h>
Declaration	Xauth *XauReadAuth (FILE *auth_file)
Exception	$return == NULL
Version		SUNW_1.1
Arch		all
End

# AuUnlock.c
Function	XauUnlockAuth
Include		<X11/Xauth.h>
Declaration	int XauUnlockAuth (const char *file_name)
Exception	$return == 0
Version		SUNW_1.1
Arch		all
End

# AuWrite.c
Function	XauWriteAuth
Include		<X11/Xauth.h>
Declaration	int XauWriteAuth (FILE *auth_file, Xauth *auth)
Exception	$return == 0
Version		SUNW_1.1
Arch		all
End

