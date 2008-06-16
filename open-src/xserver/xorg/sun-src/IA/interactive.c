/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident   "@(#)interactive.c 35.18     08/06/16 SMI"

/************************************************************
	Basic boilerplate extension.

	This file also contains the code to make the priocntl on behalf
	of the clients.

	Note that ChangePriority also sets the last client with focus
	to the normal priority.

	If there are knobs to be added to the system, for say the nice
	values for the IA class, they would be added here.
********************************************************/

/* THIS IS NOT AN X CONSORTIUM STANDARD */

#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <errno.h>
#include <sys/types.h>
#include <stdio.h>

#include <sys/priocntl.h>
#include <sys/iapriocntl.h>
#include <unistd.h>


#define NEED_REPLIES
#define NEED_EVENTS
#include <X11/X.h>
#include <X11/Xproto.h>
#include "os.h"
#include "dixstruct.h"
#include "windowstr.h"
#include "inputstr.h"
#include "extnsionst.h"
#define _XIA_SERVER_
#include <X11/extensions/interactive.h>
#include <X11/Xfuncproto.h>
#include "dix.h"

#define UNSET_PRIORITY 		0
#define SET_PRIORITY		1
#define SET_INTERACTIVE 	2

#define SERVER			0x1
#define WMGR			0x2
#define BOTH			0x3

typedef struct _ClientProcessInfo {
	int count;
	ConnectionPidPtr pids;
} ClientProcessRec, * ClientProcessPtr;

static int ProcIADispatch(ClientPtr client), SProcIADispatch(ClientPtr client);
static int ProcIASetProcessInfo(ClientPtr client), SProcIASetProcessInfo(ClientPtr client);
static int ProcIAGetProcessInfo(ClientPtr client), SProcIAGetProcessInfo(ClientPtr client);
static int ProcIAQueryVersion(ClientPtr client), SProcIAQueryVersion(ClientPtr client);
static void IACloseDown(ExtensionEntry *ext);
static void IAClientStateChange(CallbackListPtr *pcbl, pointer nulldata, pointer calldata);

static int InitializeClass(void );
static void SetIAPrivate(int*);
static void ChangeInteractive(ClientPtr);
static int SetPriority(int, int);
static void UnsetLastPriority(ClientProcessPtr LastPid); 
static void ChangePriority(register ClientPtr client);

static int SetClientPrivate(ClientPtr client, ConnectionPidPtr stuff, int length);
static void FreeProcessList(ClientPtr client);
/* static int LocalConnection(OsCommPtr); */
static int PidSetEqual(ClientProcessPtr, ClientProcessPtr);

static int IAWrapProcVectors(void);
static int IAUnwrapProcVectors(void);

static CARD32 IAInitTimerCall(OsTimerPtr timer,CARD32 now,pointer arg);

static iaclass_t 	IAClass;
static id_t		TScid;
static ClientProcessPtr	LastPids = NULL;
static int 		specialIAset = 0;
static int 		ia_nice = IA_BOOST;
static Bool 		InteractiveOS = xTrue;
static ClientPtr 	wmClientptr = NULL;
static unsigned long 	IAExtensionGeneration = 0;
static OsTimerPtr 	IAInitTimer = NULL;
static int (* IASavedProcVector[256]) (ClientPtr client);

typedef struct {
    ClientProcessPtr    process; /* Process id information */    
    Bool		wmgr;
} IAClientPrivateRec, *IAClientPrivatePtr;

static int	IAClientPrivateIndex;

#define GetIAClient(pClient)    ((IAClientPrivatePtr) (pClient)->devPrivates[IAClientPrivateIndex].ptr)
#define GetConnectionPids(pClient)	(GetIAClient(pClient)->process)

/* Set via xorg.conf option in loadable module */
int IADebugLevel = 0;

#define IA_DEBUG(level, func)	\
	if (IADebugLevel >= level) { func; } else (void)(0)

#define IA_DEBUG_BASIC		1
#define IA_DEBUG_PRIOCNTL	3

