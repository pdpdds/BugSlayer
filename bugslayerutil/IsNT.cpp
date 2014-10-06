/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#include "PCH.h"
#include "BugslayerUtil.h"
#include "Internal.h"
#include <VersionHelpers.h>

/*//////////////////////////////////////////////////////////////////////
                           File Scope Globals
//////////////////////////////////////////////////////////////////////*/
// Indicates that the version information is valid.
static BOOL g_bHasVersion = FALSE ;
// Indicates NT or 95/98.
static BOOL g_bIsNT = TRUE ;

BOOL __stdcall IsNT ( void )
{
    if ( TRUE == g_bHasVersion )
    {
        return ( TRUE == g_bIsNT ) ;
    }

   
	if (IsWindowsXPOrGreater())
    {
        g_bIsNT = TRUE ;
    }
    else
    {
        g_bIsNT = FALSE ;
    }
    g_bHasVersion = TRUE ;
    return ( TRUE == g_bIsNT ) ;
}


