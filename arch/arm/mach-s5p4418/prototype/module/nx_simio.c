//	Copyright (C) 2009 Nexell Co., All Rights Reserved
//	Nexell Co. Proprietary & Confidential
//
//	NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//	AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//	BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//	FOR A PARTICULAR PURPOSE.
//
//  Module     : rtl simulator i/o
//  File       : mes_simio00.cpp
//  Description:
//  Export     : class MES_SIMIO00
//  History    :
//     2012-06-25 Gamza convert to C prototype
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//  includes
//------------------------------------------------------------------------------
#include <nx_chip.h>
#include "nx_simio.h"
#include <stdio.h>
#include <stdlib.h>

NX_CASSERT( 1==NUMBER_OF_SIMIO_MODULE );

static	struct
{
	struct NX_SIMIO_RegisterSet *pRegister;

} __g_ModuleVariables[NUMBER_OF_SIMIO_MODULE] = { {CNULL,}, };

//	enum { MAX_NUMBER_OF_PARAMETERS = 4 };
enum { MAX_NUMBER_OF_PARAMETERS = 8 };	
static U32 MES_SIMIO_ParameterStack[MAX_NUMBER_OF_PARAMETERS];

//------------------------------------------------------------------------------
// Module Interface
//------------------------------------------------------------------------------
/**
 *	@brief	Initialize of prototype enviroment & local variables.
 *	@return \b CTRUE	indicates that Initialize is successed.\n
 *			\b CFALSE	indicates that Initialize is failed.\n
 *	@see	NX_SIMIO_GetNumberOfModule
 */
