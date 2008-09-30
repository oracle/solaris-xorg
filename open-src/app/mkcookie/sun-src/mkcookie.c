/*
*
* mkcookie.c 1.x
*
* Copyright 2004 Sun Microsystems, Inc.  All rights reserved.
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
*/
#pragma ident	"@(#)mkcookie.c	35.13	08/09/30 SMI"

/*
 * $XConsortium: auth.c,v 1.17 89/12/14 09:42:18 rws Exp $
 *
 * Copyright 1988 Massachusetts Institute of Technology
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose and without fee is hereby granted, provided
 * that the above copyright notice appear in all copies and that both that
 * copyright notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in advertising or
 * publicity pertaining to distribution of the software without specific,
 * written prior permission.  M.I.T. makes no representations about the
 * suitability of this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 *
 * Author:  Keith Packard, MIT X Consortium
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/sockio.h>
#include <netinet/in.h>
#include <net/if.h>
#include <X11/X.h>
#include <X11/Xauth.h>
#include <X11/Xos.h>
#include <rpc/rpc.h>
#ifdef SYSV
#include <sys/stropts.h>
#ifndef bcopy
#define bcopy(a,b,c) memmove(b,a,c)
#endif
#ifndef bzero
#define bzero(a,b) memset(a,0,b)
#endif
#ifndef bcmp
#define bcmp(a,b,c) memcmp(a,b,c)
#endif
#ifndef index
#define index(a,b) strchr(a,b)
#endif
#ifndef rindex
#define rindex(a,b) strrchr(a,b)
#endif
#endif

#define AUTH_DATA_LEN	16	/* bytes of authorization data */
/* 1191081:  SIGSEGV when starting openwin - core dumped by mkcookie.
 * becuase d.userAuthDir is not assigned any valid directory where
 * every one will have access to it.
 */
#define DEF_USER_AUTH_DIR "/tmp" /* Backup directory for User Auth file */
  
extern int InitAuth ();
extern Xauth *GetAuth ();
#ifdef X_NOT_STDC_ENV
extern char *malloc(), *realloc();
#else
#include <stdlib.h>
#endif

#define Local 1
#define Foreign	0

typedef struct displayType {
  unsigned int	location:1;
  unsigned int	lifetime:1;
  unsigned int	origin:1;
} DisplayType;

struct display {
  char		*name;		/* DISPLAY name */
  int		authorize;	/* enable authorization */
  Xauth		*authorization;	/* authorization data */
  char		*authFile;	/* file to store authorization in */
  char		*userAuthDir;	/* backup directory for tickets */
  char		*authName;	/* authorization protocol name */
  int	        authNameLen;	/* authorization protocol name len */
  int		resetForAuth;	/* server reads auth file at reset */
  DisplayType   displayType;
  struct sockaddr *peer;
  int           peerlen;
};

struct verify_info {
  int             uid;            /* user id */
  int             gid;            /* group id */
  char            **argv;         /* arguments to session */
  char            **userEnviron;  /* environment for session */
  char            **systemEnviron;/* environment for startup/reset */
};

struct AuthProtocol {
  int       name_length;
  char	    *name;
  int	    (*InitAuth)();
  Xauth	    *(*GetAuth)();
  int	    (*GetXdmcpAuth)();
  int	    inited;
};

struct addrList {
  unsigned short	family;
  unsigned short	address_length;
  char	                *address;
  unsigned short	number_length;
  char	                *number;
  struct addrList	*next;
};

static char	auth_name[256];
static int	auth_name_len;
static struct addrList	*addrs;

static struct AuthProtocol AuthProtocols[] = {
  { (int) 18,	"MIT-MAGIC-COOKIE-1",
      InitAuth, GetAuth, NULL,
    },
  { (int) 9, "SUN-DES-1",
      InitAuth, GetAuth, NULL,
    }
};

static char * makeEnv (name, value)
char    *name;
char    *value;
{
  char  *result;
  
  result = malloc ((unsigned) (strlen (name) + strlen (value) + 2));
  if (!result) {
    perror ("makeEnv");
    return 0;
  }
  sprintf (result, "%s=%s", name, value);
  return result;
}


