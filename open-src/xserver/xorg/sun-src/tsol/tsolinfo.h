/* Copyright 2009 Sun Microsystems, Inc.  All rights reserved.
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

#pragma ident   "@(#)tsolinfo.h 1.21     09/02/12 SMI"


#ifndef    _TSOL_INFO_H
#define    _TSOL_INFO_H

#include <sys/types.h>

#include <tsol/label.h>
#include <sys/tsol/tndb.h>
#include <bsm/audit.h>
#include <sys/mkdev.h>
#include <ucred.h>
#include "misc.h"
#include "dixstruct.h"
#include <X11/keysym.h>

/*********************************
 *
 * DEBUG INFO
 *
 *********************************/

/* Message verbosity levels passed to os/log.c functions 
 * Level 0 messages are printed by all servers to stderr.
 * Xorg defaults to logging messages in 0-3 to /var/log/Xorg.<display>.log
 * Ranges of messages to print to stderr can be changed with Xorg -verbose N
 *   and Xephyr -verbosity N
 * Ranges of messages to print to log can be changed with Xorg -logverbose N
 * Other servers don't support runtime configuration of log messages yet.
 */
 
#define TSOL_MSG_ERROR		0		/* Always printed */
#define TSOL_MSG_UNIMPLEMENTED	5
#define TSOL_MSG_POLICY_DENIED	6
#define TSOL_MSG_ACCESS_TRACE	7

#define TSOL_LOG_PREFIX		TSOLNAME ": "
extern const char *TsolDixAccessModeNameString(Mask access_mode);
extern const char *TsolErrorNameString(int req);
extern const char *TsolPolicyReturnString(int pr);
extern const char *TsolRequestNameString(int req);
extern const char *TsolResourceTypeString(RESTYPE resource);

#define MAXNAME            64             /* 63 chars of process name stored */

/*********************************
 *
 * CONSTANTS
 *
 *********************************/


/*
 * X audit events start from 9101 in audit_uevents.h. The first 2 events
 * are non-protocol ones viz. ClientConnect, mapped to 9101 and
 * ClientDisconnect, mapped to 9102.
 * The protocol events are mapped from 9103 onwards in the serial order
 * of their respective protocol opcode, for eg, the protocol UngrabPointer
 * which is has a protocol opcode 27 is mapped to 9129 (9102 + 27).
 * All extension protocols are mapped to a single audit event AUE_XExtension
 * as opcodes are assigined dynamically to these protocols. We set the
 * extension protocol opcode to be 128, one more than the last standard opcode.
 */
#define XAUDIT_Q_SIZE     1024          /* audit queue size for x server */
#define XAUDIT_OFFSET     9102
#define XAUDIT_EXTENSION  128

#define MAX_CLIENT        16
#define MAX_SLS           16            /* used in atom */
#define MAX_POLYPROPS     128           /* used in property */
#define DEF_UID           (uid_t)0      /* uid used for default objects */
#define INVALID_UID       (uid_t)0xFFFF /* invalid uid */
/*
 * Various flags for TsolInfoRec, TsolResRec
 */
#define TSOL_IIL           0x0000001    /* iil changed for window */
#define TSOL_DOXAUDIT      0x0000002    /* write X audit rec if set */
#define TSOL_AUDITEVENT    0x0000004    /* this event mask selected for audit */
#define CONFIG_AUDITED     0x0000008    /* this priv has been asserted for */
#define DAC_READ_AUDITED   0x0000010    /* the same object before */
#define DAC_WRITE_AUDITED  0x0000020
#define MAC_READ_AUDITED   0x0000040
#define MAC_WRITE_AUDITED  0x0000080
#define TRUSTED_MASK	   0x0000100	/* Window has Trusted Path */

/*
 * Polyinstantiated property/selections
 */
#define POLY_SIZE          16           /* increase the list 16 at a time */
#define CONFIG_PRIV_FILE   "config.privs"
#define CONFIG_EXTENSION_FILE "config.extensions"

#define PROCVECTORSIZE (256)

enum tsolconfig_types {
	TSOL_ATOM = 0,
	TSOL_PROPERTY,
	TSOL_SELECTION,
	TSOL_EXTENSION,
	TSOL_PRIVILEGE
};

typedef enum tsolconfig_types tsolconfig_t;

/*
 * Masks corresponding  various types
 */
#define TSOLM_ATOM	1
#define TSOLM_PROPERTY	(1 << 1)
#define TSOLM_SELECTION	(1 << 2)

#define SL_SIZE blabel_size()

/*********************************
 *
 * MACROS
 *
 *********************************/


