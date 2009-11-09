/*
 * Copyright 1996 Sun Microsystems, Inc.  All rights reserved.
 * Use is subject to license terms.
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

#pragma ident	"@(#)OWconfig.c	1.37	09/11/09 SMI"

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include "Sunowconfig.h"

/***************************************************************************
Implementation Note:  This implementation was done with some generality
	in mind.  If at some point it becomes necessary to add more
	elaborate per class or per instance data, this should be
	readily accommodated by this implementation.  

		It is assumed that the configuration database is relatively 
	small, say, fewer than 1000 entries.  There's no hard limit, but 
	there's no optimization either.

		The "package" mechanism is hokey.  It was done to
	address the concern that nobody wanted package names specified
	on a per instance basis, but there needed to be some mechanism
	that identified an instance as belonging to a package.

		The previous implementation mentioned that it was not
	MTsafe or MThot.  The same applies to this implementation.

		Error handling is very primitive.  If a more sophisticated
	means of reporting and acting upon error conditions can be
	developed..., great.

		It would be nice if we could use alloca, but since this
	is also an Xsun server library, we can't.
***************************************************************************/

/***************************************************************************
	OWconfig implementation private data structures...
***************************************************************************/

struct databaseStruct {
	int dummy;
};

struct classStruct {
	int dummy;
};

struct instanceStruct {
	char *pkg;
	int lineNumber;
	struct nodeStruct *nextInstance, *prevInstance;
};

struct attrStruct {
	char *value;
};

/*
 *  A class list is a child of the database.
 *  An instance list is a child of a class.
 *  An attribute list is a child of an instance.
 */
struct nodeStruct {
	char *name;
	unsigned int hashValue;
	struct nodeStruct *next;
	struct nodeStruct *prev;
	struct nodeStruct *parent;
	struct nodeStruct *head;	/* One end of the child list... */
	struct nodeStruct *tail;	/* The other end... */
	void (*nodeFree)();
	union {
		struct databaseStruct database;
		struct classStruct class;
		struct instanceStruct instance;
		struct attrStruct attr;
	} nodeValue;
};



/*
 *  Store comments as a singly linked list...
 */
struct commentStruct {
	char *comment;
	int lineNumber;
	struct commentStruct *next;
};


/*** Max pathname length... ***/
#define FNLENGTH 300

/*** Unit of allocation for input line buffer... ***/

#define LINEALLOCUNIT 128

/*** Max length of statically allocated strings... ***/
#define MAXVALLENGTH 128


/***************************************************************************
Data Declarations...
***************************************************************************/
static struct nodeStruct OWconfigDatabase;
static int databaseValid = 0;

static char currentPackage[MAXVALLENGTH];
static char *defaultNewPkgName="RESERVED-unassigned";
static char *defaultOldPkgName="RESERVED-oldstyle";

/*** If ever there are multiple databases, this could be moved into
     a database nodeStruct.
***/
static char *readFile1 = NULL;
static char *readFile2 = NULL;
static int OWconfigFlags = 0;
static void *(*allocMem)();
static void (*freeMem)();
static time_t readFile1TimeStamp;
static time_t readFile2TimeStamp;
static struct commentStruct *commentList = NULL;
static struct commentStruct *lastComment = NULL;
static struct nodeStruct *instanceListHead = NULL;
static struct nodeStruct *instanceListTail = NULL;
static int lineNumber;
static int lastLineNumber;





/***************************************************************************
****************************************************************************
Implementation private functions...
****************************************************************************
***************************************************************************/

#define FREENODE(np) \
	if (np->parent->head == np)\
		np->parent->head = np->next;\
	if (np->parent->tail == np)\
		np->parent->tail = np->prev;\
	if (np->next)\
		np->next->prev = np->prev;\
	if (np->prev)\
		np->prev->next = np->next;\
	freeMem(np->name);\
	freeMem((char *)np);



/***************************************************************************
freeNodeList:  The generic node list destructor...
***************************************************************************/
static void
freeNodeList(struct nodeStruct *np)
{
	struct nodeStruct *next;

	while (np) {
		if (np->head)
			freeNodeList(np->head);
		next = np->next;
		if (np->nodeFree)
			np->nodeFree(np);
		np = next;
	}
}





/***************************************************************************
freeClass:  The class node destructor...
***************************************************************************/
static void
freeClass(struct nodeStruct *cp)
{
	if (cp->head)
		freeNodeList(cp->head);

	FREENODE(cp);

	return;
}





/***************************************************************************
freeInstance:  The instance node destructor...
***************************************************************************/
static void
freeInstance(struct nodeStruct *ip)
{
	char *tmp;
	struct nodeStruct *nextInstance, *prevInstance;

	if (ip->head)
		freeNodeList(ip->head);

	tmp = ip->nodeValue.instance.pkg;
	if (tmp)
		freeMem(tmp);

	if (OWconfigFlags & OWFLAG_RETAIN) {
		nextInstance = ip->nodeValue.instance.nextInstance;
		prevInstance = ip->nodeValue.instance.prevInstance;
		if (nextInstance)
			nextInstance->nodeValue.instance.prevInstance = 
				prevInstance;
		if (prevInstance)
			prevInstance->nodeValue.instance.nextInstance = 
				nextInstance;
		if (instanceListHead == ip)
			instanceListHead = nextInstance;
		if (instanceListTail == ip)
			instanceListTail = prevInstance;
	}

	FREENODE(ip);

	return;
}




/***************************************************************************
freeAttr:  The attribute node destructor...
***************************************************************************/
static void
freeAttr(struct nodeStruct *ap)
{
	char *value;

	value = ap->nodeValue.attr.value;
	if (value)
		freeMem(value);

	FREENODE(ap);

	return;
}





