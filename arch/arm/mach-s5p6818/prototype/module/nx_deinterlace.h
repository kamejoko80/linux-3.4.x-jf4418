//------------------------------------------------------------------------------
//  Copyright (C) 2009 Nexell Co., All Rights Reserved
//  Nexell Co. Proprietary & Confidential
//
//  NEXELL INFORMS THAT THIS CODE AND INFORMATION IS PROVIDED "AS IS" BASE
//  AND WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING
//  BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS
//  FOR A PARTICULAR PURPOSE.
//
//  Module      : Deinterlace
//  File        : nx_deinterlace.h
//  Description :
//  Author      :
//  History     :
//      2013/12/05 Sei - 1st release
//------------------------------------------------------------------------------

#ifndef __NX_DEINTERLACE_H__
#define __NX_DEINTERLACE_H__

#include "../base/nx_prototype.h"

#ifdef  __cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup   DEINTERLACE
//------------------------------------------------------------------------------
//@{

    /// @brief  DEINTERLACE Module's Register List
    struct NX_DEINTERLACE_RegisterSet
    {
        // Control
        volatile U32 START;                         ///< 0x00 : Start Register
        volatile U32 MODE;                          ///< 0x04 : Mode Register
        volatile U32 INTENB;                        ///< 0x08 : Interrupt Enable Register
        volatile U32 INTPEND;                       ///< 0x0C : Interrupt Pending Register

        volatile U32 TSPARA;                        ///< 0x10 : AS Module Parameter Register
        volatile U32 TMPARA;                        ///< 0x14 : MDSAD Module Parameter Register
        volatile U32 TIPARA;                        ///< 0x18 : MI Module Parameter Register
        volatile U32 TPARA;                         ///< 0x1C : YS Module Parameter Register
        volatile U32 BLENDPARA;                     ///< 0x20 : BLEND Module Parameter Register
        volatile U32 RESERVED_0[(0x100-0x024)/4];   ///< 0x24 ~ 0x100 : Reserved

        // Y
        volatile U32 SRCSIZE_Y;                     ///< 0x100 : Y Field Image Size Register
        volatile U32 SRCADDRP_Y;                    ///< 0x104 : Y Previous Field Address Register
        volatile U32 SRCADDRC_Y;                    ///< 0x108 : Y Current Field Address Register
        volatile U32 SRCADDRN_Y;                    ///< 0x10C : Y Next Field Address Register
        volatile U32 SRCSTRD_Y;                     ///< 0x110 : Y Field Source Stride Register
        volatile U32 DESTADDRF_Y;                   ///< 0x114 : Deinterlaced Y Base Address
        volatile U32 DESTADDRD_Y;                   ///< 0x118 : Y Field Base Address
        volatile U32 DESTSTRD_Y;                    ///< 0x11C : Y Field Destination Stride Register
        volatile U32 RESERVED_1[(0x200-0x120)/4];   ///< 0x120 ~ 0x200 : Reserved

        // CB
        volatile U32 SRCSIZE_CB;                    ///< 0x200 : CB Field Image Size Register
        volatile U32 SRCADDRC_CB;                   ///< 0x204 : CB Current Field Address Register
        volatile U32 SRCSTRD_CB;                    ///< 0x208 : CB Field Source Stride Register
        volatile U32 DESTADDRF_CB;                  ///< 0x20C : Deinterlaced CB Base Address
        volatile U32 DESTADDRD_CB;                  ///< 0x210 : CB Field Base Address
        volatile U32 DESTSTRD_CB;                   ///< 0x214 : CB Field Destination Stride Register
        volatile U32 RESERVED_2[(0x300-0x218)/4];   ///< 0x218 ~ 0x300 : Reserved

        // CR
        volatile U32 SRCSIZE_CR;                    ///< 0x300 : CR Field Image Size Register
        volatile U32 SRCADDRC_CR;                   ///< 0x304 : CR Current Field Address Register
        volatile U32 SRCSTRD_CR;                    ///< 0x308 : CR Field Source Stride Register
        volatile U32 DESTADDRF_CR;                  ///< 0x30C : Deinterlaced CR Base Address
        volatile U32 DESTADDRD_CR;                  ///< 0x310 : CR Field Base Address
        volatile U32 DESTSTRD_CR;                   ///< 0x314 : CR Field Destination Stride Register
    };

    /// @brief Deinterlace interrupt for interrupt interface
	enum
	{
		NX_DEINTERLACE_INT_Y			= 0,		///< Y operation finish interrupt
		NX_DEINTERLACE_INT_CB			= 1,		///< CB operation finish interrupt
		NX_DEINTERLACE_INT_CR			= 2,		///< CR operation finish interrupt
		NX_DEINTERLACE_INT_TOP		    = 3			///< TOP operation finish interrupt
	};

	/// @brief	Field Information
	typedef enum
	{
		NX_DEINTERLACE_FIELD_EVEN		=	0UL,	///< Even Field
		NX_DEINTERLACE_FIELD_ODD		=	1UL		///< Odd Field

	}	NX_DEINTERLACE_FIELD;

