/* Copyright 2006 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident	"@(#)tsolprotocol.c 1.8	06/03/07 SMI"

#include <sys/param.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ucred.h>
#include <pwd.h>
#include <strings.h>
#include <sys/wait.h>
#include <bsm/auditwrite.h>
#include <bsm/libbsm.h>
#include <bsm/audit_uevents.h>
#include "tsol.h"

#include "inputstr.h"

#define NEED_REPLIES

#include "selection.h"
#include "osdep.h"
#include "tsolpolicy.h"
#include "swaprep.h"
#include "swapreq.h"
#include "servermd.h"
#ifdef PANORAMIX
#include "../Xext/panoramiXsrv.h"
#endif
#ifdef XCSECURITY
#define _SECURITY_SERVER
#include "security.h"
#endif

/*
 * The event # here match those in /usr/include/bsm/audit_uevents.h.
 * Changes in one should go with corresponding changes in another.
 */

#define MAX_AUDIT_EVENTS 100

int audit_eventsid[100][2] = {
    X_CreateWindow, 9103,
    X_ChangeWindowAttributes, 9104,
    X_GetWindowAttributes, 9105,
    X_DestroyWindow, 9106,
    X_DestroySubwindows, 9107,
    X_ChangeSaveSet, 9108,
    X_ReparentWindow, 9109,
    X_MapWindow, 9110,
    X_MapSubwindows, 9111,
    X_UnmapWindow, 9112, 
    X_UnmapSubwindows, 9113,
    X_ConfigureWindow, 9114,
    X_CirculateWindow, 9115,
    X_GetGeometry, 9116,
    X_QueryTree, 9117,
    X_InternAtom, 9118,
    X_GetAtomName, 9119,
    X_ChangeProperty, 9120,
    X_DeleteProperty, 9121,
    X_GetProperty, 9122,
    X_ListProperties, 9123,
    X_SetSelectionOwner, 9124,
    X_GetSelectionOwner, 9125,
    X_ConvertSelection, 9126,
    X_SendEvent, 9127,
    X_GrabPointer, 9128,
    X_UngrabPointer, 9129,
    X_GrabButton, 9130,
    X_UngrabButton, 9131,
    X_ChangeActivePointerGrab, 9132,
    X_GrabKeyboard, 9133,
    X_UngrabKeyboard, 9134,
    X_GrabKey, 9135,
    X_UngrabKey, 9136,
    X_GrabServer, 9137,
    X_UngrabServer, 9138,
    X_QueryPointer, 9139,
    X_GetMotionEvents, 9140,
    X_TranslateCoords, 9141,
    X_WarpPointer, 9142,
    X_SetInputFocus, 9143,
    X_GetInputFocus, 9144,
    X_QueryKeymap, 9145,
    X_SetFontPath, 9146,
    X_FreePixmap, 9147,
    X_ChangeGC, 9148,
    X_CopyGC, 9149,
    X_SetDashes, 9150,
    X_SetClipRectangles, 9151,
    X_FreeGC, 9152,
    X_ClearArea, 9153,
    X_CopyArea, 9154,
    X_CopyPlane, 9155,
    X_PolyPoint, 9156,
    X_PolyLine, 9157,
    X_PolySegment, 9158,
    X_PolyRectangle, 9159,
    X_PolyArc, 9160,
    X_FillPoly, 9161,
    X_PolyFillRectangle, 9162,
    X_PolyFillArc, 9163,
    X_PutImage, 9164,
    X_GetImage, 9165,
    X_PolyText8, 9166,
    X_PolyText16, 9167,
    X_ImageText8, 9168,
    X_ImageText16, 9169,
    X_CreateColormap, 9170,
    X_FreeColormap, 9171,
    X_CopyColormapAndFree, 9172,
    X_InstallColormap, 9173,
    X_UninstallColormap, 9174,
    X_ListInstalledColormaps, 9175,
    X_AllocColor, 9176,
    X_AllocNamedColor, 9177,
    X_AllocColorCells, 9178,
    X_AllocColorPlanes, 9179,
    X_FreeColors, 9180,
    X_StoreColors, 9181,
    X_StoreNamedColor, 9182,
    X_QueryColors, 9183,
    X_LookupColor, 9184,
    X_CreateCursor, 9185,
    X_CreateGlyphCursor, 9186,
    X_FreeCursor, 9187,
    X_RecolorCursor, 9188,
    X_ChangeKeyboardMapping, 9189,
    X_ChangeKeyboardControl, 9190,
    X_Bell, 9191,
    X_ChangePointerControl, 9192,
    X_SetScreenSaver, 9193,
    X_ChangeHosts, 9194,
    X_SetAccessControl, 9195,
    X_SetCloseDownMode, 9196,
    X_KillClient, 9197,
    X_RotateProperties, 9198,
    X_ForceScreenSaver, 9199,
    X_SetPointerMapping, 9200,
    X_SetModifierMapping, 9201,
    X_NoOperation, 9202
};
extern void Swap32Write();
extern int (*TsolSavedProcVector[PROCVECTORSIZE])(ClientPtr /*client*/);
extern int (*TsolSavedSwappedProcVector[PROCVECTORSIZE])(ClientPtr /*client*/);


Atom MakeTSOLAtom(ClientPtr client, char *string, unsigned int len, Bool makeit);

#define INITIAL_TSOL_NODELENGTH 1500

extern Atom tsol_lastAtom;
extern int tsol_nodelength;
extern TsolNodePtr tsol_node;
extern int NumCurrentSelections;
extern Selection *CurrentSelections;
extern WindowPtr tpwin;

static int tsol_sel_agnt = -1; /* index this to CurrentSelection to get seln */

/*
 * Get number of atoms defined in the system
 */
static Atom
GetLastAtom()
{
	Atom a = (Atom) 1; /* atoms start at 1 */

	while (ValidAtom(a)) {
		a++;
	}

	return (--a);
}

/*
 * Update Tsol info for atoms. This function gets
 * called typically during initialization. But, it could also get
 * called if some atoms are created internally by server.
 */
void
UpdateTsolNode()
{
	Atom lastAtom = GetLastAtom();
	Atom ia;

	/* Update may not be needed */
	if (lastAtom == None || lastAtom == tsol_lastAtom)
		return;
	
	if (tsol_node == NULL) {
		int newsize = (lastAtom > INITIAL_TSOL_NODELENGTH ? lastAtom : INITIAL_TSOL_NODELENGTH);

		/* Initialize */
		tsol_node = (TsolNodePtr )xalloc((newsize + 1) * sizeof(TsolNodeRec));
		tsol_nodelength = newsize;

		if (tsol_node != NULL) {
			/* Atom id 0 is invalid */
			tsol_lastAtom = 0;
			tsol_node[0].flags = 0;
			tsol_node[0].slcount = 0;
			tsol_node[0].sl = NULL;
			tsol_node[0].slsize = 0;
			tsol_node[0].IsSpecial = 0;
		}
	}

	if (tsol_nodelength <= lastAtom) {
		tsol_node = (TsolNodePtr )xrealloc(tsol_node, (lastAtom + 1) * sizeof(TsolNodeRec));
		tsol_nodelength = lastAtom + 1;
	}

	if (tsol_node == NULL) {
		ErrorF("Cannot allocate memory for Tsol node\n");
		return;
	}

	/* 
	 * Initialize the tsol node for each atom
	 */
	for (ia = tsol_lastAtom + 1; ia <= lastAtom; ia++) {
		char *atomname = NameForAtom(ia);

		tsol_node[ia].slcount = 0;
		tsol_node[ia].sl = NULL;
		tsol_node[ia].slsize= 0;
		tsol_node[ia].flags = MatchTsolConfig(atomname, strlen(atomname));
		tsol_node[ia].IsSpecial = SpecialName(atomname, strlen(atomname));

	}
	tsol_lastAtom = lastAtom;
}

int
ProcTsolInternAtom(client)
    register ClientPtr client;
{
    Atom atom;
    char *tchar;
    REQUEST(xInternAtomReq);

    REQUEST_FIXED_SIZE(xInternAtomReq, stuff->nbytes);
    if ((stuff->onlyIfExists != xTrue) && (stuff->onlyIfExists != xFalse))
    {
	client->errorValue = stuff->onlyIfExists;
        return(BadValue);
    }
    tchar = (char *) &stuff[1];
    atom = MakeTSOLAtom(client, tchar, stuff->nbytes, !stuff->onlyIfExists);
    if (atom != BAD_RESOURCE)
    {
	xInternAtomReply reply;
	reply.type = X_Reply;
	reply.length = 0;
	reply.sequenceNumber = client->sequence;
	reply.atom = atom;
	WriteReplyToClient(client, sizeof(xInternAtomReply), &reply);
	return(client->noClientException);
    }
    else
	return (BadAlloc);
}

int
ProcTsolGetAtomName(client)
    register ClientPtr client;
{
    char *str;
    xGetAtomNameReply reply;
    int len;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    /* TBD: NameForTSOLAtom */
    if ( (str = NameForAtom(stuff->id)) )
    {
	len = strlen(str);
	reply.type = X_Reply;
	reply.length = (len + 3) >> 2;
	reply.sequenceNumber = client->sequence;
	reply.nameLength = len;
	WriteReplyToClient(client, sizeof(xGetAtomNameReply), &reply);
	(void)WriteToClient(client, len, str);
	return(client->noClientException);
    }
    else 
    { 
	client->errorValue = stuff->id;
	return (BadAtom);
    }
}

