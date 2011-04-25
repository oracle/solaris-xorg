/*
 * Copyright (c) 2008, 2009, Oracle and/or its affiliates. All rights reserved.
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

/*
 * efb_properties - efb device-specific properties
 *
 *    XVR-50    (pfb w/ Xsun)
 *    XVR-100   (pfb w/ Xsun)
 *    XVR-300   (nfb w/ Xsun)
 *    XVR-300x8 (nfb w/ Xsun)
 */

#include <stdlib.h>		/* free() */

#include "fbc.h"		/* Common fbconf_xorg(1M) definitions */
#include "fbc_dev.h"		/* Identify the graphics device (-dev opt) */
#include "fbc_error.h"		/* Error reporting */
#include "fbc_getargs.h"	/* Program command line processing */
#include "fbc_help.h"		/* Program usage and help messages */
#include "fbc_properties.h"	/* Establish fbconf_xorg program properties */
#include "fbc_propt.h"		/* Display the current option settings */
#include "fbc_query_device.h"	/* Query a frame buffer device */
#include "fbc_xorg.h"		/* Edit config file data representations */

#include "efb_prconf.h"		/* Display efb hardware configuration */
#include "efb_predid.h"		/* Display EDID data */
#include "efb_properties.h"	/* efb device-specific properties */
#include "efb_query_device.h"	/* Query the efb graphics device */
#include "efb_res_try_now.h"	/* Video mode setting (-res try now) */


/*
 * Tell fbconf_xorg(1M) what API version libSUNWefb_conf is using
 */
fbc_api_ver_t SUNWefb_api_version = FBC_API_VERSION;


/*
 * Usage (error) message text
 */
static const char	*efb_usage_text_body_xvr_50 =
	" [-dev device-filename]\n"
	"\t\t  [-file machine | system | config-path]\n"
	"\t\t  [-res video-mode [nocheck | noconfirm] [try] [now]]\n"
	"\t\t  [-g gamma-value]\n"
	"\t\t  [-rscreen enable | disable]\n"
	"\t\t  [-defaults]\n"
	"\t\t  [-help]\n"
	"\t\t  [-res \\?]\n"
	"\t\t  [-prconf] [-predid [raw] [parsed]] [-propt]\n"
	"\n";

static const char	*efb_usage_text_body_xvr_100 =
	" [-dev device-filename]\n"
	"\t\t  [-file machine | system | config-path]\n"
	"\t\t  [-res video-mode [nocheck | noconfirm] [try] [now]]\n"
	"\t\t  [-clone enable | disable]\n"
	"\t\t  [-doublewide enable | disable]\n"
	"\t\t  [-doublehigh enable | disable]\n"
	"\t\t  [-g gamma-value]\n"
	"\t\t  [-offset xoff-value yoff-value]\n"
	"\t\t  [-outputs swapped | direct]\n"
	"\t\t  [-defaults]\n"
	"\t\t  [-help]\n"
	"\t\t  [-res \\?]\n"
	"\t\t  [-prconf] [-predid [raw] [parsed]] [-propt]\n"
	"\n";

static const char	*efb_usage_text_body_xvr_300 =
	" [-dev devname] [-file machine | system | config-path]\n"
	"\t\t  [-res video-mode [nocheck | noconfirm] [try] [now]]\n"
	"\t\t  [-clone enable | disable]\n"
	"\t\t  [-doublewide enable | disable]\n"
	"\t\t  [-doublehigh enable | disable]\n"
	"\t\t  [-g gamma-value]\n"
	"\t\t  [-offset xoff-value yoff-value]\n"
	"\t\t  [-outputs swapped | direct]\n"
	"\t\t  [-defaults]\n"
	"\t\t  [-help]\n"
	"\t\t  [-res \\?]\n"
	"\t\t  [-prconf] [-predid [raw] [parsed]] [-propt]\n"
	"\n";


/*
 * Help (-help) message text
 */
