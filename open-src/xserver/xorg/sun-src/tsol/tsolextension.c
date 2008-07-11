/* Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident   "@(#)tsolextension.c 1.29     08/07/10 SMI"

#include <stdio.h>
#include "auditwrite.h"
#include <bsm/libbsm.h>
#include <bsm/audit_uevents.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <ucred.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/tsol/tndb.h>
#include <strings.h>
#include <string.h>
#include <pwd.h>
#include <unistd.h>
#include <sys/stat.h>
#include <rpc/rpc.h>
#include <zone.h>


#define NEED_REPLIES

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include "misc.h"
#include "osdep.h"
#include <X11/Xauth.h>
#include "tsol.h"
#include "inputstr.h"
#include "extnsionst.h"
#ifdef PANORAMIX
#include "../Xext/panoramiXsrv.h"
#endif
#ifdef XCSECURITY
#define _SECURITY_SERVER
#include "security.h"
#endif
#include "tsolpolicy.h"

#define  BadCmapCookie      0
#define  Tsolextension      0x0080    /* Tsol extensions begin at 128 */
#define  MAX_SCREENS        3         /* screens allowed */
#define EXTNSIZE 128

#define SECURE_RPC_AUTH	"SUN-DES-1"
#define SECURE_RPC_LEN	9

extern bslabel_t *lookupSL();
extern void (*ReplySwapVector[]) ();
extern TsolInfoPtr GetClientTsolInfo();
extern char *NameForAtom(Atom atom);

static int ProcTsolDispatch(ClientPtr);
static int ProcSetPolyInstInfo(ClientPtr);
static int ProcSetPropLabel(ClientPtr);
static int ProcSetPropUID(ClientPtr);
static int ProcSetResLabel(ClientPtr);
static int ProcSetResUID(ClientPtr);
static int ProcGetClientAttributes(ClientPtr);
static int ProcGetClientLabel(ClientPtr);
static int ProcGetPropAttributes(ClientPtr);
static int ProcGetResAttributes(ClientPtr);
static int ProcMakeTPWindow(ClientPtr);
static int ProcMakeTrustedWindow(ClientPtr);
static int ProcMakeUntrustedWindow(ClientPtr);

static int SProcTsolDispatch(ClientPtr);
static int SProcSetPolyInstInfo(ClientPtr);
static int SProcSetPropLabel(ClientPtr);
static int SProcSetPropUID(ClientPtr);
static int SProcSetResLabel(ClientPtr);
static int SProcSetResUID(ClientPtr);
static int SProcGetClientAttributes(ClientPtr);
static int SProcGetClientLabel(ClientPtr);
static int SProcGetPropAttributes(ClientPtr);
static int SProcGetResAttributes(ClientPtr);
static int SProcMakeTPWindow(ClientPtr);
static int SProcMakeTrustedWindow(ClientPtr);
static int SProcMakeUntrustedWindow(ClientPtr);

static void TsolReset();
static void BreakAllGrabs(ClientPtr client);

extern void init_xtsol();
extern void init_win_privsets();
extern void free_win_privsets();
extern int DoScreenStripeHeight(int screen_num);
extern int AddUID(int *userid);

static unsigned char TsolReqCode = 0;
static int tsolEventBase = -1;
extern unsigned int StripeHeight;
int ScreenStripeHeight[MAX_SCREENS - 1] = {0, 0};

extern int tsolClientPrivateIndex;
extern int tsolWindowPrivateIndex;
extern int tsolPixmapPrivateIndex;

static HotKeyRec hotkey = {FALSE, 0, 0, 0, 0};

int tsolMultiLevel = TRUE;

int OwnerUIDint;
extern uid_t OwnerUID;
uid_t PublicObjUID = 0;
extern WindowPtr tpwin;
extern bclear_t SessionHI;        /* HI Clearance */
extern bclear_t SessionLO;        /* LO Clearance */
extern TsolPolyInstInfoRec tsolpolyinstinfo;

extern void LoadTsolConfig();
extern void MakeTSOLAtoms();
extern void UpdateTsolNode();
/*
 * Protocol handling vectors
 */
extern int (*ProcVector[])();
extern int (*SwappedProcVector[])();

int (*TsolSavedProcVector[PROCVECTORSIZE])(ClientPtr client);
int (*TsolSavedSwappedProcVector[PROCVECTORSIZE])(ClientPtr client);

extern SecurityHookPtr pSecHook;
SecurityHook tsolSecHook;

XID TsolCheckAuthorization (unsigned int name_length,
	char *name, unsigned int data_length, 
	char *data, ClientPtr client, char **reason);
void TsolDeleteClientFromAnySelections(ClientPtr);
void TsolDeleteWindowFromAnySelections(WindowPtr);

extern int TsolChangeWindowProperty(ClientPtr, WindowPtr, Atom, Atom, int, int,
	unsigned long, pointer, Bool);
extern int TsolDeleteProperty(ClientPtr, WindowPtr, Atom);
extern int TsolInitWindow(ClientPtr, WindowPtr);
extern void TsolAuditStart(ClientPtr);
extern void TsolAuditEnd(ClientPtr, int);

static void TsolClientStateCallback(CallbackListPtr *pcbl,
	pointer nulldata, pointer calldata);
static void TsolSetClientInfo(ClientPtr client);
static void TsolProcessKeyboard(xEvent *, KeyClassPtr);
static char TsolCheckPropertyAccess(ClientPtr client, WindowPtr pWin, ATOM propertyName,
	Mask access_mode);

extern int ProcTsolInternAtom(ClientPtr client);
extern int ProcTsolSetSelectionOwner(ClientPtr client);
extern int ProcTsolGetSelectionOwner(ClientPtr client);
extern int ProcTsolConvertSelection(ClientPtr client);
extern int ProcTsolGetProperty(ClientPtr client);
extern int ProcTsolListProperties(ClientPtr client);
extern int ProcTsolChangeKeyboardMapping(ClientPtr client);
extern int ProcTsolSetPointerMapping(ClientPtr client);
extern int ProcTsolChangeKeyboardControl(ClientPtr client);
extern int ProcTsolBell(ClientPtr client);
extern int ProcTsolChangePointerControl(ClientPtr client);
extern int ProcTsolSetModifierMapping(ClientPtr client);

extern int ProcTsolCreateWindow(ClientPtr client);
extern int ProcTsolChangeWindowAttributes(ClientPtr client);
extern int ProcTsolConfigureWindow(ClientPtr client);
extern int ProcTsolCirculateWindow(ClientPtr client);
extern int ProcTsolReparentWindow(ClientPtr client);
extern int ProcTsolSetInputFocus(ClientPtr client);
extern int ProcTsolGetInputFocus(ClientPtr client);
extern int ProcTsolSendEvent(ClientPtr client);
extern int ProcTsolSetInputFocus(ClientPtr client);
extern int ProcTsolGetInputFocus(ClientPtr client);
extern int ProcTsolGetGeometry(ClientPtr client);
extern int ProcTsolGrabServer(ClientPtr client);
extern int ProcTsolUngrabServer(ClientPtr client);
extern int ProcTsolCreatePixmap(ClientPtr client);
extern int ProcTsolSetScreenSaver(ClientPtr client);
extern int ProcTsolChangeHosts(ClientPtr client);
extern int ProcTsolChangeAccessControl(ClientPtr client);
extern int ProcTsolKillClient(ClientPtr client);
extern int ProcTsolSetFontPath(ClientPtr client);
extern int ProcTsolChangeCloseDownMode(ClientPtr client);
extern int ProcTsolListInstalledColormaps(ClientPtr client);
extern int ProcTsolGetImage(ClientPtr client);
extern int ProcTsolQueryTree(ClientPtr client);
extern int ProcTsolQueryPointer(ClientPtr client);
extern int ProcTsolQueryExtension(ClientPtr client);
extern int ProcTsolListExtensions(ClientPtr client);
extern int ProcTsolMapWindow(ClientPtr client);
extern int ProcTsolMapSubwindows(ClientPtr client);
extern int ProcTsolCopyArea(ClientPtr client);
extern int ProcTsolCopyPlane(ClientPtr client);
extern int ProcTsolPolySegment(ClientPtr client);
extern int ProcTsolPolyRectangle(ClientPtr client);

extern int TsolPanoramiXGetGeometry(ClientPtr client);

/*
 * Initialize the extension. Main entry point for this loadable
 * module.
 */

