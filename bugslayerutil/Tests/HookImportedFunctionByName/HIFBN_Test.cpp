/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
// I lifted Matt Pietrek's HookAPI sample program!!

#include <windows.h>
#include <stdio.h>
#include "BugslayerUtil.h"

typedef int (__stdcall *MESSAGEBOXPROC)(HWND , LPCSTR , LPCSTR , UINT) ;

MESSAGEBOXPROC pfnOrigMsgBox = NULL ;

int WINAPI SimonMsgBox ( HWND   hWnd        ,
                         LPCSTR lpText      ,
                         LPCSTR lpCaption   ,
                         UINT   uType        )
{
    int iRet ;
    LPSTR szNewTitle = new char[ strlen ( lpCaption ) + 40 ] ;

    strcpy ( szNewTitle , "Simon Sez: " ) ;
    strcat ( szNewTitle , lpCaption ) ;
    iRet = pfnOrigMsgBox ( hWnd , lpText , szNewTitle , uType ) ;
    delete [] szNewTitle ;
    return ( iRet ) ;
}

void GetAroundOptimizerOne ( char * szTitle )
{
    MessageBox ( GetForegroundWindow ( )        ,
                 "We should be hooked - Do you see Simon "
                      "Sez in title bar?",
                 szTitle                        ,
                 MB_OK                           ) ;
}

void GetAroundOptimizerTwo ( char * szTitle )
{
    MessageBox ( GetForegroundWindow ( )            ,
                 "We should not be hooked at all - "
                   "No Simon Sez in title bar!"     ,
                 szTitle                            ,
                 MB_OK                               ) ;

}


void main ( void )
{
    MessageBox ( GetForegroundWindow ( )                            ,
                 "Ain't hooked yet! - No Simon Sez in title bar"    ,
                 "HookImportedFunctionsByName! ANSI"                ,
                 MB_OK                                               ) ;

    HOOKFUNCDESC stToHook ;

    stToHook.szFunc = "MessageBoxA" ;
    stToHook.pProc  = (PROC)SimonMsgBox ;

    DWORD uiCount ;

    BOOL bRet =
        HookImportedFunctionsByName ( GetModuleHandle ( NULL ) ,
                                      "USER32.DLL"             ,
                                      1                        ,
                                      &stToHook                ,
                                      (PROC*)&pfnOrigMsgBox    ,
                                      &uiCount                  ) ;
    if ( FALSE == bRet )
    {
        printf ( "We couldn't hook it!\n" ) ;
    }

    // If the code is optimized, calls to MessageBoxA in this function
    // are enregisted (as they should be!), but it makes it difficult
    // to see the results of the hook as all the calls go through the
    // register, not the IAT!

    GetAroundOptimizerOne ( "HookImportedFunctionsByNameA!" ) ;

    // Now unhook.


    stToHook.szFunc = "MessageBoxA" ;
    stToHook.pProc  = (PROC)pfnOrigMsgBox ;

    if ( FALSE ==
            HookImportedFunctionsByName ( GetModuleHandle ( NULL ) ,
                                          "USER32.DLL"             ,
                                          1                        ,
                                          &stToHook                ,
                                          (PROC*)&pfnOrigMsgBox    ,
                                          &uiCount                   ) )
    {
        printf ( "We couldn't unhook it!\n" ) ;
    }

    // Call message box again!
    GetAroundOptimizerTwo ( "HookImportedFunctionsByNameA!" ) ;

}