Atom
MakeTSOLAtom(ClientPtr client, char *string, unsigned int len, Bool makeit)
{
	TsolNodePtr tndp;
	int count;
	int k;
	int newsize;
	Atom lastAtom;
	Atom newAtom;
	bslabel_t **newsl;

	TsolInfoPtr tsolinfo;
	bslabel_t *sl;


	/* Make the atom as usual */
	newAtom = MakeAtom(string, len, makeit);
	if (newAtom == None || newAtom == BAD_RESOURCE) {
		return (newAtom);
	}

	tsolinfo = GetClientTsolInfo(client);


	/* tsol node info already present? */
	if (newAtom <= tsol_lastAtom) {
		tndp = &(tsol_node[newAtom]);

		/* public atoms have null sl */
		if (tndp->sl == NULL) {
			return newAtom;
		}

		/* private atoms must have a matching sl */
		for (k = 0; k < tndp->slcount; k++) {
			if (tsolinfo->sl == tndp->sl[k]) {
				return newAtom; /* found one */
			}
		}

	} else {
		/* tsol node table not big enough, expand it */
		UpdateTsolNode();
		tndp = &(tsol_node[newAtom]);
	}

	/* Allocate storage for sl if needed */
	if (tndp->sl == NULL) {
		tndp->sl = (bslabel_t **)xalloc(NODE_SLSIZE * (sizeof(bslabel_t *)));
		tndp->slcount = 0;
		tndp->slsize = NODE_SLSIZE;
	}

	/* Expand storage space for sl if needed */
	if (tndp->slsize < tndp->slcount) {
		newsize = tndp->slsize + NODE_SLSIZE;
		tndp->sl = (bslabel_t **)xrealloc(tndp->sl, newsize * (sizeof(bslabel_t *)));
		tndp->slsize = newsize;
	}

	if (tndp->sl == NULL) {
		ErrorF("Not enough memory for atoms\n");
		return (Atom)None;
	}

	/* Store client's sl */
	tndp->sl[tndp->slcount] = tsolinfo->sl;
	tndp->slcount++;

	return newAtom;
}


int
ProcTsolSetSelectionOwner(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    TimeStamp time;
    REQUEST(xSetSelectionOwnerReq);

#ifdef TSOL
    TsolSelnPtr tsolseln = NULL;
    TsolSelnPtr prevtsolseln = NULL;
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);
#endif /* TSOL */

    REQUEST_SIZE_MATCH(xSetSelectionOwnerReq);
    UpdateCurrentTime();
    time = ClientTimeToServerTime(stuff->time);

    /* If the client's time stamp is in the future relative to the server's
	time stamp, do not set the selection, just return success. */
    if (CompareTimeStamps(time, currentTime) == LATER)
    	return Success;
    if (stuff->window != None)
    {
        pWin = (WindowPtr)SecurityLookupWindow(stuff->window, client,
					       SecurityReadAccess);
        if (!pWin)
            return(BadWindow);
    }
    else
        pWin = (WindowPtr)None;
    if (ValidAtom(stuff->selection))
    {
	int i = 0;

	/*
	 * First, see if the selection is already set... 
	 */
	while ((i < NumCurrentSelections) && 
	       CurrentSelections[i].selection != stuff->selection) 
            i++;

#ifdef TSOL
	/* 
	 * special processing for selection agent. Just note
	 * the owner of this special selection
	 */
	if (stuff->selection == MakeAtom("_TSOL_SEL_AGNT", 14, 1))
	{
	    if (HasWinSelection(tsolinfo))
	    {
            if (tsolinfo->flags & TSOL_AUDITEVENT)
               auditwrite(AW_USEOFPRIV, 1, PRIV_WIN_SELECTION,
                          AW_APPEND, AW_END);
            tsol_sel_agnt = i; /* owner of this seln */
	    }
	    else
	    {
            if (tsolinfo->flags & TSOL_AUDITEVENT)
               auditwrite(AW_USEOFPRIV, 0, PRIV_WIN_SELECTION,
                          AW_APPEND, AW_END);
            client->errorValue = stuff->selection;
            return(BadAtom);
	    }
	}	
#endif /* TSOL */

        if (i < NumCurrentSelections)
        {        
	    xEvent event;

	  #ifdef TSOL
	    /* for poly-selections, search further to see if sl,uid match */
	    tsolseln = (TsolSelnPtr)CurrentSelections[i].secPrivate;
	    if (PolySelection(CurrentSelections[i].selection))
	    {
	        prevtsolseln = tsolseln;
            while (tsolseln)
            {
                if (tsolseln->uid == tsolinfo->uid &&
                    tsolseln->sl == tsolinfo->sl)
		            break; /* match found */
                prevtsolseln = tsolseln;
                tsolseln = tsolseln->next;
            }
	    }	    
	    if (PolySelection(CurrentSelections[i].selection) && tsolseln)
	    {
            if (CompareTimeStamps(time, tsolseln->lastTimeChanged) == EARLIER)
                return Success;
	        if (tsolseln->client && (!pWin || (tsolseln->client != client)))
	        {
                event.u.u.type = SelectionClear;
                event.u.selectionClear.time = time.milliseconds;
                event.u.selectionClear.window = tsolseln->window;
                event.u.selectionClear.atom = CurrentSelections[i].selection;
                (void)TryClientEvents (tsolseln->client,
                                       &event,
                                       1,
                                       NoEventMask,
                                       NoEventMask /* CantBeFiltered */,
                                       NullGrab);
	        }
	    }
	    else if (tsolseln)
	    {
            /* we use the existing code. So we left it unindented */
#endif /* TSOL */

            /* If the timestamp in client's request is in the past relative
		to the time stamp indicating the last time the owner of the
		selection was set, do not set the selection, just return 
		success. */
            if (CompareTimeStamps(time, CurrentSelections[i].lastTimeChanged)
		== EARLIER)
		return Success;
	    if (CurrentSelections[i].client &&
		(!pWin || (CurrentSelections[i].client != client)))
	    {
		event.u.u.type = SelectionClear;
		event.u.selectionClear.time = time.milliseconds;
		event.u.selectionClear.window = CurrentSelections[i].window;
		event.u.selectionClear.atom = CurrentSelections[i].selection;
		(void) TryClientEvents (CurrentSelections[i].client, &event, 1,
				NoEventMask, NoEventMask /* CantBeFiltered */,
				NullGrab);
	    }
#ifdef TSOL
	    }
#endif /* TSOL */
	}
	else
	{
	    /*
	     * It doesn't exist, so add it...
	     */
	    Selection *newsels;

	    if (i == 0)
		newsels = (Selection *)xalloc(sizeof(Selection));
	    else
		newsels = (Selection *)xrealloc(CurrentSelections,
			    (NumCurrentSelections + 1) * sizeof(Selection));
	    if (!newsels)
		return BadAlloc;
	    NumCurrentSelections++;
	    CurrentSelections = newsels;
	    CurrentSelections[i].selection = stuff->selection;
	}
#ifdef TSOL
	/* 
	 * tsolseln == NULL, either seln does not exist, 
	 * or there is no sl,uid match
	 */
	if (!tsolseln)
	{
	    /* create one & put  it in place */
	    tsolseln = (TsolSelnPtr)xalloc(sizeof(TsolSelnRec));
	    if (!tsolseln)
            return BadAlloc;
	    tsolseln->next = (TsolSelnPtr)NULL;

	    /* if necessary put at the end of the list */
	    if (prevtsolseln)
            prevtsolseln->next = tsolseln;
	    else
	        CurrentSelections[i].secPrivate = (pointer)tsolseln;
	}
	/* fill it in */
    tsolseln->sl = tsolinfo->sl;
    tsolseln->uid = tsolinfo->uid;
    tsolseln->lastTimeChanged = time;
	tsolseln->window = stuff->window;
	tsolseln->pWin = pWin;
	tsolseln->client = (pWin ? client : NullClient);
	if (!PolySelection(CurrentSelections[i].selection))
	{
        /* no change to existing code. left as it is */
#endif /* TSOL */

        CurrentSelections[i].lastTimeChanged = time;
	CurrentSelections[i].window = stuff->window;
	CurrentSelections[i].pWin = pWin;
	CurrentSelections[i].client = (pWin ? client : NullClient);
	if (SelectionCallback)
	{
	    SelectionInfoRec	info;

	    info.selection = &CurrentSelections[i];
	    info.kind= SelectionSetOwner;
	    CallCallbacks(&SelectionCallback, &info);
	}
#ifdef TSOL 
	}
#endif /* TSOL */

	return (client->noClientException);
    }
    else 
    {
	client->errorValue = stuff->selection;
        return (BadAtom);
    }
}

int
ProcTsolGetSelectionOwner(client)
    register ClientPtr client;
{
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    if (ValidAtom(stuff->id))
    {
	int i;
        xGetSelectionOwnerReply reply;

	i = 0;
        while ((i < NumCurrentSelections) && 
	       CurrentSelections[i].selection != stuff->id) i++;
        reply.type = X_Reply;
	reply.length = 0;
	reply.sequenceNumber = client->sequence;
        if (i < NumCurrentSelections)
#ifdef TSOL 
	{
	    TsolSelnPtr tsolseln;
	    TsolInfoPtr tsolinfo; /* tsol client info */
	    tsolinfo = GetClientTsolInfo(client);

	    /* find matching sl,uid in case of poly selns */
	    tsolseln = (TsolSelnPtr)CurrentSelections[i].secPrivate;
	    if (PolySelection(CurrentSelections[i].selection))
	    {
            while (tsolseln)
            {
                if (tsolseln->uid == tsolinfo->uid &&
                    tsolseln->sl == tsolinfo->sl)
                    break; /* match found */
                tsolseln = tsolseln->next;
            }
            if (tsolseln)
                reply.owner = tsolseln->window;
            else
                reply.owner = None;
	    }
	    else
        {
            reply.owner = CurrentSelections[i].window;
	    }
	    /* 
	     * Selection Agent processing. Override the owner
	     */
        if (!HasWinSelection(tsolinfo) &&
            client->index != CLIENT_ID(reply.owner) &&
            reply.owner != None &&
            tsol_sel_agnt != -1 &&
            CurrentSelections[tsol_sel_agnt].client)
        {
            WindowPtr pWin;
            pWin = (WindowPtr)LookupWindow(reply.owner, client);
            if (tsolinfo->flags & TSOL_AUDITEVENT)
                auditwrite(AW_USEOFPRIV, 0, PRIV_WIN_SELECTION,
                           AW_APPEND, AW_END);
	     }
	     else if (HasWinSelection(tsolinfo) &&
                  tsolinfo->flags & TSOL_AUDITEVENT)
	     {
             auditwrite(AW_USEOFPRIV, 1, PRIV_WIN_SELECTION,
                        AW_APPEND, AW_END);             
	     }
	     /* end seln agent processing */
	}
#else /* TSOL */
            reply.owner = CurrentSelections[i].window;
#endif /* TSOL */
        else
            reply.owner = None;
        WriteReplyToClient(client, sizeof(xGetSelectionOwnerReply), &reply);
        return(client->noClientException);
    }
    else            
    {
	client->errorValue = stuff->id;
        return (BadAtom); 
    }
}

