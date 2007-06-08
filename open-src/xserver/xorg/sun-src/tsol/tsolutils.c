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

#pragma ident   "@(#)tsolutils.c 1.16     07/06/08 SMI"

#ifdef HAVE_DIX_CONFIG_H 
#include <dix-config.h> 
#endif

#define NEED_EVENTS
#include <stdio.h>
#include <X11/X.h>
#include <X11/Xproto.h>
#include <X11/Xprotostr.h>
#include <bsm/auditwrite.h>
#include <bsm/audit_uevents.h>
#include <regex.h>
#include <priv.h>
#include <X11/Xproto.h>
#include "windowstr.h"
#include "scrnintstr.h"
#include  "tsolinfo.h"
#include  <X11/keysym.h>
#include  "misc.h"
#include  "inputstr.h"
#include  "propertyst.h"

#define	MAX_SL_ENTRY	256
#define	MAX_UID_ENTRY	64
#define	ALLOCATED	1
#define	EMPTIED		0
#define	FamilyTSOL	5
#define	TSOLUIDlength	4

#define BOXES_OVERLAP(b1, b2) \
      (!( ((b1)->x2 <= (b2)->x1)  || \
	( ((b1)->x1 >= (b2)->x2)) || \
	( ((b1)->y2 <= (b2)->y1)) || \
	( ((b1)->y1 >= (b2)->y2)) ) )

Bool system_audit_on = FALSE;
Bool priv_win_colormap = FALSE;
Bool priv_win_config = FALSE;
Bool priv_win_devices = FALSE;
Bool priv_win_dga = FALSE;
Bool priv_win_fontpath = FALSE;
TsolInfoPtr dying_tsolinfo = (TsolInfoPtr)NULL;
ExtensionFlag  extflag = { FALSE, FALSE, FALSE, FALSE, FALSE,
                           FALSE, FALSE, FALSE, FALSE, FALSE,
                           FALSE, FALSE, FALSE, FALSE, FALSE,
                           FALSE, FALSE, FALSE, FALSE, FALSE,
                           FALSE, FALSE, FALSE, FALSE, FALSE,
                           FALSE };

/*
 * The following need to be moved to tsolextension.c
 * after all references in Xsun is pulled out
 */
WindowPtr tpwin = NULL;	   /* only one trusted path window at a time */
TsolPolyInstInfoRec tsolpolyinstinfo;
#define TsolMaxPolyNameSize 80
/*
 * Use the NodeRec struct in tsolinfo.h. This is referenced
 * in policy routines. So we had to move it there
 */
TsolPolyAtomRec tsolpolyprop = {FALSE, 0, 0, NULL};
TsolPolyAtomRec tsolpolyseln = {TRUE, 0, 0, NULL};


/*
 * Private indices
 */
int tsolClientPrivateIndex = -1;
int tsolWindowPrivateIndex = -1;
int tsolPixmapPrivateIndex = -1;

bclear_t SessionHI;	   /* HI Clearance */
bclear_t SessionLO;	   /* LO Clearance */
unsigned int StripeHeight = 0;
uid_t OwnerUID = (uid_t)(-1);
bslabel_t PublicObjSL;

Atom tsol_lastAtom = None;
int tsol_nodelength  = 0;
TsolNodePtr tsol_node = NULL;

/* This structure is used for protocol request ListHosts */
struct xUIDreply
{
	unsigned char	family;
	unsigned char	pad;
	unsigned short	length;
	int		uid;		/* uid type */
};

struct slentry
{
	bslabel_t	senlabel;
	char		allocated;
};

static struct slentry sltable[MAX_SL_ENTRY];

/* This table contains list of users who can connect to the server */
struct uidentry
{
	int		userid;		/* uid type */
	char		allocated;
};

static struct uidentry uidtable[MAX_UID_ENTRY];

/* Index must match with keywords */
static char *tsolconfig_keywords[] = {"atom", "property", "selection",
	"extension", "privilege"};

#define KEYWORDCOUNT sizeof(tsolconfig_keywords)/sizeof(char *)

typedef struct _TsolConfig
{
	int count;
	char **list;
} TsolConfigRec;

TsolConfigRec tsolconfig[KEYWORDCOUNT] = {
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL},
	{0, NULL}
};

#define TSOL_ATOMCOUNT 4
static char *tsolatomnames[TSOL_ATOMCOUNT] = {
	"_TSOL_CMWLABEL_CHANGE",
	"_TSOL_GRABNOTIFY",
	"_TSOL_CLIENT_TERM",
	"_TSOL_SEL_AGNT"
};