/***************************************************************************
hashValue:  Generates the arithmetic sum of the bytes of the string 
	specified by "str".
***************************************************************************/
static unsigned int
hashValue(char *str)
{
        unsigned int ret = 0;

	while (*str) {
                ret += (unsigned int)*str;
		str++;
	}
 
        return(ret);
}





/***************************************************************************
dupString:  Given a non-null pointer to a string, allocates memory
	sufficient to contain a copy of the string pointed to by
	"str", copies the string to the newly allocated memory, and
	returns a pointer to the new string.
***************************************************************************/
static char *
dupString(char *str)
{
	int len;
	char *ptr;

	if (str) {
		len = strlen(str) + 1;

		ptr = (char *)allocMem(len);
		if (ptr)
			(void)memcpy(ptr, str, len);
	
		return(ptr);
	} else
		return(NULL);
}





/***************************************************************************
searchNodeList:  Searches next-wise along a list, starting at the node
	pointed to by "node", until either the end of the list is
	found, or a node whose name matches the string pointed to by
	"name".
***************************************************************************/
static struct nodeStruct *
searchNodeList(struct nodeStruct *node, char *name)
{
	unsigned int nameHash;

	nameHash = hashValue(name);
	while (node) {
		if ((nameHash == node->hashValue) && !strcmp(name, node->name))
			break;
		node = node->next;
	}

	return(node);
}




/***************************************************************************
searchClass:  Searches for the class node whose name matches the string
	pointed to by "class".
***************************************************************************/
static struct nodeStruct *
searchClass(char *class)
{
	if (OWconfigDatabase.head)
		return(searchNodeList(OWconfigDatabase.head, class));

	return((struct nodeStruct *)NULL);
}




/***************************************************************************
searchInstance:  Searches for the instance node identified by "name",
	parented by the class node identified by "class".
***************************************************************************/
static struct nodeStruct *
searchInstance(char *class, char *name)
{
	struct nodeStruct *cp;

	cp = searchClass(class);
	if (cp && cp->head) 
		return(searchNodeList(cp->head, name));

	return((struct nodeStruct *)NULL);
}






/***************************************************************************
searchAttr:  Searches for the attribute node identified by "attr",
	parented by the instance node identified by "name",
	which in turn is parented by the class node identified by "class".
***************************************************************************/
static struct nodeStruct *
searchAttr(char *class, char *name, char *attr)
{
	struct nodeStruct *instanceNode;

	instanceNode = searchInstance(class, name);
	if (instanceNode && instanceNode->head) 
		return(searchNodeList(instanceNode->head, attr));

	return((struct nodeStruct *)NULL);
}





/***************************************************************************
createClass:  Creates a new class node within the OWconfig database.
***************************************************************************/
static struct nodeStruct *
createClass(char *class)
{
	struct nodeStruct *cp;

	cp = (struct nodeStruct *)allocMem(sizeof(struct nodeStruct));
	if (!cp)
		return(NULL);
	cp->name = dupString(class);
	if (!cp->name) {
		freeMem(cp);
		return(NULL);
	}
	cp->hashValue = hashValue(class);
	cp->parent = &OWconfigDatabase;
	cp->head = NULL;
	cp->tail = NULL;
	cp->nodeFree = freeClass;

	if (!OWconfigDatabase.head) {
		OWconfigDatabase.head = cp;
	}

	cp->prev = OWconfigDatabase.tail;
	cp->next = NULL;

	if (OWconfigDatabase.tail)
		OWconfigDatabase.tail->next = cp;

	OWconfigDatabase.tail = cp;

	return(cp);
}





/***************************************************************************
createInstance:  Creates a new instance node parented by the class node
	identified by "class".  The instance node will be identified
	by "name".  If the class node does not already exist, it will
	be created.
***************************************************************************/
static struct nodeStruct *
createInstance(char *class, char *name)
{
	struct nodeStruct *cp, *ip;

	cp = searchClass(class);
	if (!cp) {
		cp = createClass(class);
		if (!cp) {
			return(NULL);
		}
	}
	ip = (struct nodeStruct *)allocMem(sizeof(struct nodeStruct));
	if (!ip)
		return(NULL);
	ip->name = dupString(name);
	if (!ip->name) {
		freeMem(ip);
		return(NULL);
	}
	ip->hashValue = hashValue(name);
	ip->parent = cp;
	ip->head = NULL;
	ip->tail = NULL;
	ip->nodeFree = freeInstance;
	ip->nodeValue.instance.pkg = NULL;

	if (OWconfigFlags & OWFLAG_RETAIN) {
		if (instanceListTail) {
			if (instanceListTail->nodeValue.instance.lineNumber ==
			    lineNumber)
				lineNumber++;
			ip->nodeValue.instance.lineNumber = lineNumber;
			ip->nodeValue.instance.nextInstance = NULL;
			ip->nodeValue.instance.prevInstance = instanceListTail;
			instanceListTail->nodeValue.instance.nextInstance = ip;
			instanceListTail = ip;
		} else {
			instanceListHead = instanceListTail = ip;
			ip->nodeValue.instance.lineNumber = lineNumber;
			ip->nodeValue.instance.nextInstance = NULL;
			ip->nodeValue.instance.prevInstance = NULL;;
		}
	}

	if (!cp->head) {
		cp->head = ip;
	}

	ip->prev = cp->tail;
	ip->next = NULL;

	if (cp->tail)
		cp->tail->next = ip;

	cp->tail = ip;

	return(ip);
}