int
ProcTsolConvertSelection(client)
    register ClientPtr client;
{
    Bool paramsOkay;
    xEvent event;
    WindowPtr pWin;
    REQUEST(xConvertSelectionReq);

    REQUEST_SIZE_MATCH(xConvertSelectionReq);
    pWin = (WindowPtr)SecurityLookupWindow(stuff->requestor, client,
					   SecurityReadAccess);
    if (!pWin)
        return(BadWindow);

    paramsOkay = (ValidAtom(stuff->selection) && ValidAtom(stuff->target));
    if (stuff->property != None)
	paramsOkay &= ValidAtom(stuff->property);
    if (paramsOkay)
    {
	int i;

	i = 0;
	while ((i < NumCurrentSelections) && 
	       CurrentSelections[i].selection != stuff->selection) i++;
#ifdef TSOL 
	if (i < NumCurrentSelections)
	{
	    TsolSelnPtr tsolseln;
	    TsolInfoPtr tsolinfo; /* tsol client info */
	    Window twin;          /* temporary win */
	    ClientPtr tclient;    /* temporary client */

	    tsolinfo = GetClientTsolInfo(client);

	    /* find matching sl,uid in case of poly selns */
	    tsolseln = (TsolSelnPtr)CurrentSelections[i].secPrivate;
	    if (PolySelection(CurrentSelections[i].selection))
	    {
            while (tsolseln)
            {
                if (tsolseln->uid == tsolinfo->uid &&
                    tsolseln->sl == tsolinfo->sl)
                    break; /* match found */
                tsolseln = tsolseln->next;
            }
            if (!tsolseln)
            {
                client->errorValue = stuff->property;
                return (BadAtom);
            }
            twin = tsolseln->window;
            tclient = tsolseln->client;
	    }
	    else
	    {
            twin = CurrentSelections[i].window;
            tclient = CurrentSelections[i].client;
	    }
	    /*
	     * Special case for seln agent.
	     * SelectionRequest event is redirected to selection
	     * agent for unpirvileged clients and who do not own
	     * the selection
	     */	    
	    if (tsol_sel_agnt != -1 && CurrentSelections[tsol_sel_agnt].client)
	    {
            /* Redirect only if client other than owner & does not have priv */
	        if (!HasWinSelection(tsolinfo) && (client != tclient))
            {
                tclient = CurrentSelections[tsol_sel_agnt].client;
                twin = CurrentSelections[tsol_sel_agnt].window;
                if (tsolinfo->flags & TSOL_AUDITEVENT)
                    auditwrite(AW_USEOFPRIV, 1, PRIV_WIN_SELECTION,
                               AW_APPEND, AW_END);
	        }
            else if (HasWinSelection(tsolinfo) &&
                     tsolinfo->flags & TSOL_AUDITEVENT)
            {
                auditwrite(AW_USEOFPRIV, 0, PRIV_WIN_SELECTION,
                           AW_APPEND, AW_END);
            }
	    }
	    /* end of special case seln handling */

	    if (twin != None)
	    {
            event.u.u.type = SelectionRequest;
            event.u.selectionRequest.time = stuff->time;
            event.u.selectionRequest.owner = twin;
            event.u.selectionRequest.requestor = stuff->requestor;
            event.u.selectionRequest.selection = stuff->selection;
            event.u.selectionRequest.target = stuff->target;
            event.u.selectionRequest.property = stuff->property;
            if (TryClientEvents(tclient, &event, 1,
                                NoEventMask, NoEventMask /* CantBeFiltered */,
                                NullGrab))
                return (client->noClientException);
	    }
	}
#else /* TSOL */
	if ((i < NumCurrentSelections) && 
	    (CurrentSelections[i].window != None)
#ifdef XCSECURITY
	    && (!client->CheckAccess ||
		(* client->CheckAccess)(client, CurrentSelections[i].window,
					RT_WINDOW, SecurityReadAccess,
					CurrentSelections[i].pWin))
#endif
	    )
	{        
	    event.u.u.type = SelectionRequest;
	    event.u.selectionRequest.time = stuff->time;
	    event.u.selectionRequest.owner = 
			CurrentSelections[i].window;
	    event.u.selectionRequest.requestor = stuff->requestor;
	    event.u.selectionRequest.selection = stuff->selection;
	    event.u.selectionRequest.target = stuff->target;
	    event.u.selectionRequest.property = stuff->property;
	    if (TryClientEvents(
		CurrentSelections[i].client, &event, 1, NoEventMask,
		NoEventMask /* CantBeFiltered */, NullGrab))
		return (client->noClientException);
	}
#endif /* TSOL */

	event.u.u.type = SelectionNotify;
	event.u.selectionNotify.time = stuff->time;
	event.u.selectionNotify.requestor = stuff->requestor;
	event.u.selectionNotify.selection = stuff->selection;
	event.u.selectionNotify.target = stuff->target;
	event.u.selectionNotify.property = None;
	(void) TryClientEvents(client, &event, 1, NoEventMask,
			       NoEventMask /* CantBeFiltered */, NullGrab);
	return (client->noClientException);
    }
    else 
    {
	client->errorValue = stuff->property;
        return (BadAtom);
    }
}

/* Allocate and initialize a tsolprop */

TsolPropPtr
AllocTsolProp()
{
    TsolPropPtr tsolprop;

    tsolprop = (TsolPropPtr)Xcalloc(sizeof(TsolPropRec));

    if (tsolprop)
    {
	tsolprop->size = 0;
	tsolprop->data = NULL;
        tsolprop->next = NULL;
    }

    return tsolprop;
}

/*
 * property data/len is stored in pProp for single
 * instantiated properties. Polyinstanticated property
 * data/len stored in the tsolprop structure
 */

int
TsolChangeWindowProperty(client, pWin, property, type,
                         format, mode, len, value, sendevent)
    ClientPtr	client;
    WindowPtr	pWin;
    Atom	property, type;
    int		format, mode;
    unsigned long len;
    pointer	value;
    Bool	sendevent;
{
    PropertyPtr pProp;
    xEvent event;
    int sizeInBytes;
    int totalSize;
    pointer data;
    TsolPropPtr tsolprop;
    TsolInfoPtr tsolinfo;
    TsolResPtr tsolres;
    int result;
    int polyprop = PolyProperty(property, pWin);


    if (!polyprop)
    {
        result = ChangeWindowProperty(pWin, property, type,
				      format, mode, len, value, sendevent);
	if (result != Success)
	    return (result);
    }

    sizeInBytes = format>>3;
    totalSize = len * sizeInBytes;

    /* first see if property already exists */

    pProp = wUserProps (pWin);
    while (pProp )
    {
        if (pProp->propertyName == property)
            break;
        pProp = pProp->next;
    }

    tsolinfo = GetClientTsolInfo(client);
    tsolres = (TsolResPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);

    /* Initialize secPrviate if property is not polyinstantiated */
    if (!polyprop && pProp)
    {
	/* Initialize for internally created properties */
	if (!pProp->secPrivate)
            pProp->secPrivate = (pointer)AllocTsolProp();

        if (!pProp->secPrivate)
            return(BadAlloc);

        tsolprop = (TsolPropPtr)(pProp->secPrivate);
        if (WindowIsRoot(pWin))
        {
            tsolprop->sl = tsolinfo->sl;	/* use client's sl/uid */
            tsolprop->uid = tsolinfo->uid;
        }
        else
        {
            tsolprop->sl = tsolres->sl;		/* use window's sl/uid */
            tsolprop->uid = tsolres->uid;
        }

	return (result);
    }

    /* Handle polyinstantiated property */
    if (!pProp)   /* just add to list */
    {
        if (!pWin->optional && !MakeWindowOptional (pWin))
            return(BadAlloc);
        pProp = (PropertyPtr)xalloc(sizeof(PropertyRec));
        if (!pProp)
            return(BadAlloc);
        pProp->secPrivate = (pointer)Xcalloc(sizeof(TsolPropRec));
        if (!pProp->secPrivate)
            return(BadAlloc);
        data = (pointer)xalloc(totalSize);
        if (!data && len)
        {
            xfree(pProp->secPrivate);
            xfree(pProp);
            return(BadAlloc);
        }
        pProp->propertyName = property;
        pProp->type = type;
        pProp->format = format;
        pProp->data = data;
        if (len)
            bcopy((char *)value, (char *)data, totalSize);
        pProp->size = len;
        tsolprop = (TsolPropPtr)(pProp->secPrivate);
        if (WindowIsRoot(pWin))
        {
            tsolprop->sl = tsolinfo->sl;
            tsolprop->uid = tsolinfo->uid;
        }
        else
        {
            tsolprop->sl = tsolres->sl;
            tsolprop->uid = tsolres->uid;
        }
        tsolprop->data = data;
        tsolprop->size = len;
        tsolprop->next = (TsolPropPtr)NULL;
        pProp->next = pWin->optional->userProps;
        pWin->optional->userProps = pProp;
    }  /* end if !prop */
    else
    {
        /* To append or prepend to a property the request format and type
         * must match those of the already defined property.  The
         * existing format and type are irrelevant when using the mode
         * "PropModeReplace" since they will be written over.
         */
        if ((format != pProp->format) && (mode != PropModeReplace))
            return(BadMatch);
        if ((pProp->type != type) && (mode != PropModeReplace))
            return(BadMatch);

        tsolprop = (TsolPropPtr)(pProp->secPrivate);
        /* search for a matching (sl, uid) pair */
        while (tsolprop)
        {
            if (tsolprop->uid == tsolinfo->uid && tsolprop->sl == tsolinfo->sl)
                    break; /* match found */
            tsolprop = tsolprop->next;
        }

        if (!tsolprop)
        {
	    /* no match found. Create one */
	    TsolPropPtr newtsol = (TsolPropPtr)Xcalloc(sizeof(TsolPropRec));
	    if (!newtsol)
	        return(BadAlloc);
	    data = (pointer)Xcalloc(totalSize);
	    if (!data && totalSize)
	    {
	        xfree(newtsol);
	        return(BadAlloc);
	    }
	    if (len)
	        memcpy((char *)data, (char *)value, totalSize);
	    
	    newtsol->sl = tsolinfo->sl;
	    newtsol->uid = tsolinfo->uid;
	    newtsol->data = data;
	    newtsol->size = len;
            newtsol->next = (TsolPropPtr)(pProp->secPrivate);
            pProp->secPrivate = (pointer)newtsol;
        }
        else
        {
            switch (mode)
            {
                case PropModeReplace:                    
                    if (totalSize != tsolprop->size * (pProp->format >> 3))
                    {
                        data = (pointer)xrealloc(tsolprop->data, totalSize);
                        if (!data && len)
                            return(BadAlloc);
                        tsolprop->data = data;
                    }
                    if (len)
                        bcopy((char *)value, (char *)tsolprop->data, totalSize);
                    tsolprop->size = len;
                    pProp->type = type;
                    pProp->format = format;
                    break;
                    
                case PropModeAppend:
                    if (len)
                    {
                        data =
                            (pointer)xrealloc(tsolprop->data,
                                              sizeInBytes*(len+tsolprop->size));
                        if (!data)
                            return(BadAlloc);
                        tsolprop->data = data;
                        bcopy((char *)value,
                              &((char *)data)[tsolprop->size * sizeInBytes],
                              totalSize);
                        tsolprop->size += len;
                    }
                    break;
                    
                case PropModePrepend:
                    if (len)
                    {
                        data =
                            (pointer)xalloc(sizeInBytes*(len + tsolprop->size));
                        if (!data)
                            return(BadAlloc);
                        bcopy((char *)tsolprop->data,
                              &((char *)data)[totalSize],
                              (int)(tsolprop->size * sizeInBytes));
                        bcopy((char *)value, (char *)data, totalSize);
                        xfree(tsolprop->data);
                        tsolprop->data = data;
                        tsolprop->size += len;
                    }
                    break;
            }
        }
    }  /* end else if !prop */

    event.u.u.type = PropertyNotify;
    event.u.property.window = pWin->drawable.id;
    event.u.property.state = PropertyNewValue;
    event.u.property.atom = pProp->propertyName;
    event.u.property.time = currentTime.milliseconds;
    DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);

    return(Success);
}

