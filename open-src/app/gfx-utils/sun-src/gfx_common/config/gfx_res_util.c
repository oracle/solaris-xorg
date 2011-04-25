/*
 * Copyright (c) 1995, 2004, Oracle and/or its affiliates. All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <search.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>

#include <X11/Sunowconfig.h>		/* for OWconfig routines */

#include <gfx_res_util.h>		/* our definitions */
#include <gfx_list_util.h>


static char	   *owvid_class    = "XVIDEOTIMING";
static void        *res_table_root =  NULL;


typedef enum {
    video_Hact        ,
    video_Hfp         ,
    video_Hsw         ,
    video_Hbp         ,
    video_Hsrp        ,

    video_Vact        ,
    video_Vfp         ,
    video_Vsw         ,
    video_Vse         ,
    video_Vbp         ,

    video_Pclk        ,
    video_Fint        ,
    video_Fseq        ,
    video_Fst         ,

    video_Stype       ,
    video_SonG        ,
    video_SHpol       ,
    video_SVpol       ,
    video_SGpol       ,

    video_EncodStd    ,
    video_HUndScan    ,
    video_VUndScan    ,

    res_label_last
} GFXResLabel;



static OWConfigTableEntry GFXResTable[] = {
    { video_Hact        , "HorizActive"               },
    { video_Hfp         , "HorizFrontPorch"           },
    { video_Hsw         , "HorizSyncWidth"            },
    { video_Hbp         , "HorizBackPorch"            },
    { video_Hsrp        , "HorizSyncWidthDVS"         },

    { video_Vact        , "VertActive"                },
    { video_Vfp         , "VertFrontPorch"            },
    { video_Vsw         , "VertSyncWidth"             },
    { video_Vse         , "VertSyncExtension"         },
    { video_Vbp         , "VertBackPorch"             },

    { video_Pclk        , "PixelClock"                },
    { video_Fint        , "Interlace"                 },
    { video_Fseq        , "FieldSeq"                  },
    { video_Fst         , "Stereo"                    },

    { video_Stype       , "SyncType"                  },
    { video_SonG        , "SyncOnG"                   },
    { video_SHpol       , "SyncHorizPolarity"         },
    { video_SVpol       , "SyncVertPolarity"          },
    { video_SGpol       , "SyncGreenPolarity"         },

    { video_EncodStd    , "EncoderStandard"           },
    { video_HUndScan    , "EncoderHorizUnderscan"     },
    { video_VUndScan    , "EncoderVertUnderscan"      },

    { res_label_last    ,  NULL                       }
};






/* attribute values used in OWconfig file */
static char *yes = "YES";
static char *no  = "NO";


#ifndef TRUE
#define TRUE 1
#define FALSE 0
#endif






/*
 *  This function loads a video resolution from the file,
 *  and fills in the generic SunVideoTiming structure.
 *  It returns an error code if a problem occurs.
 *
 *  This function requires a wrapper to fill in optional
 *  fields that may not be in the file.
 *  A minimal one is provided below: sun_get_ow_video_values_raw().
 *  Another one fills in the values for 1152x900 as
 *  a fallback in case the file read totally fails,
 *  it will always return something valid (for X windows).
 *
 */

