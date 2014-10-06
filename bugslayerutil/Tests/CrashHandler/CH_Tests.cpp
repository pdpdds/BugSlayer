/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdio.h>
#include "BugslayerUtil.h"

// Globals to control what gets done here.
BOOL  bGetFaultReason = TRUE ;
BOOL  bGetStackTraceString = TRUE ;
BOOL  bShowRegs = TRUE ;
BOOL  bLetItCrash = FALSE ;
DWORD dwOpts ;

LONG __stdcall ExcepCallBack ( EXCEPTION_POINTERS * pExPtrs )
{
    if ( TRUE == bGetFaultReason )
    {
        printf ( "%s\n" , GetFaultReason ( pExPtrs ) ) ;
    }

    if ( TRUE == bShowRegs )
    {
        printf ( "%s\n" , GetRegisterString ( pExPtrs ) ) ;
    }

    if ( TRUE == bGetStackTraceString )
    {
        const char * szBuff = GetFirstStackTraceString ( dwOpts  ,
                                                         pExPtrs  ) ;
        do
        {
            printf ( "%s\n" , szBuff ) ;
            fflush ( stdout ) ;
            szBuff = GetNextStackTraceString ( dwOpts , pExPtrs ) ;
        }
        while ( NULL != szBuff ) ;
    }

    if ( TRUE == bLetItCrash )
    {
        return ( EXCEPTION_CONTINUE_SEARCH ) ;
    }
    else
    {
        return ( EXCEPTION_EXECUTE_HANDLER ) ;
    }
}

void Baz ( int i )
{
    if ( FALSE == bLetItCrash )
    {
        __try
        {
            int * p = NULL ;
            *p = 0 ;
        }
        __except ( ExcepCallBack ( GetExceptionInformation ( ) ) )
        {
        }
    }
    else
    {
        int * q = NULL ;
        *q = 19 ;
    }
}

void Bar ( int i )
{
    Baz ( i + 1 ) ;
}

void Foo ( int i )
{
    Bar ( i + 1 ) ;
}


