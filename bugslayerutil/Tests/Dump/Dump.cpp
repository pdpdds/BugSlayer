/*----------------------------------------------------------------------
"Debugging Applications" (Microsoft Press)
Copyright (c) 1997-2000 John Robbins -- All rights reserved.
----------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <iostream>
#include "BugslayerUtil.h"

class TestClass
{
public:
    TestClass ( void )
    {
        strcpy ( m_szData , "TestClass constructor data!" ) ;
    }
    ~TestClass ( void )
    {
        m_szData[ 0 ] = '\0' ;
    }

    // The declaration of the memory debugging stuff for C++ classes
    DECLARE_MEMDEBUG ( TestClass ) ;

private     :
    char m_szData[ 100 ] ;

} ;

// This macro sets up the static DVINFO structure.
IMPLEMENT_MEMDEBUG ( TestClass ) ;

// The methods you must implement to dump and
// validate
#ifdef _DEBUG
void TestClass::ClassDumper ( const void * pData )
{
    TestClass * pClass = (TestClass*)pData ;
    _RPT1 ( _CRT_WARN ,
            " TestClass::ClassDumper : %s\n" ,
            pClass->m_szData ) ;
}
void TestClass::ClassValidator ( const void * pData   ,
                                 const void *          )
{
    // Validate the data here.
    TestClass * pClass = (TestClass*)pData ;
    _RPT1 ( _CRT_WARN                           ,
            " TestClass::ClassValidator : %s\n" ,
            pClass->m_szData                     ) ;
}
#endif

typedef struct tag_SimpleStruct
{
    char szName[ 256 ] ;
    char szRank[ 256 ] ;
} SimpleStruct ;

// The dumper and validator for simple string data memory
void DumperOne ( const void * pData )
{
    _RPT1 ( _CRT_WARN , " Data is : %s\n" , pData ) ;
}

void ValidatorOne ( const void * pData , const void * pContext )
{
    // Validate the string data here.
    _RPT2 ( _CRT_WARN ,
            " Validator called with : %s : 0x%08X\n" ,
            pData , pContext ) ;
}

// The dumper and validator for the structure allocations
void DumperTwo ( const void * pData )
{
    _RPT2 ( _CRT_WARN                       ,
             " Data is Name : %s\n"
             "         Rank : %s\n"         ,
             ((SimpleStruct*)pData)->szName   ,
             ((SimpleStruct*)pData)->szRank    ) ;
}

void ValidatorTwo ( const void * pData , const void * pContext )
{
    // Validate any structures here.
    _RPT2 ( _CRT_WARN                       ,
             "  Validator called with :\n"
             "    Data is Name : %s\n"
             "            Rank : %s\n"        ,
             ((SimpleStruct*)pData)->szName   ,
             ((SimpleStruct*)pData)->szRank    ) ;
}

// Unfortunately, the C functions need to drag around their own
// DVINFO structures. In the real world, you'd define these structures
// as extern references and wrap the MEMDEBUG macros with your own
// macros.
static DVINFO g_dvOne ;
static DVINFO g_dvTwo ;

void main ( void )
{
    std::cout << "At start of main\n" ;

    // The memory debugging initialization for type one.
    INITIALIZE_MEMDEBUG ( &g_dvOne , DumperOne , ValidatorOne )  ;
    // The memory debugging initialization for type two.
    INITIALIZE_MEMDEBUG ( &g_dvTwo , DumperTwo , ValidatorTwo )  ;

    // Allocate the class with the MEMDEBUG new.
    TestClass * pstClass ;
    //pstClass = MEMDEBUG_NEW TestClass ;
    pstClass = new TestClass ;

    // Allocate the two C types.
    char * p = (char*)MEMDEBUG_MALLOC ( &g_dvOne , 10 ) ;
    strcpy ( p , "VC VC" ) ;

    SimpleStruct * pSt =
            (SimpleStruct*)MEMDEBUG_MALLOC ( &g_dvTwo ,
                                             sizeof ( SimpleStruct ) ) ;

    strcpy ( pSt->szName , "Pam" ) ;
    strcpy ( pSt->szRank , "CINC" ) ;

    // Validate all the blocks in the list.
    VALIDATEALLBLOCKS ( NULL ) ;

    std::cout << "At end of main\n" ;

    // Every block will get dumped as part of the memory leak checking.

}

