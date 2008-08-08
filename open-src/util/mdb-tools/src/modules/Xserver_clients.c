/*
 * Copyright 2008 Sun Microsystems, Inc.  All rights reserved.
 * Use subject to license terms.
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
 * @(#)Xserver_clients.c	1.2	08/08/08
 */

#pragma ident	"@(#)Xserver_clients.c	1.2	08/08/08 SMI"

#include <sys/mdb_modapi.h>
#include "Xserver_headers.h"
#include <X11/extensions/interactive.h>

struct client_walk_data {
    uintptr_t client_next;
    uintptr_t clients_end;
    ClientRec client_data;
};

#ifdef XORG
/* Xsun has these in a header, Xorg has them in sun-src/IA/interactive.c so
   we just copy it here for now.   XXX: Move to header in Xorg too. */
typedef struct _ClientProcessInfo {
    int count;
    ConnectionPidPtr pids;
} ClientProcessRec, * ClientProcessPtr;

typedef struct {
    ClientProcessPtr    process; /* Process id information */    
    Bool                wmgr;
} IAClientPrivateRec, *IAClientPrivatePtr;
#endif

/*
 * Initialize the client walker by either using the given starting address,
 * or reading the value of the server's "clients" pointer.  We also allocate
 * a  for storage, and save this using the walk_data pointer.
 */
static int
client_walk_init(mdb_walk_state_t *wsp)
{
    struct client_walk_data *cwda;
    short max_clients = 128;
#ifdef XSUN
# define MAX_CLIENTS "max_clients"
#else
# define MAX_CLIENTS "currentMaxClients"
#endif
    
    if (wsp->walk_addr == NULL) {
       if (mdb_readvar(&wsp->walk_addr, "clients") == -1) {
	   mdb_warn("failed to read 'clients'");
	   return (WALK_ERR);
       }
       if (mdb_readvar(&max_clients, MAX_CLIENTS) == -1) {
	   mdb_warn("failed to read '%s'", MAX_CLIENTS);
	   return (WALK_ERR);
       } else {
	   mdb_printf("%s = %d\n", MAX_CLIENTS, max_clients);
       }
    }

    wsp->walk_data = mdb_alloc(sizeof (struct client_walk_data), UM_SLEEP);
    cwda = (struct client_walk_data *) wsp->walk_data;
    cwda->clients_end = wsp->walk_addr + (max_clients * (sizeof(void *)));
    return (WALK_NEXT);
}

/*
 * At each step, read a client struct into our private storage, and then invoke
 * the callback function.  We terminate when we reach the end of the clients
 * array.
 */
static int
client_walk_step(mdb_walk_state_t *wsp)
{
	int status;
	uintptr_t clientPtr;
	struct client_walk_data *cwda =
	  (struct client_walk_data *) wsp->walk_data;

	if (wsp->walk_addr == NULL)
		return (WALK_DONE);

	do {
	    if (mdb_vread(&clientPtr, sizeof (void *), wsp->walk_addr) == -1) {
		mdb_warn("failed to read client table entry at %p", wsp->walk_addr);
		return (WALK_DONE);
	    }
	    if (clientPtr == NULL) {
#ifdef DEBUG
		mdb_printf("NULL entry at %p", wsp->walk_addr);
#endif
		wsp->walk_addr += sizeof(void *);
	    }
	    if (wsp->walk_addr >= cwda->clients_end) {
#ifdef DEBUG
		mdb_printf("End of client list at %p", cwda->clients_end);
#endif
		return (WALK_DONE);
	    }
	} while (clientPtr == NULL);

	if (clientPtr == NULL) {
	    return (WALK_DONE);
	}

	if (mdb_vread(&(cwda->client_data), sizeof (ClientRec), (uintptr_t) clientPtr) == -1) {
		mdb_warn("failed to read client data at %p", wsp->walk_addr);
		return (WALK_DONE);
	}

	status = wsp->walk_callback(wsp->walk_addr, &(cwda->client_data),
	  wsp->walk_cbdata);

	wsp->walk_addr = wsp->walk_addr + sizeof(void *);
	return (status);
}

/*
 * The walker's fini function is invoked at the end of each walk.  Since we
 * dynamically allocated a proc_t in client_walk_init, we must free it now.
 */
static void
client_walk_fini(mdb_walk_state_t *wsp)
{
	mdb_free(wsp->walk_data, sizeof (ClientRec));
}