//------------------------------------------------------------------------------
/// @name	Module Interface
//@{
CBOOL	NX_DEINTERLACE_Initialize( void );
U32		NX_DEINTERLACE_GetNumberOfModule( void );
//@}

//------------------------------------------------------------------------------
///	@name	Basic Interface
//@{
U32		NX_DEINTERLACE_GetPhysicalAddress( void );
U32		NX_DEINTERLACE_GetSizeOfRegisterSet( void );
void	NX_DEINTERLACE_SetBaseAddress( void* BaseAddress );
void*	NX_DEINTERLACE_GetBaseAddress( void );
CBOOL	NX_DEINTERLACE_OpenModule( void );
CBOOL	NX_DEINTERLACE_CloseModule( void );
CBOOL	NX_DEINTERLACE_CheckBusy( void );
CBOOL	NX_DEINTERLACE_CanPowerDown( void );
//@}

//------------------------------------------------------------------------------
///	@name	Interrupt Interface
//@{
S32		NX_DEINTERLACE_GetInterruptNumber( void );

void	NX_DEINTERLACE_SetInterruptEnable( S32 IntNum, CBOOL Enable );
CBOOL	NX_DEINTERLACE_GetInterruptEnable( S32 IntNum );
CBOOL	NX_DEINTERLACE_GetInterruptPending( S32 IntNum );
void	NX_DEINTERLACE_ClearInterruptPending( S32 IntNum );

void	NX_DEINTERLACE_SetInterruptEnableAll( CBOOL Enable );
CBOOL	NX_DEINTERLACE_GetInterruptEnableAll( void );
CBOOL	NX_DEINTERLACE_GetInterruptPendingAll( void );
void	NX_DEINTERLACE_ClearInterruptPendingAll( void );

void	NX_DEINTERLACE_SetInterruptEnable32( U32 EnableFlag );
U32		NX_DEINTERLACE_GetInterruptEnable32( void );
U32		NX_DEINTERLACE_GetInterruptPending32( void );
void	NX_DEINTERLACEX_ClearInterruptPending32( U32 PendingFlag );

S32		NX_DEINTERLACE_GetInterruptPendingNumber( void );	// -1 if None
//@}

//------------------------------------------------------------------------------
///	@name	Clock Control Interface
//@{
U32         NX_DEINTERLACE_GetClockNumber( void );
U32         NX_DEINTERLACE_GetResetNumber( void );
//@}

//--------------------------------------------------------------------------
/// @name Deinterlace Configuration Function
//--------------------------------------------------------------------------
//@{
void	NX_DEINTERLACE_SetASParameter( U32 dwTS1, U32 dwTS2 );
void	NX_DEINTERLACE_GetASParameter( U32 *pdwTS1, U32 *pdwTS2 );

void	NX_DEINTERLACE_SetMDSADParameter( U32 dwTM1, U32 dwTM2 );
void	NX_DEINTERLACE_GetMDSADParameter( U32 *pdwTM1, U32 *pdwTM2 );

void	NX_DEINTERLACE_SetMIParameter( U32 dwTI1, U32 dwTI2 );
void	NX_DEINTERLACE_GetMIParameter( U32 *pdwTI1, U32 *pdwTI2 );

void	NX_DEINTERLACE_SetYSParameter( U32 dwT1, U32 dwT2 );
void	NX_DEINTERLACE_GetYSParameter( U32 *pdwT1, U32 *pdwT2 );

void	NX_DEINTERLACE_SetBLENDParameter( U32 dwShift );
void	NX_DEINTERLACE_GetBLENDParameter( U32 *pdwShift );

void					NX_DEINTERLACE_SetCRFieldInfo( NX_DEINTERLACE_FIELD field );
NX_DEINTERLACE_FIELD	NX_DEINTERLACE_GetCRFieldInfo( void );

void					NX_DEINTERLACE_SetCBFieldInfo( NX_DEINTERLACE_FIELD field );
NX_DEINTERLACE_FIELD	NX_DEINTERLACE_GetCBFieldInfo( void );

void					NX_DEINTERLACE_SetYFieldInfo( NX_DEINTERLACE_FIELD field );
NX_DEINTERLACE_FIELD	NX_DEINTERLACE_GetYFieldInfo( void );

void	NX_DEINTERLACE_SetYCBCRField( NX_DEINTERLACE_FIELD	Yfield, NX_DEINTERLACE_FIELD CBfield, NX_DEINTERLACE_FIELD CRfield );
//@}

//--------------------------------------------------------------------------
/// @name Deinterlace Operation Function
//--------------------------------------------------------------------------
//@{
void	NX_DEINTERLACE_SetCREnable( CBOOL enable );
CBOOL	NX_DEINTERLACE_GetCREnable( void );

void	NX_DEINTERLACE_SetCBEnable( CBOOL enable );
CBOOL	NX_DEINTERLACE_GetCBEnable( void );

