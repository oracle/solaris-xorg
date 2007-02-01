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

#pragma ident "@(#)dtlogin.c	1.17	07/01/31   SMI" 

/* Implementation of DTLogin to Xsun communication pipe.
 * The Solaris Desktop Login process (dtlogin) will start 
 * the X window server at system boot time before any user 
 * has logged into the system.  The X server is by default 
 * started as the root UID "0".
 
 * At login time the Xserver local communication pipe is provided 
 * by Xsun for user specific configuration data supplied 
 * by DTLogin.  It notifies the Xserver it needs to change 
 * over to the user's credentials (UID, GID, GID_LIST) and
 * also switch CWD (current working directory) of to match 
 * the user's CWD home.
 * ASARC case 1995/390
 */

#include <X11/Xos.h>
#include <sys/param.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <X11/X.h>
#include <X11/Xmd.h>
#include "misc.h"
#include <X11/Xpoll.h>
#include "osdep.h"
#include "input.h"
#include "dixstruct.h"
#include "dixfont.h"
#include "opaque.h"
#include <pwd.h>
#include <project.h>
#include <sys/task.h>
#include <ctype.h>
#include "scrnintstr.h"
#ifdef XSUN
#include "sunIo.h"
#endif

#define DTLOGIN_PATH "/var/dt/sdtlogin/"

#define BUFLEN 1024

uid_t	root_euid;

extern char *display;

/* in Xserver/os/auth.c */
extern const char *GetAuthFilename(void);

static int  dtloginSocket = -1;

extern Bool noXkbExtension;
extern int *argcGlobalP;
extern char ***argvGlobalP;

static void DtloginBlockHandler(pointer, struct timeval **, pointer);
static void DtloginWakeupHandler(pointer, int, pointer);
static int  dtlogin_create_pipe(int);
static void dtlogin_receive_packet(void);
static void dtlogin_close_pipe(void);

#define DtloginError(str)	Error(str)
#define DtloginFatal(str)	FatalError("%s\n", str)

#ifndef XSUN
# define DtloginInfo(fmt, arg)	LogMessageVerb(X_INFO, 5, fmt, arg)
#elif defined(DEBUG)
# define DtloginInfo(fmt, arg)	fprintf(stderr, fmt, arg)
#else
# define DtloginInfo(fmt, arg)	/* Do nothing */
#endif

/*
 * initialize DTLOGIN: create pipe; set handlers.
 * Called from CreateWellKnownSockets in os/connection.c
 */

void
DtloginInit(void)
{
    int displayNumber = 0;

    root_euid = geteuid();

    if ( getuid() != 0 )  return;
    displayNumber = atoi(display); /* Assigned in dix/main.c */

    if (dtlogin_create_pipe(displayNumber)) 
        RegisterBlockAndWakeupHandlers (DtloginBlockHandler, 
            DtloginWakeupHandler, (pointer) 0);

    return;
}

static void
DtloginBlockHandler(data, wt, pReadmask)
    pointer         data;   /* unused */
    struct timeval  **wt;
    pointer         pReadmask;
{
    fd_set *LastSelectMask = (fd_set*)pReadmask;

    if ( dtloginSocket == -1 ) return;
 
    FD_SET(dtloginSocket, LastSelectMask);
}

static void
DtloginWakeupHandler(data, i, pReadmask)
    pointer data;   /* unused */
    int     i;
    pointer pReadmask;
{
    fd_set* LastSelectMask = (fd_set*)pReadmask;

    if ( dtloginSocket == -1 ) return;

    if (i > 0)
    {
        if (FD_ISSET(dtloginSocket, LastSelectMask))
        {
            dtlogin_receive_packet();
            FD_CLR(dtloginSocket, LastSelectMask);
            dtlogin_close_pipe();
        }
    }
}

static int
dtlogin_create_pipe( int port )
{
    struct stat statbuf;
    char pipename[128];


    if ( stat(DTLOGIN_PATH, &statbuf) < 0 ) /* No DTLOGIN_PATH */
        return -1;

    if ( (statbuf.st_mode & S_IFMT) != S_IFDIR )
        return -1;  /* DTLOGIN_PATH is not a directory */

    snprintf(pipename, sizeof(pipename), "%s%d", DTLOGIN_PATH, port );

    if (mknod(pipename, S_IFIFO | S_IRUSR | S_IWUSR, 0 ) < 0 ) 
        return -1;

    /* To make sure root has rw permissions. */
    (void) chmod(pipename, 0600);
 
    if ((dtloginSocket = open(pipename, O_RDWR)) < 0 ) 
        return -1;

    return 1;
}