int
TsolInitWindow(client, pWin)
    ClientPtr client;
    WindowPtr pWin;
{
    TsolInfoPtr tsolinfo;
    TsolResPtr  tsolres = (TsolResPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);

    if (client == serverClient)
    {
	tsolres->uid = 0;
	tsolres->sl = (bslabel_t *)lookupSL_low();
    }
    else
    {
        tsolinfo = GetClientTsolInfo(client);
	tsolres->uid = tsolinfo->uid;
	tsolres->sl = tsolinfo->sl;
    }
}

int
TsolDeleteProperty(client, pWin, propName)
    ClientPtr client;
    WindowPtr pWin;
    Atom propName;
{
    PropertyPtr pProp, prevProp;
    xEvent event;
    TsolPropPtr tsolprop, tail_prop, prevtsolprop;
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);

    if (!(pProp = wUserProps (pWin)))
        return(Success);

    if (!PolyProperty(propName, pWin))
	return (DeleteProperty(pWin, propName));

    prevProp = (PropertyPtr)NULL;
    while (pProp)
    {
        if (pProp->propertyName == propName)
        {
    	    tsolprop = (TsolPropPtr)(pProp->secPrivate);
            /* Found a matching name. Further match for SL,UID */
            prevtsolprop = (TsolPropPtr)NULL;
            tail_prop = tsolprop;
            while (tsolprop)
            {
                if (tsolpolyinstinfo.enabled)
                {
                    if (tsolprop->uid == tsolpolyinstinfo.uid &&
                        tsolprop->sl == tsolpolyinstinfo.sl)
                    {
                        break;
                    }
                }
                else
                {
                    if (tsolprop->uid == tsolinfo->uid &&
                        tsolprop->sl == tsolinfo->sl)
                    {
                        break;
                    }
                }
                prevtsolprop = tsolprop;
                tsolprop = tsolprop->next;
	    }
	    break;
        }
        prevProp = pProp;
        pProp = pProp->next;
    }

    if (pProp) 
    {		    
        event.u.u.type = PropertyNotify;
        event.u.property.window = pWin->drawable.id;
        event.u.property.state = PropertyDelete;
        event.u.property.atom = pProp->propertyName;
        event.u.property.time = currentTime.milliseconds;
        DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);

        if (tsolprop)
        {
	    if ((TsolPropPtr)(pProp->secPrivate) == tsolprop)
		pProp->secPrivate = (pointer)(tsolprop->next);

            if (prevtsolprop)
            {
                prevtsolprop->next = tsolprop->next;
            }
            xfree(tsolprop->data);
            xfree(tsolprop);

        }
    }
    return(Success);
}

int
ProcTsolListProperties(client)
    ClientPtr client;
{
    Atom *pAtoms, *temppAtoms;
    xListPropertiesReply xlpr;
    int	numProps = 0;
    WindowPtr pWin;
    PropertyPtr pProp;
    REQUEST(xResourceReq);
    int err_code;

    REQUEST_SIZE_MATCH(xResourceReq);
    pWin = (WindowPtr)SecurityLookupWindow(stuff->id, client,
					   SecurityReadAccess);
    if (!pWin)
        return(BadWindow);

    /* policy check for window  */
    if (err_code = xtsol_policy(TSOL_RES_PROPWIN, TSOL_READ, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        client->errorValue = stuff->id;
        return (err_code);
    }

    pProp = wUserProps (pWin);
    while (pProp)
    {        
        if (PolyProperty(pProp->propertyName, pWin))
        {
            if (PolyPropReadable(pProp, client))
                numProps++;
        }
        else
        {
            /* error ignored */
            if (!xtsol_policy(TSOL_RES_PROPERTY, TSOL_READ, pProp,
                              client, TSOL_ALL, (void *)MAJOROP))
                numProps++;
        }
        pProp = pProp->next;
    }

    if (numProps)
        if(!(pAtoms = (Atom *)ALLOCATE_LOCAL(numProps * sizeof(Atom))))
            return(BadAlloc);

    xlpr.type = X_Reply;
    xlpr.nProperties = numProps;
    xlpr.length = (numProps * sizeof(Atom)) >> 2;
    xlpr.sequenceNumber = client->sequence;
    pProp = wUserProps (pWin);
    temppAtoms = pAtoms;
    while (pProp)
    {
        if (PolyProperty(pProp->propertyName, pWin))
        {
            if (PolyPropReadable(pProp, client))
                *temppAtoms++ = pProp->propertyName;
        }
        else
        {
            /* error ignored */
            if (!xtsol_policy(TSOL_RES_PROPERTY, TSOL_READ, pProp,
                              client, TSOL_ALL, (void *)MAJOROP))
                *temppAtoms++ = pProp->propertyName;
        }
	pProp = pProp->next;
    }
    WriteReplyToClient(client, sizeof(xGenericReply), &xlpr);
    if (numProps)
    {
    	client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
        WriteSwappedDataToClient(client, numProps * sizeof(Atom), pAtoms);
        DEALLOCATE_LOCAL(pAtoms);
    }
    return(client->noClientException);
}

int
ProcTsolGetProperty(client)
    ClientPtr client;
{
    PropertyPtr pProp, prevProp;
    unsigned long n, len, ind;
    WindowPtr pWin;
    xGetPropertyReply reply;
    TsolPropPtr tsolprop;
    TsolPropPtr prevtsolprop;
    int err_code;
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);
    REQUEST(xGetPropertyReq);

    REQUEST_SIZE_MATCH(xGetPropertyReq);
    if (stuff->delete)
	UpdateCurrentTime();
    pWin = (WindowPtr)SecurityLookupWindow(stuff->window, client,
					   SecurityReadAccess);
    if (!pWin)
	return BadWindow;

    if (!ValidAtom(stuff->property))
    {
	client->errorValue = stuff->property;
	return(BadAtom);
    }
    if ((stuff->delete != xTrue) && (stuff->delete != xFalse))
    {
	client->errorValue = stuff->delete;
	return(BadValue);
    }

    if ((stuff->type != AnyPropertyType) && !ValidAtom(stuff->type))
    {
	client->errorValue = stuff->type;
	return(BadAtom);
    }


    /* policy check for window  */
    if (err_code = xtsol_policy(TSOL_RES_PROPWIN, TSOL_READ, pWin,
                                client, TSOL_ALL, (void *)MAJOROP))
    {
        client->errorValue = stuff->window;
        return (err_code);
    }

    if (!PolyProperty(stuff->property, pWin))
    {
	return (*TsolSavedProcVector[X_GetProperty])(client);
    }

    pProp = wUserProps (pWin);
    prevProp = (PropertyPtr)NULL;

    while (pProp)
    {
        if (pProp->propertyName == stuff->property)
	{
            tsolprop = (TsolPropPtr)(pProp->secPrivate);
	    prevtsolprop = tsolprop;
            while (tsolprop)
            {
                if (tsolpolyinstinfo.enabled)
                {
                    if (tsolprop->uid == tsolpolyinstinfo.uid &&
                        tsolprop->sl == tsolpolyinstinfo.sl)
                        break;
                }
                else
                {
                    if (tsolprop->uid == tsolinfo->uid &&
                        tsolprop->sl == tsolinfo->sl)
                        break; /* match found */
                }
                prevtsolprop = tsolprop;
                tsolprop = tsolprop->next;
	    }
            break;
	}
	prevProp = pProp;
	pProp = pProp->next;
    }

    reply.type = X_Reply;
    reply.sequenceNumber = client->sequence;

    if ( (!pProp) || (!tsolprop) || err_code)
    {
        reply.nItems = 0;
        reply.length = 0;
        reply.bytesAfter = 0;
        reply.propertyType = None;
        reply.format = 0;
        WriteReplyToClient(client, sizeof(xGenericReply), &reply);
    }
    else
    {
        /* If the request type and actual type don't match. Return the
           property information, but not the data. */

        if (((stuff->type != pProp->type) &&
	     (stuff->type != AnyPropertyType)))
        {
	    reply.bytesAfter = tsolprop->size;
	    reply.format = pProp->format;
	    reply.length = 0;
	    reply.nItems = 0;
	    reply.propertyType = pProp->type;
	    WriteReplyToClient(client, sizeof(xGenericReply), &reply);
	    return(Success);
        }

	/*
         *  Return type, format, value to client
         */
        n = (pProp->format/8) * tsolprop->size; 

	ind = stuff->longOffset << 2;        

        /* If longOffset is invalid such that it causes "len" to
           be negative, it's a value error. */

	if (n < ind)
	{
	    client->errorValue = stuff->longOffset;
	    return BadValue;
	}

	len = min(n - ind, 4 * stuff->longLength);

	reply.bytesAfter = n - (ind + len);
	reply.format = pProp->format;
	reply.length = (len + 3) >> 2;
	reply.nItems = len / (pProp->format / 8 );
	reply.propertyType = pProp->type;

        /* policy check for delete error ignored */
        if (stuff->delete && (reply.bytesAfter == 0) &&
            (!xtsol_policy(TSOL_RES_PROPERTY, TSOL_DESTROY, pProp,
                           client, TSOL_ALL, (void *)MAJOROP)))
        { /* send the event */
	    xEvent event;
		
	    event.u.u.type = PropertyNotify;
	    event.u.property.window = pWin->drawable.id;
	    event.u.property.state = PropertyDelete;
	    event.u.property.atom = pProp->propertyName;
	    event.u.property.time = currentTime.milliseconds;
	    DeliverEvents(pWin, &event, 1, (WindowPtr)NULL);
	}

	WriteReplyToClient(client, sizeof(xGenericReply), &reply);
	if (len)
	{
	    switch (reply.format) {
	    case 32: client->pSwapReplyFunc = (ReplySwapPtr)CopySwap32Write; break;
	    case 16: client->pSwapReplyFunc = (ReplySwapPtr)CopySwap16Write; break;
	    default: client->pSwapReplyFunc = (ReplySwapPtr)WriteToClient; break;
	    }
	    WriteSwappedDataToClient(client, len, (char *)tsolprop->data + ind);
	}

        if (stuff->delete && (reply.bytesAfter == 0))
        { /* delete the Property */
            if (prevProp == (PropertyPtr)NULL) /* takes care of head */
	    {
		if (!(pWin->optional->userProps = pProp->next))
		    CheckWindowOptionalNeed (pWin);
	    }
	    else
		prevProp->next = pProp->next;

	    /* remove the tsol struct */
            prevtsolprop->next = tsolprop->next;
            xfree(tsolprop->data);
            xfree(tsolprop);
            /* delete the prop for last reference */
            if (tsolprop == prevtsolprop)
                xfree(pProp);
	}
    }
    return (client->noClientException);
}