#ifdef DEBUG
/*
 * selectively enable debugging. upto 32 levels of debugging supported
 * flags are defined in tsolinfo.h
 */
#define	TSOL_ERR_FILE "/tmp/Xsun.err"	/* error log file */
unsigned long tsoldebug = 0; /* TSOLD_SELECTION; */
#endif /* DEBUG */

void
init_TSOL_cached_SL()
{
	sltable[0].allocated = ALLOCATED;
	bsllow (&(sltable[0].senlabel));

	sltable[1].allocated = ALLOCATED;
	bslhigh(&(sltable[1].senlabel));

}

/* Initialize UID table, this table should at least contains owner UID */
void
init_TSOL_uid_table()
{
	uidtable[0].allocated = ALLOCATED;
	uidtable[0].userid    = 0;
}

/*  Count how many valid entried in the uid table */
int
count_uid_table()
{
	int i, count = 0;

	/* Search entire table */
	for (i = 0; i < MAX_UID_ENTRY; i++)
	{
		if (uidtable[i].allocated == ALLOCATED)
			count++;
	}
	return (count);
}

/* return (1); if userid is in the table */
int
lookupUID(userid)
	int userid;
{
	int i;
	for (i = 0; i < MAX_UID_ENTRY; i++)
	{
		if (uidtable[i].allocated == ALLOCATED &&
			uidtable[i].userid == userid)
		{
			return (1); /* yes, found it */
		}
	}
	return (0); /* not found */
}

/* Passed into a pointer to a storage which is used to store UID */
/* and nUid represents how many UID in the table(returned by count_uid_table) */
int
ListUID(uidaddr, nUid)
	struct xUIDreply 	* uidaddr;
	int			nUid;
{
	int i, j = 0;

	for (i = 0; i < MAX_UID_ENTRY; i++)
	{
		if (uidtable[i].allocated == ALLOCATED)
		{
			uidaddr[j].family = FamilyTSOL;
			uidaddr[j].length = TSOLUIDlength;
			uidaddr[j].uid    = uidtable[i].userid;
			j++;
		}
	}
	if (nUid != j)
	{
		ErrorF("Invalid no. of uid entries? \n");
		return (0);
	}

	return (1);
}

/* add userid into UIDtable  */
int
AddUID(userid)
	int *userid;
{

	int i = 0;

	/*
	 * Search entire uidtable, to prevent duplicate uid
	 * entry in the table
	 */
	while (i < MAX_UID_ENTRY)
	{
		if ((uidtable[i].allocated == ALLOCATED) &&
			(uidtable[i].userid == *userid))
		{
			/* this uid entry is already in the table; no-op */
			return (1); /* Success, uid in the table */
		}
		i++;
	}

	i = 0;
	/*
	 * If we can find an empty entry, then add this uid
	 * into the table
	 */
	while (i < MAX_UID_ENTRY)
	{
		if (uidtable[i].allocated != ALLOCATED)
		{
			uidtable[i].allocated = ALLOCATED;
			uidtable[i].userid = *userid;
			return (1); /* Success, uid in the table */
		}
		i++;
	}

	/* uidtable overflow */
	ErrorF("Server problem: Please enlarge the table size of uidtable \n");
	return (0);
}


/* remove userid from UIDtable */
int
RemoveUID(userid)
	int *userid;
{
	int i = 0;

	if (*userid == 0)
	{
		ErrorF("\n UID 0 can not be removed from server UID list");
		return (0);
	}

	while (i < MAX_UID_ENTRY)
	{
		if ((uidtable[i].allocated == ALLOCATED) &&
			(uidtable[i].userid == *userid))
		{
			/* delete this entry in the table */
			uidtable[i].allocated = EMPTIED;
			return (1); /* Success, uid in the table */

		}
		i++;
	}

	/* no such entry in the table, why delete it? no-op */
	return (0);
}




bslabel_t *
lookupSL_low()
{
	return (&(sltable[0].senlabel));
}