static const char efb_help_offset[] =		/* For XVR-100 & XVR-300 */
"	-offset		Adjusts the position of the secondary stream.\n"
"			Currently only implemented in -doublewide and\n"
"			-doublehigh modes.\n"
"			With -doublewide, the xoff-value is used to position\n"
"			the secondary DVI stream.  With -doublehigh, the\n"
"			yoff-value is used instead.  The secondary stream is\n"
"			2 if the -outputs setting is direct, and 1 if it is\n"
"			swapped.  A negative value specifies the overlapped\n"
"			region with the primary stream.  A positive value is\n"
"			treated as 0.\n"
"			Default: [0,0]\n";


/*
 * Command line option descriptors for fbconf_xorg(1M) and efb devices
 *
 *    These table entries must be in -help display order.
 *
 *    When fbc_Option_keyword() is the "Option handler function"
 *    (fbopt_descr_t.fbc_getopt_fn member), the "Config Option entry
 *    name(s)" string (fbopt_descr_t.conf_name member) is composed of
 *    contiguous Nul-terminated Option names that are terminated by an
 *    additional Nul, e.g.:
 *        FBC_KEYWD_DoubleWide "\0" FBC_KEYWD_DoubleHigh "\0"
 *    or (nominally):
 *        "DoubleWide\0DoubleHigh\0\0"
 *
 *    See fbc_getopt.h for the relevant declarations and definitions.
 */

static fbopt_descr_t	efb_option_xvr_50[] = {
	{
	/* -defaults */
		"defaults",		/* Command line option name	*/
		fbc_help_defaults,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_defaults,	/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL		/* *** POSITIVELY NO RECURSIVE DEFAULTS! *** */
	},
	{
	/* -dev <device> */
		"dev",			/* Command line option name	*/
		fbc_help_dev,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_dev,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTN_Device,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -file machine|system|<config-path> */
		"file",			/* Command line option name	*/
		fbc_help_file,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_file,		/* Option handler function	*/
		fbc_keywds_file,	/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -g <gamma-value> */
		"g",			/* Command line option name	*/
		fbc_help_g,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_Gamma,		/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_Gamma,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		fbc_defargv_g		/* argv[] invoked by -defaults	*/
	},
	{
	/* -help */
		"help",			/* Command line option name	*/
		fbc_help_help,		/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_help,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -prconf */
		"prconf",		/* Command line option name	*/
		fbc_help_prconf,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_prconf,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -predid [raw] [parsed] */
		"predid",		/* Command line option name	*/
		fbc_help_predid,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_predid,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -propt */
		"propt",		/* Command line option name	*/
		fbc_help_propt,		/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_propt,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -res ? */
	/* -res <video-mode> [nocheck|noconfirm] [try] [now] */
		"res",			/* Command line option name	*/
		fbc_help_res_nntn,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_res,		/* Option handler function	*/
		fbc_keywds_res_nntn,	/* Command line option keywords	*/
		FBC_SECTN_Res,		/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -rscreen enable|disable */
		"rscreen",		/* Command line option name	*/
		fbc_help_rscreen,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_RScreen,	/* Config section code		*/
		FBC_KEYWD_RScreen "\0",	/* Config Option entry name(s)	*/
		fbc_defargv_rscreen	/* argv[] invoked by -defaults	*/
	},

	{
	/* End-of-table marker */
		NULL,			/* Command line option name	*/
		NULL,			/* Help text			*/
		0,			/* Min # of option arguments	*/
		NULL,			/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	}
};