CBOOL	NX_SIMIO_Initialize( void )
{
	static CBOOL bInit = CFALSE;
	U32 i;

	if( CFALSE == bInit )
	{
		for( i=0; i < NUMBER_OF_SIMIO_MODULE; i++ )
		{
			__g_ModuleVariables[i].pRegister = CNULL;
		}

		bInit = CTRUE;
	}

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get number of modules in the chip.
 *	@return		Module's number.
 *	@see		NX_SIMIO_Initialize
 */
U32		NX_SIMIO_GetNumberOfModule( void )
{
	return NUMBER_OF_SIMIO_MODULE;
}

//------------------------------------------------------------------------------
// Basic Interface
//------------------------------------------------------------------------------
/**
 *	@brief		Get module's physical address.
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@return		Module's physical address
 *	@see		NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_SetBaseAddress,		NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_OpenModule,			NX_SIMIO_CloseModule,
 *				NX_SIMIO_CheckBusy,			NX_SIMIO_CanPowerDown
 */
U32		NX_SIMIO_GetPhysicalAddress( void )
{
	const U32 PhysicalAddr[] =
	{
		PHY_BASEADDR_LIST( SIMIO )
	};
	NX_CASSERT( 1 == (sizeof(PhysicalAddr)/sizeof(PhysicalAddr[0])) );
	NX_ASSERT( PHY_BASEADDR_SIMIO_MODULE == PhysicalAddr[0] );
	return	PhysicalAddr[0];
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get a size, in byte, of register set.
 *	@return		Size of module's register set.
 *	@see		NX_SIMIO_GetPhysicalAddress,
 *				NX_SIMIO_SetBaseAddress,		NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_OpenModule,			NX_SIMIO_CloseModule,
 *				NX_SIMIO_CheckBusy,			NX_SIMIO_CanPowerDown
 */
U32		NX_SIMIO_GetSizeOfRegisterSet( void )
{
	return sizeof( struct NX_SIMIO_RegisterSet );
}

//------------------------------------------------------------------------------
/**
 *	@brief		Set a base address of register set.
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@param[in]	BaseAddress Module's base address
 *	@return		None.
 *	@see		NX_SIMIO_GetPhysicalAddress,	NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_OpenModule,			NX_SIMIO_CloseModule,
 *				NX_SIMIO_CheckBusy,			NX_SIMIO_CanPowerDown
 */
void	NX_SIMIO_SetBaseAddress( void* BaseAddress )
{
    U32 ModuleIndex = 0;
	NX_ASSERT( NUMBER_OF_SIMIO_MODULE > ModuleIndex );
	NX_ASSERT( CNULL != BaseAddress );

	__g_ModuleVariables[ModuleIndex].pRegister = (struct NX_SIMIO_RegisterSet *)BaseAddress;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Get a base address of register set
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@return		Module's base address.
 *	@see		NX_SIMIO_GetPhysicalAddress,	NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_SetBaseAddress,
 *				NX_SIMIO_OpenModule,			NX_SIMIO_CloseModule,
 *				NX_SIMIO_CheckBusy,			NX_SIMIO_CanPowerDown
 */
void*	NX_SIMIO_GetBaseAddress( void )
{
    U32 ModuleIndex = 0;
	NX_ASSERT( NUMBER_OF_SIMIO_MODULE > ModuleIndex );

	return (void*)__g_ModuleVariables[ModuleIndex].pRegister;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Initialize selected modules with default value.
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@return		\b CTRUE	indicates that Initialize is successed. \n
 *				\b CFALSE	indicates that Initialize is failed.
 *	@see		NX_SIMIO_GetPhysicalAddress,	NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_SetBaseAddress,		NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_CloseModule,
 *				NX_SIMIO_CheckBusy,			NX_SIMIO_CanPowerDown
 */
CBOOL	NX_SIMIO_OpenModule( void )
{
    U32 ModuleIndex = 0;
    int i;
	NX_ASSERT( NUMBER_OF_SIMIO_MODULE > ModuleIndex );

	for( i=0; i<MAX_NUMBER_OF_PARAMETERS-1; i++ )
	//for( int i=0; i<8-1; i++ )
	{
        __g_ModuleVariables[0].pRegister->PARAM[i] = 0;
	}
    __g_ModuleVariables[0].pRegister-> RESULT = 0;
 
	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Deinitialize selected module to the proper stage.
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@return		\b CTRUE	indicates that Deinitialize is successed. \n
 *				\b CFALSE	indicates that Deinitialize is failed.
 *	@see		NX_SIMIO_GetPhysicalAddress,	NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_SetBaseAddress,		NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_OpenModule,
 *				NX_SIMIO_CheckBusy,			NX_SIMIO_CanPowerDown
 */
CBOOL	NX_SIMIO_CloseModule( void )
{
    U32 ModuleIndex = 0;
	NX_ASSERT( NUMBER_OF_SIMIO_MODULE > ModuleIndex );

	return CTRUE;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Indicates whether the selected modules is busy or not.
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@return		\b CTRUE	indicates that Module is Busy. \n
 *				\b CFALSE	indicates that Module is NOT Busy.
 *	@see		NX_SIMIO_GetPhysicalAddress,	NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_SetBaseAddress,		NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_OpenModule,			NX_SIMIO_CloseModule,
 *				NX_SIMIO_CanPowerDown
 */
CBOOL	NX_SIMIO_CheckBusy( void )
{
    U32 ModuleIndex = 0;
	NX_ASSERT( NUMBER_OF_SIMIO_MODULE > ModuleIndex );

	return CFALSE;
}

//------------------------------------------------------------------------------
/**
 *	@brief		Indicaes whether the selected modules is ready to enter power-down stage
 *	@param[in]	ModuleIndex		An index of module ( 0 : First MLC ).
 *	@return		\b CTRUE	indicates that Ready to enter power-down stage. \n
 *				\b CFALSE	indicates that This module can't enter to power-down stage.
 *	@see		NX_SIMIO_GetPhysicalAddress,	NX_SIMIO_GetSizeOfRegisterSet,
 *				NX_SIMIO_SetBaseAddress,		NX_SIMIO_GetBaseAddress,
 *				NX_SIMIO_OpenModule,			NX_SIMIO_CloseModule,
 *				NX_SIMIO_CheckBusy
 */
CBOOL	NX_SIMIO_CanPowerDown( void )
{
    U32 ModuleIndex = 0;
	NX_ASSERT( NUMBER_OF_SIMIO_MODULE > ModuleIndex );

	return CTRUE;
}

//------------------------------------------------------------------------------
//  ������ ������
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
/// @brief      ������� ��ϵ� parameter�� ����صд�.
//------------------------------------------------------------------------------
void
NX_SIMIO_PushParameters( void )
{    
    int i;
	for( i=0; i<MAX_NUMBER_OF_PARAMETERS-1; i++ )
	//for( int i=0; i<8-1; i++ )
	{
        MES_SIMIO_ParameterStack[i] = __g_ModuleVariables[0].pRegister->PARAM[i];
	}
	MES_SIMIO_ParameterStack[MAX_NUMBER_OF_PARAMETERS-1] = __g_ModuleVariables[0].pRegister->RESULT;
	//MES_SIMIO_ParameterStack[8-1] = __g_ModuleVariables[0].pRegister->RESULT;
}

//------------------------------------------------------------------------------
/// @brief      ����ص� parameter�� �޸𸮿� �����Ѵ�.
//------------------------------------------------------------------------------
void
NX_SIMIO_PopParameters( void )
{
    int i;
	for( i=0; i<MAX_NUMBER_OF_PARAMETERS-1; i++ )
	//for( int i=0; i<8-1; i++ )
	{
        __g_ModuleVariables[0].pRegister->PARAM[i] = MES_SIMIO_ParameterStack[i];
	}
	__g_ModuleVariables[0].pRegister->RESULT = MES_SIMIO_ParameterStack[MAX_NUMBER_OF_PARAMETERS-1];
	//__g_ModuleVariables[0].pRegister->RESULT = MES_SIMIO_ParameterStack[8-1];
}

//------------------------------------------------------------------------------
/// @brief  ����� �޼����� ����Ѵ�.
//------------------------------------------------------------------------------
void
NX_SIMIO_PutString
(
    const char* const pString   ///< [in] ����ϰ��� �ϴ� ���ڿ�
)
{
	U32 TestCom;
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)pString;
    __g_ModuleVariables[0].pRegister->COMMAND  = (U32)COMMAND_DEBUGPRINT;
    
    TestCom = __g_ModuleVariables[0].pRegister->RESULT;
#else
    puts( pString );
#endif
}

//------------------------------------------------------------------------------
/// @brief      �ִ� �ùķ��̼� �ð��� 1024 clock������ �����Ѵ�
/// @remarks    �ùķ��̼� ��尡 �ƴҰ�� �ƹ��� ���۵� ���� �ʴ´�.
//------------------------------------------------------------------------------
void
NX_SIMIO_SetTimeOutValue
(
    U32 KClockValue ///< [in] �ִ� �ùķ��̼� �ð� ( unit : 1024 clock )
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->TIMEOUTVALUE = KClockValue;
#else
    KClockValue = KClockValue;
#endif

}

//------------------------------------------------------------------------------
/// @brief      ���ø����̼��� ��� ���߰� ����� ���� �����Ѵ�.
/// @remarks    �ùķ��̼� ����� ��� �ùķ��̼��� ��� �ߴ�($stop)�ȴ�.
//------------------------------------------------------------------------------
void
NX_SIMIO_DebugBreak( void )
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_DEBUGBREAK;
#else
    MES_DEBUG_BREAK();
#endif
}

//------------------------------------------------------------------------------
/// @brief      ���ø����̼��� �ߴ��Ѵ�.
/// @remarks    �ùķ��̼� ����� ��� �ùķ��̼��� ����($finish)�ȴ�.
//------------------------------------------------------------------------------
void
NX_SIMIO_Exit( void )
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_EXIT;
    //while( CTRUE ){}
    //@modified choiyk 2012-08-29 ���� 5:08:16 exit()�� �Լ��� �Ȱ��� ���ѷ����Դϴ�..
    exit(0);
#else
    /// @todo Gamza ��� �����ؾ� �ұ�....??
    // while( CTRUE ){}
    exit(0);
#endif
}

//------------------------------------------------------------------------------
/// @brief      OVL_signal�� 1�� �� ������ ��ٸ���.
/// @remarks    ���� ���θ� ��ȯ�Ѵ�.
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_EOS( void )
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_EOS;
    while( CTRUE ){
    	/*
    		while�� ������ ���� t.EOS_signal = 1�� �Ǹ� error�� return�Ѵ�.
    		OVL�κ��� error�� �˾Ƴ��� �� ����� return
    	*/
    	//if( __g_ModuleVariables[0].pRegister->EOS )	{
    		return CFALSE;	// ��������δ� �������θ� return�� �������� ������ ��������.
    	//}
    }
#else
#endif
}

CBOOL
NX_SIMIO_QueryPerformanceFrequency( U32* piFrequency )
{
#ifdef ENV_SIMULATION
	NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );

    __g_ModuleVariables[0].pRegister->PARAM[0] = 0xcccccccc;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_QUERYPERFORMANCEFREQUENCY;
    
	// no wait ...^-^v
    
	*piFrequency  = __g_ModuleVariables[0].pRegister->PARAM[0];
    
    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
    NX_ASSERT( !"NX_SIMIO_QueryPerformanceFrequency is not implemented" );
    return CFALSE;
#endif
}
	
CBOOL
NX_SIMIO_QueryPerformanceCounter( U32* piCounter )
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = 0xcccccccc;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_QUERYPERFORMANCECOUNTER;
    
    // no wait ...^-^v
    
	*piCounter  = __g_ModuleVariables[0].pRegister->PARAM[0];

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
    NX_ASSERT( !"NX_SIMIO_QueryPerformanceFrequency is not implemented" );
    return CFALSE;
#endif
}