bslabel_t *
lookupSL(slptr)
bslabel_t *slptr;
{
	int i = 0;

	if (slptr == NULL)
		return (slptr);

	while ((i < MAX_SL_ENTRY) && sltable[i].allocated == ALLOCATED)
	{
		if (blequal(slptr, &(sltable[i].senlabel)))
		{
			/* found a matching sensitivity label in sltable */
			return (&(sltable[i].senlabel));
		}
		i++;
	}

	if (i < MAX_SL_ENTRY)
	{
		/*
		 * can't find a matching entry in sltable,
		 * however, we have empty entry to store this
		 * new sensitivity label; store it.
		 */
		sltable[i].allocated = ALLOCATED;
		memcpy (&(sltable[i].senlabel), slptr, sizeof (bslabel_t));
		return (&(sltable[i].senlabel));
	}

	/*
	 * no matching entry in sltable, and no room to
	 * store this new sensitivity label,
	 * the server needs to recomplie with a larger slabel
	 */

	ErrorF("Server problem: Please enlarge the table size of sltable \n");
	return (0);
}

int
DoScreenStripeHeight(screen_num)
	int screen_num;
{
	extern char 	*ConnectionInfo;
	extern int	connBlockScreenStart;
	int 		i, j;
	xWindowRoot 	*root;
	register xDepth *pDepth;
	ScreenPtr	pScreen;

	root = (xWindowRoot *)(ConnectionInfo + connBlockScreenStart);
	for (i = 0; i < screen_num; i++)
	{
		pDepth = (xDepth *)(root + 1);
		for (j = 0; j < (int)root->nDepths; j++)
		{
			pDepth = (xDepth *)(((char *)(pDepth + 1)) +
				pDepth->nVisuals * sizeof (xVisualType));
		}
		root = (xWindowRoot *) pDepth;

	}

	pScreen = screenInfo.screens[screen_num];
	root->pixHeight = pScreen->height - StripeHeight;

	/* compute new millimeter height */

	root->mmHeight = (pScreen->mmHeight * root->pixHeight +
			((int)(pScreen->height)/2))/(int)(pScreen->height);

	return (0);
}
void
init_xtsol()
{
	extern Bool system_audit_on;
	extern bslabel_t	PublicObjSL;
	extern bclear_t SessionHI;	/* HI Clearance */
	extern bclear_t SessionLO;	/* LO Clearance */
	extern int cannot_audit(int);	/* bsm function */

	bclearhigh(&SessionHI);
	bclearlow(&SessionLO);
	bsllow(&PublicObjSL);
	init_TSOL_cached_SL();
	init_TSOL_uid_table();

	if (cannot_audit(TRUE))
		system_audit_on = FALSE;
	else
		system_audit_on = TRUE;

	auditwrite(AW_QUEUE, XAUDIT_Q_SIZE, AW_END);
}

/*
 * Converts keycode to keysym, helper function.
 * Modelled after Xlib code
 */
static KeySym
KeycodetoKeysym(KeyCode keycode, int col)
{
    KeySymsPtr curKeySyms = &inputInfo.keyboard->key->curKeySyms;
    int per = curKeySyms->mapWidth;
    KeySym *syms = curKeySyms->map;
    KeySym lsym, usym;

    if ((col < 0) || ((col >= per) && (col > 3)) ||
	((int)keycode < curKeySyms->minKeyCode) || 
        ((int)keycode > curKeySyms->maxKeyCode))
      return NoSymbol;

    syms = &curKeySyms->map[(keycode - curKeySyms->minKeyCode) * per];
    if (col < 4) {
	if (col > 1) {
	    while ((per > 2) && (syms[per - 1] == NoSymbol))
		per--;
	    if (per < 3)
		col -= 2;
	}
	if ((per <= (col|1)) || (syms[col|1] == NoSymbol)) {
	    if (!(col & 1))
		return lsym;
	    else if (usym == lsym)
		return NoSymbol;
	    else
		return usym;
	}
    }
    return syms[col];
}

/*
 * Converts keysym to a keycode
 * Modelled after Xlib code
 */
static KeyCode
KeysymToKeycode(KeySym ks)
{
    int i, j;
    KeySymsPtr curKeySyms = &inputInfo.keyboard->key->curKeySyms;

    for (j = 0; j < curKeySyms->mapWidth; j++) {
	for (i = curKeySyms->minKeyCode; i <= curKeySyms->maxKeyCode; i++) {
	    if (KeycodetoKeysym((KeyCode) i, j) == ks)
		return i;
	}
    }
    return 0;
}

/*
 * converts a keysym to modifier equivalent mask
 * Modelled after Xlib
 */