static fbopt_descr_t	efb_option_xvr_100[] = {
	{
	/* -clone enable|disable */
		"clone",		/* Command line option name	*/
		fbc_help_clone,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_Clone,	/* Config section code		*/
		FBC_KEYWD_Clone "\0",	/* Config Option entry name(s)	*/
		fbc_defargv_clone	/* argv[] invoked by -defaults	*/
	},
	{
	/* -defaults */
		"defaults",		/* Command line option name	*/
		fbc_help_defaults,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_defaults,	/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL		/* *** POSITIVELY NO RECURSIVE DEFAULTS! *** */
	},
	{
	/* -dev <device> */
		"dev",			/* Command line option name	*/
		fbc_help_dev,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_dev,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTN_Device,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -doublewide enable|disable */
		"doublewide",		/* Command line option name	*/
		fbc_help_doublewide,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_DoubleWide,	/* Config section code		*/
		FBC_KEYWD_DoubleWide "\0"
			FBC_KEYWD_DoubleHigh "\0",
		fbc_defargv_doublewide	/* argv[] invoked by -defaults	*/
	},
	{
	/* -doublehigh enable|disable */
		"doublehigh",		/* Command line option name	*/
		fbc_help_doublehigh,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_DoubleHigh,	/* Config section code		*/
		FBC_KEYWD_DoubleHigh "\0"
			FBC_KEYWD_DoubleWide "\0",
		fbc_defargv_doublehigh	/* argv[] invoked by -defaults	*/
	},
	{
	/* -file machine|system|<config-path> */
		"file",			/* Command line option name	*/
		fbc_help_file,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_file,		/* Option handler function	*/
		fbc_keywds_file,	/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -g <gamma-value> */
		"g",			/* Command line option name	*/
		fbc_help_g,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_Gamma,		/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_Gamma,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		fbc_defargv_g		/* argv[] invoked by -defaults	*/
	},
	{
	/* -help */
		"help",			/* Command line option name	*/
		fbc_help_help,		/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_help,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -offset <xoff> <yoff> */
		"offset",		/* Command line option name	*/
		fbc_help_offset,	/* Help text			*/
		2,			/* Min # of option arguments	*/
		fbc_Option_Stream_Offset, /* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTN_StreamXOffset, /* Config section code		*/
		FBC_KEYWD_StreamXOffset "\0" FBC_KEYWD_StreamYOffset "\0",
		fbc_defargv_offset	/* argv[] invoked by -defaults	*/
	},
	{
	/* -outputs swapped|direct */
		"outputs",		/* Command line option name	*/
		fbc_help_outputs,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_swappedirect, /* Command line option keywords */
		FBC_SECTN_Outputs,	/* Config section code		*/
		FBC_KEYWD_Outputs "\0",	/* Config Option entry name(s)	*/
		fbc_defargv_outputs	/* argv[] invoked by -defaults	*/
	},
	{
	/* -prconf */
		"prconf",		/* Command line option name	*/
		fbc_help_prconf,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_prconf,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -predid [raw] [parsed] */
		"predid",		/* Command line option name	*/
		fbc_help_predid,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_predid,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -propt */
		"propt",		/* Command line option name	*/
		fbc_help_propt,		/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_propt,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -res ? */
	/* -res <video-mode> [nocheck|noconfirm] [try] [now] */
		"res",			/* Command line option name	*/
		fbc_help_res_nntn,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_res,		/* Option handler function	*/
		fbc_keywds_res_nntn,	/* Command line option keywords	*/
		FBC_SECTN_Res,		/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},

	{
	/* End-of-table marker */
		NULL,			/* Command line option name	*/
		NULL,			/* Help text			*/
		0,			/* Min # of option arguments	*/
		NULL,			/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	}
};