CBOOL 
NX_SIMIO_FillMemory
( 
	void* const pMemory, 
	U8 Data,
	U32 ByteSize 
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Data;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize ;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_FILLMEMORY;

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);;
#else
	memset( pMemory, Data, ByteSize );
	return CTRUE;
#endif	
}

//------------------------------------------------------------------------------
/// @brief  Hexa������ ������ �޸𸮷� �о���δ�.
/// @return �о���� ����Ʈ��
//------------------------------------------------------------------------------
U32
NX_SIMIO_LoadHexFile
(
    const char* const FileName, ///< [in]  �о���� hex file name
    void* const pMemory         ///< [out] memory area
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADHEXFILE;

    return __g_ModuleVariables[0].pRegister->RESULT;
#else
    unsigned long datacount = 0;
    unsigned long data;

    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rt" );
    if( NULL == fp ) // if( ! fp )
    {
        // file open failed !!!
        return CFALSE;
    }

    while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        ((unsigned char*)pMemory)[datacount] = (unsigned char)data;
        datacount++;
    }

    fclose( fp );

    return datacount;
#endif
}

//------------------------------------------------------------------------------
/// @brief  save memory data to 8 bit hex file
/// @return writed byte count
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_SaveHexFile
(
    const char* const FileName, ///< [in] hex file name
    void* const pMemory     ,   ///< [in] source memory area
    U32 ByteSize                ///< [in] writing size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVEHEXFILE;

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
//# error "SaveHexFile is not descripted in runtime mode"
    // @modified matin 05/04/25
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "wt" );
    if( NULL == fp ) // if( ! fp )
    {
        return CFALSE;   // file open failed !!!
    }

    U32 i;
    for( i=0; i<ByteSize; i++ )
    {
        fprintf(fp, "%02x\n", ((unsigned char*)pMemory)[i]);
    }

    fclose( fp );

    return CTRUE;
//  return 0;
#endif
}

//------------------------------------------------------------------------------
/// @brief  �ΰ��� �޸� ������ ���Ѵ�.
/// @return �� �޸��� ������ ���ٸ� CTRUE�� ��ȯ.
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_CompareMemory
(
    const void* const pMemory0, ///< [in] source data pointer
    const void* const pMemory1, ///< [in] source data pointer
    U32 ByteSize                ///< [in] data size in bytes
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)pMemory0;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory1;
    __g_ModuleVariables[0].pRegister->PARAM[2] = ByteSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_COMPAREMEMORY;
    return ( 0 != __g_ModuleVariables[0].pRegister->RESULT ) ? CTRUE : CFALSE;
#else
    U32 i;
    for( i=0; i<ByteSize; i++ )
    {
        if( (((U8*)pMemory0)[i]) != (((U8*)pMemory1)[i]) )
        { return CFALSE; }
    }
    return CTRUE;
#endif
}