void	NX_DEINTERLACE_SetYEnable( CBOOL enable );
CBOOL	NX_DEINTERLACE_GetYEnable( void );

void	NX_DEINTERLACE_SetYCBCREnable( CBOOL Yenable, CBOOL CBenable, CBOOL CRenable );

CBOOL	NX_DEINTERLACE_IsCRBusy( void );
CBOOL	NX_DEINTERLACE_IsCBBusy( void );
CBOOL	NX_DEINTERLACE_IsYBusy( void );

CBOOL	NX_DEINTERLACE_IsDeinterlaceBusy( void );
void    NX_DEINTERLACE_IsDeinterlaceSwReset( void );
void	NX_DEINTERLACE_DeinterlaceStart( void );
//@}

//--------------------------------------------------------------------------
/// @name Y Field Configuration Function
//--------------------------------------------------------------------------
//@{
void	NX_DEINTERLACE_SetYSrcImageSize ( U32 YSrcHeight, U32 YSrcWidth );
void	NX_DEINTERLACE_SetYSrcAddrPrev  ( U32 YSrcAddrPrev );
void	NX_DEINTERLACE_SetYSrcAddrCurr  ( U32 YSrcAddrCurr );
void	NX_DEINTERLACE_SetYSrcAddrNext  ( U32 YSrcAddrNext );
void	NX_DEINTERLACE_SetYSrcStride	( U32 YSrcStride   );
void	NX_DEINTERLACE_SetYDestAddrDIT  ( U32 YDestAddrDIT );
void	NX_DEINTERLACE_SetYDestAddrFil  ( U32 YDestAddrFil );
void	NX_DEINTERLACE_SetYDestStride	( U32 YDestStride  );

U32		NX_DEINTERLACE_GetYSrcImageSize ( void );
U32		NX_DEINTERLACE_GetYSrcAddrPrev  ( void );
U32		NX_DEINTERLACE_GetYSrcAddrCurr  ( void );
U32		NX_DEINTERLACE_GetYSrcAddrNext  ( void );
U32		NX_DEINTERLACE_GetYSrcStride	( void );
U32		NX_DEINTERLACE_GetYDestAddrDIT  ( void );
U32		NX_DEINTERLACE_GetYDestAddrFil  ( void );
U32		NX_DEINTERLACE_GetYDestStride	( void );
//@}

//--------------------------------------------------------------------------
/// @name CB Field Configuration Function
//--------------------------------------------------------------------------
//@{
void	NX_DEINTERLACE_SetCBSrcImageSize( U32 CBSrcHeight, U32 CBSrcWidth );
void	NX_DEINTERLACE_SetCBSrcAddrCurr ( U32 CBSrcAddrCurr );
void	NX_DEINTERLACE_SetCBSrcStride	( U32 CBSrcStride   );
void	NX_DEINTERLACE_SetCBDestAddrDIT ( U32 CBDestAddrDIT );
void	NX_DEINTERLACE_SetCBDestAddrFil ( U32 CBDestAddrFil );
void	NX_DEINTERLACE_SetCBDestStride	( U32 CBDestStride	);

U32		NX_DEINTERLACE_GetCBSrcImageSize( void );
U32		NX_DEINTERLACE_GetCBSrcAddrCurr ( void );
U32		NX_DEINTERLACE_GetCBSrcStride	( void );
U32		NX_DEINTERLACE_GetCBDestAddrDIT ( void );
U32		NX_DEINTERLACE_GetCBDestAddrFil ( void );
U32		NX_DEINTERLACE_GetCBDestStride	( void );
//@}

//--------------------------------------------------------------------------
/// @name CR Field Configuration Function
//--------------------------------------------------------------------------
//@{
void	NX_DEINTERLACE_SetCRSrcImageSize( U32 CRSrcHeight, U32 CRSrcWidth );
void	NX_DEINTERLACE_SetCRSrcAddrCurr ( U32 CRSrcAddrCurr );
void	NX_DEINTERLACE_SetCRSrcStride	( U32 CRSrcStride   );
void	NX_DEINTERLACE_SetCRDestAddrDIT ( U32 CRDestAddrDIT );
void	NX_DEINTERLACE_SetCRDestAddrFil ( U32 CRDestAddrFil );
void	NX_DEINTERLACE_SetCRDestStride	( U32 CRDestStride	);

U32		NX_DEINTERLACE_GetCRSrcImageSize( void );
U32		NX_DEINTERLACE_GetCRSrcAddrCurr ( void );
U32		NX_DEINTERLACE_GetCRSrcStride	( void );
U32		NX_DEINTERLACE_GetCRDestAddrDIT ( void );
U32		NX_DEINTERLACE_GetCRDestAddrFil ( void );
U32		NX_DEINTERLACE_GetCRDestStride	( void );
//@}


//@}

#ifdef  __cplusplus
}
#endif

#endif  // __NX_DEINTERLACE_H__