void
TsolExtensionInit()
{
	ExtensionEntry *extEntry;
	ScreenPtr pScreen;
	int i;
	priv_set_t	*pset;

	/* sleep(20); */

	/* This extension is supported on a labeled system */
	if (!is_system_labeled()) {
		return;
	}

	tsolMultiLevel = TRUE;
	(void) setpflags(PRIV_AWARE, 1);

	init_xtsol();
	init_win_privsets();

	extEntry = AddExtension(TSOLNAME, TSOL_NUM_EVENTS, TSOL_NUM_ERRORS,
		ProcTsolDispatch, SProcTsolDispatch, TsolReset, 
		StandardMinorOpcode);

	if (extEntry == NULL) {
		ErrorF("TsolExtensionInit: AddExtension failed for X Trusted Extensions\n");
		return;
	}

	extEntry->secure = TRUE;
        TsolReqCode = (unsigned char) extEntry->base;
        tsolEventBase = extEntry->eventBase;

	if (!AddCallback(&ClientStateCallback, TsolClientStateCallback, NULL))
		return;

	/* Allocate the client private index */
	tsolClientPrivateIndex = AllocateClientPrivateIndex();
	if (!AllocateClientPrivate(tsolClientPrivateIndex,
		   sizeof (TsolInfoRec))) {
		ErrorF("TsolExtensionInit: Cannot allocate client private.\n");
		return;
	}

	/* Allocate per screen window/pixmap private index */
	tsolWindowPrivateIndex = AllocateWindowPrivateIndex();
	tsolPixmapPrivateIndex = AllocatePixmapPrivateIndex();

	for (i = 0; i < screenInfo.numScreens; i++) {
		pScreen = screenInfo.screens[i];
		if (!AllocateWindowPrivate(pScreen, tsolWindowPrivateIndex,
			   sizeof (TsolResRec))) {
			ErrorF("TsolExtensionInit: Cannot allocate window private.\n");
            		return;
		}

		if (!AllocatePixmapPrivate(pScreen, tsolPixmapPrivateIndex,
                                   sizeof (TsolResRec))) {
			ErrorF("TsolExtensionInit: Cannot allocate pixmap private.\n");
			return;
		}
	}

	LoadTsolConfig();

	MakeTSOLAtoms();
	UpdateTsolNode();

	/* Save original Proc vectors */
	for (i = 0; i < PROCVECTORSIZE; i++) {
		TsolSavedProcVector[i] = ProcVector[i];
		TsolSavedSwappedProcVector[i] = SwappedProcVector[i];
	}

	/* Initialize security hooks */
	tsolSecHook.CheckAuthorization = TsolCheckAuthorization;
	tsolSecHook.ChangeWindowProperty = TsolChangeWindowProperty;
	tsolSecHook.CheckPropertyAccess = TsolCheckPropertyAccess;
	tsolSecHook.DeleteProperty = TsolDeleteProperty;
	tsolSecHook.InitWindow = TsolInitWindow;
	tsolSecHook.ProcessKeyboard = TsolProcessKeyboard;
	tsolSecHook.DeleteClientFromAnySelections = TsolDeleteClientFromAnySelections;
	tsolSecHook.DeleteWindowFromAnySelections = TsolDeleteWindowFromAnySelections;
	tsolSecHook.AuditStart = TsolAuditStart;
	tsolSecHook.AuditEnd = TsolAuditEnd;
	pSecHook = &tsolSecHook;

	/* Replace some of the original Proc vectors with our own TBD */
	ProcVector[X_InternAtom] = ProcTsolInternAtom;
	ProcVector[X_SetSelectionOwner] = ProcTsolSetSelectionOwner;
	ProcVector[X_GetSelectionOwner] = ProcTsolGetSelectionOwner;
	ProcVector[X_ConvertSelection] = ProcTsolConvertSelection;
	ProcVector[X_GetProperty] = ProcTsolGetProperty;
	ProcVector[X_ListProperties] = ProcTsolListProperties;
	ProcVector[X_ChangeKeyboardMapping] = ProcTsolChangeKeyboardMapping;
	ProcVector[X_SetPointerMapping] = ProcTsolSetPointerMapping;
	ProcVector[X_ChangeKeyboardControl] = ProcTsolChangeKeyboardControl;
	ProcVector[X_Bell] = ProcTsolBell;
	ProcVector[X_ChangePointerControl] = ProcTsolChangePointerControl;
	ProcVector[X_SetModifierMapping] = ProcTsolSetModifierMapping;

	ProcVector[X_CreateWindow] = ProcTsolCreateWindow;
	ProcVector[X_ChangeWindowAttributes] = ProcTsolChangeWindowAttributes;
	ProcVector[X_ConfigureWindow] = ProcTsolConfigureWindow;
	ProcVector[X_CirculateWindow] = ProcTsolCirculateWindow;
	ProcVector[X_ReparentWindow] = ProcTsolReparentWindow;
	ProcVector[X_SetInputFocus] = ProcTsolSetInputFocus;
	ProcVector[X_GetInputFocus] = ProcTsolGetInputFocus;
	ProcVector[X_SendEvent] = ProcTsolSendEvent;
	ProcVector[X_SetInputFocus] = ProcTsolSetInputFocus;
	ProcVector[X_GetInputFocus] = ProcTsolGetInputFocus;
	ProcVector[X_GetGeometry] = ProcTsolGetGeometry;
	ProcVector[X_GrabServer] = ProcTsolGrabServer;
	ProcVector[X_UngrabServer] = ProcTsolUngrabServer;
	ProcVector[X_CreatePixmap] = ProcTsolCreatePixmap;
	ProcVector[X_SetScreenSaver] = ProcTsolSetScreenSaver;
	ProcVector[X_ChangeHosts] = ProcTsolChangeHosts;
	ProcVector[X_SetAccessControl] = ProcTsolChangeAccessControl;
	ProcVector[X_KillClient] = ProcTsolKillClient;
	ProcVector[X_SetFontPath] = ProcTsolSetFontPath;
	ProcVector[X_SetCloseDownMode] = ProcTsolChangeCloseDownMode;
	ProcVector[X_ListInstalledColormaps] = ProcTsolListInstalledColormaps;
	ProcVector[X_GetImage] = ProcTsolGetImage;
	ProcVector[X_QueryTree] = ProcTsolQueryTree;
	ProcVector[X_QueryPointer] = ProcTsolQueryPointer;
	ProcVector[X_QueryExtension] = ProcTsolQueryExtension;
	ProcVector[X_ListExtensions] = ProcTsolListExtensions;
	ProcVector[X_MapWindow] = ProcTsolMapWindow;
	ProcVector[X_MapSubwindows] = ProcTsolMapSubwindows;
	ProcVector[X_CopyArea] = ProcTsolCopyArea;
	ProcVector[X_CopyPlane] = ProcTsolCopyPlane;
	ProcVector[X_PolySegment] = ProcTsolPolySegment;
	ProcVector[X_PolyRectangle] = ProcTsolPolyRectangle;

}