static fbopt_descr_t	efb_option_xvr_300[] = {
	{
	/* -clone enable|disable */
		"clone",		/* Command line option name	*/
		fbc_help_clone,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_Clone,	/* Config section code		*/
		FBC_KEYWD_Clone "\0",	/* Config Option entry name(s)	*/
		fbc_defargv_clone	/* argv[] invoked by -defaults	*/
	},
	{
	/* -defaults */
		"defaults",		/* Command line option name	*/
		fbc_help_defaults,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_defaults,	/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL		/* *** POSITIVELY NO RECURSIVE DEFAULTS! *** */
	},
	{
	/* -dev <device> */
		"dev",			/* Command line option name	*/
		fbc_help_dev,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_dev,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTN_Device,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -doublehigh enable|disable */
		"doublehigh",		/* Command line option name	*/
		fbc_help_doublehigh,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_DoubleHigh,	/* Config section code		*/
		FBC_KEYWD_DoubleHigh "\0"
			FBC_KEYWD_DoubleWide "\0",
		fbc_defargv_doublehigh	/* argv[] invoked by -defaults	*/
	},
	{
	/* -doublewide enable|disable */
		"doublewide",		/* Command line option name	*/
		fbc_help_doublewide,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_DoubleWide,	/* Config section code		*/
		FBC_KEYWD_DoubleWide "\0"
			FBC_KEYWD_DoubleHigh "\0",
		fbc_defargv_doublewide	/* argv[] invoked by -defaults	*/
	},
	{
	/* -file machine|system|<config-path> */
		"file",			/* Command line option name	*/
		fbc_help_file,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_file,		/* Option handler function	*/
		fbc_keywds_file,	/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -g <gamma-value> */
		"g",			/* Command line option name	*/
		fbc_help_g,		/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_Gamma,		/* Option handler function	*/
		fbc_keywds_xable,	/* Command line option keywords	*/
		FBC_SECTN_Gamma,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		fbc_defargv_g		/* argv[] invoked by -defaults	*/
	},
	{
	/* -help */
		"help",			/* Command line option name	*/
		fbc_help_help,		/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_help,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -offset <xoff> <yoff> */
		"offset",		/* Command line option name	*/
		efb_help_offset,	/* Help text			*/
		2,			/* Min # of option arguments	*/
		fbc_Option_Stream_Offset, /* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTN_StreamXOffset, /* Config section code		*/
		FBC_KEYWD_StreamXOffset "\0" FBC_KEYWD_StreamYOffset "\0",
		fbc_defargv_offset	/* argv[] invoked by -defaults	*/
	},
	{
	/* -outputs swapped|direct */
		"outputs",		/* Command line option name	*/
		fbc_help_outputs,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_Option_keyword,	/* Option handler function	*/
		fbc_keywds_swappedirect, /* Command line option keywords */
		FBC_SECTN_Outputs,	/* Config section code		*/
		FBC_KEYWD_Outputs "\0",	/* Config Option entry name(s)	*/
		fbc_defargv_outputs	/* argv[] invoked by -defaults	*/
	},
	{
	/* -prconf */
		"prconf",		/* Command line option name	*/
		fbc_help_prconf,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_prconf,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -predid [raw] [parsed] */
		"predid",		/* Command line option name	*/
		fbc_help_predid,	/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_predid,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -propt */
		"propt",		/* Command line option name	*/
		fbc_help_propt,		/* Help text			*/
		0,			/* Min # of option arguments	*/
		fbc_opt_propt,		/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},
	{
	/* -res ? */
	/* -res <video_mode> [nocheck|noconfirm] [try] [now] */
		"res",			/* Command line option name	*/
		fbc_help_res_nntn,	/* Help text			*/
		1,			/* Min # of option arguments	*/
		fbc_opt_res,		/* Option handler function	*/
		fbc_keywds_res_nntn,	/* Command line option keywords	*/
		FBC_SECTN_Res,		/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	},

	{
	/* End-of-table marker */
		NULL,			/* Command line option name	*/
		NULL,			/* Help text			*/
		0,			/* Min # of option arguments	*/
		NULL,			/* Option handler function	*/
		NULL,			/* Command line option keywords	*/
		FBC_SECTION_NONE,	/* Config section code		*/
		NULL,			/* Config Option entry name(s)	*/
		NULL			/* argv[] invoked by -defaults	*/
	}
};


/*
 * List of functions to display the current option settings (-propt)
 */
static fbc_propt_fn_t	*efb_propt_fn_xvr_50[] = {
	fbc_propt_file,			/* Configuration file */
	fbc_propt_video_mode,		/* Current video mode name: -res */
	fbc_propt_screen_title,		/* Screen settings title */
	fbc_propt_rscreen,		/* Remote console setting: -rscreen */
	fbc_propt_visual_title,		/* Visual Information title */
	fbc_propt_g,			/* Gamma setting: -g only */
	NULL				/* End of table */
};

static fbc_propt_fn_t	*efb_propt_fn_xvr_100[] = {
	fbc_propt_file,			/* Configuration file */
	fbc_propt_video_mode,		/* Current video mode name: -res */
	fbc_propt_screen_title,		/* Screen settings title */
	fbc_propt_dual_screen,		/* Dual-screen: -doublexxxxx */
	fbc_propt_clone,		/* Clone setting */
	fbc_propt_offset,		/* Screen offset settings */
	fbc_propt_outputs,		/* Outputs setting */
	fbc_propt_visual_title,		/* Visual Information title */
	fbc_propt_g,			/* Gamma setting: -g only */
	NULL				/* End of table */
};