#define WindowIsRoot(pWin) (pWin && (pWin->parent == NullWindow))
#define DrawableIsRoot(pDraw)\
	(pDraw && (pDraw->id == WindowTable[pDraw->pScreen->myNum]->drawable.id))

/*
 * True if client is part of TrustedPath
 */
#define HasTrustedPath(tsolinfo)\
	(tsolinfo->trusted_path ||\
	(tsolinfo->forced_trust == 1))

#define XTSOLTrusted(pWin) \
    ((TsolWindowPriv(pWin))->flags & TRUSTED_MASK)


/*********************************
 *
 * DATA STRUCTURES
 *
 *********************************/
enum client_types {
	CLIENT_LOCAL,
	CLIENT_REMOTE
};

typedef enum client_types client_type_t;

/*
 * Extended attributes for each client.
 * Most of the information comes from getpeerucred()
 */
typedef struct _TsolInfo {
    uid_t               uid;            /* real user id */
    uid_t               euid;           /* effective user id */
    gid_t               gid;            /* real group id */
    gid_t               egid;           /* effective group id */
    pid_t               pid;            /* process id */
    zoneid_t		zid;		/* zone id */
    priv_set_t          *privs;         /* privileges */
    bslabel_t		*sl;            /* sensitivity label */
    u_long              iaddr;          /* internet addr */
    Bool		trusted_path;	/* has trusted path */
    Bool		priv_debug;	/* do privilege debugging */
    u_long              flags;          /* various flags */
    int                 forced_trust;   /* client masked as trusted */
    au_id_t		auid;		/* audit id */
    au_mask_t		amask;		/* audit mask */
    au_asid_t		asid;         	/* audit session id */
    client_type_t    	client_type;    /* Local or Remote client */
    int			asaverd;
    struct sockaddr_storage saddr;	/* socket information */
    char		pname[MAXNAME];	/* process name for debug messages */
} TsolInfoRec, *TsolInfoPtr;

/*
 * per resource info
 */
typedef struct _TsolRes {
    bslabel_t  *sl;                     /* sensitivity label */
    uid_t       uid;                    /* user id */
    u_long      flags;                  /* various flags */
    pid_t       pid;                    /* who created it */
} TsolResRec, *TsolResPtr;

/*
 * per property info. useful for polyprops
 */
typedef struct _TsolProp {
    bslabel_t         *sl;              /* sensitivity label */
    uid_t              uid;             /* user id */
    long               size;            /* size of data in (format/8) bytes */
    unsigned char     *data;            /* value */
    struct _TsolProp  *next;            /* points to next struct */
    struct _TsolProp  *head;            /* head of poly'd prop list */
    pid_t              pid;             /* who created it */
    int                serverOwned;	/* internally created by the Server */
} TsolPropRec, *TsolPropPtr;

/*
 * per selection info. useful for polyinstantiated selns
 */
typedef struct _TsolSeln {
    bslabel_t  *sl;                     /* sensitivity label */
    uid_t       uid;                    /* user id */
    TimeStamp   lastTimeChanged;
    Window      window;                 /* owner of seln */
    WindowPtr   pWin;                   /* corresponds to the owner win */
    ClientPtr   client;                 /* client that owns the window */
    struct _TsolSeln *next;             /* points to next struct */
    pid_t       pid;                    /* who created it */
} TsolSelnRec, *TsolSelnPtr;

/*
 * information stored in devPrivates
 */
typedef union {
    TsolInfoRec		clientPriv;
    TsolResRec		windowPriv;
    TsolResRec		pixmapPriv;
    TsolPropPtr		propertyPriv;
    TsolSelnPtr		selectionPriv;
} TsolPrivRec, *TsolPrivPtr;

extern DevPrivateKey tsolPrivKey;

#define TsolClientPriv(pClient) \
    ((TsolInfoPtr) dixLookupPrivate(&(pClient)->devPrivates, tsolPrivKey))

#define TsolWindowPriv(pWin)	\
    ((TsolResPtr) dixLookupPrivate(&(pWin)->devPrivates, tsolPrivKey))

#define TsolPixmapPriv(pPix)	\
    ((TsolResPtr) dixLookupPrivate(&(pPix)->devPrivates, tsolPrivKey))

#define TsolPropertyPriv(pProp)	\
    ((TsolPropPtr *) dixLookupPrivate(&(pProp)->devPrivates, tsolPrivKey))

#define TsolSelectionPriv(pSel) \
    ((TsolSelnPtr *) dixLookupPrivate(&(pSel)->devPrivates, tsolPrivKey))

#if 0
/*
 * NodeRec struct defined here is used instead of the
 * one defined in atom.c. This is used in policy functions
 */