/***************************************************************************
unlockOWconfig:  Remove the read or write lock from an OWconfig file.
***************************************************************************/
static void
unlockOWconfig(int f)
{
/* 4007038
Remvoe any read/write locking/unlocking.
	flock_t lockStruct;

	lockStruct.l_type = F_UNLCK;
	lockStruct.l_whence = 0;
	lockStruct.l_start = 0;
	lockStruct.l_len = 0;
	(void)fcntl(f, F_SETLK, &lockStruct);
*/
	return;
}





/***************************************************************************
readLockOWconfig:  Place a read lock on an OWconfig file.  Should only
	fail if the file is write locked.
***************************************************************************/
static int
readLockOWconfig(int f)
{
/* 4007038
Do not do any read locking.

	int result;
	flock_t lockStruct;

	lockStruct.l_type = F_RDLCK;
	lockStruct.l_whence = 0;
	lockStruct.l_start = 0;
	lockStruct.l_len = 0;
	result = fcntl(f, F_SETLK, &lockStruct);
	if (result != -1)
		return(OWCFG_OK);
	else
		return(OWCFG_LOCK1FAIL);
*/
	return(OWCFG_OK);
}





/***************************************************************************
writeLockOWconfig:  Place a write lock on an OWconfig file.  Will fail
	if any other locks, read or write, are already on the file.
***************************************************************************/
static int
writeLockOWconfig(int f)
{
/* 4007038
Do not do any write locking.

	int result;
	flock_t lockStruct;

	lockStruct.l_type = F_WRLCK;
	lockStruct.l_whence = 0;
	lockStruct.l_start = 0;
	lockStruct.l_len = 0;
	result = fcntl(f, F_SETLK, &lockStruct);
	if (result != -1)
		return(OWCFG_OK);
	else
		return(OWCFG_LOCK1FAIL);
*/
	return(OWCFG_OK);
}





/***************************************************************************
accumulateComment:
***************************************************************************/
static void
accumulateComment(char *comment, int lineNumber)
{
	int len;
	struct commentStruct *newComment;

	len = strlen(comment);
	if (len == 0)
		return;

	if (*comment =='\n')
		return;

	newComment = (struct commentStruct *)
		     allocMem(len + 1 + sizeof(struct commentStruct));
	newComment->comment = (char *)newComment + 
			      sizeof(struct commentStruct);
	strcpy(newComment->comment, comment);
	newComment->lineNumber = lineNumber;
	newComment->next = NULL;
	if (lastComment != NULL) {
		lastComment->next = newComment;
		lastComment = newComment;
	} else {
		commentList = lastComment = newComment;
	}

	return;
}




/***************************************************************************
freeComments:
***************************************************************************/
static void
freeComments(void)
{
	struct commentStruct *commentP = commentList;
	struct commentStruct *next;

	while (commentP) {
		next = commentP->next;
		freeMem(commentP);
		commentP = next;
	}

	commentList = NULL;

	return;
}




/***************************************************************************
initDatabase:  If a database already exists, free it, and set the 
	OWconfigDatabase structure to a known state.
***************************************************************************/
static void
initDatabase(void)
{
	if (databaseValid) {
		freeNodeList(OWconfigDatabase.head);
		freeMem(readFile1);
		readFile1 = NULL;
		freeMem(readFile2);
		readFile2 = NULL;
		freeComments();
	}

	databaseValid = 1;
	OWconfigDatabase.head = NULL;
	OWconfigDatabase.tail = NULL;
	OWconfigDatabase.next = NULL;
	OWconfigDatabase.prev = NULL;
	OWconfigDatabase.nodeFree = (void (*)())0;
	return;
}





/***************************************************************************
reAllocMem:  Because we don't really want to add another parameter to
	OWconfigInit..., we implement a realloc routine that uses
	the allocMem and freeMem routines specified by OWconfigInit.
***************************************************************************/
static char *
reAllocMem(char *ptr, unsigned int oldSize, unsigned int newSize)
{
	char *newPtr;

	if (newSize <= oldSize)
		return(ptr);

	newPtr = allocMem(newSize);
	if (newPtr) {
		memcpy(newPtr, ptr, oldSize);
		freeMem(ptr);
	}

	return(newPtr);
}





/***************************************************************************
readLine:  Read the file, line by line, until an "interesting" line
	is encountered.  Maintain a line number counter along the
	way.  When a line is found, fill "lineBuf" with the data.
	It is the responsibility of the caller to free the dynamically
	allocated line buffer.
Note:  Need to find proper way to use ctype functions...
***************************************************************************/
static int
readLine(FILE *fp, char **lineBuf, int *lineNumber)
{
	char *result;
	int lineBufSize = LINEALLOCUNIT+1;

	(*lineNumber)++;

	*lineBuf = (char *)allocMem(lineBufSize);
	if (!*lineBuf)
		return(EOF);

	result = fgets(*lineBuf, LINEALLOCUNIT, fp);

	if (result) {
		int bufLen = strlen(result);

		while (result[bufLen-1] != 0xa) {
		    char *newBuf;
		    int newBufSize;

		    newBufSize = lineBufSize + LINEALLOCUNIT;
		    newBuf = reAllocMem(*lineBuf, lineBufSize, newBufSize);

		    if (!newBuf) {
			freeMem(*lineBuf);
			*lineBuf = NULL;
			return(EOF);
		    }

		    *lineBuf = newBuf;
		    lineBufSize = newBufSize;
		    result = fgets((newBuf)+bufLen, 
				   LINEALLOCUNIT, fp);

		    if (!result) {
			freeMem(*lineBuf);
			*lineBuf = NULL;
			return(EOF);
		    }

		    bufLen = strlen(newBuf);
		    result = newBuf;
		}

		while (isspace(*result) && *result != '#' && *result)
			result++;
		if (*result == '#' || *result == (char)0) {
			if (OWconfigFlags & OWFLAG_RETAIN) {
				accumulateComment(*lineBuf, *lineNumber);
			}
			freeMem(*lineBuf);
			return(readLine(fp, lineBuf, lineNumber));
		} else
			return(1);
	} else {
		return(EOF);
	}
}





