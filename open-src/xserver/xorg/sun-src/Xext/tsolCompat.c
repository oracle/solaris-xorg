/* Copyright 2007 Sun Microsystems, Inc.  All rights reserved.
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
 */ 

/* This file provides XACE wrappers around the TSOL hooks to provide
   backwards binary compatibility for existing Xtsol extension modules. */

#include "dix-config.h"
#include "dix.h"
#include "extnsionst.h"
#include "xace.h"
#include "xacestr.h"

#define _XTSOL_SERVER
#include <X11/extensions/Xtsol.h>

#define CALLBACK(name) static void \
name(CallbackListPtr *pcbl, pointer nulldata, pointer calldata)

/*
    int (*InitWindow)(ClientPtr, WindowPtr);
*/
CALLBACK(tsolCompatInitWindow)
{
    XaceWindowRec *rec = (XaceWindowRec *) calldata;
    ClientPtr client = rec->client;
    WindowPtr pWin = rec->pWin;

    (*pSecHook->InitWindow)(client, pWin);
}

/*
    char (*CheckPropertyAccess)(ClientPtr client, WindowPtr pWin,
    				ATOM propertyName, Mask access_mode)
*/
CALLBACK(tsolCompatCheckPropertyAccess)
{
    XacePropertyAccessRec *rec = (XacePropertyAccessRec *) calldata;
    ClientPtr client = rec->client;
    WindowPtr pWin = rec->pWin;
    ATOM propertyName = rec->propertyName;
    Mask access_mode = rec->access_mode;

    rec->rval = (*pSecHook->CheckPropertyAccess)(client, pWin, propertyName,
					      access_mode);
}

/*
    void (*ProcessKeyboard)(xEvent *, KeyClassPtr);
*/
CALLBACK(tsolCompatProcessKeyboard)
{
    XaceKeyAvailRec *rec = (XaceKeyAvailRec *) calldata;
    xEventPtr event = rec->event;
    DeviceIntPtr keybd = rec->keybd;
/*  int count = rec->count; */
    KeyClassPtr keyc = keybd->key;

    (*pSecHook->ProcessKeyboard)(event, keyc);
}

/*
    void (*AuditStart)(ClientPtr client);
*/
CALLBACK(tsolCompatAuditStart)
{
    XaceAuditRec *rec = (XaceAuditRec *) calldata;
    ClientPtr client = rec->client;
    
    (*pSecHook->AuditStart)(client);
}

/*
    void (*AuditEnd)(ClientPtr client, int result);
*/
CALLBACK(tsolCompatAuditEnd)
{
    XaceAuditRec *rec = (XaceAuditRec *) calldata;
    ClientPtr client = rec->client;
    int result = rec->requestResult;

    (*pSecHook->AuditEnd)(client, result);
}

/*
    pointer (* CheckAccess)(ClientPtr pClient, XID id, RESTYPE classes,
	Mask access_mode, pointer resourceval);
*/
CALLBACK(tsolCompatCheckResourceIDAccess)
{
    XaceResourceAccessRec *rec = (XaceResourceAccessRec *) calldata;
    ClientPtr client = rec->client;
    XID id = rec->id;
    RESTYPE rtype = rec->rtype;
    Mask access_mode = rec->access_mode;
    pointer res = rec->res;

    if (client->CheckAccess) {
	if ((*client->CheckAccess)(client, id, rtype, access_mode, res)
	    == NULL) {
	    rec->rval = FALSE;
	}
    }
}


_X_HIDDEN void
tsolCompatRegisterHooks(void)
{
    if (pSecHook != NULL) {
	/* register callbacks */
	XaceRegisterCallback(XACE_WINDOW_INIT, tsolCompatInitWindow, NULL);
	XaceRegisterCallback(XACE_PROPERTY_ACCESS,
			     tsolCompatCheckPropertyAccess, NULL);
	XaceRegisterCallback(XACE_KEY_AVAIL, tsolCompatProcessKeyboard, NULL);
	XaceRegisterCallback(XACE_AUDIT_BEGIN, tsolCompatAuditStart, NULL);
	XaceRegisterCallback(XACE_AUDIT_END, tsolCompatAuditEnd, NULL);
	XaceRegisterCallback(XACE_RESOURCE_ACCESS,
			     tsolCompatCheckResourceIDAccess, NULL);
	DeclareExtensionSecurity(TSOLNAME, TRUE);
    }
}