void main ( int argc , char * argv[] )
{
    // Test 1 - SetCrashHandler with NULL.
    printf ( "Test 1 - SetCrashHandler with NULL\n\n" ) ;
    if ( TRUE != SetCrashHandlerFilter ( NULL ) )
    {
        printf ( "Failed!! -> Test 1 - SetCrashHandler with NULL\n" ) ;
        return ;
    }

    // Test 2 - SetCrashHandler with good callback.
    printf ( "Test 2 - SetCrashHandler with good callback\n\n" ) ;
    if ( TRUE != SetCrashHandlerFilter ( ExcepCallBack ) )
    {
        printf ( "Failed!! -> Test 2 - SetCrashHandler with good "
                 "callback\n" ) ;
        return ;
    }

    // Test 3 - Do just GetFaultReason (Need to check output!)
    bGetFaultReason = TRUE ;
    bGetStackTraceString = FALSE ;
    bShowRegs = FALSE ;
    dwOpts = 0 ;
    printf ( "Test 3 - Do just GetFaultReason\n" ) ;
    Foo ( 3 ) ;
    printf ( "\n\n" ) ;

    // Test 4 - Do StackWalk - Options = 0
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = 0 ;
    printf ( "Test 4 - Do StackWalk - Options = 0\n" ) ;
    Foo ( 4 ) ;
    printf ( "\n\n" ) ;

    // Test 5 - Do StackWalk - Options = GSTSO_PARAMS
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_PARAMS ;
    printf ( "Test 5 - Do StackWalk - Options = GSTSO_PARAMS\n" ) ;
    Foo ( 5 ) ;
    printf ( "\n\n" ) ;

    // Test 6 - Do StackWalk - Options = GSTSO_MODULE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_MODULE ;
    printf ( "Test 6 - Do StackWalk - Options = GSTSO_MODULE\n" ) ;
    Foo ( 6 ) ;
    printf ( "\n\n" ) ;

    // Test 7 - Do StackWalk - Options = GSTSO_SYMBOL
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_SYMBOL ;
    printf ( "Test 7 - Do StackWalk - Options = GSTSO_SYMBOL\n" ) ;
    Foo ( 7 ) ;
    printf ( "\n\n" ) ;

    // Test 8 - Do StackWalk - Options = GSTSO_SRCLINE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_SRCLINE ;
    printf ( "Test 8 - Do StackWalk - Options = GSTSO_SRCLINE\n" ) ;
    Foo ( 8 ) ;
    printf ( "\n\n" ) ;

    // Test 9 - Do StackWalk - Options = GSTSO_PARAMS | GSTSO_MODULE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_PARAMS | GSTSO_MODULE ;
    printf ( "Test 9 - Do StackWalk - Options = "
             "GSTSO_PARAMS | GSTSO_MODULE\n" ) ;
    Foo ( 9 ) ;
    printf ( "\n\n" ) ;

    // Test 10 - Do StackWalk - Options = GSTSO_PARAMS | GSTSO_SYMBOL
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_PARAMS | GSTSO_SYMBOL ;
    printf ( "Test 10 - Do StackWalk - Options = "
             "GSTSO_PARAMS | GSTSO_SYMBOL\n" ) ;
    Foo ( 10 ) ;
    printf ( "\n\n" ) ;

    // Test 11 - Do StackWalk - Options = GSTSO_PARAMS | GSTSO_SRCLINE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_PARAMS | GSTSO_SRCLINE ;
    printf ( "Test 11 - Do StackWalk - Options = "
             "GSTSO_PARAMS | GSTSO_SRCLINE\n" ) ;
    Foo ( 11 ) ;
    printf ( "\n\n" ) ;

    // Test 12 - Do StackWalk - Options = GSTSO_MODULE | GSTSO_SYMBOL
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_MODULE | GSTSO_SYMBOL ;
    printf ( "Test 12 - Do StackWalk - Options = "
             "GSTSO_MODULE | GSTSO_SYMBOL\n" ) ;
    Foo ( 12 ) ;
    printf ( "\n\n" ) ;

    // Test 13 - Do StackWalk - Options = GSTSO_MODULE | GSTSO_SRCLINE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_MODULE | GSTSO_SRCLINE ;
    printf ( "Test 13 - Do StackWalk - Options = "
             "GSTSO_MODULE | GSTSO_SRCLINE\n" ) ;
    Foo ( 13 ) ;
    printf ( "\n\n" ) ;

    // Test 14 - Do StackWalk - Options = GSTSO_SYMBOL | GSTSO_SRCLINE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_SYMBOL | GSTSO_SRCLINE ;
    printf ( "Test 14 - Do StackWalk - Options = "
             "GSTSO_SYMBOL | GSTSO_SRCLINE\n" ) ;
    Foo ( 14 ) ;
    printf ( "\n\n" ) ;

    // Test 15 - Do StackWalk - Options = GSTSO_PARAMS|GSTSO_MODULE
    //                                    |GSTSO_SYMBOL|GSTSO_SRCLINE
    bGetFaultReason = FALSE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;
    printf ( "Test 15 - Do StackWalk - Options = "
             "GSTSO_PARAMS|GSTSO_MODULE|GSTSO_SYMBOL|GSTSO_SRCLINE\n" );
    Foo ( 15 ) ;
    printf ( "\n\n" ) ;

    // Test 16 - Full GetFaultReason and GetStackTraceString
    bGetFaultReason = TRUE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = FALSE ;
    dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;
    printf ( "Test 16 - Full GetFaultReason and GetStackTraceString\n");
    Foo ( 16 ) ;
    printf ( "\n\n" ) ;

    // Test 17 - Show registers only.
    bGetFaultReason = FALSE ;
    bGetStackTraceString = FALSE ;
    bShowRegs = TRUE ;
    dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;
    printf ( "Test 17 - Show registers only\n");
    Foo ( 17 ) ;
    printf ( "\n\n" ) ;

    // Test the module limiting stuff.
    // Test 18 - Add three good limit module handles.
    printf ( "Test 18 - Add three good limit module handles\n\n" ) ;
    if ( ( FALSE ==
             AddCrashHandlerLimitModule ( GetModuleHandle ( NULL ) ) )||
          ( FALSE ==
             AddCrashHandlerLimitModule(GetModuleHandle("DBGHELP.DLL")))||
          ( FALSE ==
             AddCrashHandlerLimitModule(
                                    GetModuleHandle("KERNEL32.DLL")))  )
    {
        printf ( "Test 18 - Add three good limit module "
                 "handles -> failed!\n" ) ;
        return ;
    }

    // Test 19 - Check limit module count.
    printf ( "Test 19 - Check limit module count\n\n" ) ;
    if ( 3 != GetLimitModuleCount ( ) )
    {
        printf ( "Test 19 - Check limit module count -> failed!!\n" ) ;
        return ;
    }

    // Test 20 - Try and add a bad module.
    printf ( "Test 20 - Try and add a bad module\n\n" ) ;
    if ( TRUE == AddCrashHandlerLimitModule ( NULL ) )
    {
        printf ( "Test 20 - Try and add a bad module -> failed!\n" ) ;
        return ;
    }

    // Test 21 - Get the three good modules.
    printf ( "Test 21 - Get the three good modules.\n\n" ) ;
    HMODULE ahMod[ 3 ] ;
    if ( GLMA_SUCCESS != GetLimitModulesArray ( ahMod , 3 ) )
    {
        printf ( "Test 21 - Get the three good modules -> failed!\n" ) ;
        return ;
    }


    // Test 22 - Check the same in and same out for modules.
    printf ( "Test 22 - Check the same in and same out for modules\n\n");
    if ( ( GetModuleHandle ( NULL           ) != ahMod[ 0 ] ) ||
         ( GetModuleHandle ( "DBGHELP.DLL"  ) != ahMod[ 1 ] ) ||
         ( GetModuleHandle ( "KERNEL32.DLL" ) != ahMod[ 2 ] )   )
    {
        printf ( "Test 22 - Check the same in and same out "
                 "for modules -> failed!\n" ) ;
        return ;
    }

    // Test 23 - Bad array param to GetLimitModulesArray.
    printf ( "Test 23 - Bad array param to GetLimitModulesArray\n\n" ) ;
    if ( GLMA_SUCCESS == GetLimitModulesArray ( (HMODULE*)NULL , 3 ) )
    {
        printf ( "Test 23 - Bad array param to "
                 "GetLimitModulesArray ->failed!\n" ) ;
        return ;
    }

    // Test 24 - Bad array param to GetLimitModulesArray.
    printf ( "Test 24 - Bad array param to GetLimitModulesArray.\n\n" );
    if ( GLMA_SUCCESS == GetLimitModulesArray (
                                      (HMODULE*)GetModuleHandle , 3 ) )
    {
        printf ( "Test 24 - Bad array param to "
                 "GetLimitModulesArray ->failed!\n" ) ;
        return ;
    }

    // Test 25 - Bad size param to GetLimitModulesArray.
    printf ( "Test 25 - Bad size param to GetLimitModulesArray.\n\n" ) ;
    if ( GLMA_SUCCESS == GetLimitModulesArray (
                                      (HMODULE*)GetModuleHandle , 0 ) )
    {
        printf ( "Test 25 - Bad size param to "
                 "GetLimitModulesArray ->failed!\n" ) ;
        return ;
    }

    HMODULE ahModTwo[ 1 ] ;
    // Test 26 - Too small an array.
    printf ( "Test 26 - Too small an array\n\n" ) ;
    if ( GLMA_SUCCESS == GetLimitModulesArray ( ahModTwo , 1 ) )
    {
        printf ( "Test 26 - Too small an array to "
                 "GetLimitModulesArray ->failed!\n" ) ;
        return ;
    }

    // Test 27 - Show all, but let it crash.
    bGetFaultReason = TRUE ;
    bGetStackTraceString = TRUE ;
    bShowRegs = TRUE ;
    bLetItCrash = TRUE ;
    dwOpts = GSTSO_PARAMS | GSTSO_MODULE | GSTSO_SYMBOL | GSTSO_SRCLINE;
    printf ( "Test 27 - Show all, but let it crash\n");
    Foo ( 27 ) ;
    printf ( "\n\n" ) ;

}