/***************************************************************************
readPair:  Starting at character position "charPos" in "lineBuf",
	search for an expression of appx. the following syntax:

	<alnum>{<whitespace>}<equal>{<whitespace>}\
		(<quote><anything goes, even \ stuff><quote>) | \
		<alnum>

	If such an expression is found, "nameBuf" is filled with the
	value to the left of the = and "valBuf" is filled with the
	value to the right of the =.  "charPos" will be updated to
	point at the next candidate expression.
***************************************************************************/
static int
readPair(char *lineBuf, char *nameBuf, char *valBuf, int *charPos)
{
	char *ptr = lineBuf + *charPos;

	/*** Skip whitespace preceding name... ***/
	while (*ptr && isspace(*ptr)) 
		ptr++;

	if ((!isalnum(*ptr)) || *ptr == (char)0 || *ptr == ';') {
		*charPos = (int)(ptr - lineBuf);
		if (*ptr == (char)0 || *ptr == ';')
			return(!OWCFG_OK);
		else
			return(OWCFG_SYNTAX1);
	}

	/*** Get name... ***/
	while (*ptr && isalnum(*ptr)) {
		*nameBuf = *ptr;
		nameBuf++;
		ptr++;
	}

	*nameBuf = (char)0;

	/*** Skip "=" and whitespace preceding open quote or string...***/
	while (*ptr && (*ptr=='=' || isspace(*ptr)))
		ptr++;

	if (*ptr == (char)0) {
		*charPos = (int)(ptr - lineBuf);
		return(OWCFG_SYNTAX1);
	}

	if (*ptr == '"') {
		/*** Fetch quote delimited string... ***/
		ptr++;
		while (*ptr && *ptr != '"') {
		  if (*ptr == '\\') {
		    if (!*(++ptr))
		      break;
		  }
		  *valBuf = *ptr;
		  valBuf++;
		  ptr++;
		}
	} else {
		/*** Fetch white space terminated string... ***/
		while (*ptr && (*ptr != ';') && (!isspace(*ptr))) {
		  if (*ptr == '\\') {
		    if (!*(++ptr))
		      break;
		  }
		  *valBuf = *ptr;
		  valBuf++;
		  ptr++;
		}
	}

	*valBuf = (char)0;

	/*** Skip past terminating character, if any. ***/
	if (*ptr)
		ptr++;

	*charPos = (int)(ptr - lineBuf);
	return(OWCFG_OK);
}






/***************************************************************************
readOWconfig:  Read the OWconfig database file named by "fileName".
	The internal database is built and the file's last modified time
	is returned in "timeStamp".  If anything goes wrong, it is
	the caller's responsibility to clean up the database.
***************************************************************************/
static int
readOWconfig(char *fileName, time_t *timeStamp)
{
	char *lineBuf = NULL;
	char nameBuf[MAXVALLENGTH];
	char *valBuf = NULL;
	char classVal[MAXVALLENGTH];
	char nameVal[MAXVALLENGTH];
	char pkgVal[MAXVALLENGTH];
	int skipping = 0;
	FILE *fp;
	struct stat statbuf;
	int result;
	int syntaxResult = OWCFG_OK;

	fp = fopen(fileName, "r");
	if (!fp)
		return(OWCFG_OPEN1FAIL);
	if (readLockOWconfig(fileno(fp)) != OWCFG_OK) {
		(void)fclose(fp);
		return(OWCFG_LOCK1FAIL);
	}

	classVal[0] = (char)0;
	nameVal[0] = (char)0;
	pkgVal[0] = (char)0;

	strcpy(currentPackage, defaultOldPkgName);

	while (readLine(fp, &lineBuf, &lineNumber) != EOF) {
		int charPos;

		valBuf = allocMem(strlen(lineBuf)+1);
		if (!valBuf) {
			freeMem(lineBuf);
			unlockOWconfig(fileno(fp));
			fclose(fp);
			return(OWCFG_ALLOC);
		}

		charPos = 0;
		while ((result = readPair(lineBuf, nameBuf, valBuf, 
					      &charPos)) == OWCFG_OK) 
		{
		  /*** class=<value> ??? ***/
		  if (!strcmp(nameBuf, "class")) {
		    (void)strcpy(classVal, valBuf);
		    skipping = 0;

		  /*** name=<value> ??? ***/
		  } else if (!strcmp(nameBuf, "name")) {
		    (void)strcpy(nameVal, valBuf);
		    skipping = 0;

		  /*** package=<value> ??? ***/
		  } else if (!strcmp(nameBuf, "package")) {
		    (void)strcpy(currentPackage, valBuf);
		    skipping = 0;

		  /*** <attr>=<value> ??? ***/
		  } else if (*classVal != (char)0 && *nameVal != (char)0) {
		    result = OWconfigSetAttribute(classVal, nameVal,
		                                  nameBuf, valBuf);
		    if (result != OWCFG_OK) {
		      unlockOWconfig(fileno(fp));
		      (void)fclose(fp);
		      if (lineBuf)
		        freeMem(lineBuf);
		      if (valBuf)
			freeMem(valBuf);
		      return(result);
		    }
		    skipping = 0;

		  /*** BLAMMO (TM)... ***/
		  } else {
		    if (!skipping) {
		      (void)fprintf(stderr,"%s\n",lineBuf);
		      (void)fprintf(stderr,"^^^ Bad class instance, line %d, ^^^\n", lineNumber-lastLineNumber);
		      (void)fprintf(stderr,"    in file %s.\n", fileName);
		      skipping = 1;
		    }
		    classVal[0] = (char)0;
		    nameVal[0] = (char)0;
		  }

		} /* while readPair... */
		
		if (result == OWCFG_SYNTAX1)
			syntaxResult = result;

		if (lineBuf[charPos] == ';') {
			classVal[0] = (char)0;
			nameVal[0] = (char)0;
		}

		if (lineBuf) {
			freeMem(lineBuf);
			lineBuf = NULL;
		}

		if (valBuf) {
			freeMem(valBuf);
			valBuf = NULL;
		}


	} /* while readLine... */

	if (lineBuf)
		freeMem(lineBuf);

	if (valBuf)
		freeMem(valBuf);

	(void)fstat(fileno(fp), &statbuf);
	*timeStamp = statbuf.st_mtime;

	unlockOWconfig(fileno(fp));
	(void)fclose(fp);

	return(syntaxResult);
}





