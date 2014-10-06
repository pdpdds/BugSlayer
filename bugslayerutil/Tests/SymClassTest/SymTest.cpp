/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include <windows.h>
#include "imagehlp.h"
#include <stdio.h>
#include "SymbolEngine.h"

// These are just here for jollys and to see if they show up in the
// symbol dump.
int FuncADelic ( int i )
{
    return ( i++ ) ;
}

int FuncORama ( int i )
{
    return ( i-- ) ;
}

int FuncORific ( int i )
{
    return ( (i--) + 1 ) ;
}

BOOL CALLBACK EnumeratorDude ( LPSTR SymbolName      ,
                               ULONG SymbolAddress   ,
                               ULONG SymbolSize      ,
                               PVOID UserContext      )
{
    printf ( "%-30s 0x%08X %d\n" ,
             SymbolName         ,
             SymbolAddress      ,
             SymbolSize          ) ;
    return ( TRUE ) ;
}

void main ( int argc , char * argv[] )
{

    LPAPI_VERSION pAPIV = ImagehlpApiVersion ( ) ;

    printf ( "IMAGEHLP version %d.%d.%d\n\n" ,
              pAPIV->MajorVersion            ,
              pAPIV->MinorVersion            ,
              pAPIV->Revision                 ) ;

    CSymbolEngine cSym ;

    // Initialize the symbol engine.
    BOOL bRet = cSym.SymInitialize ( (HANDLE)1 , NULL , FALSE ) ;

    if ( FALSE == bRet )
    {
        printf ( "SymInitialize failed!!!\n" ) ;
        return ;
    }

    DWORD dwMS = 0 ;
    DWORD dwLS = 0 ;
    if ( FALSE == cSym.GetImageHlpVersion ( dwMS , dwLS ) )
    {
        printf ( "cSym.GetImageHlpVersion FAILED!!!\n" ) ;
        DebugBreak ( ) ;
    }
    else
    {
        printf ( "DBGHELP.DLL File Version : " ) ;
        printf ( _T ( "%d.%02d.%d.%d" ) ,
                HIWORD ( dwMS )        ,
                LOWORD ( dwMS )        ,
                HIWORD ( dwLS )        ,
                LOWORD ( dwLS )         ) ;
        printf ( "\n\n" ) ;
    }

    DWORD dwOpts = SymGetOptions ( ) ;
    SymSetOptions ( SYMOPT_LOAD_LINES        |
                    SYMOPT_OMAP_FIND_NEAREST  ) ;

    // Load this module.
    HANDLE hFile = CreateFile ( argv[ 0 ]               ,
                                GENERIC_READ            ,
                                FILE_SHARE_READ         ,
                                NULL                    ,
                                OPEN_EXISTING           ,
                                FILE_ATTRIBUTE_NORMAL   ,
                                NULL                     ) ;
    if ( INVALID_HANDLE_VALUE == hFile )
    {
        printf ( "CreateFile failed\n" ) ;
    }

    bRet = cSym.SymLoadModule (
                           hFile            ,
                           argv[ 0 ]        ,
                           NULL             ,
                           0                ,
                           0                 ) ;
    if ( FALSE == bRet )
    {
        printf ( "SymLoadModule failed\n" ) ;
        return ;
    }

    if ( FALSE == cSym.GetPDBReaderVersion ( dwMS , dwLS ) )
    {
        printf ( "cSym.GetPDBReaderVersion FAILED!!!\n" ) ;
    }
    else
    {
        printf ( "PDB Reader File Version : " ) ;
        printf ( _T ( "%d.%02d.%d.%d" ) ,
                HIWORD ( dwMS )        ,
                LOWORD ( dwMS )        ,
                HIWORD ( dwLS )        ,
                LOWORD ( dwLS )         ) ;
        printf ( "\n\n" ) ;
    }

    // Get information about the module just loaded.
    IMAGEHLP_MODULE stIM ;
    memset ( &stIM , NULL , sizeof ( IMAGEHLP_MODULE ) ) ;
    stIM.SizeOfStruct = sizeof ( IMAGEHLP_MODULE ) ;

    bRet = cSym.SymGetModuleInfo ( 0x400000     ,
                                   &stIM         ) ;

    if ( FALSE == bRet )
    {
        printf ( "SymGetModuleInfo failed\n" ) ;
        return ;
    }

    printf ( "Module info    :\n" ) ;
    printf ( "  Image base   : 0x%08X\n" , stIM.BaseOfImage ) ;
    printf ( "  Image size   : %d\n"     , stIM.ImageSize ) ;
    printf ( "  Time stamp   : 0x%08X\n" , stIM.TimeDateStamp ) ;
    printf ( "  Checksum     : 0x%08X\n" , stIM.CheckSum ) ;
    printf ( "  Symbol count : %d\n"     , stIM.NumSyms  ) ;
    printf ( "  Symbol type  : " ) ;
    switch ( stIM.SymType )
    {
        case SymNone            :
            printf ( "SymNone\n" ) ;
            break ;
        case SymCoff            :
            printf ( "SymCoff\n" ) ;
            break ;
        case SymCv              :
            printf ( "SymCv\n" ) ;
            break ;
        case SymPdb             :
            printf ( "SymPdb\n" ) ;
            break ;
        case SymExport          :
            printf ( "SymExport\n" ) ;
            break ;
        case SymDeferred        :
            printf ( "SymDeferred\n" ) ;
            break ;
        default :
            printf ( "**UNKNOWN!!!!!\n" ) ;
    }
    printf ( "  Module Name  : %s\n"     , stIM.ModuleName  ) ;
    printf ( "  Image  Name  : %s\n"     , stIM.ImageName  ) ;
    printf ( "  Loaded Name  : %s\n"     , stIM.LoadedImageName  ) ;


    bRet = cSym.SymEnumerateSymbols ( 0x400000       ,
                                      EnumeratorDude ,
                                      0               ) ;

    if ( FALSE == bRet )
    {
        printf ( "SymEnumerateSymbols failed\n" ) ;
        return ;
    }

    PIMAGEHLP_SYMBOL pSym = (PIMAGEHLP_SYMBOL)new char[ 256 ] ;
    memset ( pSym , NULL , 256 ) ;
    pSym->SizeOfStruct = sizeof ( IMAGEHLP_SYMBOL ) ;
    pSym->MaxNameLength = 200 ;

    bRet = cSym.SymGetSymFromName ( "main" , pSym ) ;
    if ( FALSE == bRet )
    {
        printf ( "GetSymFromName failed looking up main!\n" ) ;
        return ;
    }

    printf ( "main is at 0x%08X\n" , pSym->Address ) ;

    IMAGEHLP_LINE stLine ;
    memset ( &stLine , NULL , sizeof ( IMAGEHLP_LINE ) ) ;
    stLine.SizeOfStruct = sizeof ( IMAGEHLP_LINE ) ;

    DWORD dwDis ;
    bRet = cSym.SymGetLineFromAddr ( pSym->Address ,
                                     &dwDis     ,
                                     &stLine     ) ;
    if ( FALSE ==bRet )
    {
        printf ( "SymGetLineFromAddr failed\n" ) ;
        return ;
    }
    printf ( "Line information :\n" ) ;
    printf ( "  File           : %s\n" , stLine.FileName ) ;
    printf ( "  Line Number    : %d\n" , stLine.LineNumber ) ;
    printf ( "  Address        : 0x%08X\n" , stLine.Address ) ;
    printf ( "  Displacement   : %d\n" , dwDis ) ;

    // Iterate backwards from main to the beginning of the line info.
    IMAGEHLP_LINE stTempLine ;
    stTempLine = stLine ;

    while ( TRUE == cSym.SymGetLinePrev ( &stLine ) )
    {
        stTempLine = stLine ;
    }

    stLine = stTempLine ;

    // Now iterate through forwards printing out each line record.
    printf ( "Line information  :\n" ) ;
    do
    {
        printf ( "  File : %-15s Line : %d Address : 0x%08X\n" ,
                 stLine.FileName    ,
                 stLine.LineNumber  ,
                 stLine.Address      ) ;
    } while ( cSym.SymGetLineNext ( &stLine ) ) ;


    bRet = cSym.SymCleanup ( ) ;
    if ( FALSE == bRet )
    {
        printf ( "SymCleanup failed!!!\n" ) ;
        return ;
    }

}