void
IAExtensionInit(void)
{
    IA_DEBUG(IA_DEBUG_BASIC, 
      LogMessage(X_INFO, "SolarisIA: Initializing (generation %ld)\n",
	IAExtensionGeneration));

    if (IAExtensionGeneration == serverGeneration)
	return;

    InteractiveOS=xFalse;

    if (InitializeClass() != Success)
	return;

    if (SetPriority(P_MYID, SET_INTERACTIVE) != Success)
	return;

    if (SetPriority(P_MYID, SET_PRIORITY) != Success)
	return;

    IAClientPrivateIndex = AllocateClientPrivateIndex();
    if (IAClientPrivateIndex < 0)
	return;
    if (!AllocateClientPrivate (IAClientPrivateIndex,
				      sizeof (IAClientPrivateRec)))
        return;

    if (!AddCallback(&ClientStateCallback, IAClientStateChange, NULL))
        return;

    if (IAWrapProcVectors() != 0)
	return;

    if (!AddExtension(IANAME, IANumberEvents, IANumberErrors,
				 ProcIADispatch, SProcIADispatch,
				 IACloseDown, StandardMinorOpcode))
	return;

    /* InitExtensions is called before InitClientPrivates(serverClient)
       so we set this timer to callback as soon as we hit WaitForSomething
       to initialize the serverClient */
    IAInitTimer = TimerSet(IAInitTimer, 0, 1, IAInitTimerCall, NULL);

    specialIAset = 0;
    InteractiveOS = xTrue;
    IAExtensionGeneration = serverGeneration;

    IA_DEBUG(IA_DEBUG_BASIC, 
      LogMessage(X_INFO, "SolarisIA: Finished initializing (generation %ld)\n",
	IAExtensionGeneration));
}

/* Called when we first hit WaitForSomething to initialize serverClient */
static CARD32 
IAInitTimerCall(OsTimerPtr timer,CARD32 now,pointer arg)
{
    ConnectionPidRec serverPid;

    if (InteractiveOS != xTrue)
	return 0;

    GetConnectionPids(serverClient) = NULL;
    GetIAClient(serverClient)->wmgr = FALSE;

    serverPid = getpid();
    SetClientPrivate(serverClient, &serverPid, 1);

    ChangePriority(serverClient);
    return 0;
}

/* Called when new client connects or existing client disconnects */
static void
IAClientStateChange(CallbackListPtr *pcbl, pointer nulldata, pointer calldata)
{
    NewClientInfoRec *pci = (NewClientInfoRec *)calldata;
    ClientPtr pClient = pci->client;
    ClientProcessPtr CurrentPids;

    switch (pClient->clientState) {
    case ClientStateGone:
    case ClientStateRetained:
	CurrentPids = GetConnectionPids(pClient);

	if (pClient==wmClientptr) {
	    IA_DEBUG(IA_DEBUG_BASIC,
	      LogMessage(X_INFO, "SolarisIA: WindowManager closed (pid %d)\n",
	      (CurrentPids && CurrentPids->pids) ? CurrentPids->pids[0] : -1));
	    wmClientptr=NULL;
	}

	if (CurrentPids && LastPids && PidSetEqual(CurrentPids, LastPids))
	    LastPids=NULL;

	FreeProcessList(pClient);
	GetIAClient(pClient)->wmgr = FALSE;
	break;
	
    case ClientStateInitial:
	GetConnectionPids(pClient) = NULL;
	GetIAClient(pClient)->wmgr = FALSE;
	break;

    default:
	break;
    }
} 


static int
ProcIADispatch (ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data)
    {
    case X_IAQueryVersion:
	return ProcIAQueryVersion(client);
    case X_IASetProcessInfo:
	return ProcIASetProcessInfo(client);
    case X_IAGetProcessInfo:
	return ProcIAGetProcessInfo(client);
    default:
	return BadRequest;
    }
}

static int
ProcIAQueryVersion(ClientPtr client)
{
    REQUEST(xIAQueryVersionReq);
    xIAQueryVersionReply rep;

    REQUEST_SIZE_MATCH(xIAQueryVersionReq);
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    rep.majorVersion = IA_MAJOR_VERSION;
    rep.minorVersion = IA_MINOR_VERSION;
    WriteToClient(client, sizeof(xIAQueryVersionReply), (char *)&rep);
    return (client->noClientException);
}