//------------------------------------------------------------------------------
/// @brief  load Hexa-image file to memory
/// @return if success, return CTRUE
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_LoadImgFile
(
    const char* const FileName, ///< [in]  hex image file name
    U32 Stride,                 ///< [in]  stride
    U32* pWidth,                ///< [out] image width
    U32* pHeight,               ///< [out] image height
    U32* pBPP,                  ///< [out] image bits/pixel
    void* const pMemory         ///< [out] memory pointer
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADIMGFILE;

    // no wait ...^-^v

    *pWidth  = __g_ModuleVariables[0].pRegister->PARAM[0];
    *pHeight = __g_ModuleVariables[0].pRegister->PARAM[1];
    *pBPP    = __g_ModuleVariables[0].pRegister->PARAM[2];
    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
//# error "LoadHexFile is not descripted in runtime mode"
    // @modified matin 05/04/25
//  unsigned long datacount = 0;
    unsigned long data;
    unsigned long pWidth0, pWidth1, pWidth2, pWidth3;
    unsigned long pHeight0, pHeight1, pHeight2, pHeight3;
    unsigned long pBPP0, pBPP1, pBPP2, pBPP3;
    int     i, x, y;
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rt" );
    if( NULL == fp ) // if( ! fp )
    {
        // @modified 050503 Gamza
        return CFALSE;
    }

    for(i=0; i<12; i++)
    {
        fscanf( fp, "%x\n", &data );
        if ( i==0  ) pWidth0  = (data & 0xff);
        if ( i==1  ) pWidth1  = (data & 0xff);
        if ( i==2  ) pWidth2  = (data & 0xff);
        if ( i==3  ) pWidth3  = (data & 0xff);
        if ( i==4  ) pHeight0 = (data & 0xff);
        if ( i==5  ) pHeight1 = (data & 0xff);
        if ( i==6  ) pHeight2 = (data & 0xff);
        if ( i==7  ) pHeight3 = (data & 0xff);
        if ( i==8  ) pBPP0    = (data & 0xff);
        if ( i==9  ) pBPP1    = (data & 0xff);
        if ( i==10 ) pBPP2    = (data & 0xff);
        if ( i==11 ) pBPP3    = (data & 0xff);
    }

    *pWidth  = (pWidth3<<24) | (pWidth2<<16) | (pWidth1<<8) | (pWidth0<<0);
    *pHeight = (pHeight3<<24) | (pHeight2<<16) | (pHeight1<<8) | (pHeight0<<0);
    *pBPP = (pBPP3<<24) | (pBPP2<<16) | (pBPP1<<8) | (pBPP0<<0);

    // @modified 050503 Gamza
    //while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        for(y=0; y<*pHeight; y++)
        {
            for(x=0; x<((*pWidth)*(*pBPP)/8); x++)
            {
                // @modified 050503 Gamza
                if( EOF == fscanf( fp, "%x\n", &data ) ){ fclose( fp ); return CFALSE; }
                ((unsigned char*)pMemory)[y*Stride+x] = (unsigned char)data;
            }
        }
    }

    fclose( fp );
    // @modified 050503 Gamza
    return CTRUE;
#endif
}

//------------------------------------------------------------------------------
/// @brief  save memory to Hexa-image file
/// @return if success, return CTRUE
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_SaveImgFile
(
    const char* const FileName, ///< [in]  hex image file name
    U32 Stride,                 ///< [in]  stride
    U32 Width,                  ///< [in]  image width
    U32 Height,                 ///< [in]  image height
    U32 BPP,                    ///< [in]  image bits/pixel
    void* const pMemory         ///< [in]  memory pointer
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)Width;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Height;
    __g_ModuleVariables[0].pRegister->PARAM[4] = (U32)BPP;
    __g_ModuleVariables[0].pRegister->PARAM[5] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVEIMGFILE;

    // no wait ...^-^v

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
//# error "LoadHexFile is not descripted in runtime mode"
    // @modified matin 05/04/25
    FILE *fp;
    int x, y;

    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    fp = fopen( pathname, "w" );

    fprintf(fp, "%02x\n", (U8)(Width >>0  & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Width >>8  & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Width >>16 & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Width >>24 & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Height>>0  & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Height>>8  & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Height>>16 & 0xff) );
    fprintf(fp, "%02x\n", (U8)(Height>>24 & 0xff) );
    fprintf(fp, "%02x\n", (U8)(BPP   >>0  & 0xff) );
    fprintf(fp, "%02x\n", (U8)(BPP   >>8  & 0xff) );
    fprintf(fp, "%02x\n", (U8)(BPP   >>16 & 0xff) );
    fprintf(fp, "%02x\n", (U8)(BPP   >>24 & 0xff) );

    for(y=0; y<Height; y++)
    {
        for(x=0; x<(Width*BPP/8); x++)
        {
            //fprintf(fp, "%02x\n", ((const U8*)pMemory)[datacount]);
            fprintf(fp, "%02x\n", ((const U8*)pMemory)[y*Stride+x]);
       }
    }

    return CTRUE;
//  return CFALSE;
#endif
}

