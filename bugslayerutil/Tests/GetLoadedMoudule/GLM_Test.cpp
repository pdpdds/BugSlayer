/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include <stdio.h>
#include <windows.h>
#include "BugslayerUtil.h"

void main ( void )
{
    HMODULE ahMods[ 250 ] ;
    DWORD   dwNumMods ;
    BOOL    bRet ;

    // Get the loaded modules for this process.
    memset ( &ahMods , NULL , sizeof ( ahMods ) ) ;
    printf ( "About to call GetLoadedModules - 1\n" ) ;
    _flushall ( ) ;
    bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
                              250                     ,
                              (HMODULE*)&ahMods       ,
                              &dwNumMods               ) ;
    if ( ( FALSE == bRet ) || ( 0 == dwNumMods ) )
    {
        printf ( "Failed getting loaded modules for this process!\n" ) ;
        return ;
    }

    printf ( "Modules for this process - \n" ) ;
    _flushall ( ) ;

    char szBuff[ MAX_PATH ] ;

    for ( UINT i = 0 ; i < dwNumMods ; i++ )
    {
        GetModuleFileName ( ahMods[ i ] , szBuff , sizeof ( szBuff ) ) ;
        printf ( "   0x%08X - %s\n" , ahMods[ i ] , szBuff ) ;
    }

    // Now test passing in NULL values to get just the count.
    DWORD dwZeroTest = 0 ;
    printf ( "About to call GetLoadedModules - 2\n" ) ;
    bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
                              0                       ,
                              NULL                    ,
                              &dwZeroTest              ) ;
    if ( ( FALSE == bRet ) || ( dwZeroTest != dwNumMods ) )
    {
        printf ( "FAILED - test passing in NULL values to get "
                 "just the count.\n" ) ;
        _flushall ( ) ;
        DebugBreak ( ) ;
    }

    // Pass in an invalid process handle.
    printf ( "About to call GetLoadedModules - 3\n" ) ;
    DWORD dwBadPID = 0 ;
    bRet = GetLoadedModules ( 1 , 250 , (HMODULE*)&ahMods , &dwBadPID );

    if ( ( TRUE == bRet ) || ( 0 != dwBadPID ) )
    {
        printf ( "FAILED - Pass in an invalid process handle.\n" ) ;
        _flushall ( ) ;
        DebugBreak ( ) ;
    }

    // Check a bad parameter as the module array.
    DWORD dwBadParam = 0 ;
    printf ( "About to call GetLoadedModules - 4\n" ) ;
    bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
                               250                    ,
                               NULL                   ,
                               &dwBadParam             ) ;

    if ( ( TRUE == bRet                                ) ||
         ( ERROR_INVALID_PARAMETER != GetLastError ( ) )    )
    {
        printf("FAILED - Check a bad parameter as the module array.\n");
        _flushall ( ) ;
        DebugBreak ( ) ;
    }

    // Do an invalid size bad parameter
    dwBadParam = 0 ;
    printf ( "About to call GetLoadedModules - 5\n" ) ;
    bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
                              5000                    ,
                              (HMODULE*)&ahMods       ,
                              &dwBadParam              ) ;

    if ( ( TRUE == bRet                                ) ||
         ( ERROR_INVALID_PARAMETER != GetLastError ( ) )    )
    {
        printf ( "FAILED - Do an invalid size bad parameter\n" ) ;
        _flushall ( ) ;
        DebugBreak ( ) ;
    }

    // Pass in a bad real count value.
    dwBadParam = 0 ;
    printf ( "About to call GetLoadedModules - 6\n" ) ;
    bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
                              250                     ,
                              (HMODULE*)&ahMods       ,
                              NULL                     ) ;

    if ( ( TRUE == bRet                                ) ||
         ( ERROR_INVALID_PARAMETER != GetLastError ( ) )    )
    {
        printf ( "FAILED - Pass in a bad real count value.\n" ) ;
        _flushall ( ) ;
        DebugBreak ( ) ;
    }

    // Pass in a buffer size that is to small to hold the output.
    memset ( &ahMods , NULL , sizeof ( ahMods ) ) ;
    dwBadParam = 0 ;
    printf ( "About to call GetLoadedModules - 7\n" ) ;
    bRet = GetLoadedModules ( GetCurrentProcessId ( ) ,
                              1                       ,
                              (HMODULE*)&ahMods       ,
                              &dwBadParam              ) ;

    if ( ( TRUE == bRet                                  ) ||
         ( ERROR_INSUFFICIENT_BUFFER != GetLastError ( ) )    )
    {
        printf ( "FAILED - Pass in a buffer size that is to small "
                 "to hold the output.\n" ) ;
        _flushall ( ) ;
        DebugBreak ( ) ;
    }

    printf ( "Correct end of program\n" ) ;
}