static int
sun_get_ow_video_values_file (char             *timingname,
	                      SunVideoTiming   *vidtiming   )
{
	OWconfigAttribute  *ap;
	OWconfigAttribute  *inst_ptr;
	OWConfigTableEntry *entry;
	int                 attr_cnt;
	int                 i;

	if (timingname==NULL || *timingname==0) {
#ifdef DEBUG1
	    fprintf( stderr, "Timing Name was empty or NULL.\n");
#endif
	    return -1;
	}


	/* get all the attributes for our class and instance */

	inst_ptr = OWconfigGetInstance(owvid_class, timingname, &attr_cnt);
	if (!inst_ptr) {
#ifdef DEBUG1
	    fprintf( stderr, "Video Timing Information was missing in file.\n");
	    fprintf( stderr, "    Requested Resolution: (%s)\n", timingname);
#endif
	    return -1;
	} else {
	    vidtiming->id_string = strdup(timingname);
	}

	ow_table_init(&res_table_root, GFXResTable);

	/* now extract the values we're interested in */
	for (ap = inst_ptr, i = 0; i < attr_cnt; ap++, i++) {
	    entry = ow_table_find(&res_table_root, ap->attribute);
	    if ( entry == NULL ) {
#ifdef DEBUG1
	        fprintf( stderr, "OWconfig entry invalid: %s\n", ap->attribute);
#endif
	    } else {
#ifdef DEBUG1
#if 0           /* Helpful for intense debugging of file problems */
	        fprintf(stderr, "%s, %s %d\n",
	                ap->attribute, entry->name, entry->label);
#endif
#endif
	        switch (entry->label) {
	          case   video_Hact:
	            vidtiming->Hact = atoi(ap->value);
	            break;
	          case   video_Hfp:
	            vidtiming->Hfp  = atoi(ap->value);
	            break;
	          case   video_Hsw:
	            vidtiming->Hsw  = atoi(ap->value);
	            break;
	          case   video_Hbp:
	            vidtiming->Hbp  = atoi(ap->value);
	            break;
	          case   video_Hsrp:
	            vidtiming->Hsrp = atoi(ap->value);
	            break;

	          case   video_Vact:
	            vidtiming->Vact = atoi(ap->value);
	            break;
	          case   video_Vfp:
	            vidtiming->Vfp  = atoi(ap->value);
	            break;
	          case   video_Vsw:
	            vidtiming->Vsw  = atoi(ap->value);
	            break;
	          case   video_Vse:
	            vidtiming->Vse  = atoi(ap->value);
	            break;
	          case   video_Vbp:
	            vidtiming->Vbp  = atoi(ap->value);
	            break;
	          case   video_Pclk:
	            vidtiming->Pclk = atoi(ap->value);
	            break;
	          case   video_Fint:
	            vidtiming->Fint =
	                (!strncasecmp(ap->value,  yes , 3)) ? TRUE : FALSE;
	            break;
	          case   video_Fseq:
	            vidtiming->Fseq =
	                (!strncasecmp(ap->value,  yes , 3)) ? TRUE : FALSE;
	            break;
	          case   video_Fst:
	            vidtiming->Fst =
	                (!strncasecmp(ap->value,  yes , 3)) ? TRUE : FALSE;
	            break;
	          case   video_Stype:
	            vidtiming->Stype =
	                (!strncasecmp(ap->value, "sep", 3)) ? TRUE : FALSE;
	            break;
	          case   video_SonG:
	            vidtiming->SonG  =
	                (!strncasecmp(ap->value,  yes , 3)) ? TRUE : FALSE;
	            break;
	          case   video_SHpol:
	            vidtiming->SHpol =
	                (!strncasecmp(ap->value, "pos", 3)) ? TRUE : FALSE;
	            break;
	          case   video_SVpol:
	            vidtiming->SVpol =
	                (!strncasecmp(ap->value, "pos", 3)) ? TRUE : FALSE;
	            break;
	          case   video_SGpol:
	            vidtiming->SGpol =
	                (!strncasecmp(ap->value, "pos", 3)) ? TRUE : FALSE;
	            break;
	          case video_EncodStd:
	            if ( !strcasecmp(ap->value, "ntsc") ) {
	              vidtiming->Encod = SunVideoEncodingNTSC;
	            }
	            if ( !strcasecmp(ap->value, "pal") ) {
	              vidtiming->Encod = SunVideoEncodingPAL;
	            }
	            break;
	          case video_HUndScan:
	            vidtiming->Hund = (double)atof(ap->value);
	            break;
	          case video_VUndScan:
	            vidtiming->Vund = (double)atof(ap->value);
	            break;
	          default:
#ifdef DEBUG1
                    fprintf(stderr, "Timing: Invalid attribute: %s = %s\n",
                            ap->attribute, entry->name);
#endif
	            break;
	        }

	    }
	}

	/* free up attribute memory */
	OWconfigFreeInstance(inst_ptr, attr_cnt);

	return 0;
}



/*
 *  This function loads a video resolution from the file.
 *  It returns an error code if a problem occurs.
 *
 *  This wrapper fills in optional fields, and
 *  it flags an error if required fields are missing.
 *
 */

int
sun_get_ow_video_values_raw (char             *timingname,
	                     SunVideoTiming   *vidtiming   )
{
	if (timingname==NULL || *timingname==0) {
#ifdef DEBUG1
	    fprintf( stderr, "Timing Name was empty or NULL.\n");
#endif
	    return -1;
	}

	/* Default values if field is missing (optional) in the file */
	/*    Fields set to -1 hear are mandatory                    */

	vidtiming->Hact      =    -1 ;
	vidtiming->Hfp       =    -1 ;
	vidtiming->Hsw       =    -1 ;
	vidtiming->Hbp       =    -1 ;
	vidtiming->Hsrp      =     0 ; /* Default to OFF, zero */
	vidtiming->Vact      =    -1 ;
	vidtiming->Vfp       =    -1 ;
	vidtiming->Vsw       =    -1 ;
	vidtiming->Vse       =     0 ;
	vidtiming->Vbp       =    -1 ;
	vidtiming->Pclk      =    -1 ;
	vidtiming->Fint      = FALSE ; /* Defaults, if not specified */
	vidtiming->Fseq      = FALSE ;
	vidtiming->Fst       = FALSE ;
	vidtiming->Stype     = FALSE ; /* Default composite */
	vidtiming->SonG      = FALSE ;
	vidtiming->SHpol     = FALSE ; /* Default negative */
	vidtiming->SVpol     = FALSE ; /* Default negative */
	vidtiming->SGpol     = FALSE ; /* Default negative */
	vidtiming->Encod     = SunVideoEncodingNone;
	vidtiming->Hund      =   0.0 ;
	vidtiming->Vund      =   0.0 ;



	if ( sun_get_ow_video_values_file (timingname, vidtiming) != 0 ) {
#ifdef DEBUG1
	    fprintf( stderr, "Video Timing Information was missing in file\n");
	    fprintf( stderr, "    Requested Resolution: %s\n", timingname);
#endif
	    return -1;
	}


	/* Make sure all mandatory fields got read from file */

	if (vidtiming->Hact == -1) return -1;
	if (vidtiming->Hfp  == -1) return -1;
	if (vidtiming->Hsw  == -1) return -1;
	if (vidtiming->Hbp  == -1) return -1;
	if (vidtiming->Vact == -1) return -1;
	if (vidtiming->Vfp  == -1) return -1;
	if (vidtiming->Vsw  == -1) return -1;
	if (vidtiming->Vbp  == -1) return -1;
	if (vidtiming->Pclk == -1) return -1;

	return 0;
}