//------------------------------------------------------------------------------
/// @brief  compare image data
/// @return if source0 equal to source1, return CTRUE
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_CompareImage
(
    const void* Source0,    ///< [in]   color data pointer of image0
    U32 Source0Stride,      ///< [in]   bytes per line of image0
    const void* Source1,    ///< [in]   color data pointer of image1
    U32 Source1Stride,      ///< [in]   bytes per line of image1
    U32 Width,              ///< [in]   width of compare region
    U32 Height,             ///< [in]   height of compare region
    U32 BPP                 ///< [in]   bits per pixel of image0/1
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)Source0;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Source0Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)Source1;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Source1Stride;
    __g_ModuleVariables[0].pRegister->PARAM[4] = (U32)Width;
    __g_ModuleVariables[0].pRegister->PARAM[5] = (U32)Height;
    __g_ModuleVariables[0].pRegister->PARAM[6] = (U32)BPP;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_COMPAREIMAGE;

    // no wait ...^-^v

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
    U32 ByteWidth = (Width * BPP) / 8;
    U32 x,y;
    for(  y=0; y<Height; y++ )
    {
        for(  x=0; x<ByteWidth; x++ )
        {
            U8 color[2];
            color[0] = ((U8*)Source0)[x+y*Source0Stride];
            color[1] = ((U8*)Source1)[x+y*Source1Stride];
            if( color[0] != color[1] )
            {
                return CFALSE;
            }
        }
    }
    return CTRUE;
#endif
}



//--------------------------------------------------------------------------
//
//
//  deprecated Gamza 2012-06-27
//
//
//--------------------------------------------------------------------------

//------------------------------------------------------------------------------
/// @brief  load Hexa-image file to memory
/// @return if success, return CTRUE
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_LoadImgFileBin
(
    const char* const FileName, ///< [in]  hex image file name
    U32 Stride,                 ///< [in]  stride
    U32* pWidth,                ///< [out] image width
    U32* pHeight,               ///< [out] image height
    U32* pBPP,                  ///< [out] image bits/pixel
    void* const pMemory         ///< [out] memory pointer
)
{
#ifdef ENV_SIMULATION
    NX_DISABLE_UNUSED_VAR_WARNING(Stride );
    NX_DISABLE_UNUSED_VAR_WARNING(pWidth );
    NX_DISABLE_UNUSED_VAR_WARNING(pHeight);
    NX_DISABLE_UNUSED_VAR_WARNING(pBPP   );
    NX_DISABLE_UNUSED_VAR_WARNING(pMemory);
    NX_DISABLE_UNUSED_VAR_WARNING(FileName);
    return CFALSE;
#else
//# error "LoadHexFile is not descripted in runtime mode"
    // @modified matin 05/04/25
//  unsigned long datacount = 0;
    unsigned long data;
    unsigned long pWidth0, pWidth1, pWidth2, pWidth3;
    unsigned long pHeight0, pHeight1, pHeight2, pHeight3;
    unsigned long pBPP0, pBPP1, pBPP2, pBPP3;
    int     i, x, y;
    int     read_size;
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rb" );
    if( NULL == fp ) // if( ! fp )
    {
        // @modified 050503 Gamza
        return CFALSE;
    }
/*
    for(i=0; i<12; i++)
    {
        fscanf( fp, "%x\n", &data );
        if ( i==0  ) pWidth0  = (data & 0xff);
        if ( i==1  ) pWidth1  = (data & 0xff);
        if ( i==2  ) pWidth2  = (data & 0xff);
        if ( i==3  ) pWidth3  = (data & 0xff);
        if ( i==4  ) pHeight0 = (data & 0xff);
        if ( i==5  ) pHeight1 = (data & 0xff);
        if ( i==6  ) pHeight2 = (data & 0xff);
        if ( i==7  ) pHeight3 = (data & 0xff);
        if ( i==8  ) pBPP0    = (data & 0xff);
        if ( i==9  ) pBPP1    = (data & 0xff);
        if ( i==10 ) pBPP2    = (data & 0xff);
        if ( i==11 ) pBPP3    = (data & 0xff);
    }
*/
    for(i=0; i<12; i++)
    {
        fread(&data, 1, 1, fp);
        if ( i==0  ) pWidth0  = (data & 0xff);
        if ( i==1  ) pWidth1  = (data & 0xff);
        if ( i==2  ) pWidth2  = (data & 0xff);
        if ( i==3  ) pWidth3  = (data & 0xff);
        if ( i==4  ) pHeight0 = (data & 0xff);
        if ( i==5  ) pHeight1 = (data & 0xff);
        if ( i==6  ) pHeight2 = (data & 0xff);
        if ( i==7  ) pHeight3 = (data & 0xff);
        if ( i==8  ) pBPP0    = (data & 0xff);
        if ( i==9  ) pBPP1    = (data & 0xff);
        if ( i==10 ) pBPP2    = (data & 0xff);
        if ( i==11 ) pBPP3    = (data & 0xff);
    }

    *pWidth  = (pWidth3<<24) | (pWidth2<<16) | (pWidth1<<8) | (pWidth0<<0);
    *pHeight = (pHeight3<<24) | (pHeight2<<16) | (pHeight1<<8) | (pHeight0<<0);
    *pBPP = (pBPP3<<24) | (pBPP2<<16) | (pBPP1<<8) | (pBPP0<<0);

    // @modified 050503 Gamza
    //while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        for(y=0; y<*pHeight; y++)
        {
            /*
            for(x=0; x<((*pWidth)*(*pBPP)/8); x++)
            {
                // @modified 050503 Gamza
                if( EOF == fread(&data, 1, 1, fp) ){ fclose( fp ); return CFALSE; }
                //if( EOF == fscanf( fp, "%x\n", &data ) ){ fclose( fp ); return CFALSE; }
                ((unsigned char*)pMemory)[y*Stride+x] = (unsigned char)data;
            }
            */
            x = 0;
            U8 *pData = (U8*)&((unsigned char*)pMemory)[y*Stride+x];
            U32 LineByteSize = ((*pWidth)*(*pBPP)/8);
            if( EOF == fread(pData, (unsigned int)LineByteSize, 1, fp) ){ fclose( fp ); return CFALSE; }

        }
    }

    fclose( fp );


    // @modified 050503 Gamza
    return CTRUE;