int
ProcTsolChangeKeyboardMapping(client)
    ClientPtr client;
{
    int err_code;

    if (err_code = xtsol_policy(TSOL_RES_KEYMAP, TSOL_MODIFY, 
	NULL, client, TSOL_ALL, (void *)MAJOROP))
    {
	/* Ignore error */
	return client->noClientException;
    }
    else
    {
	return (*TsolSavedProcVector[X_ChangeKeyboardMapping])(client);
    }
}

int
ProcTsolSetPointerMapping(client)
    ClientPtr client;
{
    int err_code;

    if (err_code = xtsol_policy(TSOL_RES_PTRMAP, TSOL_MODIFY, 
	NULL, client, TSOL_ALL, (void *)MAJOROP))
    {
	/* Ignore error */
	return Success;
    }
    else
    {
	return (*TsolSavedProcVector[X_SetPointerMapping])(client);
    }
}

int
ProcTsolChangeKeyboardControl(client)
    ClientPtr client;
{
    int err_code;

    if (err_code = xtsol_policy(TSOL_RES_KBDCTL, TSOL_MODIFY, 
	NULL, client, TSOL_ALL, (void *)MAJOROP))
    {
	/* Ignore error */
	return Success;
    }
    else
    {
	return (*TsolSavedProcVector[X_ChangeKeyboardControl])(client);
    }
}

int
ProcTsolBell(client)
    ClientPtr client;
{
    int err_code;

    if (err_code = xtsol_policy(TSOL_RES_BELL, TSOL_MODIFY, 
	NULL, client, TSOL_ALL, (void *)MAJOROP))
    {
	/* Ignore error */
	return Success;
    }
    else
    {
	return (*TsolSavedProcVector[X_Bell])(client);
    }
}

int
ProcTsolChangePointerControl(client)
    ClientPtr client;
{
    int err_code;

    if (err_code = xtsol_policy(TSOL_RES_PTRCTL, TSOL_MODIFY, 
	NULL, client, TSOL_ALL, (void *)MAJOROP))
    {
	/* Ignore error */
	return Success;
    }
    else
    {
	return (*TsolSavedProcVector[X_ChangePointerControl])(client);
    }
}

int 
ProcTsolSetModifierMapping(client)
    ClientPtr client;
{
    xSetModifierMappingReply rep;
    REQUEST(xSetModifierMappingReq);
    KeyCode *inputMap;
    int inputMapLen;
    register int i;
    int err_code;
    DeviceIntPtr keybd = inputInfo.keyboard;
    register KeyClassPtr keyc = keybd->key;
    
    REQUEST_AT_LEAST_SIZE(xSetModifierMappingReq);

    if (client->req_len != ((stuff->numKeyPerModifier<<1) +
			    (sizeof (xSetModifierMappingReq)>>2)))
	return BadLength;

    inputMapLen = 8*stuff->numKeyPerModifier;
    inputMap = (KeyCode *)&stuff[1];

    /*
     *	Now enforce the restriction that "all of the non-zero keycodes must be
     *	in the range specified by min-keycode and max-keycode in the
     *	connection setup (else a Value error)"
     */
    i = inputMapLen;
    while (i--)
    {
	if (inputMap[i]
	    && (inputMap[i] < keyc->curKeySyms.minKeyCode
		|| inputMap[i] > keyc->curKeySyms.maxKeyCode))
	{
	    client->errorValue = inputMap[i];
	    return BadValue;
	}
    }

    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.success = MappingSuccess;


    if (err_code = xtsol_policy(TSOL_RES_MODMAP, TSOL_MODIFY, 
	NULL, client, TSOL_ALL, (void *)MAJOROP))
    {
	/* 
	 * silently ignore the request. xview apps 
	 * complain if we return error code. If we don't
	 * send the map notify event application hangs
	 */
         SendMappingNotify(MappingModifier, 0, 0,client);
	 WriteReplyToClient(client, sizeof(xSetModifierMappingReply), &rep);
	 return(client->noClientException);
    }
    else
    {
	return (*TsolSavedProcVector[X_SetModifierMapping])(client);
    }
}

void
RemoveStripeWindow()
{
    WindowPtr pParent;
    WindowPtr pHead;

    if (!tpwin)
	return;

    pParent = tpwin->parent;
    pHead = pParent->firstChild;
    if (tpwin == pHead) {
	pHead = tpwin->nextSib;
	tpwin->nextSib->prevSib = tpwin->prevSib;
    }

    if (tpwin == pParent->lastChild) {
	pParent->lastChild = tpwin->nextSib;
    }
}

void
ResetStripeWindow()
{
    WindowPtr pParent;

    /* Ignore if stripe is not set */
    if (!tpwin)
	return;

    pParent = tpwin->parent;
    /* stripe is already at head, nothing to do */
    if (!pParent || pParent->firstChild == tpwin)
	return;

     ReflectStackChange(tpwin, pParent->firstChild, VTStack);
}

int
ProcTsolCreateWindow(client)
    ClientPtr client;
{
    int result;
    WindowPtr pParent;
    WindowPtr pWin;
    bslabel_t admin_low;
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);
    TsolResPtr  tsolres;

    REQUEST(xCreateWindowReq);

    REQUEST_AT_LEAST_SIZE(xCreateWindowReq);
    
    LEGAL_NEW_RESOURCE(stuff->wid, client);
    if (!(pParent = (WindowPtr)SecurityLookupWindow(stuff->parent, client,
						    SecurityWriteAccess)))
        return BadWindow;


    if (result = xtsol_policy(TSOL_RES_WINDOW, TSOL_CREATE, pParent,
		client, TSOL_ALL, (void *)MAJOROP))
	return (result);

    /* Initialize tsol security attributes */
    result = (*TsolSavedProcVector[X_CreateWindow])(client);
    pWin = pParent->firstChild;
    tsolres = (TsolResPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);

    /* stuff tsol info into window from client */
    if (tsolinfo == NULL || client == serverClient) {
	/* Client is Server itself */
	tsolres->uid = 0;
	tsolres->sl = (bslabel_t *)lookupSL_low();
    }
    else
    {
	tsolres->uid = tsolinfo->uid;
	tsolres->sl = tsolinfo->sl;
    }

    bsllow(&admin_low);
    if (blequal(tsolres->sl, &admin_low))
        tsolres->flags = TRUSTED_MASK;
    else
        tsolres->flags = 0;

    ResetStripeWindow();

    return result;
}