static pointer
TsolCheckResourceIDAccess(
    ClientPtr client,
    XID id,
    RESTYPE rtype,
    Mask access_mode,
    pointer rval)
{
    int cid = CLIENT_ID(id);
    int reqtype = ((xReq *)client->requestBuffer)->reqType;  /* protocol */
    pointer retval;
    char msgbuf[1024];


    retval = rval;

    switch (rtype) {
	case RT_GC:
		switch (access_mode) {
		case SecurityReadAccess:
		    if (xtsol_policy(TSOL_RES_GC, TSOL_READ, (void *)id,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;

		case SecurityWriteAccess:
		    if (xtsol_policy(TSOL_RES_GC, TSOL_MODIFY, (void *)id,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;

		case SecurityDestroyAccess:
		    if (xtsol_policy(TSOL_RES_GC, TSOL_DESTROY, (void *)id,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;
		}
		break;

	case RT_WINDOW:		/* Drawables */
	case RT_PIXMAP:
	    /* Drawing operations use pixel access policy */
	    switch (reqtype) {
		case X_PolyPoint:
		case X_PolyLine:
		case X_PolyArc:
		case X_FillPoly:
		case X_PolyFillRectangle:
		case X_PolyFillArc:
		case X_PutImage:
		case X_PolyText8:
		case X_PolyText16:
		case X_ImageText8:
		case X_ImageText16:
		switch (access_mode) {
		case SecurityReadAccess:
		    if (xtsol_policy(TSOL_RES_PIXEL, TSOL_READ, (void *)rval,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;

		case SecurityWriteAccess:
		    if (xtsol_policy(TSOL_RES_PIXEL, TSOL_MODIFY, (void *)rval,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;
    		}
		break;

		/* Property protocols */
		case X_ChangeProperty:
		case X_DeleteProperty:
		case X_GetProperty:
		case X_ListProperties:
		case X_RotateProperties:
		switch (access_mode) {
		case SecurityReadAccess:
		    if (xtsol_policy(TSOL_RES_PROPWIN, TSOL_READ, (void *)rval,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;

		case SecurityWriteAccess:
		    if (xtsol_policy(TSOL_RES_PROPWIN, TSOL_MODIFY, (void *)rval,
			client, TSOL_ALL, (void *)MAJOROP))
				retval = NULL;
		    break;
    		}
		break;
		}
		break;
    }

    if (retval == NULL) {
    	TsolInfoPtr tsolinfo, res_tsolinfo;
    	tsolinfo = GetClientTsolInfo(client);

	snprintf(msgbuf, sizeof (msgbuf), 
	    "Access failed: cid = %d, rtype=%X, access=%d, xid=%X, proto = %d, pid = %d\n",
		cid, rtype, access_mode, id, reqtype, tsolinfo->pid);
	ErrorF(msgbuf);
    }

    return retval;
}

static void
TsolClientStateCallback(CallbackListPtr *pcbl,
	pointer nulldata,
	pointer calldata)
{
 	NewClientInfoRec *pci = (NewClientInfoRec *)calldata;
	ClientPtr client = pci->client;
	TsolInfoPtr tsolinfo = (TsolInfoPtr)
		(client->devPrivates[tsolClientPrivateIndex].ptr);

	switch (client->clientState) {

	case ClientStateInitial:
		/* Got a new connection */
		TsolSetClientInfo(client);
		client->CheckAccess = TsolCheckResourceIDAccess;
		break;

	case ClientStateRunning:
		break;

	case ClientStateRetained:	/* client disconnected */
		break;
	case ClientStateGone:
		if (tpwin && wClient(tpwin) == client)
		    tpwin = NULL; /* reset tpwin */

		if (tsolinfo != NULL && tsolinfo->privs != NULL) {
			priv_freeset(tsolinfo->privs);
		}
		/* Audit disconnect */
		if (system_audit_on && (au_preselect(AUE_ClientDisconnect, &(tsolinfo->amask),
                              AU_PRS_BOTH, AU_PRS_USECACHE) == 1)) {
			auditwrite(AW_PRESELECT, &(tsolinfo->amask),AW_END);
			auditwrite(AW_EVENTNUM, AUE_ClientDisconnect,
                               AW_XCLIENT, client->index,
			       AW_SLABEL, tsolinfo->sl,
                               AW_RETURN, 0, 0, AW_WRITE, AW_END);

			tsolinfo->flags &= ~TSOL_DOXAUDIT;
			tsolinfo->flags &= ~TSOL_AUDITEVENT;
			auditwrite(AW_FLUSH, AW_END);
			auditwrite(AW_DISCARDRD, tsolinfo->asaverd, AW_END);
			auditwrite(AW_NOPRESELECT, AW_END);
		}
		break;

	default:
                break;
	}

}

static void
TsolReset()
{
	free_win_privsets();
}

/*
 * Dispatch routine
 *
 */
static int
ProcTsolDispatch(client)
register ClientPtr client;
{
    int retval;

    REQUEST(xReq);

    switch (stuff->data)
    {
        case X_SetPolyInstInfo:
            retval =  ProcSetPolyInstInfo(client);
            break;
        case X_SetPropLabel:
            retval =  ProcSetPropLabel(client);
            break;
        case X_SetPropUID:
            retval =  ProcSetPropUID(client);
            break;
        case X_SetResLabel:
            retval =  ProcSetResLabel(client);
            break;
        case X_SetResUID:
            retval =  ProcSetResUID(client);
            break;
        case X_GetClientAttributes:
            retval =  ProcGetClientAttributes(client);
            break;
        case X_GetClientLabel:
            retval = ProcGetClientLabel(client);
            break;
        case X_GetPropAttributes:
            retval =  ProcGetPropAttributes(client);
            break;            
        case X_GetResAttributes:
            retval =  ProcGetResAttributes(client);
            break;
        case X_MakeTPWindow:
            retval =  ProcMakeTPWindow(client);
            break;
        case X_MakeTrustedWindow:
            retval =  ProcMakeTrustedWindow(client);
            break;
        case X_MakeUntrustedWindow:
            retval =  ProcMakeUntrustedWindow(client);
            break;
        default:
            SendErrorToClient(client, TsolReqCode, stuff->data, 0, BadRequest);
            retval = BadRequest;
    }
    return (retval);
}


static int
SProcTsolDispatch(client)
register ClientPtr client;
{
    int n;
    int retval;

    REQUEST(xReq);

    swaps(&stuff->length, n);
    switch (stuff->data)
    {
        case X_SetPolyInstInfo:
            retval =  SProcSetPolyInstInfo(client);
            break;
        case X_SetPropLabel:
            retval =  SProcSetPropLabel(client);
            break;
        case X_SetPropUID:
            retval =  SProcSetPropUID(client);
            break;
        case X_SetResLabel:
            retval =  SProcSetResLabel(client);
            break;
        case X_SetResUID:
            retval =  SProcSetResUID(client);
            break;
        case X_GetClientAttributes:
            retval =  SProcGetClientAttributes(client);
            break;
        case X_GetClientLabel:
            retval = SProcGetClientLabel(client);
            break;
        case X_GetPropAttributes:
            retval =  SProcGetPropAttributes(client);
            break;            
        case X_GetResAttributes:
            retval =  SProcGetResAttributes(client);
            break;
        case X_MakeTPWindow:
            retval =  SProcMakeTPWindow(client);
            break;
        case X_MakeTrustedWindow:
            retval =  SProcMakeTrustedWindow(client);
            break;
        case X_MakeUntrustedWindow:
            retval =  SProcMakeUntrustedWindow(client);
            break;
        default:
            SendErrorToClient(client, TsolReqCode, stuff->data, 0, BadRequest);
            retval = BadRequest;
    }
    return (retval);
}


/*
 * Individual routines
 */

static int
SProcSetPolyInstInfo(ClientPtr client)
{
    int n;

    REQUEST(xSetPolyInstInfoReq);
    swapl(&stuff->uid, n);
    swapl(&stuff->enabled, n);
    swaps(&stuff->sllength, n);

    return (ProcSetPolyInstInfo(client));
}

static int
SProcSetPropLabel(ClientPtr client)
{
    int n;

    REQUEST(xSetPropLabelReq);
    swapl(&stuff->id, n);
    swapl(&stuff->atom, n);
    swaps(&stuff->labelType, n);
    swaps(&stuff->sllength, n);
    swaps(&stuff->illength, n);

    return (ProcSetPropLabel(client));
}

static int
SProcSetPropUID(ClientPtr client)
{
    int n;

    REQUEST(xSetPropUIDReq);
    swapl(&stuff->id, n);
    swapl(&stuff->atom, n);
    swapl(&stuff->uid, n);

    return (ProcSetPropUID(client));
}

static int
SProcSetResLabel(ClientPtr client)
{
    int n;

    REQUEST(xSetResLabelReq);
    swapl(&stuff->id, n);
    swaps(&stuff->resourceType, n);
    swaps(&stuff->labelType, n);
    swaps(&stuff->sllength, n);
    swaps(&stuff->illength, n);

    return (ProcSetResLabel(client));
}

static int
SProcSetResUID(ClientPtr client)
{
    int n;

    REQUEST(xSetResUIDReq);
    swapl(&stuff->id, n);
    swaps(&stuff->resourceType, n);
    swapl(&stuff->uid, n);

    return (ProcSetResUID(client));
}

static int
SProcGetClientAttributes(ClientPtr client)
{
    int n;

    REQUEST(xGetClientAttributesReq);
    swapl(&stuff->id, n);

    return (ProcGetClientAttributes(client));
}

static int
SProcGetClientLabel(ClientPtr client)
{
    int n;

    REQUEST(xGetClientLabelReq);
    swapl(&stuff->id, n);
    swaps(&stuff->mask, n);

    return (ProcGetClientLabel(client));
}

static int
SProcGetPropAttributes(ClientPtr client)
{
    int n;

    REQUEST(xGetPropAttributesReq);
    swapl(&stuff->id, n);
    swapl(&stuff->atom, n);
    swaps(&stuff->mask, n);

    return (ProcGetPropAttributes(client));
}

static int
SProcGetResAttributes(ClientPtr client)
{
    int n;

    REQUEST(xGetResAttributesReq);
    swapl(&stuff->id, n);
    swaps(&stuff->resourceType, n);
    swaps(&stuff->mask, n);

    return (ProcGetResAttributes(client));
}

static int
SProcMakeTPWindow(ClientPtr client)
{
    int n;

    REQUEST(xMakeTPWindowReq);
    swapl(&stuff->id, n);

    return (ProcMakeTPWindow(client));
}

static int
SProcMakeTrustedWindow(ClientPtr client)
{
    int n;

    REQUEST(xMakeTrustedWindowReq);
    swapl(&stuff->id, n);

    return (ProcMakeTrustedWindow(client));
}

static int
SProcMakeUntrustedWindow(ClientPtr client)
{
    int n;

    REQUEST(xMakeUntrustedWindowReq);
    swapl(&stuff->id, n);

    return (ProcMakeUntrustedWindow(client));
}

/*
 * Set PolyInstantiation Info. 
 * Normally a get(prop) will
 * get the prop. that has match sl, uid of the client. Setting
 * enabled to true will get only the prop. corresponding to
 * sl, uid specified instead of that of client. This is used
 * by dtwm/dtfile in special motif lib. 
 */
static int
ProcSetPolyInstInfo(ClientPtr client)
{
    bslabel_t *sl;
    int        err_code;

    REQUEST(xSetPolyInstInfoReq);
    REQUEST_AT_LEAST_SIZE(xSetPolyInstInfoReq);

    /*
     * Check for policy here
     */
    if (err_code = xtsol_policy(TSOL_RES_POLYINFO, TSOL_MODIFY, NULL,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    sl = (bslabel_t *)(stuff + 1);

    tsolpolyinstinfo.enabled = stuff->enabled;
    tsolpolyinstinfo.uid = (uid_t) stuff->uid;
    tsolpolyinstinfo.sl = lookupSL(sl);

    return (client->noClientException);
}

static int
ProcSetPropLabel(ClientPtr client)
{
    bslabel_t   *sl;
    WindowPtr    pWin;
    TsolPropPtr  tsolprop;
    PropertyPtr  pProp;
    int          err_code;
    
    REQUEST(xSetPropLabelReq);

    REQUEST_AT_LEAST_SIZE(xSetPropLabelReq);


    pWin = LookupWindow(stuff->id, client);
    if (!pWin)
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_WINDOW, TSOL_MODIFY, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    if (!ValidAtom(stuff->atom))
    {
        client->errorValue = stuff->atom;
        return (BadAtom);
    }

    /* first see if property already exists */
    pProp = wUserProps (pWin);
    while (pProp)
    {
        if (pProp->propertyName == stuff->atom)
            break;
        pProp = pProp->next;
    }

    if (!pProp)
    {
        /* property does not exist */
        client->errorValue = stuff->atom;
        return (BadAtom);
    }

    /* Initialize property created internally by server */
    if (pProp->secPrivate == NULL)
    {
        pProp->secPrivate = (pointer)AllocServerTsolProp();
        if (pProp->secPrivate == NULL)
	    return(BadAlloc);
    }

    tsolprop = (TsolPropPtr)(pProp->secPrivate);

    sl = (bslabel_t *)(stuff + 1);

    if (!blequal(tsolprop->sl, sl))
    {
	if (err_code = xtsol_policy(TSOL_RES_SL, TSOL_MODIFY, sl,
				    client, TSOL_ALL, tsolprop->sl))
	{
	    return (err_code);
	}
	tsolprop->sl = lookupSL(sl);
    }
    
    return (client->noClientException);
}

static int
ProcSetPropUID(ClientPtr client)
{
    WindowPtr   pWin;
    TsolPropPtr tsolprop;
    PropertyPtr pProp;
    int         err_code;

    REQUEST(xSetPropUIDReq);

    REQUEST_SIZE_MATCH(xSetPropUIDReq);
    
    pWin = LookupWindow(stuff->id, client);
    if (!pWin)
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_WINDOW, TSOL_MODIFY, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    if (!ValidAtom(stuff->atom))
    {
        client->errorValue = stuff->atom;
        return (BadAtom);
    }

    /* first see if property already exists */
    pProp = wUserProps (pWin);
    while (pProp)
    {
        if (pProp->propertyName == stuff->atom)
            break;
        pProp = pProp->next;
    }

    if (!pProp)
    {
        /* property does not exist */
        client->errorValue = stuff->atom;
        return (BadAtom);
    }
    if (err_code = xtsol_policy(TSOL_RES_UID, TSOL_MODIFY, NULL,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    /* Initialize property created internally by server */
    if (pProp->secPrivate == NULL)
    {
        pProp->secPrivate = (pointer)AllocServerTsolProp();
        if (pProp->secPrivate == NULL)
	    return(BadAlloc);
    }

    tsolprop = (TsolPropPtr)(pProp->secPrivate);
    tsolprop->uid = stuff->uid;

    return (client->noClientException);
}

static int
ProcSetResLabel(ClientPtr client)
{
    bslabel_t  *sl;
    PixmapPtr   pMap;
    WindowPtr   pWin;
    xEvent      message;
    TsolResPtr  tsolres;
    DrawablePtr pDraw;
    int         err_code;
    
    REQUEST(xSetResLabelReq);

    REQUEST_AT_LEAST_SIZE(xSetResLabelReq);

    sl = (bslabel_t *)(stuff + 1);
    switch (stuff->resourceType)
    {
        case SESSIONHI: /* set server session HI */
            if (err_code = xtsol_policy(TSOL_RES_SL, TSOL_MODIFY, sl,
                                        client, TSOL_ALL, (void *)NULL))
            {
                return (err_code);
            }
            memcpy(&SessionHI, sl, SL_SIZE);
            return (client->noClientException);
        case SESSIONLO: /* set server session LO */
            if (err_code = xtsol_policy(TSOL_RES_SL, TSOL_MODIFY, sl,
                                        client, TSOL_ALL, (void *)NULL))
            {
                return (err_code);
            }
            memcpy(&SessionLO, sl, SL_SIZE);
            return (client->noClientException);
        case IsWindow:
            pWin = LookupWindow(stuff->id, client);
            if (pWin)
            {
                tsolres =
                    (TsolResPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);
            }
            else
            {
                client->errorValue = stuff->id;
                return (BadWindow);
            }
            if (err_code = xtsol_policy(TSOL_RES_WINDOW, TSOL_MODIFY, pWin,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                return (err_code);
            }
            break;
        case IsPixmap:
            pMap = (PixmapPtr)LookupIDByType(stuff->id, RT_PIXMAP);
            if (pMap)
            {
                tsolres =
                    (TsolResPtr)(pMap->devPrivates[tsolPixmapPrivateIndex].ptr);
            }
            else
            {
                client->errorValue = stuff->id;
                return (BadPixmap);
            }
            if (err_code = xtsol_policy(TSOL_RES_PIXMAP, TSOL_MODIFY, pMap,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                return (err_code);
            }
            break;
    }

    if (!blequal(tsolres->sl, sl))
    {
	if (err_code = xtsol_policy(TSOL_RES_SL, TSOL_MODIFY, sl,
				    client, TSOL_ALL, tsolres->sl))
	{
	    return (err_code);
	}
	tsolres->sl = lookupSL(sl);
    }

    /* generate the notify event for windows */

    if (stuff->resourceType == IsWindow)
    {
        pWin = LookupWindow(stuff->id, client);
        message.u.u.type = ClientMessage; /* 33 */
        message.u.u.detail = 32;
        message.u.clientMessage.window = RootOf(pWin);
        message.u.clientMessage.u.l.type =
            MakeAtom("_TSOL_CMWLABEL_CHANGE", 21, 1);
        message.u.clientMessage.u.l.longs0 = RootOfClient(pWin);
        message.u.clientMessage.u.l.longs1 = stuff->id;
        DeliverEventsToWindow(pWin, &message, 1,
                              SubstructureRedirectMask, NullGrab, 0);

    }
    return (client->noClientException);
}

static int
ProcSetResUID(ClientPtr client)
{
    int       ScreenNumber;
    PixmapPtr pMap;
    WindowPtr pWin;
    TsolResPtr tsolres;
    int        err_code;

    REQUEST(xSetResUIDReq);

    REQUEST_SIZE_MATCH(xSetResUIDReq);

    switch (stuff->resourceType)
    {
        case STRIPEHEIGHT:
            if (err_code = xtsol_policy(TSOL_RES_STRIPE, TSOL_MODIFY, NULL,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                return (err_code);
            }
            StripeHeight = stuff->uid;
            ScreenNumber = stuff->id;

        /* set Screen Stripe Size  */
        DoScreenStripeHeight(ScreenNumber);
        ScreenStripeHeight [ScreenNumber] = StripeHeight;

        return (client->noClientException);
        case RES_OUID:
            if (err_code = xtsol_policy(TSOL_RES_WOWNER, TSOL_MODIFY, NULL,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                return (err_code);
            }
            OwnerUID = stuff->uid;
            OwnerUIDint = OwnerUID;
            AddUID(&OwnerUIDint);
            return (client->noClientException);
        case IsWindow:
            pWin = LookupWindow(stuff->id, client);
            if (pWin)
            {
                tsolres =
                    (TsolResPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);
            }
            else
            {
                client->errorValue = stuff->id;
                return (BadWindow);
            }
            if (err_code = xtsol_policy(TSOL_RES_WINDOW, TSOL_MODIFY, pWin,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                return (err_code);
            }
            break;
        case IsPixmap:
            pMap = (PixmapPtr)LookupIDByType(stuff->id, RT_PIXMAP);
            if (pMap)
            {
                tsolres =
                    (TsolResPtr)(pMap->devPrivates[tsolPixmapPrivateIndex].ptr);
            }
            else
            {
                client->errorValue = stuff->id;
                return (BadPixmap);
            }
            if (err_code = xtsol_policy(TSOL_RES_PIXMAP, TSOL_MODIFY, pMap,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                return (err_code);
            }
            break;
        default:
            return (BadValue);
    }
    
    if (err_code = xtsol_policy(TSOL_RES_UID, TSOL_MODIFY, NULL,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    tsolres->uid = stuff->uid;

    return (client->noClientException);
}

static int
ProcGetClientAttributes(ClientPtr client)
{
    int         n;
    int         err_code;
    ClientPtr   res_client; /* resource owner client */
    TsolInfoPtr tsolinfo, res_tsolinfo;
    WindowPtr	pWin;

    xGetClientAttributesReply rep;
    
    REQUEST(xGetClientAttributesReq);
    REQUEST_SIZE_MATCH(xGetClientAttributesReq);

    /* Valid window check */
    /* Valid window check */
    if ((pWin = LookupWindow(stuff->id, client)) == NULL)
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }

    if (!(res_client = clients[CLIENT_ID(stuff->id)]))
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_CLIENT, TSOL_READ, (void *)stuff->id,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    tsolinfo = GetClientTsolInfo(client);
    res_tsolinfo = GetClientTsolInfo(res_client);

    /* Transfer the client info to reply rec */
    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.trustflag = (res_tsolinfo->forced_trust == 1 
	|| res_tsolinfo->trusted_path) ? (BYTE)1 : (BYTE)0;
    rep.uid = (CARD32) res_tsolinfo->uid;
    rep.pid = (CARD32) res_tsolinfo->pid;
    rep.gid = (CARD32) res_tsolinfo->gid;
    rep.auditid = (CARD32) res_tsolinfo->auid;
    rep.sessionid = (CARD32) res_tsolinfo->asid;
    rep.iaddr = (CARD32) res_tsolinfo->iaddr;
    rep.length = (CARD32) 0;

    if (client->swapped)
    {
        swaps(&rep.sequenceNumber, n);
        swapl(&rep.length, n);
        swapl(&rep.uid, n);
        swapl(&rep.pid, n);
        swapl(&rep.gid, n);
        swapl(&rep.auditid, n);
        swapl(&rep.sessionid, n);
        swapl(&rep.iaddr, n);
    }

    WriteToClient(client, sizeof(xGetClientAttributesReply), (char *)&rep);

    return (client->noClientException);
}

static int
ProcGetClientLabel(ClientPtr client)
{
    int         n;
    int         reply_length;
    int         err_code;
    Bool        write_to_client = 0;
    bslabel_t   *sl;
    ClientPtr   res_client; /* resource owner client */
    TsolInfoPtr tsolinfo, res_tsolinfo;
    WindowPtr	pWin;

    xGenericReply rep;

    REQUEST(xGetClientLabelReq);
    REQUEST_SIZE_MATCH(xGetClientLabelReq);

    /* Valid window check */
    if ((pWin = LookupWindow(stuff->id, client)) == NULL)
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }

    if (!(res_client = clients[CLIENT_ID(stuff->id)]))
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_CLIENT, TSOL_READ, (void *)stuff->id,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    tsolinfo = GetClientTsolInfo(client);
    res_tsolinfo = GetClientTsolInfo(res_client);

    /* Transfer the client info to reply rec */
    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;

    /* allocate temp storage for labels */
    sl = (bslabel_t *)(ALLOCATE_LOCAL(SL_SIZE));
    rep.data00 = rep.data01 = 0;
    if (sl == NULL)
        return (BadAlloc);
    
    /* fill the fields as per request mask */
    if (stuff->mask & RES_SL)
    {
        memcpy(sl, res_tsolinfo->sl, SL_SIZE);
        rep.data00 = SL_SIZE;
    }
    
    rep.length = (CARD32)(rep.data00)/4;

    if (rep.length > 0)
    {
        reply_length = rep.length*4;
        write_to_client = 1;
    }
    if (client->swapped)
    {
        swaps(&rep.sequenceNumber, n);
        swapl(&rep.length, n);
        swapl(&rep.data00, n);
        swapl(&rep.data01, n);
    }

    WriteToClient(client, sizeof(xGenericReply), (char *)&rep);

    if (write_to_client == 1)
    {
        WriteToClient(client, reply_length, (char *)sl);
    }
    DEALLOCATE_LOCAL(sl);

    return (client->noClientException);
}

static int
ProcGetPropAttributes(ClientPtr client)
{
    int          n;
    int          reply_length;
    int          extralen;
    int          err_code;
    Bool         write_to_client = 0;
    PropertyPtr  pProp;
    bslabel_t   *sl;
    WindowPtr    pWin;
    TsolPropPtr  tsolprop, tmp_prop;
    TsolInfoPtr  tsolinfo = GetClientTsolInfo(client);

    xGetPropAttributesReply rep;

    REQUEST(xGetPropAttributesReq);

    REQUEST_SIZE_MATCH(xGetPropAttributesReq);

    pWin = LookupWindow(stuff->id, client);
    if (pWin)
    {
        tsolprop = (TsolPropPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);
    }
    else
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    if (!ValidAtom(stuff->atom))
    {
        client->errorValue = stuff->atom;
        return (BadAtom);
    }

    /* first see if property already exists */
    pProp = wUserProps (pWin);
    while (pProp)
    {
        if (pProp->propertyName == stuff->atom)
            break;
        pProp = pProp->next;
    }

    if (!pProp)
    {
        /* property does not exist */
        client->errorValue = stuff->atom;
        return (BadAtom);
    }
    tsolprop = (TsolPropPtr)(pProp->secPrivate);
    tmp_prop = tsolprop;
    while (tmp_prop)
    {
        if (tsolpolyinstinfo.enabled)
        {
            if (tmp_prop->uid == tsolpolyinstinfo.uid &&
                tmp_prop->sl == tsolpolyinstinfo.sl)
            {
                tsolprop = tmp_prop;
                break;
            }
        }
        else
        {
            if (tmp_prop->uid == tsolinfo->uid &&
                tmp_prop->sl == tsolinfo->sl)
            {
                tsolprop = tmp_prop;
                break;
            }
        }
        tmp_prop = tmp_prop->next;
    }
    if (!tsolprop)
    {
        return (client->noClientException);
    }
    if (stuff->mask & RES_UID)
    {
        rep.uid = tsolprop->uid;
    }

    /* allocate temp storage for labels */
    sl = (bslabel_t *)(ALLOCATE_LOCAL(SL_SIZE));
    rep.sllength = rep.illength = 0;
    if (sl == NULL)
        return (BadAlloc);
    
    /* fill the fields as per request mask */
    if (stuff->mask & RES_SL)
    {
        memcpy(sl, tsolprop->sl, SL_SIZE);
        rep.sllength = SL_SIZE;
    }

    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.length = (CARD32) (rep.sllength)/4;

    if (rep.length > 0)
    {
        reply_length = rep.length*4;
        write_to_client = 1;
    }
    if (client->swapped)
    {
        swaps(&rep.sequenceNumber, n);
        swapl(&rep.length, n);
        swapl(&rep.uid, n);
        swaps(&rep.sllength, n);
        swaps(&rep.illength, n);
    }

    WriteToClient(client, sizeof(xGetPropAttributesReply), (char *)&rep);

    if (write_to_client == 1)
    {
        WriteToClient(client, reply_length, (char *)sl);
    }
    DEALLOCATE_LOCAL(sl);

    return (client->noClientException);
}

static int
ProcGetResAttributes(ClientPtr client)
{
    int         n;
    int         reply_length;
    int         extralen;
    int         err_code;
    Bool        write_to_client = 0;
    bslabel_t  *sl;
    PixmapPtr   pMap;
    WindowPtr   pWin;
    TsolResPtr  tsolres;

    xGetResAttributesReply rep;

    REQUEST(xGetResAttributesReq);

    REQUEST_SIZE_MATCH(xGetResAttributesReq);

    if (stuff->mask & RES_STRIPE)
    {
        rep.uid = ScreenStripeHeight[stuff->id];
    }
    if (stuff->mask & RES_OUID)
    {
        rep.owneruid = OwnerUID;
    }
    if (stuff->resourceType == IsWindow && 
        (stuff->mask & (RES_UID | RES_SL )))
    {
        pWin = LookupWindow(stuff->id, client);
        if (pWin)
        {
            tsolres = (TsolResPtr)
                (pWin->devPrivates[tsolWindowPrivateIndex].ptr);
        }
        else
        {
            client->errorValue = stuff->id;
            return (BadWindow);
        }
        if (err_code = xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pWin,
                                    client, TSOL_ALL, (void *)MAJOROP))
        {
            return (err_code);
        }
    }
    if (stuff->resourceType == IsPixmap && 
        (stuff->mask & (RES_UID | RES_SL )))
    {
        pMap = (PixmapPtr)LookupIDByType(stuff->id, RT_PIXMAP);
        if (pMap)
        {
            tsolres = (TsolResPtr)
                (pMap->devPrivates[tsolPixmapPrivateIndex].ptr);
        }
        else
        {
            client->errorValue = stuff->id;
            return (BadPixmap);
        }
        if (err_code = xtsol_policy(TSOL_RES_PIXMAP, TSOL_READ, pMap,
                                    client, TSOL_ALL, (void *)MAJOROP))
        {
            return (err_code);
        }
    }

    if (stuff->mask & RES_UID)
    {
        rep.uid = tsolres->uid;
    }

    /* allocate temp storage for labels */
    sl = (bslabel_t *)(ALLOCATE_LOCAL(SL_SIZE));
    rep.sllength = rep.illength = rep.iillength = 0;
    if (sl == NULL)
        return (BadAlloc);
    
    /* fill the fields as per request mask */
    if (stuff->mask & RES_SL)
    {
        memcpy(sl, tsolres->sl, SL_SIZE);
        rep.sllength = SL_SIZE;
    }

    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.length = (CARD32) (rep.sllength)/4;

    if (rep.length > 0)
    {
        reply_length = rep.length*4;
        write_to_client = 1;
    }
    if (client->swapped)
    {
        swaps(&rep.sequenceNumber, n);
        swapl(&rep.length, n);
        swapl(&rep.uid, n);
        swapl(&rep.owneruid, n);
        swaps(&rep.sllength, n);
        swaps(&rep.illength, n);
        swaps(&rep.iillength, n);
    }

    WriteToClient(client, sizeof(xGetResAttributesReply), (char *)&rep);

    if (write_to_client == 1)
    {
            WriteToClient(client, reply_length, (char *)sl);
    }
    DEALLOCATE_LOCAL(sl);

    return (client->noClientException);
}

int
ProcMakeTPWindow(ClientPtr client)
{
    WindowPtr pWin, pParent;
    int       err_code;
    TsolInfoPtr  tsolinfo;
    extern void ReflectStackChange(WindowPtr, WindowPtr, VTKind);


    REQUEST(xMakeTPWindowReq);
    REQUEST_SIZE_MATCH(xMakeTPWindowReq);

    /*
     * Session type single-level? This is set by the 
     * label builder
     */
    tsolinfo = GetClientTsolInfo(client);
    if (tsolinfo && HasTrustedPath(tsolinfo) && 
		blequal(&SessionLO, &SessionHI) && stuff->id == 0) {
	tsolMultiLevel = FALSE;
	return (client->noClientException);
    }

#if defined(PANORAMIX) && !defined(IN_MODULE)
    if (!noPanoramiXExtension) 
    {
        PanoramiXWindow     *pPanoramiXWin = PanoramiXWinRoot;
        int         j;

        PANORAMIXFIND_ID(pPanoramiXWin, stuff->id);
        IF_RETURN(!pPanoramiXWin, BadWindow);

	FOR_NSCREENS_OR_ONCE(pPanoramiXWin, j)
	{
		pWin = LookupWindow(pPanoramiXWin->info[j].id, client);

		/* window should not be root but child of root */
		if (!pWin || (!pWin->parent))
		{
		    client->errorValue = stuff->id;
		    return (BadWindow);
		}
		if (err_code = xtsol_policy(TSOL_RES_TPWIN, TSOL_MODIFY, pWin,
					client, TSOL_ALL, (void *)MAJOROP))
		{
		    return (err_code);
		}

		pParent = pWin->parent;
		if (pParent->firstChild != pWin)
		{
		    tpwin = (WindowPtr)NULL;
		    ReflectStackChange(pWin, pParent->firstChild, VTStack);
		}
	}

    } else 
#endif
    {
	pWin = LookupWindow(stuff->id, client);

	/* window should not be root but child of root */
	if (!pWin || (!pWin->parent))
	{
            client->errorValue = stuff->id;
            return (BadWindow);
	}
	if (err_code = xtsol_policy(TSOL_RES_TPWIN, TSOL_MODIFY, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
	{
            return (err_code);
	}

	pParent = pWin->parent;
	if (pParent->firstChild != pWin)
	{
	    tpwin = (WindowPtr)NULL;
	    ReflectStackChange(pWin, pParent->firstChild, VTStack);
	}
    }

    tpwin = pWin;

    /*
     * Force kbd & ptr ungrab. This will cause
     * screen to lock even when kbd/ptr grabbed by
     * a client
     */
    BreakAllGrabs(client);
    return (client->noClientException);
}

/*
 * Turn on window's Trusted bit
 */
static int
ProcMakeTrustedWindow(ClientPtr client)
{
    WindowPtr    pWin;
    int          err_code;
    TsolInfoPtr  tsolinfo;

    REQUEST(xMakeTrustedWindowReq);
    REQUEST_SIZE_MATCH(xMakeTrustedWindowReq);

    pWin = LookupWindow(stuff->id, client);

    /* window should not be root but child of root */
    if (!pWin || (!pWin->parent))
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_TPWIN, TSOL_MODIFY, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    tsolinfo = GetClientTsolInfo(client);    
    /* Turn on Trusted bit of the window */
    tsolinfo->forced_trust = 1;
    return (client->noClientException);
}

/*
 * Turn off window's Trusted bit
 */
static int
ProcMakeUntrustedWindow(ClientPtr client)
{
    WindowPtr    pWin;
    int          err_code;
    TsolInfoPtr  tsolinfo;

    REQUEST(xMakeUntrustedWindowReq);
    REQUEST_SIZE_MATCH(xMakeUntrustedWindowReq);

    pWin = LookupWindow(stuff->id, client);

    /* window should not be root but child of root */
    if (!pWin || (!pWin->parent))
    {
        client->errorValue = stuff->id;
        return (BadWindow);
    }
    if (err_code = xtsol_policy(TSOL_RES_TPWIN, TSOL_MODIFY, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        return (err_code);
    }
    tsolinfo = GetClientTsolInfo(client);    
    tsolinfo->forced_trust = 0;
    tsolinfo->trusted_path = FALSE;
    
    return (client->noClientException);
}

/*
 * Break keyboard & ptr grabs of clients other than
 * the requesting client.
 * Called from ProcMakeTPWindow.
 */
static void
BreakAllGrabs(ClientPtr client)
{
    ClientPtr	    grabclient;
    DeviceIntPtr    keybd = inputInfo.keyboard;
    GrabPtr         kbdgrab = keybd->grab;
    DeviceIntPtr    mouse = inputInfo.pointer;
    GrabPtr         ptrgrab = mouse->grab;

	if (kbdgrab) {
	    	grabclient = clients[CLIENT_ID(kbdgrab->resource)];
		if (client->index != grabclient->index)
			(*keybd->DeactivateGrab)(keybd);
	}

	if (ptrgrab) {
	    	grabclient = clients[CLIENT_ID(ptrgrab->resource)];
		if (client->index != grabclient->index)
			(*mouse->DeactivateGrab)(mouse);
        }
}

/*
 * Trusted Network interface module. Uses tsix API
 */

static void
TsolSetClientInfo(ClientPtr client)
{
	bslabel_t *sl;
	bslabel_t admin_low;
	priv_set_t *privs;
	const au_tid64_addr_t *tid64;
	const au_mask_t *amask;
	au_mask_t mask;		/* user audit mask */
	socklen_t namelen;
	struct passwd *pw;
	struct auditinfo auinfo;
	struct auditinfo *pauinfo;
	OsCommPtr oc = (OsCommPtr)client->osPrivate;
	register ConnectionInputPtr oci = oc->input;
	int fd = oc->fd;
	ucred_t *uc = NULL;
	extern  au_id_t ucred_getauid(const ucred_t *uc);
	extern  au_asid_t ucred_getasid(const ucred_t *uc);
	extern  const au_mask_t *ucred_getamask(const ucred_t *uc);

	TsolInfoPtr tsolinfo = (TsolInfoPtr)
		(client->devPrivates[tsolClientPrivateIndex].ptr);

	/* Get client attributes from the socket */
	if (getpeerucred(fd, &uc) == -1) {
		tsolinfo->uid = (uid_t)(-1);
		tsolinfo->sl = NULL;
		perror("getpeerucred failed\n");
		ErrorF("SUN_TSOL: Cannot get client attributes\n");
		return;
	}

	/* Extract individual fields from the cred structure */
	tsolinfo->zid = ucred_getzoneid(uc);
	tsolinfo->uid = ucred_getruid(uc);
	tsolinfo->euid = ucred_geteuid(uc);
	tsolinfo->gid = ucred_getrgid(uc);
	tsolinfo->egid = ucred_getegid(uc);
	tsolinfo->pid = ucred_getpid(uc);
	sl = ucred_getlabel(uc);
	tsolinfo->sl = (bslabel_t *)lookupSL(sl);

	/* Set privileges */
	if ((tsolinfo->privs = priv_allocset()) != NULL) {
		if (tsolMultiLevel) {
			privs = (priv_set_t *)ucred_getprivset(uc, PRIV_EFFECTIVE);
			if (privs == NULL) {
				priv_emptyset(tsolinfo->privs);
			} else {
				priv_copyset(privs, tsolinfo->privs);
			}
		} else {
			priv_fillset(tsolinfo->privs); 
		}
	}

	tsolinfo->priv_debug = FALSE;


	/* 
	 * For remote hosts, the uid is determined during access control
	 * using Secure RPC
	 */
	if (tsolinfo->zid == (zoneid_t)-1) {
		tsolinfo->client_type = CLIENT_REMOTE;
	} else {
		tsolinfo->client_type = CLIENT_LOCAL;
	}

	
	/* Set Trusted Path for local clients */
	if (tsolinfo->zid == GLOBAL_ZONEID) {
		tsolinfo->trusted_path = TRUE;
	}else {
		tsolinfo->trusted_path = FALSE;
	}

	if (tsolinfo->trusted_path || !tsolMultiLevel)
		client->trustLevel = XSecurityClientTrusted;
	else
		client->trustLevel = XSecurityClientUntrusted;

        tsolinfo->forced_trust = 0;
        tsolinfo->iaddr = 0;

	bsllow(&admin_low);

	/* Set reasonable defaults for remote clients */
	namelen = sizeof (tsolinfo->saddr);
	if (getpeername(fd, (struct sockaddr *)&tsolinfo->saddr, &namelen) == 0
	  && (tsolinfo->client_type == CLIENT_REMOTE)) {
		int errcode;
		char hostbuf[NI_MAXHOST];
		tsol_host_type_t host_type; 
		struct sockaddr sname;
		extern tsol_host_type_t tsol_getrhtype(char *);

		/* Use NI_NUMERICHOST to avoid DNS lookup */
		errcode = getnameinfo((struct sockaddr *)&(tsolinfo->saddr), namelen,
			hostbuf, sizeof(hostbuf), NULL, 0, NI_NUMERICHOST);

		if (errcode) {
			perror(gai_strerror(errcode));
		} else {
			host_type = tsol_getrhtype(hostbuf);
			if ((host_type == SUN_CIPSO) && 
				blequal(tsolinfo->sl, &admin_low)) {
				tsolinfo->trusted_path = TRUE;
				client->trustLevel = XSecurityClientTrusted;
				priv_fillset(tsolinfo->privs);
			}
		}
	}

	/* setup audit context */
	if (getaudit(&auinfo) == 0) {
	    pauinfo = &auinfo;
	} else {
	    pauinfo = NULL;
	}

	/* Audit id */
	tsolinfo->auid = ucred_getauid(uc);
	if (tsolinfo->auid == AU_NOAUDITID) {
	    tsolinfo->auid = UID_NOBODY;
	}

	/* session id */
	tsolinfo->asid = ucred_getasid(uc);

	/* Audit mask */
	if ((amask = ucred_getamask(uc)) != NULL) {
	    tsolinfo->amask = *amask;
	} else {
	    if (pauinfo != NULL) {
	        tsolinfo->amask = pauinfo->ai_mask;
	    } else {
	        tsolinfo->amask.am_failure = 0; /* clear the masks */
	        tsolinfo->amask.am_success = 0;
	    }
	}

	tsolinfo->asaverd = 0;

	ucred_free(uc);
}

static enum auth_stat tsol_why;

static char * 
tsol_authdes_decode(inmsg, len)
char *inmsg;
int  len;
{
    struct rpc_msg  msg;
    char            cred_area[MAX_AUTH_BYTES];
    char            verf_area[MAX_AUTH_BYTES];
    char            *temp_inmsg;
    struct svc_req  r;
    bool_t          res0, res1, auth_ret;
    XDR             xdr;
    SVCXPRT         xprt;
    extern bool_t xdr_opaque_auth(XDR *, struct opaque_auth *);

    temp_inmsg = (char *) xalloc(len);
    memmove(temp_inmsg, inmsg, len);

    memset((char *)&msg, 0, sizeof(msg));
    memset((char *)&r, 0, sizeof(r));
    memset(cred_area, 0, sizeof(cred_area));
    memset(verf_area, 0, sizeof(verf_area));

    msg.rm_call.cb_cred.oa_base = cred_area;
    msg.rm_call.cb_verf.oa_base = verf_area;
    tsol_why = AUTH_FAILED; 
    xdrmem_create(&xdr, temp_inmsg, len, XDR_DECODE);

    if ((r.rq_clntcred = (caddr_t) xalloc(MAX_AUTH_BYTES)) == NULL)
        goto bad1;
    r.rq_xprt = &xprt;

    /* decode into msg */
    res0 = xdr_opaque_auth(&xdr, &(msg.rm_call.cb_cred)); 
    res1 = xdr_opaque_auth(&xdr, &(msg.rm_call.cb_verf));
    if ( ! (res0 && res1) )
         goto bad2;

    /* do the authentication */

    r.rq_cred = msg.rm_call.cb_cred;        /* read by opaque stuff */
    if (r.rq_cred.oa_flavor != AUTH_DES) {
        tsol_why = AUTH_TOOWEAK;
        goto bad2;
    }
#ifdef SVR4
    if ((tsol_why = __authenticate(&r, &msg)) != AUTH_OK) {
#else
    if ((tsol_why = _authenticate(&r, &msg)) != AUTH_OK) {
#endif
            goto bad2;
    }
    return (((struct authdes_cred *) r.rq_clntcred)->adc_fullname.name); 

bad2:
    Xfree(r.rq_clntcred);
bad1:
    return ((char *)0); /* ((struct authdes_cred *) NULL); */
}
static Bool
TsolCheckNetName (unsigned char *addr, short len, pointer closure)
{
    return (len == (short) strlen ((char *) closure) &&
            strncmp ((char *) addr, (char *) closure, len) == 0);
}


XID
TsolCheckAuthorization(unsigned int name_length, char *name, unsigned int data_length, 
	char *data, ClientPtr client, char **reason)
{
	char	domainname[128];
	char	netname[128];
	char	audit_ret;
	u_int	audit_val;
	uid_t	client_uid;
	gid_t	client_gid;
	int	client_gidlen;
	char	*fullname;
	gid_t	client_gidlist;
	XID	auth_token = (XID)(-1);
	TsolInfoPtr tsolinfo = GetClientTsolInfo(client);
	extern	int getdomainname(char *, int);

	if (tsolinfo->uid == -1) {
		/* Retrieve uid from SecureRPC */
		if (strncmp(name, SECURE_RPC_AUTH, (size_t)name_length) == 0) {
			fullname = tsol_authdes_decode(data, data_length);
			if (fullname == NULL) {
				ErrorF("Unable to authenticate Secure RPC client");
			} else {
				if (netname2user(fullname, 
					&client_uid, &client_gid, 
					&client_gidlen, &client_gidlist)) {
					tsolinfo->uid = client_uid;
				} else {
					ErrorF("netname2user failed");
				}
			}
		}
	}
	 
	if (tsolinfo->uid == (uid_t)-1) {
		tsolinfo->uid = UID_NOBODY; /* uid not available */
	}

	/* 
	 * For multilevel desktop, limit connections to the trusted path
	 * i.e. global zone until a user logs in and the trusted stripe 
	 * is in place. Unlabeled connections are rejected.
	 */
	if ((OwnerUID == (uid_t )(-1)) || (tsolMultiLevel && tpwin == NULL)) {
		if (HasTrustedPath(tsolinfo)) {
			auth_token = CheckAuthorization(name_length, name, data_length,
				data, client, reason);
		}
	} else {
		/* 
		 * Workstation Owner set, client must be within label
		 * range or have trusted path
		 */
		if (tsolinfo->uid == OwnerUID) {
			if (tsolinfo->sl != NULL && 
					(bldominates(tsolinfo->sl, &SessionLO) &&
					bldominates(&SessionHI, tsolinfo->sl)) || 
					(HasTrustedPath(tsolinfo))) {
				auth_token = (XID)(tsolinfo->uid);
			}
		} else {
			/* Allow root from global zone */
			if (tsolinfo->uid == 0 && HasTrustedPath(tsolinfo)) {
				auth_token = (XID)(tsolinfo->uid);
			} else {
				/* 
				 * Access check based on uid. Check if
				 * roles or other uids have  been added by
				 * xhost +role@
				 */
				getdomainname(domainname, sizeof(domainname));
				if (!user2netname(netname, tsolinfo->uid, domainname)) {
					return ((XID)-1);
				}
				if (ForEachHostInFamily (FamilyNetname, TsolCheckNetName,
						(pointer) netname)) {
					return ((XID)(tsolinfo->uid));
				} else {
					return (CheckAuthorization(name_length, name, data_length,
						data, client, reason));
				}
			}
		}
	}

	/* Audit the connection */
	if (auth_token == (XID)(-1)) {
		audit_ret = (char )-1; /* failure */
		audit_val = 1;
	} else {
		audit_ret = 0; /* success */
		audit_val = 0;
	}

	if (system_audit_on &&
		(au_preselect(AUE_ClientConnect, &(tsolinfo->amask),
                      AU_PRS_BOTH, AU_PRS_USECACHE) == 1)) {
		int status;
		u_short connect_port = 0;
		struct in_addr *connect_addr = NULL;
		struct sockaddr_in *sin;
		struct sockaddr_in6 *sin6;

		switch (tsolinfo->saddr.ss_family) {
                        case AF_INET:
                                sin = (struct sockaddr_in *)&(tsolinfo->saddr);
                                connect_addr = &(sin->sin_addr);
                                connect_port = sin->sin_port;
                                break;
                        case AF_INET6:
                                sin6 = (struct sockaddr_in6 *)&(tsolinfo->saddr);
                                connect_addr = (struct in_addr *)&(sin6->sin6_addr);
                                connect_port = sin6->sin6_port;
                                break;
		}

		if (connect_addr == NULL || connect_port == 0) {
        		status = auditwrite(AW_EVENTNUM, AUE_ClientConnect,
				AW_XCLIENT, client->index,
				AW_SLABEL, tsolinfo->sl,
				AW_RETURN, audit_ret, audit_val,
				AW_WRITE, AW_END);
		} else {
        		status = auditwrite(AW_EVENTNUM, AUE_ClientConnect,
				AW_XCLIENT, client->index,
				AW_SLABEL, tsolinfo->sl,
				AW_INADDR, connect_addr,
				AW_IPORT, connect_port,
				AW_RETURN, audit_ret, audit_val,
				AW_WRITE, AW_END);
		}

		if (!status)
			(void) auditwrite(AW_FLUSH, AW_END);
		tsolinfo->flags &= ~TSOL_DOXAUDIT;
		tsolinfo->flags &= ~TSOL_AUDITEVENT;
	}

	return (auth_token);
}

static void
TsolProcessKeyboard(xEvent *xE, KeyClassPtr keyc)
{
    extern void InitHotKey(HotKeyPtr hk);
    extern void HandleHotKey();

    if (xE->u.u.type == KeyPress)
    {
	if (!hotkey.initialized)
	    InitHotKey(&hotkey);

        if (((xE->u.u.detail == hotkey.key) &&
		(keyc->state != 0 && keyc->state == hotkey.shift)) ||
            ((xE->u.u.detail == hotkey.altkey) &&
		(keyc->state != 0 && keyc->state == hotkey.altshift)))
            		HandleHotKey();
    }
}

static char
TsolCheckPropertyAccess(ClientPtr client, WindowPtr pWin, ATOM propertyName,
	Mask access_mode)
{
	char	action;
	PropertyPtr pProp;

        pProp = wUserProps (pWin);
        while (pProp)
        {
            if (pProp->propertyName == propertyName)
		break;
	    pProp = pProp->next;
        }

	if (pProp == NULL)
		return SecurityAllowOperation;

	if (access_mode & SecurityReadAccess) {
		if (!PolyProperty(propertyName, pWin) &&
		    xtsol_policy(TSOL_RES_PROPERTY, TSOL_READ,
			pProp, client, TSOL_ALL, (void *)MAJOROP))
		   return SecurityIgnoreOperation;
		else
		   return SecurityAllowOperation;
	}

	if (access_mode & SecurityWriteAccess) {
		if (!PolyProperty(propertyName, pWin) &&
		    xtsol_policy(TSOL_RES_PROPERTY, TSOL_MODIFY,
			pProp, client, TSOL_ALL, (void *)MAJOROP))
		   return SecurityIgnoreOperation;
		else
		   return SecurityAllowOperation;
	}

	if (access_mode & SecurityDestroyAccess) {
		if (!PolyProperty(propertyName, pWin) &&
		    xtsol_policy(TSOL_RES_PROPERTY, TSOL_DESTROY,
			pProp, client, TSOL_ALL, (void *)MAJOROP))
		   return SecurityIgnoreOperation;
		else
		   return SecurityAllowOperation;
	}

	return SecurityAllowOperation;
}

/*
 * Return TRUE if host is cipso
 */
int
host_is_cipso(int fd)
{
	struct sockaddr sname;
	socklen_t namelen;
	char *rhost;
	tsol_host_type_t host_type; 
	struct sockaddr_in *so = (struct sockaddr_in *)&sname;
	extern tsol_host_type_t tsol_getrhtype(char *);

	namelen = sizeof (sname);
	if (getpeername(fd, &sname, &namelen) == -1) {
		perror("getsockname: failed\n");
		return FALSE;
	}

	rhost = inet_ntoa(so->sin_addr);
	host_type = tsol_getrhtype(rhost);
	if (host_type == SUN_CIPSO) {
		return TRUE;
	}

	return FALSE;
}