/***************************************************************************
writeString:  Writes to "fp" a quoted string specified by "str".  Does
	the necessary \ stuff.
***************************************************************************/
static void
writeString(FILE *fp, char *str)
{
	if (str) {
		(void)fputc('"', fp);
		while (*str) {
			if (*str == '\\' || *str == '"')
				(void)fputc('\\', fp);
			(void)fputc(*str, fp);
			str++;
		}
		(void)fputc('"', fp);
	} else {
		(void)fprintf(fp, "\"\"");
	}

	return;
}





/***************************************************************************
writeDatabase:
***************************************************************************/
static void
writeDatabase(FILE *fp)
{
	struct nodeStruct *cp, *ip, *ap;

	/*** Need to do something about Copyright notices... ***/

	currentPackage[0] = (char)0;
	
	if (OWconfigFlags && OWFLAG_RETAIN) {
	    struct commentStruct *commentP = commentList;
	
	    ip = instanceListHead;
	
	    /*** Write database in something resembling
		 original source order...
	    ***/
	    while (ip) {
	        char *pkg = ip->nodeValue.instance.pkg;
	
		cp = ip->parent;

		while (commentP && (commentP->lineNumber <
		       ip->nodeValue.instance.lineNumber))
		{
		    (void)fprintf(fp, "%s", commentP->comment);
		    commentP = commentP->next;
		}

	        /*** Start new package? ***/
	        if (strcmp(currentPackage, pkg)) {
	          (void)strcpy(currentPackage, pkg);
	          (void)fprintf(fp, "\npackage=");
	          writeString(fp, currentPackage);
	          (void)fprintf(fp, "\n\n");
	        }
	        (void)fprintf(fp, "class=");
	        writeString(fp, cp->name);
	        (void)fprintf(fp, " ");
	        (void)fprintf(fp, "name=");
	        writeString(fp, ip->name);
	        (void)fprintf(fp, "\n");
	        for (ap = ip->head;
	             ap;
	             ap = ap->next)
	        {
	          (void)fprintf(fp, "    %s=", ap->name);
	          writeString(fp, ap->nodeValue.attr.value);
	          if (ap->next)
	            (void)fprintf(fp, "\n");
	          else
	            (void)fprintf(fp, ";\n");
	        }

		ip = ip->nodeValue.instance.nextInstance;
	    }

	    while (commentP) {
		(void)fprintf(fp, "%s", commentP->comment);
		commentP = commentP->next;
	    }

	} else {

	    /*** Write database, class by class... ***/
	    for (cp = OWconfigDatabase.head;
	         cp;
	         cp = cp->next)
	    {
	      for (ip = cp->head;	
	           ip;
	           ip = ip->next)
	      {
	        char *pkg = ip->nodeValue.instance.pkg;
    
	        /*** Start new package? ***/
	        if (strcmp(currentPackage, pkg)) {
	          (void)strcpy(currentPackage, pkg);
	          (void)fprintf(fp, "package=");
	          writeString(fp, currentPackage);
	          (void)fprintf(fp, "\n\n");
	        }
	        (void)fprintf(fp, "class=");
	        writeString(fp, cp->name);
	        (void)fprintf(fp, " ");
	        (void)fprintf(fp, "name=");
	        writeString(fp, ip->name);
	        (void)fprintf(fp, "\n");
	        for (ap = ip->head;
	             ap;
	             ap = ap->next)
	        {
	          (void)fprintf(fp, "    %s=", ap->name);
	          writeString(fp, ap->nodeValue.attr.value);
	          if (ap->next)
	            (void)fprintf(fp, "\n");
	          else
	            (void)fprintf(fp, ";\n");
	        }
	      } /* for */
	    } /* for */
	}
}





