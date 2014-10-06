/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include <windows.h>
#include <stdio.h>
#include "BugslayerUtil.h"
#include "AssertTest.h"

int Baz ( int i )
{
    SUPERASSERT ( FALSE ) ;
    return ( i * 9 ) ;
}

int Bar ( int i )
{
    return ( Baz ( i * 5 ) ) ;
}

int Foo ( int i )
{
    return ( Bar ( i + 3 ) ) ;
}

void main ( void )
{
    int x = 0 ;

    ADDDIAGASSERTMODULE ( GetModuleHandle ( NULL ) ) ;

    HANDLE hFile = CreateFile ( "AssertText.txt"        ,
                                GENERIC_WRITE           ,
                                0                       ,
                                NULL                    ,
                                CREATE_ALWAYS           ,
                                FILE_ATTRIBUTE_NORMAL   ,
                                NULL                     ) ;

    SetDiagAssertFile ( hFile ) ;

    SetLastError ( MSG_DEOPPRESSOLIBER ) ;
    TRACE ( "Shows ODS and MB, no stack.\n" ) ;
    ASSERT ( x != 0 ) ;

    TRACE ( "Shows ODS, MB w/ stack.\n" ) ;
    SUPERASSERT ( x != 0 ) ;

    SetDiagAssertFile ( INVALID_HANDLE_VALUE ) ;

    Foo ( 2 ) ;

    SetDiagOutputFile ( hFile ) ;

    TRACE ( "Show just ODS, w/o stack.\n" ) ;
    SetDiagOutputFile ( INVALID_HANDLE_VALUE ) ;
    SETDIAGASSERTOPTIONS ( DA_SHOWODS ) ;
    ASSERT ( x != 0 ) ;

    TRACE ( "Just MB, w/o stack.\n" ) ;
    SETDIAGASSERTOPTIONS ( DA_SHOWMSGBOX ) ;
    ASSERT ( x != 0 ) ;

    TRACE ( "Show just ODS, w/ stack.\n" ) ;
    SETDIAGASSERTOPTIONS ( DA_SHOWODS | DA_SHOWSTACKTRACE ) ;
    ASSERT ( x != 0 ) ;

    TRACE ( "Just MB, w/ stack.\n" ) ;
    SETDIAGASSERTOPTIONS ( DA_SHOWMSGBOX | DA_SHOWSTACKTRACE ) ;
    ASSERT ( x != 0 ) ;

    TRACE ( "Just like SUPERASSERT.\n" ) ;
    SETDIAGASSERTOPTIONS ( DA_SHOWMSGBOX     |
                           DA_SHOWODS        |
                           DA_SHOWSTACKTRACE  ) ;
    ASSERT ( x != 0 ) ;

    TRACE ( "Normal SUPERASSERT.\n" ) ;
    int i = 4 ;
    TRACE ( "Calling with with %d (0x%08X) (%c)\n" , i , i , i ) ;
    Foo ( i ) ;

    DiagAssertW ( DA_USEDEFAULTS , L"x != 0" , __FILE__ , __LINE__ ) ;
    DiagOutputW ( L"This is wide output!\n" ) ;
}