static unsigned
KeysymToModifier(KeySym ks)
{
    CARD8 code, mods;
    KeySym *kmax;
    KeySym *k;
    KeySymsPtr keysyms = &inputInfo.keyboard->key->curKeySyms;
    KeyClassPtr key = inputInfo.keyboard->key;

    kmax = keysyms->map + (keysyms->maxKeyCode - keysyms->minKeyCode + 1) *
	keysyms->mapWidth;
    k = keysyms->map;
    mods = 0;
    while (k < kmax) {
        if (*k == ks ) {
            int j = key->maxKeysPerModifier << 3;

            code = (((k - keysyms->map) / keysyms->mapWidth) + keysyms->minKeyCode);

            while (--j >= 0) {
                if (code == key->modifierKeyMap[j])
                    mods |= (1 << (j / key->maxKeysPerModifier));
            }
        }
        k++;
    }
    return mods;
}

/*
 * Initialize Hot Key keys. On A Sun type 5/6 keyboard
 * It's Meta(Diamond) + Stop. On a non-Sun keyboard, it's
 * Alt + Break(Pause) key. Hold down the meta or alt key
 * press stop or break key.
 *
 * NOTE:
 * Both Left & Right keys for (Meta or Alt) return the
 * same modifier mask
 */
void
InitHotKey(HotKeyPtr hk)
{
	/* Meta + Stop */
	hk->shift = KeysymToModifier(XK_Meta_L);
	hk->key = KeysymToKeycode(XK_L1);

	/* Alt + Break/Pause */
	hk->altshift = KeysymToModifier(XK_Alt_L);
	hk->altkey = KeysymToKeycode(XK_Pause);

	hk->initialized = TRUE;
}
void
UpdateTsolConfig(char *keyword, char *value)
{
	int i;
	int count;
	char **newlist;

	if (keyword == NULL || value == NULL)
		return; /* ignore incomplete entries */

	/* find a matching keyword */
	for (i = 0; i < KEYWORDCOUNT; i++) {
		if (strcmp(keyword, tsolconfig_keywords[i]) == 0) {
			break;
		}
	}

	/* Invalid keyword */
	if (i >= KEYWORDCOUNT) {
		ErrorF("Invalid keyword : %s\n", keyword);
		return;
	}

	count = tsolconfig[i].count;
	newlist = (char **)Xrealloc(tsolconfig[i].list, (count + 1) * sizeof(char **));
	if (newlist == NULL) {
		ErrorF("Not enough memory for %s %s\n", keyword, value);
		return;
	}

	newlist[count] = strdup(value);
	tsolconfig[i].list = newlist;
	tsolconfig[i].count++;
}

void
InitPrivileges()
{
	int i;
	int count;
	char **list;

	count = tsolconfig[TSOL_PRIVILEGE].count;
	list = tsolconfig[TSOL_PRIVILEGE].list;

	for (i = 0; i < count; i++) {
		if (strcmp(list[i], PRIV_WIN_COLORMAP) == 0)
			priv_win_colormap = TRUE;
		else if (strcmp(list[i], PRIV_WIN_CONFIG) == 0)
			priv_win_config = TRUE;
		else if (strcmp(list[i], PRIV_WIN_DEVICES) == 0)
			priv_win_devices = TRUE;
		else if (strcmp(list[i], PRIV_WIN_FONTPATH) == 0)
			priv_win_fontpath = TRUE;
		else if (strcmp(list[i], PRIV_WIN_DGA) == 0)
			priv_win_dga = TRUE;
	}
}

/*
 * Load Trusted Solaris configuration file
 * TBD: Process extension keywords
 */
void
LoadTsolConfig()
{
	FILE *fp;
	char buf[BUFSIZ];
	char *keyword;
	char *value;

	/* open the file from /etc first followed by /usr */
	if ((fp = fopen(TSOLPOLICYFILE, "r")) == NULL) {
		ErrorF("Cannot load %s. Some desktop applications may not\n"
			"work correctly\n", TSOLPOLICYFILE);
			return;
	}

	/* Read and parse the config file */
	while (fgets(buf, sizeof (buf), fp) != NULL) {

		/* ignore all comments, lines starting with white space */
		if (buf[0] == '#' || isspace((int)buf[0]))
			continue;

		keyword = strtok(buf, " \t");
		value = strtok(NULL, " \t\n");
		UpdateTsolConfig(keyword, value);
	}

	InitPrivileges();
}


/*
 *	It does not really tell if this atom is to be polyinstantiated
 *	or not. Further check should be done to determine this.
 */