/***************************************************************************
writeOWconfig:  Writes the internal database, in OWconfig syntax, to
	the file specified by fileName.  As a precaution, the original
	file is preserved, renamed as <filename>.BAK.

Note:  I'm concerned that a more sophisticated locking mechanism might be
more trouble prone than this one.
***************************************************************************/
static int
writeOWconfig(char *fileName)
{
	FILE *fp;
	char tmpFileName[FNLENGTH];
	char bakFileName[FNLENGTH];
	char pidString[FNLENGTH];
	int f;
	
	(void)sprintf(pidString, ".%d", getpid());
	(void)strcpy(tmpFileName, fileName);
	(void)strcat(tmpFileName, pidString);
	(void)strcpy(bakFileName, fileName);
	(void)strcat(bakFileName, ".BAK");
	fp = fopen(tmpFileName, "w");
	if (!fp)
		return(OWCFG_OPENTMPFAIL);

	writeDatabase(fp);

	(void)fclose(fp);

	/*** Try write locking current OWconfig file... ***/
	f = open(fileName, O_WRONLY);
	if (f == -1) {
		if (errno != ENOENT) {
			(void)unlink(tmpFileName);
			return(OWCFG_OPENWFAIL);
		}
	} else {
		if (writeLockOWconfig(f) != OWCFG_OK) {
			(void)close(f);
			(void)unlink(tmpFileName);
			return(OWCFG_LOCKWFAIL);
		}
		if (rename(fileName, bakFileName) == -1) {
			unlockOWconfig(f);
			(void)close(f);
			(void)unlink(tmpFileName);
			return(OWCFG_RENAMEFAIL);
		}
	}
	if (rename(tmpFileName, fileName) == -1) {
		if (f != -1) {
			(void)rename(bakFileName, fileName);
			unlockOWconfig(f);
			(void)close(f);
		}
		(void)unlink(tmpFileName);
		return(OWCFG_RENAMEFAIL);
	}

	unlockOWconfig(f);
	(void)close(f);

	return(OWCFG_OK);
}





/***************************************************************************
****************************************************************************
Exported functions...
****************************************************************************
***************************************************************************/




/***************************************************************************
OWconfigSetPackage:

This function establishes the name of the package to associate with
database resources created by subsequent calls to OWconfigSetAttribute or
OWconfigCreateClass.

Return:  None.
***************************************************************************/
void
OWconfigSetPackage(char *package)
{
	if (package)
		(void)strcpy(currentPackage, package);

	return;
}





/***************************************************************************
OWconfigRemovePackage:

This function removes any resources from the database that were
associated with the named package.

Return:  OWCFG_OK or !OWCFG_OK.

Note:  The data structure is not really optimal for searching by package,
but performance isn't really critical here.
***************************************************************************/
int
OWconfigRemovePackage(char *package)
{
	struct nodeStruct *cp, *ip;
	int result = OWCFG_PKGNAME;

	if (!package)
		return(OWCFG_ARGS);

	cp = OWconfigDatabase.head;
	while (cp) {
		ip = cp->head;
		while(ip) {
		  char *pkg = ip->nodeValue.instance.pkg;

		  if (pkg && !strcmp(pkg, package)) {
		    struct nodeStruct *next = ip->next;
		    
		    ip->nodeFree(ip);
		    ip = next;
		    result = OWCFG_OK;
		    continue;
		  }
		  ip = ip->next;
		}
		cp = cp->next;
	}

	return(result);
}





/***************************************************************************
OWconfigSetInstance:

"OWconfigSetInstance" is a convenient front end to "OWconfigSetAttribute".
It takes a list of attributes and adds them to an instance of a class.
If that class and/or instance does not already exist, they are or it is created.If that instance does exist, replacements, when necessary, occur on a per
attribute/value pair basis, otherwise they are merely added to the instance.
 
Return:  OWCFG_OK or !OWCFG_OK(could occur during an out of memory condition).
***************************************************************************/
int
OWconfigSetInstance(char *class, char *name, OWconfigAttributePtr attr,
                    int numberInattr)
{
	OWconfigAttributePtr limit;
	int result = OWCFG_OK;

	if (!(class && name && attr))
		return(OWCFG_ARGS);

	limit = attr + numberInattr;
	
	for (;attr < limit;  attr++) {
		result = OWconfigSetAttribute(class, name, 
					      attr->attribute,
					      attr->value);
		if (result != OWCFG_OK)
			break;
	}

	return(result);
}





/***************************************************************************
OWconfigRemoveInstance:

"OWconfigRemoveInstance" removes an instance of a class, identified by
"class" and "name".

Return:  OWCFG_OK or !OWCFG_OK
***************************************************************************/
int
OWconfigRemoveInstance(char *class, char *name)
{
	struct nodeStruct *ip;

	if (!(class && name))
		return(OWCFG_ARGS);

	ip = searchInstance(class, name);
	if (!ip)
		return(OWCFG_INSTANCENAME);

	ip->nodeFree(ip);

	return(OWCFG_OK);
}







/***************************************************************************
OWconfigSetAttribute:

"OWconfigSetAttribute" takes an attribute/value pair and adds it to an
instance of a class.  If that class and/or instance does not already
exist, they are or it is created.  If the named attribute already exists
within the instance, it is replaced.

Return:  OWCFG_OK or !OWCFG_OK(could occur during with out of memory condition).
***************************************************************************/
int
OWconfigSetAttribute(char *class, char *name, char *attribute, char *value)
{
	struct nodeStruct *cp, *ip, *ap;
	char *tmpPkg;

	if (!(class && name && attribute && value))
		return(OWCFG_ARGS);

	cp = searchClass(class);
	if (!cp) {
		cp = createClass(class);
		if (!cp)
			return(OWCFG_ALLOC);
	}

	ip = searchNodeList(cp->head, name);
	if (!ip) {
		ip = createInstance(class, name);
		if (!ip)
			return(OWCFG_ALLOC);
	}

	ap = searchNodeList(ip->head, attribute);
	if (ap) {
		if (ap->nodeValue.attr.value)
			freeMem(ap->nodeValue.attr.value);
	} else {
		ap = (struct nodeStruct *)
		     allocMem(sizeof(struct nodeStruct));
		if (!ap)
			return(OWCFG_ALLOC);
		ap->name = dupString(attribute);
		if (!ap->name) {
			freeMem(ap);
			return(OWCFG_ALLOC);
		}
		ap->hashValue = hashValue(attribute);
		ap->parent = ip;
		ap->head = NULL;
		ap->tail = NULL;
		ap->nodeFree = freeAttr;

		if (!ip->head)
			ip->head = ap;

		ap->prev = ip->tail;
		ap->next = NULL;

		if (ip->tail)
			ip->tail->next = ap;

		ip->tail = ap;
	}

	ap->nodeValue.attr.value = dupString(value);
	if (!ap->nodeValue.attr.value) {
		freeAttr(ap);
		return(OWCFG_ALLOC);
	}

	if (!ip->nodeValue.instance.pkg) {
		tmpPkg = dupString(currentPackage);
		if (!tmpPkg)
			return(OWCFG_ALLOC);
		ip->nodeValue.instance.pkg = tmpPkg;
	}

	return(OWCFG_OK);
}