char ** setEnv (e, name, value)
char    **e;
char    *name;
char    *value;
{
  char    **new, **old;
  char    *newe;
  int     envsize;
  int     l;
  
  l = strlen (name);
  newe = makeEnv (name, value);
  if (!newe) {
    perror ("setEnv");
    return e;
  }
  if (e) {
    for (old = e; *old; old++)
      if (strlen (*old) > l && 
	  !strncmp (*old, name, l) && (*old)[l] == '=')
	break;
    if (*old) {
      free (*old);
      *old = newe;
      return e;
    }
    envsize = old - e;
    new = (char **) realloc ((char *) e,
			     (unsigned) ((envsize + 2) * sizeof (char *)));
  } else {
    envsize = 0;
    new = (char **) malloc (2 * sizeof (char *));
  }
  if (!new) {
    perror ("setEnv");
    free (newe);
    return e;
  }
  new[envsize] = newe;
  new[envsize+1] = 0;
  return new;
}


binaryEqual (a, b, len)
     char	*a, *b;
     int	len;
{
  while (len-- > 0)
    if (*a++ != *b++)
      return 0;
  return 1;
}

#ifdef DEBUG
dumpBytes (len, data)
     int	len;
     char	*data;
{
  int	i;
  
  printf ("%d: ", len);
  for (i = 0; i < len; i++)
    printf ("%02x ", data[i] & 0377);
  printf ("\n");
}

dumpAuth (auth)
     Xauth	*auth;
{
  printf ("family: %d\n", auth->family);
  printf ("addr:   ");
  dumpBytes (auth->address_length, auth->address);
  printf ("number: ");
  dumpBytes (auth->number_length, auth->number);
  printf ("name:   ");
  dumpBytes (auth->name_length, auth->name);
  printf ("data:   ");
  dumpBytes (auth->data_length, auth->data);
}


#endif

checkAddr (family, address_length, address, number_length, number)
     int	family;
     unsigned short	address_length, number_length;
     char	*address, *number;
{
  struct addrList	*a;
  
  for (a = addrs; a; a = a->next) {
    if (a->family == family &&
	a->address_length == address_length &&
	binaryEqual (a->address, address, address_length) &&
	a->number_length == number_length &&
	binaryEqual (a->number, number, number_length))
      {
	return 1;
      }
  }
  return 0;
}

void
saveAddr (family, address_length, address, number_length, number)
     unsigned short	family;
     unsigned short	address_length, number_length;
     char	*address, *number;
{
  struct addrList	*new;
  
  if (checkAddr (family, address_length, address, number_length, number))
    return;
  new = (struct addrList *) malloc (sizeof (struct addrList));
  if (!new) {
    perror ("saveAddr");
    return;
  }
  if ((new->address_length = address_length) > 0) {
    new->address = malloc (address_length);
    if (!new->address) {
      perror ("saveAddr");
      free ((char *) new);
      return;
    }
    bcopy (address, new->address, (int) address_length);
  } else
    new->address = 0;
  if ((new->number_length = number_length) > 0) {
    new->number = malloc (number_length);
    if (!new->number) {
      perror ("saveAddr");
      free (new->address);
      free ((char *) new);
      return;
    }
    bcopy (number, new->number, (int) number_length);
  } else
    new->number = 0;
  new->family = family;
  new->next = addrs;
  addrs = new;
}

writeLocalAuth (file, auth, name)
     FILE	*file;
     Xauth	*auth;
     char	*name;
{
  int	fd;

  setAuthNumber (auth, name);
#ifdef TCPCONN
#ifdef IPv6
  fd = socket (AF_INET6, SOCK_STREAM, 0);
#else
  fd = socket (AF_INET, SOCK_STREAM, 0);
#endif
  DefineSelf (fd, file, auth);
  close (fd);
#endif
#ifdef NOTDEF  /* DNETCONN */
  fd = socket (AF_DECnet, SOCK_STREAM, 0);
  DefineSelf (fd, file, auth);
  close (fd);
#endif

  DefineLocal (file, auth);
}


writeAddr (family, addr_length, addr, file, auth)
     int	family;
     int	addr_length;
     char	*addr;
     FILE	*file;
     Xauth	*auth;
{

  auth->family = (unsigned short) family;
  auth->address_length = addr_length;
  auth->address = addr;
  writeAuth (file, auth);
}