int
SpecialName(char *string, int len)
{

	return (MatchTsolConfig(string, len));
}


void
MakeTSOLAtoms()
{
	int i;
	char *atomname;
	Atom a;

	/* Create new TSOL atoms */
	for (i = 0; i < TSOL_ATOMCOUNT; i++) {
		if (MakeAtom(tsolatomnames[i], strlen(tsolatomnames[i]), TRUE) == None)
			AtomError();
	}

	/* Create atoms defined in config file */
	for (i = 0; i < tsolconfig[TSOL_ATOM].count; i++) {
		atomname = tsolconfig[TSOL_ATOM].list[i];
		if (MakeAtom(atomname, strlen(atomname), TRUE) == None) {
			AtomError();
		}
	}
}

/*
 *	Names starting with a slash in selection.atoms and property.atoms
 *	are treated as regular expressions to be matched against the 
 *	selection and property names.  They may optionally end with a slash.
 */
int
regexcompare(char *string, int len, char *regexp)
{
	int	status;
	regex_t	compiledregexp;
	char	*regexpstrp;
	int	regexpstrlen;
	char	buffer[BUFSIZ];

	if (regexp[0] == '/' && len < BUFSIZ) {
		/* Extract regular expression from between slashes */
		regexpstrp = regexp + 1;
		regexpstrlen = strlen(regexpstrp);
		if (regexpstrp[regexpstrlen - 1] == '/')
			regexpstrp[regexpstrlen - 1] = '\0';
		/* Compile the regular expression */
		status = regcomp(&compiledregexp, regexpstrp,
		    REG_EXTENDED | REG_NOSUB);
		if (status == 0) {
			/* Make null-terminated copy of string */
			memcpy(buffer, string, len);
			buffer[len] = '\0';
			/* Compare string to regular expression */
			status = regexec(&compiledregexp,
			    buffer, (size_t) 0, NULL, 0);
			regfree(&compiledregexp);

			if (status == 0)
				return (TRUE);
			else
				return (FALSE);
		}
	} else if (strncmp(string, regexp, len) == 0) {
		return (TRUE);
	}

	return (FALSE);
}

int
MatchTsolConfig(char *name, int len)
{
	int i;
	int count;
	char **list;
	unsigned int flags = 0;

	count = tsolconfig[TSOL_PROPERTY].count;
	list = tsolconfig[TSOL_PROPERTY].list;
	for (i = 0; i < count; i++) {
		if (regexcompare(name, len, list[i])) {
			flags |= TSOLM_PROPERTY;
			break;
		}
	}

	count = tsolconfig[TSOL_SELECTION].count;
	list = tsolconfig[TSOL_SELECTION].list;
	for (i = 0; i < count; i++) {
		if (regexcompare(name, len, list[i])) {
			flags |= TSOLM_SELECTION;
			break;
		}
	}

	return (flags);
}

TsolInfoPtr
GetClientTsolInfo(client)
    ClientPtr client;
{
	return	(TsolInfoPtr)(client->devPrivates[tsolClientPrivateIndex].ptr);
}

/* Property is polyinstantiated only on root window */
int
PolyProperty(Atom atom, WindowPtr pWin)
{
	if (WindowIsRoot(pWin) && 
		((!tsolpolyprop.polyinst && !(tsol_node[atom].IsSpecial & TSOLM_PROPERTY)) || 
		(tsolpolyprop.polyinst && (tsol_node[atom].IsSpecial & TSOLM_PROPERTY))))
		return TRUE;
	return FALSE;
}

int
PolySelection(Atom atom)
{
	if ((tsolpolyseln.polyinst && (tsol_node[atom].IsSpecial & TSOLM_SELECTION)) || 
		(!tsolpolyseln.polyinst && !(tsol_node[atom].IsSpecial & TSOLM_SELECTION)))
		return TRUE;
	return FALSE;
}

/*
 * Returns true if  a matching sl.uid pair found. Must be applied
 * only to polyprops.
 */
int
PolyPropReadable(PropertyPtr pProp, ClientPtr client)
{
    TsolPropPtr tsolprop = (TsolPropPtr)(pProp->secPrivate);
    TsolInfoPtr tsolinfo = GetClientTsolInfo(client);

    while (tsolprop)
    {
        if (tsolpolyinstinfo.enabled)
        {
           if (tsolprop->uid == tsolpolyinstinfo.uid &&
               tsolprop->sl == tsolpolyinstinfo.sl)
               return TRUE;
        }
        else
        {
            if (tsolprop->uid == tsolinfo->uid &&
                tsolprop->sl == tsolinfo->sl)
                return TRUE;
        }
        tsolprop = tsolprop->next;
    }
    return FALSE;
}