int
ProcTsolChangeWindowAttributes(client)
    register ClientPtr client;
{
    register WindowPtr pWin;
    REQUEST(xChangeWindowAttributesReq);
    int result;

    REQUEST_AT_LEAST_SIZE(xChangeWindowAttributesReq);
    pWin = (WindowPtr)SecurityLookupWindow(stuff->window, client,
					   SecurityWriteAccess);
    if (!pWin)
        return(BadWindow);

    if (result = xtsol_policy(TSOL_RES_WINDOW, TSOL_MODIFY, pWin,
    	client, TSOL_ALL, (void *)MAJOROP))
    {
        if (!WindowIsRoot(pWin))
            return (result);
    }

    result = (*TsolSavedProcVector[X_ChangeWindowAttributes])(client);
    ResetStripeWindow();

    return result;
}

int
ProcTsolConfigureWindow(client)
    register ClientPtr client;
{
    int result;

    result = (*TsolSavedProcVector[X_ConfigureWindow])(client);
    ResetStripeWindow();

    return result;
}

int
ProcTsolCirculateWindow(client)
    register ClientPtr client;
{
    int result;

    result = (*TsolSavedProcVector[X_CirculateWindow])(client);
    ResetStripeWindow();

    return result;
}

int
ProcTsolReparentWindow(client)
    register ClientPtr client;
{
    int result;

    result = (*TsolSavedProcVector[X_ReparentWindow])(client);
    ResetStripeWindow();

    return result;
}

int
ProcTsolSendEvent(client)
    register ClientPtr client;
{
    WindowPtr pWin;
    REQUEST(xSendEventReq);

    REQUEST_SIZE_MATCH(xSendEventReq);

    pWin = LookupWindow(stuff->destination, client);

    if (!pWin)
	return BadWindow;

    if (xtsol_policy(TSOL_RES_EVENTWIN, TSOL_MODIFY,
		pWin, client, TSOL_ALL, (void *)MAJOROP))
	return Success; /* ignore error */

    return (*TsolSavedProcVector[X_SendEvent])(client);
}


/*
 * HandleHotKey - 
 * HotKey is Meta(Diamond)+ Stop Key
 * Breaks untusted Ptr and Kbd grabs.
 * Trusted Grabs are NOT broken 
 * Warps pointer to the Trusted Stripe if not Trusted grabs in force.
 */
void
HandleHotKey()
{
    extern unsigned int StripeHeight;
    int	            x, y;
    Bool            trusted_grab = FALSE;
    ClientPtr       client;
    DeviceIntPtr    mouse = inputInfo.pointer;
    DeviceIntPtr    keybd = inputInfo.keyboard;
    TsolInfoPtr	    tsolinfo;    
    GrabPtr         ptrgrab = mouse->grab;
    GrabPtr         kbdgrab = keybd->grab;
    ScreenPtr       pScreen;

    if (kbdgrab)
    {
	client = clients[CLIENT_ID(kbdgrab->resource)];
	tsolinfo = GetClientTsolInfo(client);

        if (tsolinfo)
        {
            if (HasTrustedPath(tsolinfo))
                trusted_grab = TRUE;
            else
	        (*keybd->DeactivateGrab)(keybd);
	}

	if (ptrgrab)
	{
	    client = clients[CLIENT_ID(ptrgrab->resource)];
	    tsolinfo = GetClientTsolInfo(client);

            if (tsolinfo)
            {
                if (HasTrustedPath(tsolinfo))
                    trusted_grab = TRUE;
                else
	            (*mouse->DeactivateGrab)(mouse);
	    }
        }
    }

    if (!trusted_grab)
    {
        /*
         * Warp the pointer to the Trusted Stripe
         */
	    pScreen = screenInfo.screens[0];
	    x = pScreen->width/2;
	    y = pScreen->height - StripeHeight/2;
        (*pScreen->SetCursorPosition)(pScreen, x, y, TRUE);
    }
}

int
ProcTsolSetInputFocus(client)
    ClientPtr client;
{

    REQUEST(xSetInputFocusReq);

    REQUEST_SIZE_MATCH(xSetInputFocusReq);

    if (stuff->focus != None)
    {
        WindowPtr focuswin;

	focuswin = LookupWindow(stuff->focus, client);
	if ((focuswin != NullWindow) &&
	    xtsol_policy(TSOL_RES_FOCUSWIN, TSOL_MODIFY, focuswin,
		client, TSOL_ALL, (void *)MAJOROP))
	{
	    return (client->noClientException);
	}
    }
    return (*TsolSavedProcVector[X_SetInputFocus])(client);
}

int
ProcTsolGetInputFocus(client)
    ClientPtr client;
{
    xGetInputFocusReply rep;
    REQUEST(xReq);
    FocusClassPtr focus = inputInfo.keyboard->focus;

    REQUEST_SIZE_MATCH(xReq);
#ifdef PANORAMIX
    if ( !noPanoramiXExtension )
        return PanoramiXGetInputFocus(client);
#endif
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    if (focus->win == NoneWin)
	rep.focus = None;
    else if (focus->win == PointerRootWin)
	rep.focus = PointerRoot;
    else if (xtsol_policy(TSOL_RES_FOCUSWIN, TSOL_READ,
	    focus->win, client, TSOL_ALL, (void *)MAJOROP))
	rep.focus = RootOf(focus->win); /* root window on access failure */
    else rep.focus = focus->win->drawable.id;
    rep.revertTo = focus->revert;
    WriteReplyToClient(client, sizeof(xGetInputFocusReply), &rep);
    return Success;
}

void
PrintSiblings(p1)
    WindowPtr p1;
{
    WindowPtr p2;

    if (p1 == NULL || p1->parent == NULL) return;

    p2 = p1->parent->firstChild;
    while (p2)
    {
	ErrorF( "(%x, %d, %d, %x)\n", p2, p2->drawable.width, 
	    p2->drawable.height, p2->prevSib);
	p2 = p2->nextSib;
    }
}

/*
 * Checks that tpwin & its siblings have same
 * parents. Returns 0 if OK, a # indicating which
 * Sibling has a bad parent
 */
int
CheckTPWin()
{
	WindowPtr pWin;
	int count = 1;

	pWin = tpwin->nextSib;
	while (pWin)
	{
		if (pWin->parent->parent)
			return count;
		pWin = pWin->nextSib;
		++count;
	}
	return 0;
}

/* NEW */

int
ProcTsolGetGeometry(client)
    register ClientPtr client;
{
    xGetGeometryReply rep;
    int status;

    REQUEST(xResourceReq);

    if ((status = GetGeometry(client, &rep)) != Success)
	return status;

    /* Reduce root window height = stripe height */
    if (stuff->id == rep.root)
    {
        extern unsigned int StripeHeight;
        rep.height -= StripeHeight;
    }

    WriteReplyToClient(client, sizeof(xGetGeometryReply), &rep);
    return(client->noClientException);
}

int
ProcTsolGrabServer(client)
    register ClientPtr client;
{
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xReq);

    if (xtsol_policy(TSOL_RES_SRVGRAB, TSOL_CREATE, NULL,
        	client, TSOL_ALL, (void *)MAJOROP))
    {
	/* turn off auditing because operation ignored */
        tsolinfo->flags &= ~TSOL_DOXAUDIT;
        tsolinfo->flags &= ~TSOL_AUDITEVENT;

        return(client->noClientException);
    }

    return (*TsolSavedProcVector[X_GrabServer])(client);
}

int
ProcTsolUngrabServer(client)
    register ClientPtr client;
{
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);

    REQUEST(xResourceReq);
    REQUEST_SIZE_MATCH(xReq);

    if (xtsol_policy(TSOL_RES_SRVGRAB, TSOL_DESTROY, NULL,
        	client, TSOL_ALL, (void *)MAJOROP))
    {
	/* turn off auditing because operation ignored */
        tsolinfo->flags &= ~TSOL_DOXAUDIT;
        tsolinfo->flags &= ~TSOL_AUDITEVENT;

        return(client->noClientException);
    }

    return (*TsolSavedProcVector[X_UngrabServer])(client);
}

int
ProcTsolCreatePixmap(client)
    register ClientPtr client;
{
    PixmapPtr pMap;
    int result;

    REQUEST(xCreatePixmapReq);

    REQUEST_SIZE_MATCH(xCreatePixmapReq);

    result = (*TsolSavedProcVector[X_CreatePixmap])(client);

    pMap = (PixmapPtr)SecurityLookupIDByType(client, stuff->pid, RT_PIXMAP,
					     SecurityDestroyAccess);
    if (pMap) 
    {
	/* Initialize security info */
        TsolInfoPtr tsolinfo = GetClientTsolInfo(client);
        TsolResPtr tsolres =
            (TsolResPtr)(pMap->devPrivates[tsolPixmapPrivateIndex].ptr);

        if (tsolinfo == NULL || client == serverClient)
	{
	    /* Client is Server itself */
	    tsolres->uid = 0;
	    tsolres->sl = (bslabel_t *)lookupSL_low();
        }
        else
        {
	    tsolres->uid = tsolinfo->uid;
	    tsolres->sl = tsolinfo->sl;
        }
        tsolres->flags = 0;
    }

    return result;
}
int
ProcTsolSetScreenSaver(client)
    register ClientPtr client;
{
    int result;

    REQUEST(xSetScreenSaverReq);

    REQUEST_SIZE_MATCH(xSetScreenSaverReq);

    if (result = xtsol_policy(TSOL_RES_SCRSAVER, TSOL_MODIFY, NULL,
             client, TSOL_ALL, (void *)MAJOROP))
        return (result);

    return (*TsolSavedProcVector[X_SetScreenSaver])(client);
}