#endif
}

	// @modified matin 2008/01/28 text array function add
U32   
NX_SIMIO_LoadTextHexFile
( 
	const char* const FileName, 
	void* const pMemory 
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADTEXTHEXFILE;

    return __g_ModuleVariables[0].pRegister->RESULT;
#else
#endif
}
CBOOL 
NX_SIMIO_SaveTextHexFile
( 
	const char* const FileName, 
	void* const pMemory,
	U32 ByteSize
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVETEXTHEXFILE;

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}
CBOOL 
NX_SIMIO_CompareTextMemory
( 
	const void* const pMemory0,
	const void* const pMemory1,
	U32 ByteSize
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)pMemory0;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory1;
    __g_ModuleVariables[0].pRegister->PARAM[2] = ByteSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_COMPARETEXTMEMORY;
    return ( 0 != __g_ModuleVariables[0].pRegister->RESULT ) ? CTRUE : CFALSE;
#else
	return CTRUE;
#endif
}
CBOOL 
NX_SIMIO_LoadTextImgFile
( 
	const char* const FileName, 
	U32 Stride,
	U32* pWidth, U32* pHeight, U32* pBPP,
	void* const pMemory 
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADTEXTIMGFILE;

    // no wait ...^-^v

    *pWidth  = __g_ModuleVariables[0].pRegister->PARAM[0];
    *pHeight = __g_ModuleVariables[0].pRegister->PARAM[1];
    *pBPP    = __g_ModuleVariables[0].pRegister->PARAM[2];
    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}
CBOOL 
NX_SIMIO_SaveTextImgFile
( 
	const char* const FileName, U32 Stride,
	U32  Width, U32  Height, U32 BPP,
	void* const pMemory
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)Width;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Height;
    __g_ModuleVariables[0].pRegister->PARAM[4] = (U32)BPP;
    __g_ModuleVariables[0].pRegister->PARAM[5] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVETEXTIMGFILE;

    // no wait ...^-^v

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}

CBOOL 
NX_SIMIO_CompareTextImage
( 
	const void* Source0, U32 Source0Stride,
	const void* Source1, U32 Source1Stride,
	U32 Width, U32 Height, U32 BPP 
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)Source0;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Source0Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)Source1;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Source1Stride;
    __g_ModuleVariables[0].pRegister->PARAM[4] = (U32)Width;
    __g_ModuleVariables[0].pRegister->PARAM[5] = (U32)Height;
    __g_ModuleVariables[0].pRegister->PARAM[6] = (U32)BPP;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_COMPARETEXTIMAGE;

    // no wait ...^-^v

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}
CBOOL 
NX_SIMIO_SaveTextImgFileEx
( 
	const char* const FileName, U32 Stride,
	U32  Width, U32  Height, U32 BPP,
	void* const pMemory,
	U32 Log2BlockSize // Log2BlockSize=5 ==> ImageSize=2^5(32)
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)Width;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Height;
    __g_ModuleVariables[0].pRegister->PARAM[4] = (U32)BPP;
    __g_ModuleVariables[0].pRegister->PARAM[5] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[6] = (U32)Log2BlockSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVETEXTIMGFILE_EX;

    // no wait ...^-^v

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}
CBOOL 
NX_SIMIO_CompareTextImageEx
( 
	const void* Source0, U32 Source0Stride, U32 Log2BlockSize0,
	const void* Source1, U32 Source1Stride, U32 Log2BlockSize1,
	U32 Width, U32 Height, U32 BPP
	// Log2BlockSize=5 ==> ImageSize=2^5(32)
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)Source0;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Source0Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)Source1;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Source1Stride;
    __g_ModuleVariables[0].pRegister->PARAM[4] = (U32)Width;
    __g_ModuleVariables[0].pRegister->PARAM[5] = (U32)Height;
    __g_ModuleVariables[0].pRegister->PARAM[6] = (U32)BPP;
    __g_ModuleVariables[0].pRegister->PARAM[7] = (U32)Log2BlockSize0;
    __g_ModuleVariables[0].pRegister->PARAM[8] = (U32)Log2BlockSize1;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_COMPARETEXTIMAGE_EX;

    // no wait ...^-^v
    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}

/*
CBOOL 
NX_SIMIO_LoadTextImgFileEx
( 
	const char* const FileName, 
	U32 Stride,
	U32* pWidth, U32* pHeight, U32* pBPP,
	void* const pMemory,
	U32 Log2BlockSize // Log2BlockSize=5 ==> ImageSize=2^5(32)
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Log2BlockSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADTEXTIMGFILE_EX;

    // no wait ...^-^v

    *pWidth  = __g_ModuleVariables[0].pRegister->PARAM[0];
    *pHeight = __g_ModuleVariables[0].pRegister->PARAM[1];
    *pBPP    = __g_ModuleVariables[0].pRegister->PARAM[2];
    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
	return CTRUE;
#endif
}
*/