/*
 * client_private returns true if xid is owned/created by
 * client or is a default server xid
 */
int  
client_private (ClientPtr client, XID xid)
{
	if (same_client(client, xid) || (xid & SERVER_BIT))
		return TRUE;
	else
		return FALSE;
}
/*
 * Same as TopClientWin()
 * except that it returns a Window ID
 * and not a ptr
 */
Window
RootOfClient(WindowPtr pWin)
{
    if (pWin)
    {
	return (TopClientWin(pWin)->drawable.id);
    }
    return (NULL);
}
/*
 * Return root window of pWin
 */
WindowPtr
RootWin(WindowPtr pWin)
{
    if (pWin)
    {
        while (pWin->parent)
            pWin = pWin->parent;
    }
    return (pWin);
}
Window
RootOf(WindowPtr pWin)
{
    if (pWin)
    {
        while (pWin->parent)
            pWin = pWin->parent;
        return (pWin->drawable.id);
    }
    return (NULL);
}
	

/*
 * same_client returns true if xid is owned/created by
 * client
 */
int  
same_client (ClientPtr client, XID xid)
{
	TsolInfoPtr tsolinfo_client;
	TsolInfoPtr tsolinfo_xid;
	ClientPtr   xid_client;

	if (CLIENT_ID(xid) == 0 || (clients[CLIENT_ID(xid)] == NULL))
		return FALSE;

	if((SERVER_BIT & xid) == 0)
	{
		if (client->index == CLIENT_ID(xid))
			return TRUE;
		xid_client = clients[CLIENT_ID(xid)];
		tsolinfo_client = GetClientTsolInfo(client);
		tsolinfo_xid = GetClientTsolInfo(xid_client);
		if (tsolinfo_client && tsolinfo_xid && tsolinfo_client->pid > 0)
		{
			if (tsolinfo_client->pid == tsolinfo_xid->pid)
				return TRUE;
		}
	}
        return FALSE;
}
WindowPtr
AnyWindowOverlapsJustMe(pWin, pHead, box)
    WindowPtr pWin, pHead;
    register BoxPtr box;
{
    register WindowPtr pSib;
    BoxRec sboxrec;
    register BoxPtr sbox;
    TsolResPtr win_res =
        (TsolResPtr)(pWin->devPrivates[tsolWindowPrivateIndex].ptr);

    for (pSib = pWin->prevSib; (pSib != NULL && pSib != pHead); pSib = pSib->prevSib)
    {
        TsolResPtr sib_res =
            (TsolResPtr)(pSib->devPrivates[tsolWindowPrivateIndex].ptr);
        if (pSib->mapped && !bldominates(win_res->sl, sib_res->sl))
        {
            sbox = WindowExtents(pSib, &sboxrec);
            if (BOXES_OVERLAP(sbox, box)
#ifdef SHAPE
                && ShapeOverlap (pWin, box, pSib, sbox)
#endif
                )
                return(pSib);
        }
    }
    return((WindowPtr)NULL);
}
/*
 * Return Top level client window of pWin
 */
WindowPtr
TopClientWin(WindowPtr pWin)
{
    ClientPtr client;

    if (pWin)
    {
	client = wClient(pWin);
        while (pWin->parent)
	{
	    if (client != wClient(pWin->parent))
		break;
            pWin = pWin->parent;
        }
    }
    return (pWin);
}
/*
 * returns the window under pointer. This is function because
 * sprite is static & TsolPointerWindow is called in policy functions.
 */
WindowPtr
TsolPointerWindow()
{
	return (GetSpriteWindow());	/* Window currently under mouse */
}

/*
 * Matches in the list of disabled extensions via 
 * the policy file (TrustedExtensionsPolicy)
 * Returns
 *  TRUE  - if a match is found
 *  FALSE - otherwise
 */
int
TsolDisabledExtension(char *extname, int extlen)
{
	int i;

	for (i = 0; i < tsolconfig[TSOL_EXTENSION].count; i++) {
		if (strncmp(extname, tsolconfig[TSOL_EXTENSION].list[i], extlen) == 0) {
			return TRUE;
		}
	}

	return FALSE;
}
