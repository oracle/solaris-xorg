/*
 * Trusted xscreensaver
 *
 * Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, and/or sell copies of the Software, and to permit persons
 * to whom the Software is furnished to do so, provided that the above
 * copyright notice(s) and this permission notice appear in all copies of
 * the Software and that both the above copyright notice(s) and this
 * permission notice appear in supporting documentation.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT
 * OF THIRD PARTY RIGHTS. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * HOLDERS INCLUDED IN THIS NOTICE BE LIABLE FOR ANY CLAIM, OR ANY SPECIAL
 * INDIRECT OR CONSEQUENTIAL DAMAGES, OR ANY DAMAGES WHATSOEVER RESULTING
 * FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION
 * WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 *
 * Except as contained in this notice, the name of a copyright holder
 * shall not be used in advertising or otherwise to promote the sale, use
 * or other dealings in this Software without prior written authorization
 * of the copyright holder.
 *
 *
 *
 * Based on work by Erwann Chenede, Ghee Teo
 *
 * Used to check if we are in a multilabel session and to load
 * additional functionality within the multilabel session.
 */

#include <dlfcn.h>
#include <link.h>
#include <stdlib.h>
#include <user_attr.h>
#include <sys/types.h>
#include <unistd.h>
#include <strings.h>

#include "trusted-utils.h"

/* 
 * Checks for Multi label session 
 */
gboolean
tsol_is_multi_label_session (void)
{
       static char *session = NULL;

       if (!session)
               session = (char *)getenv("TRUSTED_SESSION");

       if (!session)
               return FALSE;

       return TRUE;
}

/*
 * dynamicly load the libxtsol library
 */
static
void * dlopen_xtsol (void)
{
   void  *handle = NULL;

   if ((handle = dlopen ("/usr/lib/libXtsol.so.1", RTLD_LAZY)) != NULL)
       return handle;
   if ((handle = dlopen ("/usr/openwin/lib/libXtsol.so.1", RTLD_LAZY)) != NULL)
       return handle;

   return handle;
}

/*
 * dynamicly load the libDtTsol library
 */
static
void * dlopen_gnometsol (void)
{
   void  *handle = NULL;

   if ((handle = dlopen ("/usr/lib/libgnometsol.so.1", RTLD_LAZY)) != NULL)
       return handle;

   return handle;
}

xtsol_XTSOLgetWorkstationOwner      libxtsol_XTSOLgetWorkstationOwner = NULL;

void
XTSOLgetWorkstationOwner(Display *dpy, uid_t *WorkstationOwner)
{
  static gpointer xtsol_handle = NULL;
  static gboolean _xtsol_initialized = FALSE;

  if ( ! _xtsol_initialized ) {
    _xtsol_initialized = TRUE;
    xtsol_handle = dlopen_xtsol ();
    if (xtsol_handle != NULL)
      libxtsol_XTSOLgetWorkstationOwner = (xtsol_XTSOLgetWorkstationOwner) dlsym(xtsol_handle,
					     "XTSOLgetWorkstationOwner");
  }

  if (libxtsol_XTSOLgetWorkstationOwner == NULL) {
    *WorkstationOwner = getuid();
  } else
    libxtsol_XTSOLgetWorkstationOwner(dpy, WorkstationOwner);
}

gnome_tsol_get_usrattr_val		libgnome_tsol_get_usrattr_val = NULL;

/*
 * Returns a value from uattr for the given key.
 * If there is no value in user_attr, then it returns the
 * system wide default from policy.conf or labelencodings
 * as appropriate.
 */
char *
getusrattrval(userattr_t *uattr, char *keywd)
{
  static gpointer gnometsol_handle = NULL;
  static gboolean _gnometsol_initialized = FALSE;
  char *value;

  if ( ! _gnometsol_initialized ) {
    _gnometsol_initialized = TRUE;
    gnometsol_handle = dlopen_gnometsol ();
    if (gnometsol_handle != NULL)
      libgnome_tsol_get_usrattr_val = (gnome_tsol_get_usrattr_val) dlsym(gnometsol_handle,
					     "gnome_tsol_get_usrattr_val");
  }

  if (libgnome_tsol_get_usrattr_val == NULL) {
    if (strcmp(keywd, USERATTR_IDLETIME_KW) == 0)
      value = strdup("15");
    else if (strcmp(keywd, USERATTR_IDLECMD_KW) == 0)
      value = strdup(USERATTR_IDLECMD_LOCK_KW);
  } else
    value = libgnome_tsol_get_usrattr_val(uattr, keywd);
  
  return ( value );
}