//------------------------------------------------------------------------------
/// @brief  save memory data to binary file
/// @return writed byte count
//------------------------------------------------------------------------------
CBOOL
NX_SIMIO_SaveCodecStream
(
    const char* const FileName, ///< [in] hex file name
    void* const pMemory     ,   ///< [in] source memory area
    U32 ByteSize,               ///< [in] writing size in byte
    U32 PrevSize                ///< [in] writing size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)PrevSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVECODECSTREAM;

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
//# error "SaveHexFile is not descripted in runtime mode"
    // @added yoan 20120723
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "wb" );
    if( NULL == fp )
    {
        return CFALSE;   // file open failed !!!
    }

    U32 i;

    for( i=0; i<ByteSize; i+=4 )
    {
        fwrite(((unsigned int*)pMemory)[i], 4, 1, fp[PrevSize]);
    }

    fclose( fp );

    return CTRUE;

#endif
}

CBOOL
NX_SIMIO_SaveCodecYUV
(
    const char* const FileName, ///< [in] hex file name
    void* const pMemory     ,   ///< [in] source memory area
    U32 ByteSize,               ///< [in] writing size in byte
    U32 PrevSize                ///< [in] writing size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)PrevSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_SAVECODECYUV;

    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
//# error "SaveHexFile is not descripted in runtime mode"
    // @added yoan 20120723
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "wb" );
    if( NULL == fp )
    {
        return CFALSE;   // file open failed !!!
    }

    U32 i;

    for( i=0; i<ByteSize; i+=4 )
    {
        fwrite(((unsigned int*)pMemory)[i], 4, 1, fp[PrevSize]);
    }

    fclose( fp );

    return CTRUE;

#endif
}

CBOOL
NX_SIMIO_MEMCOPY
(
	const void* const pMemory0, ///< [in] source data pointer
	const void* const pMemory1, ///< [in] source data pointer
	U32 ByteSize				///< [in] data size in bytes
)
{
#ifdef ENV_SIMULATION
	NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
	__g_ModuleVariables[0].pRegister->PARAM[0] = (U32)pMemory0;
	__g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory1;
	__g_ModuleVariables[0].pRegister->PARAM[2] = ByteSize;
	__g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_MEMCOPY;
	return ( 0 != __g_ModuleVariables[0].pRegister->RESULT ) ? CTRUE : CFALSE;

#else
//# error "SaveHexFile is not descripted in runtime mode"
    // @added yoan 20120723
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "wb" );
    if( NULL == fp )
    {
        return CFALSE;   // file open failed !!!
    }

    U32 i;

    for( i=0; i<ByteSize; i+=4 )
    {
        fwrite(((unsigned int*)pMemory)[i], 4, 1, fp[PrevSize]);
    }

    fclose( fp );

    return CTRUE;

#endif
}

CBOOL
NX_SIMIO_CMPBINFILE
(
    const char* const FileName0, ///< [in]  �о���� hex file name
    const char* const FileName1, ///< [in]  �о���� hex file name
    U32 ByteSize                 ///< [in]  Skip size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName0;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)FileName1;
	__g_ModuleVariables[0].pRegister->PARAM[2] = ByteSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_CMPBINFILE;

    return __g_ModuleVariables[0].pRegister->RESULT;
#else
    unsigned long datacount = 0;
    unsigned long data;

    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rb" );
    if( NULL == fp ) // if( ! fp )
    {
        // file open failed !!!
        return CFALSE;
    }

    while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        ((unsigned char*)pMemory)[datacount] = (unsigned char)data;
        datacount++;
    }

    fclose( fp );

    return datacount;
#endif
}

//------------------------------------------------------------------------------
/// @brief  Hexa������ ������ �޸𸮷� �о���δ�.
/// @return �о���� ����Ʈ��
//------------------------------------------------------------------------------
U32
NX_SIMIO_LoadCodecYUV
(
    const char* const FileName, ///< [in]  �о���� hex file name
    void* const pMemory,        ///< [out] memory area
    U32 ByteSize,               ///< [in] writing size in byte
    U32 PrevSize                ///< [in] writing size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)PrevSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADCODECYUV;

    return __g_ModuleVariables[0].pRegister->RESULT;
#else
    unsigned long datacount = 0;
    unsigned long data;

    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rb" );
    if( NULL == fp ) // if( ! fp )
    {
        // file open failed !!!
        return CFALSE;
    }

    while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        ((unsigned char*)pMemory)[datacount] = (unsigned char)data;
        datacount++;
    }

    fclose( fp );

    return datacount;
#endif
}

U32
NX_SIMIO_LoadCodecStream
(
    const char* const FileName, ///< [in]  �о���� hex file name
    void* const pMemory,        ///< [out] memory area
    U32 ByteSize,               ///< [in] writing size in byte
    U32 PrevSize                ///< [in] writing size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)PrevSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADCODECSTREAM;

    return __g_ModuleVariables[0].pRegister->RESULT;
#else
    unsigned long datacount = 0;
    unsigned long data;

    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rb" );
    if( NULL == fp ) // if( ! fp )
    {
        // file open failed !!!
        return CFALSE;
    }

    while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        ((unsigned char*)pMemory)[datacount] = (unsigned char)data;
        datacount++;
    }

    fclose( fp );

    return datacount;
#endif
}

U32
NX_SIMIO_LoadCodecBitCode
(
    const char* const FileName, ///< [in]  �о���� hex file name
    void* const pMemory,        ///< [out] memory area
    U32 ByteSize,               ///< [in] writing size in byte
    U32 PrevSize                ///< [in] writing size in byte
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)ByteSize;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)PrevSize;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADCODECBITCODE;

    return __g_ModuleVariables[0].pRegister->RESULT;