typedef struct _Node {
    struct _Node  *left,   *right;
    Atom           a;
    unsigned int   fingerPrint;
    char          *string;
#ifdef TSOL
    int            slsize;              /* size of the sl array below */
    int            clientCount;         /* actual no. of clients */
    int            IsSpecial;           /* special atoms for polyprops */
    bslabel_t    **sl;                  /* an array of sl's. */
#endif /* TSOL */
} NodeRec, *NodePtr;

#endif

#define NODE_SLSIZE	16	/* increase sl array by this amount */
typedef struct _TsolNodeRec {
	unsigned int flags;
	int slcount; 		/* no. of SLs referenced */
	int slsize;		/* size of the sl array */
	int IsSpecial;
	bslabel_t **sl;

} TsolNodeRec, *TsolNodePtr;

/*
 * if polyinst true, the name list is polyinstantiated
 * if false, the everything except the list is polyinstantiated
 * NOTE: Default for seln: polyinstantiate the list
 *       Default for prop: polyinstantiate everything except the list
 */
typedef struct _TsolPolyAtom {
    int     polyinst;
    int     size;                       /* max size of the list */
    int     count;                      /* how many are actually valid */
    char  **name;
} TsolPolyAtomRec, *TsolPolyAtomPtr;

/*
 * PolyInstInfo represents if a get request will match the
 * client's sl,uid for this or it will use the polyinstinfo
 * information to retrieve values for prop/selection
 */
typedef struct _TsolPolyInstInfo {
    int        enabled;                 /* if true use following sl, uid */
    uid_t      uid;
    bslabel_t  *sl;
} TsolPolyInstInfoRec, *TsolPolyInstInfoPtr;


/*
 *  Disable flags for extensions
 */
typedef struct _extensionFlag {
    Bool disableACCESSX;
    Bool disableDPS;
    Bool disableDBE;
    Bool disableDPMS;
    Bool disableEVI;
    Bool disableFBPM;
    Bool disableLBX;
    Bool disableSCREENSAVER;
    Bool disableMITSHM;
    Bool disableMITMISC;
    Bool disableMULTIBUFFER;
    Bool disableSECURITY;
    Bool disableSHAPE;
    Bool disableALLPLANES;
    Bool disableDGA;
    Bool disableOVL;
    Bool disableRECORD;
    Bool disableSYNC;
    Bool disableIA;
    Bool disableCUP;
    Bool disableAPPGROUP;
    Bool disableXCMISC;
    Bool disableXIE;
    Bool disableXINPUT;
    Bool disableXINERAMA;
    Bool disableXTEST;
} ExtensionFlag;


/*
 * Hot Key structure
 * caches keycode/mask for
 * a primary & alternate
 * Hot Keys
 */
typedef struct _HotKeyRec {
	int      initialized;
	KeyCode  key;	/* Primary key */
	unsigned shift;	/* Primary modifier/shift */
	KeyCode	 altkey;	/* Alternate key */
	unsigned altshift;	/* Alternate modifier/shift */
} HotKeyRec, *HotKeyPtr;

/*********************************
 *
 * EXTERNS
 *
 *********************************/


extern  WindowPtr *WindowTable;
extern  int PolyProperty(Atom atom, WindowPtr pWin);
extern  int PolySelection(Atom atom);
extern  TsolPolyInstInfoRec tsolpolyinstinfo;
extern  uid_t OwnerUID;                 /* Workstation owner uid */
extern Bool system_audit_on;

/*********************************
 *
 * FUNCTION PROTOTYPES
 *
 *********************************/


void  TsolReadPolyAtoms(char *filename, TsolPolyAtomPtr polyatomptr);
extern WindowPtr TopClientWin(WindowPtr pWin);
extern WindowPtr RootWin(WindowPtr pWin);
extern Window RootOf(WindowPtr pWin);
extern Window RootOfClient(WindowPtr pWin);
extern int TsolDisabledExtension(const char *extname);
extern int MatchTsolConfig(const char *name, int len);
extern int HasWinSelection(TsolInfoPtr tsolinfo);
extern int same_client (ClientPtr client, XID xid);
extern int client_private (ClientPtr client, XID xid);
extern TsolPropPtr AllocTsolProp(void);
extern TsolPropPtr AllocServerTsolProp(void);
extern bslabel_t *lookupSL_low(void);
extern bslabel_t *lookupSL(bslabel_t *slptr);
extern BoxPtr WindowExtents(WindowPtr pWin, BoxPtr pBox);
extern Bool ShapeOverlap(WindowPtr pWin, BoxPtr pWinBox,
	WindowPtr pSib, BoxPtr pSibBox);

#endif    /* _TSOL_INFO_H */