static int
ProcIASetProcessInfo(ClientPtr client)
{

    REQUEST(xIASetProcessInfoReq);
    register int length;
    static uid_t ServerUid = (uid_t)-1;

    REQUEST_AT_LEAST_SIZE(xIASetProcessInfoReq);

    if (ServerUid == (uid_t)-1)
	ServerUid=getuid();

    if ((stuff->flags & INTERACTIVE_INFO) && 
	(stuff->uid==ServerUid || ServerUid==0 || stuff->uid==0) &&
	LocalClient(client)) {
	length=stuff->length-(sizeof(xIASetProcessInfoReq)>>2);
	SetClientPrivate(client, (ConnectionPidPtr)&stuff[1], length);
	ChangeInteractive(client);
    }

    if ((stuff->flags & INTERACTIVE_SETTING) && 
	(stuff->uid==ServerUid || ServerUid==0) &&
	LocalClient(client)) {
	SetIAPrivate((int*)&stuff[1]);
    }

    return (client->noClientException);
}

static int
ProcIAGetProcessInfo(ClientPtr client)
{
    ClientProcessPtr CurrentPids=GetConnectionPids(client);
    REQUEST(xIAGetProcessInfoReq);
    xIAGetProcessInfoReply rep;
    register int length=0;
    caddr_t write_back=NULL;

    REQUEST_SIZE_MATCH(xIAGetProcessInfoReq);
    rep.type = X_Reply;
    rep.length = 0;
    rep.sequenceNumber = client->sequence;
    if (stuff->flags & INTERACTIVE_INFO) {
	    if (!CurrentPids) 
    		rep.count = 0;
	    else {
    		rep.count = CurrentPids->count;
       		length = rep.count << 2;
	        write_back=(caddr_t)CurrentPids->pids;
	    }
    }
    if (stuff->flags & INTERACTIVE_SETTING) {
	rep.count=1;
	length=rep.count << 2;
	write_back=(caddr_t)&ia_nice;
    }

    WriteToClient(client, sizeof(xIAGetProcessInfoReply), (char *)&rep);
    WriteToClient(client, length, write_back);
    return (client->noClientException);
}

static void
IACloseDown(ExtensionEntry *ext)
{
    InteractiveOS=xFalse;

    IAUnwrapProcVectors();

    DeleteCallback(&ClientStateCallback, IAClientStateChange, NULL);
}

/* 
   The SProc* functions are here for completeness. They should never get
   called. But since they do the server has to eat the request and
   return thanks for sharing.
*/

/*ARGSUSED*/
static int
SProcIADispatch (ClientPtr client)
{
    REQUEST(xReq);
    switch (stuff->data)
    {
    case X_IAQueryVersion:
	return SProcIAQueryVersion(client);
    case X_IASetProcessInfo:
	return SProcIASetProcessInfo(client);
    case X_IAGetProcessInfo:
	return SProcIAGetProcessInfo(client);
    default:
	return BadRequest;
    }
}

/*ARGSUSED*/
static int
SProcIAQueryVersion(ClientPtr client)
{
    REQUEST_SIZE_MATCH(xIAQueryVersionReq);
    return (client->noClientException);
}
 
/*ARGSUSED*/
static int
SProcIASetProcessInfo(ClientPtr client)
{
    REQUEST(xIASetProcessInfoReq);
    REQUEST_AT_LEAST_SIZE(xIASetProcessInfoReq);

    return (client->noClientException);
}

/*ARGSUSED*/
static int
SProcIAGetProcessInfo(ClientPtr client)
{
    REQUEST(xIAGetProcessInfoReq);
    REQUEST_SIZE_MATCH(xIAGetProcessInfoReq);

    return (client->noClientException);
}

static void
ChangeInteractive(ClientPtr client)
{
   ClientProcessPtr CurrentPids=GetConnectionPids(client);
   register int count;

   if (InteractiveOS==xFalse)
        return;

   if (!CurrentPids || !CurrentPids->pids)
	return;

   count=CurrentPids->count;

   while(count--)
      SetPriority(CurrentPids->pids[count], SET_INTERACTIVE);
}