static fbc_propt_fn_t	*efb_propt_fn_xvr_300[] = {
	fbc_propt_file,			/* Configuration file */
	fbc_propt_video_mode,		/* Current video mode name: -res */
	fbc_propt_screen_title,		/* Screen settings title */
	fbc_propt_dual_screen,		/* Dual-screen: -doublexxxxx */
	fbc_propt_clone,		/* Clone setting */
	fbc_propt_offset,		/* Screen offset settings */
	fbc_propt_outputs,		/* Outputs setting */
	fbc_propt_visual_title,		/* Visual Information title */
	fbc_propt_default_visual,	/* Default visual: -defxxxxx" */
	fbc_propt_g,			/* Gamma setting: -g only */
	NULL				/* End of table */
};


/*
 * SUNWefb_get_properties()
 *
 *    Return the fbconf_xorg(1M) properties for the efb frame buffer
 *    device type.
 */

int
SUNWefb_get_properties(
	fbc_dev_t	*device,	/* Frame buffer device info (-dev) */
	fbc_varient_t	*fbvar)		/* Updated fbconf_xorg properties */
{
	char		*full_model_name; /* Frame buf full name w/ "SUNW," */
	char		*simple_model_name; /* Frame buf simple model name */

	/*
	 * Provide some more frame buffer device information
	 */
	device->max_streams = 1;	/* XVR-50 has no 'a'|'b' suffixes */

	/*
	 * Establish the device properties and the fbconf_xorg(1M) behavior
	 */
	fbvar->usage_text_body   = efb_usage_text_body_xvr_300;

	fbvar->gamma_default     = FBC_GAMMA_DEFAULT;

	fbvar->lut_size          = 0;	/* No gamma look-up table w/ efb */
	fbvar->fbc_option        = &efb_option_xvr_300[0];

	fbvar->xf86_entry_mods.Option_mods_size =
			sizeof (efb_option_xvr_300) / sizeof (fbopt_descr_t)
			+ 1;		/* StreamYOffset */

	fbvar->get_edid_res_info = &efb_get_edid_res_info;
	fbvar->revise_settings   = NULL;
	fbvar->init_device       = NULL;
	fbvar->prconf            = &efb_prconf;
	fbvar->predid            = &efb_predid;
	fbvar->propt_fn          = &efb_propt_fn_xvr_300[0];
	fbvar->res_mode_try      = &efb_res_mode_try;
	fbvar->res_mode_now      = &efb_res_mode_now;

	/*
	 * Distinguish between the various frame buffer models
	 */
	full_model_name =
		fbc_get_fb_model_name(device->fd, &simple_model_name);
	if (full_model_name != NULL) {
		if (strcmp(full_model_name, "SUNW,XVR-50") == 0) {
			fbvar->usage_text_body = efb_usage_text_body_xvr_50;
			fbvar->fbc_option      = &efb_option_xvr_50[0];
			fbvar->xf86_entry_mods.Option_mods_size =
				sizeof (efb_option_xvr_50)
						/ sizeof (fbopt_descr_t);
			fbvar->propt_fn        = &efb_propt_fn_xvr_50[0];
		} else
		if (strcmp(full_model_name, "SUNW,XVR-100") == 0) {
			device->max_streams    = 2;  /* 'a'|'b' suffixes */

			fbvar->usage_text_body = efb_usage_text_body_xvr_100;
			fbvar->fbc_option      = &efb_option_xvr_100[0];
			fbvar->xf86_entry_mods.Option_mods_size =
				sizeof (efb_option_xvr_100)
						/ sizeof (fbopt_descr_t);
			fbvar->propt_fn        = &efb_propt_fn_xvr_100[0];
		} else {
			/*
			 * XVR-300[x8]
			 */
			device->max_streams    = 2;  /* 'a'|'b' suffixes */

			fbvar->usage_text_body = efb_usage_text_body_xvr_300;
			fbvar->fbc_option      = &efb_option_xvr_300[0];
			fbvar->xf86_entry_mods.Option_mods_size =
				sizeof (efb_option_xvr_300)
						/ sizeof (fbopt_descr_t);
			fbvar->propt_fn        = &efb_propt_fn_xvr_300[0];
		}
		free(full_model_name);
	}

	return (FBC_SUCCESS);

}	/* SUNWefb_get_properties() */


/* End of efb_properties.c */