int
ProcTsolChangeHosts(client)
    register ClientPtr client;
{
    int result;

    REQUEST(xChangeHostsReq);

    REQUEST_FIXED_SIZE(xChangeHostsReq, stuff->hostLength);

    if (result = xtsol_policy(TSOL_RES_ACL, TSOL_MODIFY, NULL,
             client, TSOL_ALL, (void *)MAJOROP))
        return (result);

    return (*TsolSavedProcVector[X_ChangeHosts])(client);
}

int
ProcTsolChangeAccessControl(client)
    register ClientPtr client;
{
    int result;

    REQUEST(xSetAccessControlReq);

    REQUEST_SIZE_MATCH(xSetAccessControlReq);

    if (result = xtsol_policy(TSOL_RES_ACL, TSOL_MODIFY,
		(void *)stuff->mode, client, TSOL_ALL, (void *)MAJOROP))
    {
        client->errorValue = stuff->mode;
        return (result);
    }

    return (*TsolSavedProcVector[X_SetAccessControl])(client);
}

int
ProcTsolKillClient(client)
    register ClientPtr client;
{
    int result;

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);

    if (result = xtsol_policy(TSOL_RES_CLIENT, TSOL_DESTROY,
		(void *)stuff->id, client, TSOL_ALL, (void *)MAJOROP))
    {
        client->errorValue = stuff->id;
        return (result);
    }

    return (*TsolSavedProcVector[X_KillClient])(client);
}

int
ProcTsolSetFontPath(client)
    register ClientPtr client;
{
    REQUEST(xSetFontPathReq);
    
    REQUEST_AT_LEAST_SIZE(xSetFontPathReq);
    
    if (xtsol_policy(TSOL_RES_FONTPATH, TSOL_MODIFY, NULL,
                     client, TSOL_ALL, (void *)MAJOROP))
    {
        return (BadValue);
    }

    return (*TsolSavedProcVector[X_SetFontPath])(client);
}

int
ProcTsolChangeCloseDownMode(client)
    register ClientPtr client;
{
    REQUEST(xSetCloseDownModeReq);

    REQUEST_SIZE_MATCH(xSetCloseDownModeReq);

    if (xtsol_policy(TSOL_RES_CLIENT, TSOL_MODIFY, NULL,
		client, TSOL_ALL, (void *)MAJOROP))
        return (client->noClientException); /* Ignore error */

    return (*TsolSavedProcVector[X_SetCloseDownMode])(client);
}

int ProcTsolForceScreenSaver(client)
    register ClientPtr client;
{    
    int result;

    REQUEST(xForceScreenSaverReq);

    REQUEST_SIZE_MATCH(xForceScreenSaverReq);
    

    if (result = xtsol_policy(TSOL_RES_SCRSAVER, TSOL_MODIFY, NULL,
             client, TSOL_ALL, (void *)MAJOROP))
        return (result);

    return (*TsolSavedProcVector[X_ForceScreenSaver])(client);
}

void
TsolDeleteWindowFromAnySelections(pWin)
    WindowPtr pWin;
{
    register int i;
    TsolSelnPtr tsolseln = NULL;
    TsolSelnPtr prevtsolseln = NULL;

    for (i = 0; i< NumCurrentSelections; i++)
    {
        if (PolySelection(CurrentSelections[i].selection))
        {
            tsolseln = (TsolSelnPtr)CurrentSelections[i].secPrivate;
            prevtsolseln = tsolseln;
            while (tsolseln)
            {
                if (tsolseln->pWin == pWin)
		    break; /* match found */
                prevtsolseln = tsolseln;
                tsolseln = tsolseln->next;
            }

            if (tsolseln)
            {
	        if (SelectionCallback)
	        {
		    SelectionInfoRec    info;

		    info.selection = &CurrentSelections[i];
		    info.kind = SelectionClientClose;
		    CallCallbacks(&SelectionCallback, &info);
	        }

                /* first on the list */
                if (prevtsolseln == tsolseln)
                    CurrentSelections[i].secPrivate = (pointer)tsolseln->next;
                else
                    prevtsolseln->next = tsolseln->next;
                xfree(tsolseln);

		/* handle the last reference */
                if (CurrentSelections[i].secPrivate == NULL)
                {
                    CurrentSelections[i].pWin = (WindowPtr)NULL;
                    CurrentSelections[i].window = None;
                    CurrentSelections[i].client = NullClient;
                }
            }
        }
        else
        {
            if (CurrentSelections[i].pWin == pWin)
            {
                CurrentSelections[i].pWin = (WindowPtr)NULL;
                CurrentSelections[i].window = None;
                CurrentSelections[i].client = NullClient;
            }
        }
   }
}

void
TsolDeleteClientFromAnySelections(client)
    ClientPtr client;
{
    register int i;
    TsolSelnPtr tsolseln = NULL;
    TsolSelnPtr prevtsolseln = NULL;

    for (i = 0; i< NumCurrentSelections; i++)
    {
        if (PolySelection(CurrentSelections[i].selection))
        {
            tsolseln = (TsolSelnPtr)CurrentSelections[i].secPrivate;
            prevtsolseln = tsolseln;
            while (tsolseln)
            {
                if (tsolseln->client == client)
		    break; /* match found */
                prevtsolseln = tsolseln;
                tsolseln = tsolseln->next;
            }

            if (tsolseln)
            {
	        if (SelectionCallback)
	        {
		    SelectionInfoRec    info;

		    info.selection = &CurrentSelections[i];
		    info.kind = SelectionClientClose;
		    CallCallbacks(&SelectionCallback, &info);
	        }

                /* first on the list */
                if (prevtsolseln == tsolseln)
                    CurrentSelections[i].secPrivate = (pointer)tsolseln->next;
                else
                    prevtsolseln->next = tsolseln->next;
                xfree(tsolseln);

		/* handle the last reference */
                if (CurrentSelections[i].secPrivate == NULL)
                {
                    CurrentSelections[i].pWin = (WindowPtr)NULL;
                    CurrentSelections[i].window = None;
                    CurrentSelections[i].client = NullClient;
                }
            }
        }
        else
        {
            if (CurrentSelections[i].client == client)
            {
                CurrentSelections[i].pWin = (WindowPtr)NULL;
                CurrentSelections[i].window = None;
                CurrentSelections[i].client = NullClient;
            }
        }
   }
}

int
ProcTsolListInstalledColormaps(client)
    register ClientPtr client;
{
    xListInstalledColormapsReply *preply; 
    int nummaps;
    WindowPtr pWin;
    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    pWin = (WindowPtr)SecurityLookupWindow(stuff->id, client,
					   SecurityReadAccess);

    if (!pWin)
        return(BadWindow);

    preply = (xListInstalledColormapsReply *) 
		ALLOCATE_LOCAL(sizeof(xListInstalledColormapsReply) +
		     pWin->drawable.pScreen->maxInstalledCmaps *
		     sizeof(Colormap));
    if(!preply)
        return(BadAlloc);

    preply->type = X_Reply;
    preply->sequenceNumber = client->sequence;
    nummaps = (*pWin->drawable.pScreen->ListInstalledColormaps)
        (pWin->drawable.pScreen, (Colormap *)&preply[1]);
    preply->nColormaps = nummaps;
    preply->length = nummaps;
#ifdef TSOL
    {
        int err_code, i;
        Colormap *pcmap = (Colormap *)&preply[1];
        ColormapPtr pcmp;

	    /*
         * check every colormap id for access. return default colormap
	     * id in case of failure
	     */
        for (i = 0; i < nummaps; i++, pcmap++)
        {
            pcmp = (ColormapPtr )LookupIDByType(*pcmap, RT_COLORMAP);
            if (err_code = xtsol_policy(TSOL_RES_CMAP, TSOL_READ, pcmp,
                                        client, TSOL_ALL, (void *)MAJOROP))
            {
                *pcmap = pWin->drawable.pScreen->defColormap;
            }
        }
    }
#endif /* TSOL */
    WriteReplyToClient(client, sizeof (xListInstalledColormapsReply), preply);
    client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
    WriteSwappedDataToClient(client, nummaps * sizeof(Colormap), &preply[1]);
    DEALLOCATE_LOCAL(preply);
    return(client->noClientException);
}

int
ProcTsolGetImage(client)
    register ClientPtr	client;
{
    REQUEST(xGetImageReq);

    REQUEST_SIZE_MATCH(xGetImageReq);

    return DoGetImage(client, stuff->format, stuff->drawable,
		      stuff->x, stuff->y,
		      (int)stuff->width, (int)stuff->height,
		      stuff->planeMask, (xGetImageReply **)NULL);
}

int
ProcTsolQueryTree(client)
    register ClientPtr client;
{
    xQueryTreeReply reply;
    int numChildren = 0;
    register WindowPtr pChild, pWin, pHead;
    Window  *childIDs = (Window *)NULL;
#if defined(PANORAMIX) && !defined(IN_MODULE)
    PanoramiXWindow     *pPanoramiXWin = PanoramiXWinRoot;
    int         j, thisScreen;
#endif

#ifdef TSOL
    TsolInfoPtr  tsolinfo = GetClientTsolInfo(client);
#endif  /* TSOL */

    REQUEST(xResourceReq);

    REQUEST_SIZE_MATCH(xResourceReq);
    pWin = (WindowPtr)SecurityLookupWindow(stuff->id, client,
					   SecurityReadAccess);
    if (!pWin)
        return(BadWindow);

#ifdef TSOL
    if (xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pWin,
                     client, TSOL_ALL, (void *)MAJOROP))
    {
	    return(BadWindow);
    }
    /*
     * Because of its recursive nature, QuerryTree can leave a huge trail
     * of audit records which could make deciphering the audit log for
     * critical records difficult. So we turn off any more auditing of
     * this protocol.
     */
    tsolinfo->flags &= ~TSOL_DOXAUDIT;
    tsolinfo->flags &= ~TSOL_AUDITEVENT;