#else
    unsigned long datacount = 0;
    unsigned long data;

    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rb" );
    if( NULL == fp ) // if( ! fp )
    {
        // file open failed !!!
        return CFALSE;
    }

    while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        ((unsigned char*)pMemory)[datacount] = (unsigned char)data;
        datacount++;
    }

    fclose( fp );

    return datacount;
#endif
}

//@choiyk
U32
NX_SIMIO_TD_READ
( 
	U32  Addr
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)Addr;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_TD_READ;
    
	while( !__g_ModuleVariables[0].pRegister->RESULT )	NULL;
    // no wait ...^-^v
    return (U32)(__g_ModuleVariables[0].pRegister->PARAM[1]);
#else
	return 0;
#endif
}

CBOOL
NX_SIMIO_TD_WRITE
( 
	U32  Addr, U32 Data
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)Addr;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Data;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_TD_WRITE;
    
	while( !__g_ModuleVariables[0].pRegister->RESULT )	NULL;
    // no wait ...^-^v
    return (U32)(CTRUE);
#else
	return 0;
#endif
}

//------------------------------------------------------------------------------
/// @brief  load Hexa-image file to memory
/// @return if success, return CTRUE
//------------------------------------------------------------------------------
CBOOL NX_SIMIO_LoadImgFile_A8 // Only A8R8G8B8
( 	
	const char* const FileName, 
	U32 Stride, 
	U32* pWidth, 
	U32* pHeight,
	U32* pBPP, 
	void* const pMemory, 
	U32 Alpha
)
{
#ifdef ENV_SIMULATION
    NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->PARAM[0] = (U32)FileName;
    __g_ModuleVariables[0].pRegister->PARAM[1] = (U32)Stride;
    __g_ModuleVariables[0].pRegister->PARAM[2] = (U32)pMemory;
    __g_ModuleVariables[0].pRegister->PARAM[3] = (U32)Alpha;
    __g_ModuleVariables[0].pRegister->COMMAND  = COMMAND_LOADIMGFILE_A8;

    // no wait ...^-^v

    *pWidth  = __g_ModuleVariables[0].pRegister->PARAM[0];
    *pHeight = __g_ModuleVariables[0].pRegister->PARAM[1];
    *pBPP    = __g_ModuleVariables[0].pRegister->PARAM[2];
    return (CBOOL)(0!=__g_ModuleVariables[0].pRegister->RESULT);
#else
//# error "LoadHexFile is not descripted in runtime mode"
    // @modified matin 05/04/25
//  unsigned long datacount = 0;
    unsigned long data;
    unsigned long pWidth0, pWidth1, pWidth2, pWidth3;
    unsigned long pHeight0, pHeight1, pHeight2, pHeight3;
    unsigned long pBPP0, pBPP1, pBPP2, pBPP3;
    int     i, x, y;
    char pathname[256];
    sprintf( pathname, "../../%s", FileName );
    FILE* fp = fopen( pathname, "rt" );
    if( NULL == fp ) // if( ! fp )
    {
        // @modified 050503 Gamza
        return CFALSE;
    }

    for(i=0; i<12; i++)
    {
        fscanf( fp, "%x\n", &data );
        if ( i==0  ) pWidth0  = (data & 0xff);
        if ( i==1  ) pWidth1  = (data & 0xff);
        if ( i==2  ) pWidth2  = (data & 0xff);
        if ( i==3  ) pWidth3  = (data & 0xff);
        if ( i==4  ) pHeight0 = (data & 0xff);
        if ( i==5  ) pHeight1 = (data & 0xff);
        if ( i==6  ) pHeight2 = (data & 0xff);
        if ( i==7  ) pHeight3 = (data & 0xff);
        if ( i==8  ) pBPP0    = (data & 0xff);
        if ( i==9  ) pBPP1    = (data & 0xff);
        if ( i==10 ) pBPP2    = (data & 0xff);
        if ( i==11 ) pBPP3    = (data & 0xff);
    }

    *pWidth  = (pWidth3<<24) | (pWidth2<<16) | (pWidth1<<8) | (pWidth0<<0);
    *pHeight = (pHeight3<<24) | (pHeight2<<16) | (pHeight1<<8) | (pHeight0<<0);
    *pBPP = (pBPP3<<24) | (pBPP2<<16) | (pBPP1<<8) | (pBPP0<<0);
	
	*pBPP = *pBPP + 8;
	
    // @modified 050503 Gamza
    //while( EOF != fscanf( fp, "%x\n", &data ) )
    {
        for(y=0; y<*pHeight; y++)
        {
            for(x=0; x<((*pWidth)*(*pBPP)/8); x++)
            {
            	if( x%4 == 3 ) // Alpha Insert
            	{
            		((unsigned char*)pMemory)[y*Stride+x] = (unsigned char) Alpha;
            		continue;
            	}
            	else // RGB Value
                {
                	// @modified 050503 Gamza
                	if( EOF == fscanf( fp, "%x\n", &data ) ){ fclose( fp ); return CFALSE; }
                	((unsigned char*)pMemory)[y*Stride+x] = (unsigned char)data;
                }
            }
        }
    }

    fclose( fp );
    // @modified 050503 Gamza
    return CTRUE;
#endif
}

void NX_SIMIO_SetWFI ( U32 WFI ) 
{
	NX_ASSERT( CNULL != __g_ModuleVariables[0].pRegister );
    __g_ModuleVariables[0].pRegister->STANDBYWFI = (U32)WFI;

}

CBOOL NX_SIMIO_GetEOS ( void )
{
	
}