static int
client_pids(uintptr_t addr, uint_t flags, int argc, const mdb_arg_t *argv)
{
    uintptr_t clientP;
    ClientRec client_data;
    
	if (argc != 0)
		return (DCMD_USAGE);

	/*
	 * If no client address was specified on the command line, we can
	 * print out all clients by invoking the walker, using this
	 * dcmd itself as the callback.
	 */
	if (!(flags & DCMD_ADDRSPEC)) {
		if (mdb_walk_dcmd("client_walk", "client_pids",
		    argc, argv) == -1) {
			mdb_warn("failed to walk 'client_walk'");
			return (DCMD_ERR);
		}
		return (DCMD_OK);
	}

	/*
	 * If this is the first invocation of the command, print a nice
	 * header line for the output that will follow.
	 */
	if (DCMD_HDRSPEC(flags))
		mdb_printf("CLIENT SEQUENCE #  FD  PIDS\n");


	if (mdb_vread(&clientP, sizeof (clientP), addr) == sizeof (clientP)) {

	    if (mdb_vread(&client_data, sizeof (client_data), clientP) == sizeof (client_data)) {
		mdb_printf("%5d  %10d", client_data.index, client_data.sequence);

		if (client_data.osPrivate != NULL) {
		    OsCommRec oscomm;

		    if (mdb_vread(&oscomm, sizeof (oscomm),
		      (uintptr_t)client_data.osPrivate) == sizeof (oscomm)) {
			ClientProcessPtr cpp;
#ifdef XORG
			int IAClientPrivateIndex;
#endif
			
			mdb_printf("%4d ", oscomm.fd);

#ifdef XSUN
			cpp = oscomm.process;
#else /* XORG */
			if (mdb_readvar(&IAClientPrivateIndex,
					"libIA.so`IAClientPrivateIndex") == -1) {
			    mdb_warn("failed to read 'IAClientPrivateIndex'");
			} else {
			    DevUnion *cppaddr =	&(client_data.devPrivates[IAClientPrivateIndex]);
			    if (mdb_vread(&cpp, sizeof (cpp), (uintptr_t) cppaddr) != sizeof (cpp)) {
				cpp = NULL;
				mdb_warn("failed to read client_data.devPrivates[IAClientPrivateIndex]");
			    }

			}
#endif			
			
			if (cpp != NULL) {
			    ClientProcessRec cpr;
			    ConnectionPidRec pid;

			    if (mdb_vread(&cpr, sizeof (cpr),
					  (uintptr_t)cpp) == sizeof (cpr)) {
				int i;
				uintptr_t pidP = (uintptr_t) cpr.pids;

				for (i = 0; i < cpr.count; i++, pidP += sizeof(pid)) {
				    if (mdb_vread(&pid, sizeof (pid), pidP) == sizeof (pid)) {
					mdb_printf("%d ", pid);
				    } else {
					mdb_warn("failed to read pid #%d from %p", i, pidP);
				    }
				}
				mdb_printf("\n");

			    } else {
				mdb_warn("failed to read struct ClientProcessRec at %p", client_data.osPrivate);
			    }
			} else {
			    mdb_printf(" ??? - NULL ClientProcessPtr\n");
			}
		    } else {
			mdb_warn("failed to read struct OsCommRec at %p", client_data.osPrivate);
		    }
		} else {
		    mdb_printf(" ??? - NULL ClientPtr->osPrivate\n");
		}
	    } else {
		mdb_warn("failed to read ClientRec at %p", clientP);
	    }
	} else {
	    mdb_warn("failed to read ClientPtr at %p", addr);
	}

	return (DCMD_OK);
}

/*
 * MDB module linkage information:
 *
 * We declare a list of structures describing our dcmds, a list of structures
 * describing our walkers, and a function named _mdb_init to return a pointer
 * to our module information.
 */

static const mdb_dcmd_t dcmds[] = {
	{ "client_pids", NULL, "client process list", client_pids },
	{ NULL }
};

static const mdb_walker_t walkers[] = {
	{ "client_walk", "walk list of clients connected to Xsun",
		client_walk_init, client_walk_step, client_walk_fini },
	{ NULL }
};

static const mdb_modinfo_t modinfo = {
	MDB_API_VERSION, dcmds, walkers
};

const mdb_modinfo_t *
_mdb_init(void)
{
	return (&modinfo);
}