/*
Loop through pids associated with client. Magically make last focus
group go non-interactive -IA_BOOST.
*/
static void
ChangePriority(register ClientPtr client)
{
   ClientProcessPtr CurrentPids=GetConnectionPids(client);
   register int count;

   /* If no pid info for current client make sure to unset last focus group. */
   /* This can happen if we have a remote client with focus or if the client */
   /* is statically linked or if it is using a down rev version of libX11.   */
   if (!CurrentPids || !CurrentPids->pids) {
	if (LastPids && LastPids->pids) {
	    UnsetLastPriority(LastPids);
	    LastPids=NULL;
	}
	return;
   }

   /* Make sure server or wmgr isn't unset by testing for them */
   /* this way LastPids is never set to point to the server or */
   /* wmgr pid.						       */
   if ((client->index==serverClient->index || 
     GetIAClient(client)->wmgr==xTrue)) {

       if ((specialIAset < BOTH) && CurrentPids->pids) {

	   if (client->index == serverClient->index) {
	       specialIAset |= SERVER;	
	   }
	   else {
	       specialIAset |= WMGR; 
	   }
	   SetPriority(CurrentPids->pids[0], SET_PRIORITY);
       }
       return;
   }

   if (LastPids && LastPids->pids) {
	if (CurrentPids && LastPids && PidSetEqual(CurrentPids, LastPids))
		return;				/*Shortcut. Focus changed
						  between two windows with
						  same pid */
	UnsetLastPriority(LastPids);
   }
  
   count=CurrentPids->count;
   while(count--)
      SetPriority(CurrentPids->pids[count], SET_PRIORITY);
   LastPids=CurrentPids;
}

static void
UnsetLastPriority(ClientProcessPtr LastPids)
{
    register int LastPidcount=LastPids->count;

    while(LastPidcount--)
      	SetPriority(LastPids->pids[LastPidcount], UNSET_PRIORITY);
}

static int
InitializeClass(void)
{
   pcinfo_t  pcinfo;

   /* Get TS class information 					*/ 

   strcpy (pcinfo.pc_clname, "TS");
   priocntl(0, 0, PC_GETCID, (caddr_t)&pcinfo); 
   TScid=pcinfo.pc_cid;

   /* Get IA class information */
   strcpy (pcinfo.pc_clname, "IA");
   if ((priocntl(0, 0, PC_GETCID, (caddr_t)&pcinfo)) == -1)
        return ~Success;
 
   IAClass.pc_cid=pcinfo.pc_cid;
   ((iaparms_t*)IAClass.pc_clparms)->ia_uprilim=IA_NOCHANGE;
   ((iaparms_t*)IAClass.pc_clparms)->ia_upri=IA_NOCHANGE;

   return Success;
}

static int
SetPriority(int pid, int cmd)
{
    pcparms_t pcinfo;
    long	ret;
    gid_t	usr_egid = getegid();

    if ( setegid(0) < 0 )
	Error("Error in setting egid to 0");


    pcinfo.pc_cid=PC_CLNULL;
    if ((priocntl(P_PID, pid, PC_GETPARMS, (caddr_t)&pcinfo)) < 0) {
	if ( setegid(usr_egid) < 0 )
	    Error("Error in resetting egid");

	return ~Success; /* Scary time; punt */
    }

    /* If process is in TS or IA class we can safely set parameters */
    if ((pcinfo.pc_cid == IAClass.pc_cid) || (pcinfo.pc_cid == TScid)) {

       switch (cmd) {
       case UNSET_PRIORITY:
   		((iaparms_t*)IAClass.pc_clparms)->ia_mode=IA_INTERACTIVE_OFF;
		break;
       case SET_PRIORITY:
   		((iaparms_t*)IAClass.pc_clparms)->ia_mode=IA_SET_INTERACTIVE;
		break;
       case SET_INTERACTIVE: 
      /* If this returns true, the process is already in the IA class */
      /* So just return.						   */
		 if ( pcinfo.pc_cid == IAClass.pc_cid)
			return Success;

   		((iaparms_t*)IAClass.pc_clparms)->ia_mode=IA_INTERACTIVE_OFF;
		break;
       }

	if ( priocntl(P_PID, pid, PC_SETPARMS, (caddr_t)&IAClass) == -1 )
	    {
	    ret = ~Success;
	    }
	else
	    {
	    ret = Success;
	    }


	IA_DEBUG(IA_DEBUG_PRIOCNTL,
	{
	    const char *cmdmsg;

	    switch (cmd) {
	    case UNSET_PRIORITY:   cmdmsg = "UNSET_PRIORITY"; break;
	    case SET_PRIORITY:     cmdmsg = "SET_PRIORITY"; break;
	    case SET_INTERACTIVE:  cmdmsg = "SET_INTERACTIVE"; break;
	    default:		   cmdmsg = "UNKNOWN_CMD!!!"; break;
	    }
	    LogMessage(X_INFO, "SolarisIA: SetPriority(%d, %s): %s\n", 
	      pid, cmdmsg, (ret == Success) ? "succeeeded" : "failed");
	});


	if ( setegid(usr_egid) < 0 )
	    Error("Error in resetting egid");

	return ret;
    }

    return ~Success;
}