#if defined(SYSV) && defined(TCPCONN)
int
ifioctl(s, cmd, arg)
        int s;
        int cmd;
        char *arg;
{
        struct strioctl ioc;
        int ret;

        bzero((char *) &ioc, sizeof(ioc));
        ioc.ic_cmd = cmd;
        ioc.ic_timout = 0;
        if (cmd == SIOCGIFCONF) {
/*                ioc.ic_len = ((struct ifconf *) arg)->ifc_len; */
/*	For jup_alpha3, it wants a smaller size in the length field, uncomment above
	line and delete the next line once that is fixed. Should not give EINVAL */
				ioc.ic_len = 1024;
                ioc.ic_dp = ((struct ifconf *) arg)->ifc_buf;
        } else {
                ioc.ic_len = sizeof(struct ifreq);                ioc.ic_dp = arg;
        }
        ret = ioctl(s, I_STR, (char *) &ioc);
        if (ret != -1 && cmd == SIOCGIFCONF)
                ((struct ifconf *) arg)->ifc_len = ioc.ic_len;
        return(ret);
}
#endif /* SYSV && TCPCONN */


DefineSelf (fd, file, auth)
     int fd;
     FILE	*file;
     Xauth	*auth;
{
  char		buf[2048];
  void *        bufptr = buf;
  register int	n;
  int 		len;
  char 		*addr;
  int 		family;
#ifdef SIOCGLIFCONF
  struct lifconf      ifc;
  register struct lifreq *ifr;
#ifdef SIOCGLIFNUM
  struct lifnum       ifn;
#endif
#else
  register struct ifreq *ifr;
  struct ifconf	ifc;
#endif
  
  len = sizeof(buf);
    
#if defined(SIOCGLIFNUM) && defined(SIOCGLIFCONF)
  ifn.lifn_family = AF_UNSPEC;
  ifn.lifn_flags = 0;
  if (ioctl (fd, (int) SIOCGLIFNUM, (char *) &ifn) < 0)
      perror ("Getting interface count");    
  if (len < (ifn.lifn_count * sizeof(struct lifreq))) {
      len = ifn.lifn_count * sizeof(struct lifreq);
      bufptr = malloc(len);
  }
#endif

#ifdef SIOCGLIFCONF
  ifc.lifc_family = AF_UNSPEC;
  ifc.lifc_flags = 0;
  ifc.lifc_len = len;
  ifc.lifc_buf = bufptr;
  if (ioctl (fd, (int) SIOCGLIFCONF, (char *) &ifc) < 0)
      return;
  for (ifr = ifc.lifc_req, n = ifc.lifc_len / sizeof (struct lifreq);
       --n >= 0; ifr++)
  {
      len = sizeof(ifr->lifr_addr);
      { 
	  family = ConvertAddr ((struct sockaddr *) &ifr->lifr_addr, &len,
	    &addr);
#else
  ifc.ifc_len = len;
  ifc.ifc_buf = buf;
#if defined(SYSV) && defined(TCPCONN)
  ifioctl (fd, SIOCGIFCONF, (char *) &ifc);
#else
  ioctl (fd, SIOCGIFCONF, (char *) &ifc);
#endif /* SYSV && TCPCONN */

  for (ifr = ifc.ifc_req, n = ifc.ifc_len / sizeof (struct ifreq); --n >= 0;
       ifr++)
  {
#ifdef NOTDEF	/* DNETCONN */
      /*
       * this is ugly but SIOCGIFCONF returns decnet addresses in
       * a different form from other decnet calls
       */
      if (ifr->ifr_addr.sa_family == AF_DECnet) {
	len = sizeof (struct dn_naddr);
	addr = (char *)ifr->ifr_addr.sa_data;
	family = FamilyDECnet;
      } else
#endif
	{
	  family = ConvertAddr (&ifr->ifr_addr, &len, &addr);
#endif
	  if (family <= 0)
	      continue;

	  /*
	   * If we get back a 0.0.0.0 IP address, ignore this entry. This
	   * typically happens when a machine is in a standalone mode.
	   */
	  if(len == 4 && addr[0] == 0 && addr[1] == 0 && 
			addr[2] == 0 && addr[3] == 0)
	      continue;

#ifdef IPv6
	  if(family == AF_INET6) {
	      if (IN6_IS_ADDR_LOOPBACK(((struct in6_addr *)addr)))
		  continue;
	      else
		  family = FamilyInternet6;
	  } else 
#endif
	  family = FamilyInternet;
	}

      writeAddr (family, len, addr, file, auth);
  }
}


DefineLocal (file, auth)
     FILE	*file;
     Xauth	*auth;
{
  char	displayname[100];
  
  /* stolen from xinit.c */
#ifdef hpux
  /* Why not use gethostname()?  Well, at least on my system, I've had to
   * make an ugly kernel patch to get a name longer than 8 characters, and
   * uname() lets me access to the whole string (it smashes release, you
   * see), whereas gethostname() kindly truncates it for me.
   */
  {
    struct utsname name;
    
    uname(&name);
    strcpy(displayname, name.nodename);
  }
#else
  gethostname(displayname, sizeof(displayname));
#endif
  writeAddr (FamilyLocal, strlen (displayname), displayname, file, auth);
}

/* code stolen from server/os/4.2bsd/access.c */

static int
ConvertAddr (saddr, len, addr)
     register struct sockaddr	*saddr;
     int				*len;
     char			**addr;
{
  if (len == 0)
    return (0);
  switch (saddr->sa_family)
    {
    case AF_UNSPEC:
#ifndef hpux
    case AF_UNIX:
#endif
      return (0);
#ifdef TCPCONN
    case AF_INET:
      *len = sizeof (struct in_addr);
      *addr = (char *) &(((struct sockaddr_in *) saddr)->sin_addr);
      return (AF_INET);
#ifdef IPv6
    case AF_INET6:
      *len = sizeof (struct in6_addr);
      *addr = (char *) &(((struct sockaddr_in6 *) saddr)->sin6_addr);
      return (AF_INET6);
#endif      
#endif
      
#ifdef NOTDEF	/* DNETCONN */
    case AF_DECnet:
      *len = sizeof (struct dn_naddr);
      *addr = (char *) &(((struct sockaddr_dn *) saddr)->sdn_add);
      return (AF_DECnet);
#endif
    default:
      break;
    }
  return (-1);
}


setAuthNumber (auth, name)
     Xauth   *auth;
     char    *name;
{
  char	*colon;
  char	*dot, *number;
  
  colon = rindex (name, ':');
  if (colon) {
    ++colon;
    if (dot = index (colon, '.'))
      auth->number_length = dot - colon;
    else
      auth->number_length = strlen (colon);
    number = malloc (auth->number_length + 1);
    if (number) {
      strncpy (number, colon, auth->number_length);
      number[auth->number_length] = '\0';
    } else {
      perror ("setAuthNumber");
      auth->number_length = 0;
    }
    auth->number = number;
  }
}

static
  openFiles (name, new_name, oldp, newp)
char	*name, *new_name;
FILE	**oldp, **newp;
{
  int	mask;
  
  strcpy (new_name, name);
  strcat (new_name, "-n");
  mask = umask (0077);
  (void) unlink (new_name);
  *newp = fopen (new_name, "w");
  (void) umask (mask);
  if (!*newp) {
    return 0;
  }
  *oldp = fopen (name, "r");

  return 1;
}

writeAuth (file, auth)
     FILE	*file;
     Xauth	*auth;
{
  saveAddr (auth->family, auth->address_length, auth->address,
	    auth->number_length,  auth->number);
  if (!XauWriteAuth (file, auth)) {
	fprintf(stderr,"mkcookie: Could not write authorization info. for user. exiting\n");
	exit(-1);
  }
}
doneAddrs ()
{
  struct addrList	*a, *n;
  for (a = addrs; a; a = n) {
    n = a->next;
    if (a->address)
      free (a->address);
    if (a->number)
      free (a->number);
    free ((char *) a);
  }
}

InitAuth (name_len, name)
     unsigned short  name_len;
     char	    *name;
{
  if (strcmp(name,"SUN-DES-1") != 0) {
    if (name_len > 256)
      name_len = 256;
  }
  auth_name_len = name_len;
  bcopy (name, auth_name, name_len);
}

Xauth * GetAuth (namelen, name)
     int  namelen;
     char	    *name;
{
  Xauth   *new;
  int uid = getuid();
  char netname[MAXNETNAMELEN];
  new = (Xauth *) malloc (sizeof (Xauth));
  
  if (!new)
    return (Xauth *) 0;
  new->family = FamilyWild;
  new->address_length = 0;
  new->address = 0;
  new->number_length = 0;
  new->number = 0;
  
  if (strncmp(name,"MIT-",4) == 0)
    new->data = (char *) malloc (AUTH_DATA_LEN);
  else 
    new->data = (char *) malloc (MAXNETNAMELEN);
  if (!new->data)
    {
      free ((char *) new);
      return (Xauth *) 0;
    }
  new->name = (char *) malloc (namelen);
  if (!new->name)
    {
      free ((char *) new->data);
      free ((char *) new);
      return (Xauth *) 0;
    }
  bcopy (name, new->name, namelen);
  new->name_length = namelen;
  if (strncmp(name,"MIT-",4) == 0) {
    GenerateCryptoKey (new->data, AUTH_DATA_LEN);
    new->data_length = AUTH_DATA_LEN;
  } else {
    if (!user2netname(netname, uid, 0))
      perror("user2netname");
    new->data_length = strlen(netname);
    bcopy(netname, new->data, new->data_length);
  }
  return new;
}

static long	key[2];

GenerateCryptoKey (auth, len)
     char	*auth;
     int	len;
{
  long    data[2];
  int	  seed;
  int	  value;
  int	  i, t;
  char   *ran_file = "/dev/random";
  int     fd;

  if ((fd = open(ran_file, O_RDONLY)) == -1)
  {
    struct timeval  now;
    struct timezone zone;
    gettimeofday (&now, &zone);
    data[0] = now.tv_sec;
    data[1] = now.tv_usec;
  
    seed = (data[0]) + (data[1] << 16);
    srand (seed);
    for (i = 0; i < (len - 1); i++)
      {
        value = rand ();
        auth[i] = value & 0xff;
      }
  } else {
    for (i = 0; i < (len - 1); i++)
       {
            read(fd, &t, sizeof(t));
            auth[i] = (
                  ( (t & 0xff000000) >> 24) ^
                  ( (t & 0xff0000) >> 16) ^
                  ( (t & 0xff00) >> 8)  ^
                  ( (t & 0xff) )
                  ) & 0xff;
       }
    close(fd);
  }
  auth[len-1] = '\0';
}

SaveServerAuthorization (authFile, auth)
     char *authFile;
     Xauth	    *auth;
{
  FILE	*auth_file;
  int		mask;
  int		ret;
  
  mask = umask (0077);
  (void) unlink (authFile);
  auth_file = fopen (authFile, "w");
  umask (mask);
  if (!auth_file) {
    ret = FALSE;
  }
  else
    {
      if (!XauWriteAuth (auth_file, auth) || fflush (auth_file) == EOF)
    	{
	  ret = FALSE;
    	}
      else
	ret = TRUE;
      fclose (auth_file);
    }
  chmod(authFile, S_IREAD);
  return ret;
}

#define NUM_AUTHORIZATION (sizeof (AuthProtocols) / sizeof (AuthProtocols[0]))

static struct AuthProtocol *
  findProtocol (name_length, name)
int  name_length;
char	    *name;
{
  int	i;
  
  for (i = 0; i < NUM_AUTHORIZATION; i++)
    if (AuthProtocols[i].name_length == name_length &&
	bcmp (AuthProtocols[i].name, name, name_length) == 0)
      {
	return &AuthProtocols[i];
      }
  return (struct AuthProtocol *) 0;
}

Xauth *GenerateAuthorization (name, name_length)
     char		*name;
     int	name_length;
{
  struct AuthProtocol	*a;
  Xauth   *auth = 0;
  
  a = findProtocol (name_length, name);
  if (a)
    {
      if (!a->inited)
	{
	  (*a->InitAuth) (name_length, name);
	  a->inited = TRUE;
	}
      auth = (*a->GetAuth) (name_length, name);
    }
  return auth;
}

void
SetLocalAuthorization (d)
     struct display	*d;
{
  Xauth	*auth;
  int nl = d->authNameLen;
  
  if (d->authorization)
    {
      XauDisposeAuth (d->authorization);
      d->authorization = (Xauth *) NULL;
    }
  if (d->authName && !d->authNameLen)
    d->authNameLen = strlen (d->authName);
  auth = GenerateAuthorization ( d->authName, nl);
  if (!auth)
    return;

  /* Change to real user id, before writing any files */

  setuid(getuid());
  if (SaveServerAuthorization (d->authFile, auth))
    d->authorization = auth;
  else {
    XauDisposeAuth (auth);
	fprintf(stderr,"mkcookie: Could not write server authorization file. exiting\n");
	exit(-1);
  }
}

void
SetUserAuthorization (d, verify)
     struct display		*d;
     struct verify_info	*verify;
{
  FILE	*old, *new;
  char	home_name[1024], backup_name[1024], new_name[1024];
  char	*name;
  char	*home;
  char	*envname = 0;
  int	lockStatus;
  Xauth	*entry, *auth;
  int	setenv;
  char	**setEnv ();
  extern char *getenv ();
  struct stat	statb;
  
  if (auth = d->authorization) {
    home = getenv("HOME");
    lockStatus = LOCK_ERROR;
    if (home) {
      snprintf (home_name, 1024, "%s/.Xauthority", home);
      lockStatus = XauLockAuth (home_name, 1, 2, 10);
      if (lockStatus == LOCK_SUCCESS) {
	if (openFiles (home_name, new_name, &old, &new)) {
	  name = home_name;
	  setenv = 0;
	} else {
	  XauUnlockAuth (home_name);
	  lockStatus = LOCK_ERROR;
	}	
      }
    }
    if (lockStatus != LOCK_SUCCESS) {
      sprintf (backup_name, "%s/.XauthXXXXXX", d->userAuthDir);
      mktemp (backup_name);
      lockStatus = XauLockAuth (backup_name, 1, 2, 10);

      if (lockStatus == LOCK_SUCCESS) {
	if (openFiles (backup_name, new_name, &old, &new)) {
	  name = backup_name;
	  setenv = 1;
	} else {
	  XauUnlockAuth (backup_name);
	  lockStatus = LOCK_ERROR;
	}	
      }
    }
    if (lockStatus != LOCK_SUCCESS) {
      return;
    }
    addrs = 0;
    if (d->displayType.location == Local)
      writeLocalAuth (new, auth, d->name);

    if (old) {
      if (fstat (fileno (old), &statb) != -1)
	chmod (new_name, (int) (statb.st_mode & 0777));
      while (entry = XauReadAuth (old)) {
	if (!checkAddr (entry->family,
			entry->address_length, entry->address,
			entry->number_length, entry->number))
	  {
	    writeAuth (new, entry);
	  }
	XauDisposeAuth (entry);
      }
      fclose (old);
    }
    doneAddrs ();
    fclose (new);
    unlink (name);

    envname = name;
    if (link (new_name, name) == -1) {
      setenv = 1;
      envname = new_name;
    } else {
      unlink (new_name);
    }
    if (setenv) {
      verify->userEnviron = setEnv (verify->userEnviron,
				    "XAUTHORITY", envname);
      verify->systemEnviron = setEnv (verify->systemEnviron,
				      "XAUTHORITY", envname);
    }
    XauUnlockAuth (name);
    if (envname) 
      chown (envname, verify->uid, verify->gid);
  }

}

usage(str)
char *str;
{
  fprintf(stderr,"usage: %s Server_auth_file [-auth protocol]\n",str);
  fprintf(stderr,"      where protocol is one of magic-cookie or sun-des\n");
}

main(argc, argv)
int argc;
char *argv[];
{
  struct display d;
  struct verify_info verify;
  int server_number;
  char *au_name;


  if (argc < 2) {
    usage(argv[0]);
    fprintf(stderr,"WARNING: This program will overwrite existing files.\n");
    fprintf(stderr,"This may cause your X server to not accept any\n");
    fprintf(stderr,"new client connections.\n");
    exit(-1);
  }

  au_name = NULL;
  d.authFile = argv[1];

  if ((d.name = rindex(d.authFile,':')) == NULL) {
    fprintf(stderr,"mkcookie: Invalid filename: %s\nFilename should include display\n",
		d.authFile);
    exit(-1);
  }
  d.authorize = TRUE;
  d.authorization = NULL;
  /* 1191081:  SIGSEGV when starting openwin - core dumped by mkcookie.
   * becuase d.userAuthDir is not assigned any valid directory where
   * every one will have access to it.
   */
  d.userAuthDir =  DEF_USER_AUTH_DIR; /* Backup directory for User Auth file */
  d.authName = "MIT-MAGIC-COOKIE-1";

  if (argv[2] && strcmp(argv[2],"-auth") == 0) {
    if (argv[3])
      au_name = argv[3];
    else {
      usage(argv[0]);
      exit(-1);
    }
  } else if (argv[2]) {
    usage(argv[0]);
    exit(-1);
  }

  if (au_name) {
    if(strcmp(au_name, "sun-des") == 0)
      d.authName = "SUN-DES-1";
    else if (strncmp(au_name,"magic",5) != 0) {
      usage(argv[0]);
      exit(-1);
    }
  }
  d.authNameLen = strlen(d.authName);
  d.peer = 0;
  d.peerlen = 0;
  d.displayType.location = Local;
  
  verify.uid = getuid();
  verify.gid = getgid();
  verify.userEnviron = NULL;
  verify.systemEnviron = NULL;

  SetLocalAuthorization(&d);
  SetUserAuthorization(&d, &verify);
  exit(0);
}