/***************************************************************************
OWconfigGetClassNames:

"OWconfigGetClassNames" returns a list of the names of all the
instances of the named class.  The end of the list is indicated by
a NULL pointer.  All users of this function may call "OWconfigFreeClassNames"
to free the list and the strings it points to.

Return:  (char **) to list of class instance names or NULL if class did
         not exist.
***************************************************************************/
char **
OWconfigGetClassNames(char *class)
{
	struct nodeStruct *cp, *ip;
	int instanceCount = 0;
	char **listOfString, **ptr;

	if (!class)
		return(NULL);

	cp = searchClass(class);
	if (!cp)
		return(NULL);
	
	/*** Count the number of instances in the class... ***/
	for (ip=cp->head;  ip;  ip=ip->next)
		instanceCount++;

	listOfString = (char **)allocMem((instanceCount+1) * sizeof(char *));
	if (listOfString) {
		for (ptr=listOfString, ip=cp->head;
		     ip;
		     ptr++, ip=ip->next)
		{
			*ptr = dupString(ip->name);
			
			/*** If dupString fails, free previously allocated
			     strings and return NULL...
			***/
			if (!(*ptr)) {
			  char **limit = ptr;
			
			  for (ptr = listOfString;
			       ptr < limit;
			       ptr++)
			  {
			    freeMem(*ptr);
			  }

			  freeMem(listOfString);
			  return(NULL);
			}
		}
		*ptr = NULL;
	}

	return(listOfString);
}





/***************************************************************************
OWconfigFreeClassNames:

"OWconfigFreeClassNames" frees the list of class names returned by
"OWconfigGetClassNames".
***************************************************************************/
void
OWconfigFreeClassNames(char **list)
{
	char **ptr;

	if (!list)
		return;

	for (ptr = list;  *ptr;  ptr++)
		freeMem(*ptr);

	freeMem(list);

	return;
}




/***************************************************************************
OWconfigGetInstance:

"OWconfigGetInstance" returns a list of attribute definitions belonging
to the instance identified by "class" and "name".  Use "OWconfigFreeInstance" 
to free the memory allocated to the information returned by 
"OWconfigGetInstance".

Return:
	OWconfigAttributePtr or NULL (if instance doesn't exist).
***************************************************************************/
OWconfigAttributePtr
OWconfigGetInstance(char *class, char *name, int *numberInAttr)
{
	struct nodeStruct *ip, *ap;
	int attrCount = 0;
	OWconfigAttributePtr replyPtr, ptr;

	if ((!class) || (!name) || (!numberInAttr))
		return(NULL);

	ip = searchInstance(class, name);
	if (!ip)
		return(NULL);

	/*** Count the number of attributes in the instance... ***/
	for (ap = ip->head;  ap;  ap=ap->next)
		attrCount++;

	replyPtr = (OWconfigAttributePtr)allocMem(attrCount * sizeof(*replyPtr));
	if (replyPtr) {
		for (ptr=replyPtr, ap=ip->head;
		     ap;
		     ptr++, ap=ap->next)
		{
			ptr->attribute = dupString(ap->name);
			ptr->value = dupString(ap->nodeValue.attr.value);
			if ((!ptr->attribute) || (!ptr->value)) {
			  OWconfigAttributePtr limit=ptr;

			  if (ptr->attribute)
			    freeMem(ptr->attribute);
			  else
			    freeMem(ptr->value);

			  for (ptr = replyPtr;
			       ptr < limit;
			       ptr++)
			  {
			    freeMem(ptr->attribute);
			    freeMem(ptr->value);
			  }

			  freeMem(replyPtr);
			  return(NULL);
			}
		}
		*numberInAttr = attrCount;
	}

	return(replyPtr);
}





/***************************************************************************
OWconfigFreeInstance:

"OWconfigFreeInstance" frees the data returned by OWconfigGetInstance.
***************************************************************************/
void
OWconfigFreeInstance(OWconfigAttributePtr attr, int numberInAttr)
{
	OWconfigAttributePtr ptr, limit;

	if ((!attr) || (!numberInAttr))
		return;

	for (ptr = attr, limit = attr + numberInAttr;
	     ptr < limit;
	     ptr++)
	{
		freeMem(ptr->attribute);
		freeMem(ptr->value);
	}
	
	freeMem(attr);
	return;
}








/***************************************************************************
OWconfigGetAttribute:

"OWconfigGetAttribute" returns the string value of the requested attribute.

Return:  (char *) to value of attribute or NULL if attribute could not
         be found.
***************************************************************************/
char *
OWconfigGetAttribute(char *class, char *name, char *attribute)
{
	struct nodeStruct *ap;

	if (!(class && name && attribute))
		return(NULL);

	ap = searchAttr(class, name, attribute);
	if (ap)
		return(dupString(ap->nodeValue.attr.value));
	else
		return(NULL);
}





/***************************************************************************
OWconfigFreeAttribute:

"OWconfigFreeAttribute" frees the string returned by OWconfigGetAttribute.
***************************************************************************/
void
OWconfigFreeAttribute(char *attribute)
{
	if (attribute)
		freeMem(attribute);

	return;
}