static void dtlogin_close_pipe(void)
{
    RemoveBlockAndWakeupHandlers (DtloginBlockHandler, 
                     DtloginWakeupHandler,0);
    close(dtloginSocket);
    dtloginSocket = -1;
}

static void
dtlogin_receive_packet(void)
{
    /* contains characters to be processed */
    char *buf;

    /* temporary vars */
    char *s, *t, *tGID, *tUID, *tHOME, *tG_LIST_ID, *tEOF;
    char *tXSERVERFLAGS;
    int tokLen = 0;

    int ctr = 0; int aposFlag = FALSE;
    int testFlag = TRUE;
    int testFlagUID, testFlagGID, testFlagHOME, testFlagEOF, testFlagG_LIST_ID;
    int testFlagXSERVERFLAGS;

    int uid, gid, gid_cnt = 0;
    char *Home = NULL, *env_str = NULL;
    const char *auth_file = NULL;
    gid_t groupids[NGROUPS_UMAX];


    int To = 0; int From = 0;

    int notEvenOnce = TRUE;

    buf = (char *) xalloc(BUFLEN);

    while (1) {
        if (s = (char *) strstr(buf, ";")) {
            int l;

            l = strlen(buf) - strlen(s);
            
            /* ; exists, get the token */
            s = (char *) strtok(buf, ";");
            t = (char *) strdup(s);

            /* save stuff for next iteration */
            strcpy(buf, buf + l + 1);
        }
        else {
            t = (char *) NULL;
        }

        if (t) {
            tokLen = strlen(t);  
            testFlag = TRUE;
            while (testFlag == TRUE) {
                /* look for GID */
                if (tGID = (char *) strstr(t, "GID=\"")) {
    
                    testFlagGID = TRUE;
                    gid = atoi(tGID + 5);
                    To = tokLen - strlen(tGID);
    
                    aposFlag = FALSE; ctr = 0;
                    while (tGID[ctr]) {
                        if (tGID[ctr] == '\"' && aposFlag == FALSE) {
                            aposFlag = TRUE;
                            ctr++;
                            continue;
                        }
                        else if (tGID[ctr] == '\"' && aposFlag == TRUE) {
                            ctr++;
                            break;
                        }
                        ctr++;
                    }
                    /* printf("ctr=%d\n", ctr); */
                    From = To + ctr;
                    strcpy(t + To, t + From);
    
                    /* update tokLen */
                    tokLen -= ctr;
                }
                else
                    testFlagGID = FALSE;
   
                /* look for UID */
                if (tUID = (char *) strstr(t, "UID=\"")) {
                    testFlagUID = TRUE;
                    uid =  atoi(tUID + 5);
                    To = tokLen - strlen(tUID);
    
                    aposFlag = FALSE; ctr = 0;
                    while (tUID[ctr]) {
                        if (tUID[ctr] == '\"' && aposFlag == FALSE) {
                            aposFlag = TRUE;
                            ctr++;
                            continue;
                        }
                        else if (tUID[ctr] == '\"' && aposFlag == TRUE) {
                            ctr++;
                            break;
                        }
                        ctr++;
                    }
                    /* printf("ctr=%d\n", ctr); */
                    From = To + ctr;
                    strcpy(t + To, t + From);
  
                    /* update tokLen */
                    tokLen -= ctr;
                }
                else
                    testFlagUID = FALSE;
    
                /* look for HOME */
                if (tHOME = (char *) strstr(t, "HOME=\"")) {
                    testFlagHOME = TRUE;
                    Home = (char *) strdup(tHOME+6);
                    To = tokLen - strlen(tHOME);
    
                    aposFlag = FALSE; ctr = 0;
                    while (tHOME[ctr]) {
                        if (tHOME[ctr] == '\"' && aposFlag == FALSE) {
                            aposFlag = TRUE;
                            ctr++;
                            continue;
                        }
                        else if (tHOME[ctr] == '\"' && aposFlag == TRUE) {
                            ctr++;
                            break;
                        }
                        ctr++;
                    }
                    /* printf("ctr=%d\n", ctr); */
                    Home[ctr-7] = '\0';
                    From = To + ctr;
                    strcpy(t + To, t + From);
    
                    /* update tokLen */
                    tokLen -= ctr;
                }
                else
                    testFlagHOME = FALSE;
    
                /* look for G_LIST_ID */
                if (tG_LIST_ID = (char *) strstr(t, "G_LIST_ID=\"")) {
                    testFlagG_LIST_ID = TRUE;
                    groupids[gid_cnt++] = atoi(tG_LIST_ID + 11); 
                    To = tokLen - strlen(tG_LIST_ID);
    
                    aposFlag = FALSE; ctr = 0;
                    while (tG_LIST_ID[ctr]) {
                        if (tG_LIST_ID[ctr] == '\"' && aposFlag == FALSE) {
                            aposFlag = TRUE;
                            ctr++;
                            continue;
                        }
                        else if (tG_LIST_ID[ctr] == '\"' && aposFlag == TRUE) {
                            ctr++;
                            break;
                        }
                        ctr++;
                    }
                    /* printf("ctr=%d\n", ctr); */
                        From = To + ctr;
                        strcpy(t + To, t + From);
    
                        /* update tokLen */
                        tokLen -= ctr;
		}
		else
		    testFlagG_LIST_ID = FALSE;

                /* look for XSERVERFLAGS */
                if (tXSERVERFLAGS = (char *) strstr(t, "XSERVERFLAGS=\"")) {
		    int spaces = 0, content = 0;
                    testFlagXSERVERFLAGS = TRUE;

                    To = tokLen - strlen(tXSERVERFLAGS);
    
                    aposFlag = FALSE; ctr = 0;
                    while (tXSERVERFLAGS[ctr]) {
                        if (tXSERVERFLAGS[ctr] == '\"' && aposFlag == FALSE) {
                            aposFlag = TRUE;
                            ctr++;
                            continue;
                        }
                        else if (aposFlag == TRUE) {
			    if (tXSERVERFLAGS[ctr] == '\"') {
				ctr++;
				break;
			    }
			    else if (isspace(tXSERVERFLAGS[ctr])) {
				spaces++;
			    } 
			    else {
				content++;
			    }
			}		      
                        ctr++;
                    }

#if 0
		    if (content > 0) {
			char **newArgv = (char **) xalloc(sizeof(char *) * 
			  (argcGlobal + spaces + 3));
			int i, j, k, fpset, fpcount, fplength;
			unsigned char *fpes;
			char *fp, *curfp;
			int needToRestart;

			for (i = 0; i < argcGlobal; i++) {
			    newArgv[i] = argvGlobal[i];
			}
			content = 0;
			for (ctr = 14; tXSERVERFLAGS[ctr] != 0; ctr++) {
			    if (isspace(tXSERVERFLAGS[ctr]) ||
				tXSERVERFLAGS[ctr] == '\"') {
				if (content != 0) {
				    newArgv[i] 
				      = (char *) xalloc(ctr-content+1);
				    memcpy(newArgv[i], &tXSERVERFLAGS[content],
				      ctr-content);
				    newArgv[i][ctr-content] = 0;
				    i++;
				}
				if (tXSERVERFLAGS[ctr] == '\"') {
				    break;
				}
				content = 0;
			    } else if (content == 0) {
				content = ctr;
			    }
			}

			/* Check through new arguments to see if we can skip
			   resetting the server */
			needToRestart = FALSE;
			for (j = argcGlobal; 
			     (j < i) && (needToRestart == FALSE); j++) {

			    if ((strcmp(newArgv[j], "-defdepth") == 0) &&
			        ((j + 1) < i) ) {
				/* Don't need to restart if setting default
				   depth to the current default depth. */
				int newDepthArg = atoi(newArgv[++j]);

				for (k=0; k < screenInfo.numScreens; k++) {
				    ScreenPtr pScreen = screenInfo.screens[k];

				    if (pScreen->rootDepth != newDepthArg) {
					needToRestart = TRUE;
				    }

				}				    
			    } else if (strcmp(newArgv[j], "+kb") == 0) {
				if (noXkbExtension) {
				    needToRestart = TRUE;
				}
			    } else if (strcmp(newArgv[j], "-kb") == 0) {
				if (!noXkbExtension) {
				    needToRestart = TRUE;
				}
			    } else {
			    /* Unrecognized argument, assume restart needed */
				needToRestart = TRUE;
			    }
			}

			if (needToRestart == TRUE) {
			    /* Make sure to preserve font path since locale
			     * specific fonts have already been added and if
			     * we don't preserve them, they are lost. (4809632)
			     */
			    fpes = GetFontPath(&fpcount,&fplength);
			    fp = malloc(fplength + fpcount + 1);
			    curfp = fp;
			    for (j = 0; j < fpcount ; j++) {
				unsigned int fpe_length = (unsigned int) *fpes;
				memcpy(curfp, fpes + 1, fpe_length);
				fpes += fpe_length + 1;
				curfp += fpe_length;
				*curfp++ = ',';
			    }
			    *(curfp - 1) = 0;
			    for (j = 0, fpset = 0; (j < i - 1) && !fpset ; j++)
			    {
				if (strcmp(newArgv[j], "-fp") == 0) {
				    newArgv[j+1] = fp;
				    fpset = 1;
				}
			    }
			    if (!fpset) {
				newArgv[i++] = strdup("-fp");
				newArgv[i++] = fp;
			    }
			    newArgv[i] = NULL;
			    argvGlobal = *argvGlobalP = newArgv;
			    argcGlobal = *argcGlobalP = i;
			    dispatchException |= DE_RESET;
			} else {
			    for (j = argcGlobal; (j < i); j++) {
				free(newArgv[j]);
			    }
			    free(newArgv);
			}
		    }
#endif
                    /* printf("ctr=%d\n", ctr); */
                    From = To + ctr;
                    strcpy(t + To, t + From);
    
                    /* update tokLen */
                    tokLen -= ctr;

                }
                else
                    testFlagXSERVERFLAGS = FALSE;

    
                    /* look for EOF */
                    if (tEOF = (char *) strstr(t, "EOF=\"")) {
			struct project proj;
			char proj_buf[PROJECT_BUFSZ];
                        struct passwd *ppasswd;

                        testFlagEOF = TRUE;

                        env_str = (char *) xalloc (strlen(Home)+strlen("HOME=") + 1);

                        if ( !env_str )
                            DtloginFatal("Not enough memory");  
                                /* Memory error */
 
                        auth_file = GetAuthFilename();

                        if (auth_file)
                            if (chown((const char*)auth_file, uid, gid) < 0)
                                DtloginError("Error in changing owner");

			/* This gid dance is necessary in order to make sure
			   our "saved-set-gid" is 0 so that we can regain gid
			   0 when necessary for priocntl & power management.
			   The first step sets rgid to the user's gid and 
			   makes the egid & saved-gid be 0.  The second then
			   sets the egid to the users gid, but leaves the
			   saved-gid as 0.  */

			DtloginInfo("Setting gid to %d\n", gid);
                        if ( setregid(gid,0) < 0 )
                            DtloginError("Error in setting regid");

                        if ( setegid(gid) < 0 )
                            DtloginError("Error in setting egid");

                        if ( setgroups(gid_cnt, groupids) < 0 )
                            DtloginError("Error in setting groups");

			/*
			 * BUG: 4462531: Set project ID for Xserver
			 *               Get user name and default project.
			 *               Set before the uid value is set.
			 */
			ppasswd = getpwuid(uid);
			if ( ppasswd == NULL)
			    DtloginError("Error in getting user name");
			if ( getdefaultproj(ppasswd->pw_name, &proj, 
                                            (void *)&proj_buf, 
                                            PROJECT_BUFSZ) == NULL)
			    DtloginError("Error in getting project id");

			DtloginInfo("Setting project to %s\n", proj.pj_name);
                        if ( setproject(proj.pj_name, ppasswd->pw_name, 
                                        TASK_NORMAL) == -1)
			    DtloginError("Error in setting project");

			DtloginInfo("Setting uid to %d\n", uid);
                        if ( setreuid(uid,uid) < 0 )
                            DtloginError("Error in setting uid");

                        sprintf(env_str, "%s=%s", "HOME", Home);
			DtloginInfo("Setting %s\n",env_str);

                        if ( putenv(env_str) < 0 )
                            DtloginError("Error in setting HOME");  

                        if ( chdir(Home) < 0 )
                            DtloginError("Error in changing working directory");  

                        xfree(buf);
                        return;
			/* exit from program */
                    }
                    else
                        testFlagEOF = FALSE;

                    testFlag = testFlagGID | testFlagUID | testFlagHOME 
                        | testFlagG_LIST_ID | testFlagEOF;
                }
            }
        else {
            /* We didn't find ;, so get more data! */
            int bufLen, nbRead;
            static int totalLen = BUFLEN;

            bufLen = strlen(buf);
            notEvenOnce = TRUE;


	    /*
	     * Realloc only if buf has filled up and we don't have a record
             * delimiter yet. Keep track of alloced size.
             */

            if (bufLen > totalLen/2) {
                buf = (char *) xrealloc(buf, bufLen + BUFLEN);
                totalLen += BUFLEN;
            }
            memset(buf + bufLen, '\0', totalLen - bufLen);
            nbRead = read(dtloginSocket, buf + bufLen, totalLen - bufLen);
        }
    }
}