static void
SetIAPrivate(int * value)
{
	ia_nice=*value;
}

/*****************************************************************************
 * Various utility functions - in Xsun these lived in Xserver/os/process.c
 */

/* In Xsun we used the osPrivate in OsCommPtr, so this was SetOsPrivate. */
static int
SetClientPrivate(ClientPtr client, ConnectionPidPtr stuff, int length)
{	
    ClientProcessPtr	cpp;

    FreeProcessList(client);
	
    cpp = (ClientProcessPtr)xalloc(sizeof(ClientProcessRec));

    if (cpp == NULL)
	return BadAlloc;

    cpp->pids = (ConnectionPidPtr)xalloc(sizeof(ConnectionPidRec)*length);  

    if (cpp->pids == NULL) {
	xfree(cpp);
	return BadAlloc;
    }

    GetConnectionPids(client) = cpp;
    cpp->count = length;
    memcpy(cpp->pids, stuff, sizeof(ConnectionPidRec)*length);
    
    return Success;
}

static void
FreeProcessList(ClientPtr client)
{
    ClientProcessPtr	cpp = GetConnectionPids(client);
    
    if (cpp == NULL)
	return;

    if ( LastPids == cpp )
	LastPids = NULL;

    if (cpp->pids != NULL)
	xfree(cpp->pids);

    xfree(cpp);

    GetConnectionPids(client) = NULL;
}

/*
        Check to see that all in current (a) are in
        last (b). And that a and b have the same number
        of members in the set.
*/
int
PidSetEqual(ClientProcessPtr a, ClientProcessPtr b)
{
        register int currentcount=a->count;
        register int lastcount=b->count;
        int retval;

        if (currentcount != lastcount)  
                return 0; /* definately NOT the same set */

        while(currentcount--) {
            retval=0;
            while(lastcount--)
                if (a->pids[currentcount]==b->pids[lastcount]) {
                        retval=1;
                        break;
                }
            if (retval==0)
                return retval;
            lastcount=b->count;
        }

        return retval;
}


/*****************************************************************************
 * Wrappers for normal procs - in Xsun we modified the original procs directly
 * in dix, but here we wrap them for a small performance loss but a large
 * increase in maintainability and ease of porting to new releases.
 */

static int
IAProcSetInputFocus(ClientPtr client)
{
    int res;
    Window focusID;
    register WindowPtr focusWin;
    REQUEST(xSetInputFocusReq);

    res = (*IASavedProcVector[X_SetInputFocus])(client);
    if ((res != Success) || (InteractiveOS != xTrue))
	return res;

    focusID = stuff->focus;

    switch (focusID) {
      case None:
	focusWin = NullWindow;
	break;
      case PointerRoot:
	focusWin = PointerRootWin;
	break;
      default:
	if (!(focusWin = SecurityLookupWindow(focusID, client,
                                               SecurityReadAccess)))
	    return BadWindow;
    }

    if ((focusWin != NullWindow) && (focusWin != PointerRootWin)) {
	register ClientPtr requestee;
        ClientProcessPtr wmPid=NULL;
        ClientProcessPtr RequesteePids=NULL;
 
        if (wmClientptr)
                wmPid=GetConnectionPids(wmClientptr);
 
        requestee=wClient(focusWin);
        RequesteePids=GetConnectionPids(requestee);
 
        /* if wm is not setting focus to himself */
 
        if (wmPid && RequesteePids && !PidSetEqual(wmPid, RequesteePids))
            ChangePriority(requestee);
        else  {
 
            /* If wm is setting focus to himself and Lastpids exists and
               LastPids pids are valid Unset the priority for the LastPids
               focus group */
 
            if (wmPid && RequesteePids && PidSetEqual(wmPid, RequesteePids))
                if (LastPids && LastPids->pids) {
                    UnsetLastPriority(LastPids);
                    LastPids=NULL;
                }
        }
    }

    return res;
}