/***************************************************************************
OWconfigClose:

If writefile was specified, the existing target file, if any, is write locked 
using fcntl.  If the lock succeeds, the new OWconfig file is written to the 
same directory, with a temporary name.  The old file is removed and the new 
file is renamed to match the old file.

The internal database is freed.
 
Return:  OWCFG_OK or !OWCFG_OK(could occur if write or write lock failed).
 
Note:  When a file is written, all comments are lost from the
original file(s).  When written, the OWconfig entries will be grouped by class.
***************************************************************************/
int
OWconfigClose(char *writeFile)
{
	int result = OWCFG_OK;
	
	if (writeFile != NULL)
		result = writeOWconfig(writeFile);

	initDatabase();
	return(result);
}





/***************************************************************************
OWconfigValidate:  Verifies that the internal database is up to date
	with respect to the database file indicated by the global "readFlags".
	If it isn't, the files are re-read and a new internal database is 
	created.

Return:
	OWCFG_OK or !OWCFG_OK (if re-read fails).
***************************************************************************/
int
OWconfigValidate(void)
{
	int f;
	int result=OWCFG_OK;
	int validate = 0;
	struct stat statbuf;

	if (readFile1) {
		f = open(readFile1, O_RDONLY);
		if (f == -1)
			return(OWCFG_OPEN1FAIL); /*Leave well enough alone*/
		if (fstat(f, &statbuf) == -1) {
			(void)close(f);
			return(OWCFG_FSTAT1FAIL); /*Leave well enough alone*/
		}
		if (statbuf.st_mtime > readFile1TimeStamp)
			validate = 1;

		(void)close(f);
	}

	if (!validate && readFile2) {
		f = open(readFile2, O_RDONLY);
		if (f == -1)
			return(OWCFG_OPEN2FAIL); /*Leave well enough alone*/
		if (fstat(f, &statbuf) == -1) {
			(void)close(f);
			return(OWCFG_FSTAT2FAIL); /*Leave well enough alone*/
		}
		if (statbuf.st_mtime > readFile2TimeStamp)
			validate = 1;

		(void)close(f);
	}

	if (validate) {
		struct nodeStruct *oldHead, *oldTail;

		oldHead = OWconfigDatabase.head;
		oldTail = OWconfigDatabase.tail;
		/***  Setting databaseValid to zero causes initDatabase()
		      to NOT free existing database...
		***/
		databaseValid = 0;
		result = OWconfigInit(readFile1, readFile2, OWconfigFlags,
				      allocMem, freeMem);
		if (result != OWCFG_OK) {
			OWconfigDatabase.head = oldHead;
			OWconfigDatabase.tail = oldTail;
			databaseValid = 1;
		} else {
			freeNodeList(oldHead);
		}
	}

	return(result);
}









/***************************************************************************
OWconfigInit:

"OWconfigInit" will read the OWconfig files named by readfile1 and
readfile2.
Class instances and attribute/value pairs read from readfile2 will override
those read from readfile1.  The replacement will occur on a per
attribute/value pair basis.  Before a database file is read, it is
read locked using the fcntl facility.  Immediately after the file
is read, it is unlocked.
 
Return:  OWCFG_OK or !OWCFG_OK.  Bogus filepaths can return !OWCFG_OK.
	If readfile1 and readfile2 are both non-NULL, and the read of
	readfile1 succeeds but the read of readfile2 fails, OWCFG_OK
	will be returned.  Lock test failures will return !OWCFG_OK.
***************************************************************************/

int
OWconfigInit(char *readfile1, char *readfile2, int flags,
	     void *(*allocmem)(unsigned), void (*freemem)(void *))
{
	int result, result1, result2;

	if (allocmem)
		allocMem = allocmem;
	else
		allocMem = malloc;

	if (freemem)
		freeMem = freemem;
	else
		freeMem = free;

	initDatabase();

	if (readfile1 != readFile1)
		readFile1 = dupString(readfile1);

	if (readfile2 != readFile2)
		readFile2 = dupString(readfile2);

	OWconfigFlags = flags;
	lineNumber = 0;
	lastLineNumber = 0;

	/*** Check for specific corner cases... ***/
	if (readFile1 == NULL && readFile2 == NULL)
		return(OWCFG_OK);


	/*** Read file 1, if specified... ***/
	result1 = OWCFG_OK;
	if (readFile1) {
		result1 = readOWconfig(readFile1, &readFile1TimeStamp);
		if (result1 != OWCFG_OK && result1 != OWCFG_OPEN1FAIL) {
			initDatabase();
			return(result1);
		}
	}


	/*** Read file 2, if specified... ***/
	lastLineNumber = lineNumber;
	result2 = OWCFG_OK;
	if (readFile2) {
		result2 = readOWconfig(readFile2, &readFile2TimeStamp);
		if (result2 != OWCFG_OK && result2 != OWCFG_OPEN1FAIL) {
			initDatabase();
			if (result2 == OWCFG_LOCK1FAIL)
				result2 = OWCFG_LOCK2FAIL;
			if (result2 == OWCFG_SYNTAX1)
				result2 = OWCFG_SYNTAX2;
			return(result2);
		}
	}

	(void)strcpy(currentPackage, defaultNewPkgName);

	if (result1 == result2) {
		if (result1 == OWCFG_OK)
			return(OWCFG_OK);
		else
			return(OWCFG_OPENBOTHFAIL);
	} else {
		if (result1 == OWCFG_OPEN1FAIL)
			return(OWCFG_OPEN1FAIL);
		else
			return(OWCFG_OPEN2FAIL);
	}

}