#endif /* TSOL */

    reply.type = X_Reply;
    reply.root = WindowTable[pWin->drawable.pScreen->myNum]->drawable.id;
    reply.sequenceNumber = client->sequence;
    if (pWin->parent)
	reply.parent = pWin->parent->drawable.id;
    else
        reply.parent = (Window)None;
#if defined(SUNSOFT) && defined(PANORAMIX) && !defined(IN_MODULE)
    if ( !noPanoramiXExtension ) {
        thisScreen = 0;
        for (j = 0; j <= PanoramiXNumScreens - 1; j++) {
          if ( pWin->winSize.extents.x1 <  (panoramiXdataPtr[j].x  + 
					    panoramiXdataPtr[j].width)) {
             thisScreen = j;
             break;
          }
        }
    }
    if ( !noPanoramiXExtension  && thisScreen ) {
        PANORAMIXFIND_ID(pPanoramiXWin, pWin->drawable.id);
        IF_RETURN(!pPanoramiXWin, BadWindow);
        pWin = 
	(WindowPtr)SecurityLookupWindow(pPanoramiXWin->info[thisScreen].id,
		   client,
                   SecurityReadAccess);
        if (!pWin)
            return(BadWindow);
        pHead = RealChildHead(pWin);
        for(pChild = pWin->lastChild;pChild != pHead; pChild = pChild->prevSib)
             numChildren++;
        if (numChildren)
        {
          int curChild = 0;
          childIDs = (Window *) ALLOCATE_LOCAL(numChildren * sizeof(Window));
          if (!childIDs)
              return BadAlloc;
          for (pChild = pWin->lastChild; pChild != pHead; 
		                       pChild = pChild->prevSib) {
              pPanoramiXWin = PanoramiXWinRoot;
              PANORAMIXFIND_ID_BY_SCRNUM(pPanoramiXWin, pChild->drawable.id, 
				         thisScreen);
              IF_RETURN(!pPanoramiXWin, BadWindow);
              childIDs[curChild++] = pPanoramiXWin->info[0].id;
          }
        } /* numChildren */
    }else { /* otherwise its screen 0, and nothing changes */
      pHead = RealChildHead(pWin);
      for (pChild = pWin->lastChild; pChild != pHead; pChild = pChild->prevSib)
#ifdef TSOL
      {
		/* error ignored */
		if (!xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pChild,
						  client, TSOL_ALL, (void *)MAJOROP))
		{
			numChildren++;
		}
      }
#else /* !TSOL */
	  numChildren++;
#endif /* TSOL */
      if (numChildren)
      {
        int curChild = 0;

        childIDs = (Window *) ALLOCATE_LOCAL(numChildren * sizeof(Window));
        if (!childIDs)
            return BadAlloc;
        for (pChild = pWin->lastChild; pChild != pHead; 
			pChild = pChild->prevSib)
#ifdef TSOL
	{

	    /* error ignored */
	    if (!xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pChild,
                          client, TSOL_ALL, (void *)MAJOROP))
        {
	        childIDs[curChild++] = pChild->drawable.id;
        }
	}
#else /* !TSOL */
	    childIDs[curChild++] = pChild->drawable.id;
#endif /* TSOL */
      }
    }
#else
    pHead = RealChildHead(pWin);
    for (pChild = pWin->lastChild; pChild != pHead; pChild = pChild->prevSib)
#ifdef TSOL
    {
		/* error ignored */
		if (!xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pChild,
						  client, TSOL_ALL, (void *)MAJOROP))
		{
			numChildren++;
		}
    }
#else /* !TSOL */
	numChildren++;
#endif /* TSOL */
    if (numChildren)
    {
	int curChild = 0;

	childIDs = (Window *) ALLOCATE_LOCAL(numChildren * sizeof(Window));
	if (!childIDs)
	    return BadAlloc;
	for (pChild = pWin->lastChild; pChild != pHead; pChild = pChild->prevSib)
#ifdef TSOL
	{

	    /* error ignored */
	    if (!xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, pChild,
                          client, TSOL_ALL, (void *)MAJOROP))
        {
	        childIDs[curChild++] = pChild->drawable.id;
        }
	}
#else /* !TSOL */
	    childIDs[curChild++] = pChild->drawable.id;
#endif /* TSOL */
    }
#endif
    
    reply.nChildren = numChildren;
    reply.length = (numChildren * sizeof(Window)) >> 2;
    
    WriteReplyToClient(client, sizeof(xQueryTreeReply), &reply);
    if (numChildren)
    {
    	client->pSwapReplyFunc = (ReplySwapPtr) Swap32Write;
	WriteSwappedDataToClient(client, numChildren * sizeof(Window), childIDs);
	DEALLOCATE_LOCAL(childIDs);
    }

    return(client->noClientException);
}

void
TsolAuditStart(ClientPtr client)
{
    extern Bool system_audit_on;
    unsigned int protocol;
    int xevent_num;
    int count = 0;
    int status = 0;
    Bool do_x_audit = FALSE;
    Bool audit_event = FALSE;
    char audit_ret = (char)NULL;
    TsolInfoPtr tsolinfo = (TsolInfoPtr)NULL;
    tsolinfo = GetClientTsolInfo(client);
    if (system_audit_on &&
    (tsolinfo->aw_auinfo.ai_mask.am_success ||
    tsolinfo->aw_auinfo.ai_mask.am_failure))
    {
        do_x_audit = TRUE;
        auditwrite(AW_PRESELECT, &(tsolinfo->aw_auinfo.ai_mask), AW_END);
    }
    return;
            /*
             * X audit events start from 9101 in audit_uevents.h. The first two
             * events are non-protocol ones viz. ClientConnect, mapped to 9101
             * and ClientDisconnect, mapped to 9102.
             * The protocol events are mapped from 9103 onwards in the serial
             * order of their respective protocol opcode, for eg, the protocol
             * UngrabPointer which is has a protocol opcode 27 is mapped to
             * 9129 (9102 + 27).
             * All extension protocols are mapped to a single audit event
             * AUE_XExtension as opcodes are assigined dynamically to these
             * protocols. We set the extension protocol opcode to be 128, one
             * more than the last standard opcode.
             */
            protocol = (unsigned int)MAJOROP;
            if (protocol > X_NoOperation)
            {
                xevent_num = audit_eventsid[MAX_AUDIT_EVENTS - 1][1];
                audit_event = TRUE;
            }
            else
            {
                for (count = 0; count < MAX_AUDIT_EVENTS; count++)
                {
                    if (protocol == audit_eventsid[count][0])
                    {
                        xevent_num = audit_eventsid[count][1];
                        audit_event = TRUE;
                        break;
                    }
                }
            }
            if (audit_event &&
                do_x_audit &&
                (au_preselect(xevent_num,
                              &(tsolinfo->aw_auinfo.ai_mask),
                              AU_PRS_BOTH,
                              AU_PRS_USECACHE) == 1))
            {
                tsolinfo->flags |= TSOL_AUDITEVENT;
                status = auditwrite(AW_EVENTNUM, xevent_num, AW_APPEND, AW_END);

            }
            else
            {
                tsolinfo->flags &= ~TSOL_AUDITEVENT;
                tsolinfo->flags &= ~TSOL_DOXAUDIT;
            }
}

void
TsolAuditEnd(ClientPtr client, int result)
{
    extern Bool system_audit_on;
    unsigned int protocol;
    int xevent_num;
    int count = 0;
    int status = 0;
    Bool do_x_audit = FALSE;
    Bool audit_event = FALSE;
    char audit_ret = (char)NULL;
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);

    if (tsolinfo->flags & TSOL_DOXAUDIT)
    {
                tsolinfo->flags &= ~TSOL_DOXAUDIT;
                if (tsolinfo->flags & TSOL_AUDITEVENT)
                    tsolinfo->flags &= ~TSOL_AUDITEVENT;
                if (result != Success)
                    audit_ret = -1;
                else
                    audit_ret = 0;
                auditwrite(AW_RETURN, audit_ret, (u_int)result,
                           AW_WRITE, AW_END);
    }
    else if (tsolinfo->flags & TSOL_AUDITEVENT)
    {
        tsolinfo->flags &= ~TSOL_AUDITEVENT;
        auditwrite(AW_DISCARDRD, -1, AW_END);
    }
}

int
ProcTsolQueryPointer(client)
    ClientPtr client;
{
    xQueryPointerReply rep;
    WindowPtr pWin, t, ptrWin;
    REQUEST(xResourceReq);
    DeviceIntPtr mouse = inputInfo.pointer;

    REQUEST_SIZE_MATCH(xResourceReq);
    pWin = SecurityLookupWindow(stuff->id, client, SecurityReadAccess);
    if (!pWin)
	return BadWindow;

    ptrWin = TsolPointerWindow();
    if (!xtsol_policy(TSOL_RES_WINDOW, TSOL_READ, ptrWin,
	    client, TSOL_ALL, (void *)MAJOROP))
    	return (*TsolSavedProcVector[X_QueryPointer])(client);

    if (mouse->valuator->motionHintWindow)
	MaybeStopHint(mouse, client);
    rep.type = X_Reply;
    rep.sequenceNumber = client->sequence;
    rep.mask = mouse->button->state | inputInfo.keyboard->key->state;
    rep.length = 0;
    rep.root = RootOf(pWin);
    rep.rootX = 0;
    rep.rootY = 0;
    rep.child = None;
    rep.sameScreen = xTrue;
    rep.winX = 0;
    rep.winY = 0;

    WriteReplyToClient(client, sizeof(xQueryPointerReply), &rep);

    return(Success);    
}