static int
IAProcSendEvent(ClientPtr client)
{
    int res;
    REQUEST(xSendEventReq);

    res = (*IASavedProcVector[X_SendEvent])(client);
    if ((res != Success) || (InteractiveOS != xTrue))
	return res;

    if ((InteractiveOS==xTrue) &&
        (client == wmClientptr) &&
        (stuff->event.u.u.type == ClientMessage) &&
        (stuff->event.u.u.detail == 32) ) {
 
        register ClientPtr requestee;
        ClientProcessPtr wmPid=NULL;
        ClientProcessPtr RequesteePids=NULL;
	WindowPtr pWin;

	if (stuff->destination == PointerWindow)
	    pWin = GetSpriteWindow();
	else if (stuff->destination == InputFocus)
	{
	    WindowPtr inputFocus = inputInfo.keyboard->focus->win;

	    if (inputFocus == NoneWin)
		return Success;
	    
	 /* If the input focus is PointerRootWin, send the event to where
	    the pointer is if possible, then perhaps propogate up to root. */
	    if (inputFocus == PointerRootWin)
		inputFocus = GetCurrentRootWindow();
	    
	    if (IsParent(inputFocus, GetSpriteWindow()))
		pWin = GetSpriteWindow();
	    else
		pWin = inputFocus;
	}
	else
	    pWin = SecurityLookupWindow(stuff->destination, client,
	    				SecurityReadAccess);
	if (!pWin)
	    return BadWindow;
 
        if (wmClientptr)
	    wmPid=GetConnectionPids(wmClientptr);
        requestee=wClient(pWin);
        RequesteePids=GetConnectionPids(requestee);
 
        /* if wm is not setting focus to himself */
        if (wmPid && RequesteePids && !PidSetEqual(wmPid, RequesteePids)) {
            ChangePriority(requestee);
	}
	else {
 
            /* If wm is setting focus to himself and Lastpids exists and
               LastPids pids are valid Unset the priority for the LastPids
               focus group */
 
                if (LastPids && LastPids->pids) {
                    UnsetLastPriority(LastPids);
                    LastPids=NULL;
                }
        }
    }
    return res;
}

static Bool
IAProcChangeWindowAttributes(ClientPtr client)
{
    REQUEST(xChangeWindowAttributesReq);

    if ((InteractiveOS==xTrue) && (stuff->valueMask & CWEventMask) &&
	(GetIAClient(client)->wmgr == xFalse) ) {

	register XID *pVlist = (XID *) &stuff[1];
	register Mask tmask = stuff->valueMask;
	register Mask index2 = 0;

	while (tmask) {
	    index2 = (Mask) lowbit (tmask);
	    tmask &= ~index2;
	    if (index2 == CWEventMask) {
		break;
	    }
	    pVlist++;
	}

	if ((index2 == CWEventMask) && (*pVlist & SubstructureRedirectMask)) {
	    IA_DEBUG(IA_DEBUG_BASIC,
	    ClientProcessPtr CurrentPids=GetConnectionPids(client);

	    LogMessage(X_INFO, "SolarisIA: WindowManager detected (pid %d)\n",
	      (CurrentPids && CurrentPids->pids) ? CurrentPids->pids[0] : -1));


	    GetIAClient(client)->wmgr=xTrue;
	    wmClientptr = client;
	    ChangePriority(client);
	}
    }

    return (*IASavedProcVector[X_ChangeWindowAttributes])(client);
}


static int 
IAWrapProcVectors(void)
{
    IASavedProcVector[X_SetInputFocus] = ProcVector[X_SetInputFocus];
    ProcVector[X_SetInputFocus] = IAProcSetInputFocus;

    IASavedProcVector[X_SendEvent] = ProcVector[X_SendEvent];
    ProcVector[X_SendEvent] = IAProcSendEvent;

    IASavedProcVector[X_ChangeWindowAttributes] 
      = ProcVector[X_ChangeWindowAttributes];
    ProcVector[X_ChangeWindowAttributes] = IAProcChangeWindowAttributes;

    return 0;
}

static int 
IAUnwrapProcVectors(void)
{
    ProcVector[X_SetInputFocus] = IASavedProcVector[X_SetInputFocus];
    ProcVector[X_SendEvent] = IASavedProcVector[X_SendEvent];
    ProcVector[X_ChangeWindowAttributes] = IASavedProcVector[X_ChangeWindowAttributes];

    return 0;
}