/*
 *  This functions looks up a video resolutions in OWconfig,
 *  and fills in the generic SunVideoTiming structure.
 *
 *  This is the "nice" version that prints out error
 *  messages on stderr (non debug ones, that is).
 *  It also returns one hardcoded resolution (1152x900x66),
 *  if things fail, just so you can always see that something
 *  has gone wrong (for corrupt or missing resolution files).
 *  Each card writer should consider whether this is a 
 *  reasonable default resolution.  If not they should 
 *  make thier own version of this wrapper, and call the
 *  raw version of this function.  This may also be
 *  required if printing messages on stderr is a problem
 *  while not doing debugging (even for fatal errors).
 *
 */

int
sun_get_ow_video_values_1152 (char             *timingname,
	                      SunVideoTiming   *vidtiming   )
{
	/* fallback values */

	vidtiming->Hact      =  1152 ; /* 1152x900x66 is default resolution */
	vidtiming->Hfp       =    40 ;
	vidtiming->Hsw       =   128 ;
	vidtiming->Hbp       =   208 ;
	vidtiming->Hsrp      =     0 ; /* Default to OFF, zero */
	vidtiming->Vact      =   900 ;
	vidtiming->Vfp       =     2 ;
	vidtiming->Vsw       =     4 ;
	vidtiming->Vbp       =    31 ;
	vidtiming->Pclk      = 94500 ;
	vidtiming->Fint      = FALSE ; /* Defaults, if not specified */
	vidtiming->Fseq      = FALSE ;
	vidtiming->Fst       = FALSE ;
	vidtiming->Stype     = FALSE ; /* Default composite */
	vidtiming->SonG      = FALSE ;
	vidtiming->SHpol     = FALSE ; /* Default negative */
	vidtiming->SVpol     = FALSE ; /* Default negative */
	vidtiming->SGpol     = FALSE ; /* Default negative */
	vidtiming->Encod     = SunVideoEncodingNone;
	vidtiming->Hund      = 0.0;
	vidtiming->Vund      = 0.0;

	if (timingname==NULL || *timingname==0) {
	    vidtiming->id_string = strdup("FALLBACK_1152x900x66");
#ifdef DEBUG1
	    fprintf( stderr, "Timing Name was empty or NULL.\n");
	    fprintf( stderr, "Using Fallback  Resolution: 1152x900x66 (hardcoded)\n");
#endif

	    return 0;        /* not an error for this wrapper */
	}

	if ( sun_get_ow_video_values_file (timingname, vidtiming) != 0 ) {
	    vidtiming->id_string = strdup("FALLBACK_1152x900x66");

#ifdef DEBUG1  /* Don't print this twice in debug mode */
	    fprintf( stderr, "Video Timing Information was missing in file\n");
	    fprintf( stderr, "    Requested Resolution: %s\n", timingname);
	    fprintf( stderr, "    Fallback  Resolution: 1152x900x66 (hardcoded)\n");
	    fprintf( stderr, "Recheck the resolution name with fbconfig,\n");
	    fprintf( stderr, "  or reinstall package SUNWvid.\n");
#endif

	    return 0;   /* not an error for this wrapper */
	}

   	return 0;
}




char **
sun_get_ow_video_timing_names	( )
{
	char **name_list;

	name_list = OWconfigGetClassNames(owvid_class);
   	if (name_list == NULL) {
#ifdef DEBUG1
	    fprintf( stderr, "Timing Names are inaccessable in OWconfig\n" );
	    fprintf( stderr, "   Class Name  = %s\n", owvid_class          );
#endif
	}

	return name_list;
}






#ifdef MAIN_TEST

#include <ow_file_util.h>

main( int argc ,char **argv )
{
        char    **video_names ;
        char    *video_name ;
        int     i;

	sun_gfx_load_owconfig( OWCONFIG_SYSTEM, OWCONFIG_MACHINE );

	video_names = sun_get_ow_video_timing_names( );

	if (video_names != NULL) {
	    for (i=0 ;  ; i++) {
	        video_name = video_names[i];
	        if (video_name == NULL) {
	            break;
	        } else {
	             printf( "Video Name (%2d) = %s\n", i, video_name);
	        }
	    }
	}
}

#endif /* MAIN_TEST */


