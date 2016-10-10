#ifndef __NX_SSP_H__
#define __NX_SSP_H__

#include "../base/nx_prototype.h"

//#include <nx_rstcon.h>
//#include <nx_clkgen.h>

#ifdef	__cplusplus
extern "C"
{
#endif

//------------------------------------------------------------------------------
/// @defgroup	SSP
//------------------------------------------------------------------------------
//@{

	/// @brief	SSP Module's Register List
	struct	NX_SSP_RegisterSet
	{
		volatile U32	SSPCR0;				///< 0x00 :
		volatile U32	SSPCR1;				///< 0x04 :
		volatile U32	SSPDR;				///< 0x08 :
		volatile U32	SSPSR;				///< 0x0C :
		volatile U32	SSPCPSR;			///< 0x10 :
		volatile U32	SSPIMSC;			///< 0x14 :
		volatile U32	SSPRIS;				///< 0x18 :
		volatile U32	SSPMIS;				///< 0x1C :
		volatile U32	SSPICR;				///< 0x20 :
		volatile U32	SSPDMACR;			///< 0x24 :
		volatile U32	_Reserved0[(0xFE0-0x28)/4]; /// < 0x28 to 0xFDC
		volatile U32	SSPPeriphID0;		///< 0xFE0 :
		volatile U32	SSPPeriphID1;		///< 0xFE4 :
		volatile U32	SSPPeriphID2;		///< 0xFE8 :
		volatile U32	SSPPeriphID3;		///< 0xFEC :
		volatile U32	SSPCellID0;			///< 0xFF0 :
		volatile U32	SSPCellID1;			///< 0xFF4 :
		volatile U32	SSPCellID2;			///< 0xFF8 :
		volatile U32	SSPCellID3;			///< 0xFFC :
	};

	// PAD�� prototype ���.
	//typedef enum{
	//
	//}

//------------------------------------------------------------------------------
/// @name	Module Interface
//@{
CBOOL	NX_SSP_Initialize				( void );
U32		NX_SSP_GetNumberOfModule		( void );
//@}

//------------------------------------------------------------------------------
///	@name	Basic Interface
//@{
U32		NX_SSP_GetPhysicalAddress		( U32 ModuleIndex );
U32		NX_SSP_GetSizeOfRegisterSet		( void );
void	NX_SSP_SetBaseAddress			( U32 ModuleIndex, void* BaseAddress );
void*	NX_SSP_GetBaseAddress			( U32 ModuleIndex );
CBOOL	NX_SSP_OpenModule				( U32 ModuleIndex );
CBOOL	NX_SSP_CloseModule				( U32 ModuleIndex );
CBOOL	NX_SSP_CheckBusy				( U32 ModuleIndex );
//@}

//------------------------------------------------------------------------------
///	@name	Interrupt Interface
//@{
// SSP�� �ܺ� ���� 1���� ����Ǿ� ������, ���������δ� 4���� ���� �ٸ� INT ������ ������ �ִ�.
// ���� �� ��⺰�� Interrupt Number�� 1��������, Interrupt Enable �� Channel�� ����Ѵ�. (��� Enable�� All ���)

// Interrupt Channel ENUM
#ifndef __DEF__NX_SSP_INTR_MASK
#define __DEF__NX_SSP_INTR_MASK
typedef enum {
	 SSPRORINTR
	,SSPRTINTR
	,SSPRXINTR
	,SSPTXINTR
} NX_SSP_INTR_MASK;
#endif

#define  NX_SSP_INTR_MAXNUMBER   4

U32		NX_SSP_GetInterruptNumber		( U32 ModuleIndex );
void	NX_SSP_SetInterruptEnable		( U32 ModuleIndex, U32 IntNum, CBOOL Enable );
CBOOL	NX_SSP_GetInterruptEnable		( U32 ModuleIndex, U32 IntNum );
CBOOL	NX_SSP_GetInterruptPending		( U32 ModuleIndex, U32 IntNum );
void	NX_SSP_ClearInterruptPending	( U32 ModuleIndex, U32 IntNum );

void	NX_SSP_SetInterruptEnableAll	( U32 ModuleIndex, CBOOL Enable );
CBOOL	NX_SSP_GetInterruptEnableAll	( U32 ModuleIndex );
CBOOL	NX_SSP_GetInterruptPendingAll	( U32 ModuleIndex );
void	NX_SSP_ClearInterruptPendingAll	( U32 ModuleIndex );

U32		NX_SSP_GetInterruptPendingNumber( U32 ModuleIndex );	// -1 if None


//------------------------------------------------------------------------------
///	@name	DMA Interface
//@{
U32		NX_SSP_GetDMAIndex_Tx( U32 ModuleIndex );
U32		NX_SSP_GetDMAIndex_Rx( U32 ModuleIndex );
U32		NX_SSP_GetDMABusWidth( U32 ModuleIndex );
//@}

//------------------------------------------------------------------------------
///	@name	Clock Control Interface
//@{

// Clock Gen�� �����Ƿ� �켱�� �ּ�ó���մϴ�.
//	void			NX_SSP_SetClockPClkMode( U32 ModuleIndex, NX_PCLKMODE mode );
//	NX_PCLKMODE	NX_SSP_GetClockPClkMode( U32 ModuleIndex );
//	void			NX_SSP_SetClockSource( U32 ModuleIndex, U32 Index, U32 ClkSrc );
//	U32			NX_SSP_GetClockSource( U32 ModuleIndex, U32 Index );
//	void			NX_SSP_SetClockDivisor( U32 ModuleIndex, U32 Index, U32 Divisor );
//	U32			NX_SSP_GetClockDivisor( U32 ModuleIndex, U32 Index );
//	void			NX_SSP_SetClockDivisorEnable( U32 ModuleIndex, CBOOL Enable );
//	CBOOL			NX_SSP_GetClockDivisorEnable( U32 ModuleIndex );

// pl02212 ��
void		NX_SSP_SetClockPrescaler( U32 ModuleIndex, U32 CPSDVR, U32 SCR );
U32			NX_SSP_GetClockPrescaler( U32 ModuleIndex );
//@}

//--------------------------------------------------------------------------
/// @name Configuration Function
//--------------------------------------------------------------------------
//@{
// Transmit/ Receive �Ѵ� Enable.

typedef enum
{
	NX_SSP_MODE_MASTER	= 0,        // Master Mode
	NX_SSP_MODE_SLAVE   = 1         // Slave  Mode
} NX_SSP_MODE;

void	NX_SSP_SetDMATransferMode( U32 ModuleIndex, CBOOL bEnable );
CBOOL	NX_SSP_GetDMATransferMode( U32 ModuleIndex );

void	NX_SSP_SetDMATransmitMode( U32 ModuleIndex, CBOOL bEnable );
CBOOL	NX_SSP_GetDMATransmitMode( U32 ModuleIndex );
void	NX_SSP_SetDMAReceiveMode( U32 ModuleIndex, CBOOL bEnable );
CBOOL	NX_SSP_GetDMAReceiveMode( U32 ModuleIndex );
void	NX_SSP_SetBitWidth( U32 ModuleIndex, U32 bitWidth );
U32		NX_SSP_GetBitWidth( U32 ModuleIndex );
void	NX_SSP_SetSlaveMode( U32 ModuleIndex, NX_SSP_MODE bSlave );
NX_SSP_MODE	NX_SSP_GetSlaveMode( U32 ModuleIndex );

void	NX_SSP_SetSlaveOutputEnable( U32 ModuleIndex, CBOOL Enable );
CBOOL	NX_SSP_GetSlaveOutputEnable( U32 ModuleIndex );

void	NX_SSP_SetClockPolarityInvert( U32 ModuleIndex, CBOOL bInvert );
CBOOL	NX_SSP_GetClockPolarityInvert( U32 ModuleIndex );

typedef enum
{
	NX_SSP_FORMAT_A	= 0,	///< Format A, SPH = 0, SPO = 0
	NX_SSP_FORMAT_B	= 1,	///< Format B, SPH = 0, SPO = 1
	NX_SSP_FORMAT_C	= 2,	///< Format B, SPH = 1, SPO = 0
	NX_SSP_FORMAT_D	= 3,	///< Format B, SPH = 1, SPO = 1
}NX_SSP_FORMAT;

void			NX_SSP_SetSPIFormat( U32 ModuleIndex, NX_SSP_FORMAT format);
NX_SSP_FORMAT	NX_SSP_GetSPIFormat( U32 ModuleIndex );

void			NX_SSP_SetClockPhase( U32 ModuleIndex, CBOOL	Phase );
U32				NX_SSP_GetClockPhase( U32 ModuleIndex );


/// @brief	Communication protocol
typedef enum
{
	NX_SSP_PROTOCOL_SPI	= 0,	///< SSP (Motorola Synchronous Serial Protocol)
	NX_SSP_PROTOCOL_SSP	= 1,	///< SPI (TI Serial Peripheral Interface Protocol )
	NX_SSP_PROTOCOL_NM	= 2	///< National Microwire frame format )
} NX_SSP_PROTOCOL ;

void				NX_SSP_SetProtocol( U32 ModuleIndex, NX_SSP_PROTOCOL protocol );
NX_SSP_PROTOCOL	NX_SSP_GetProtocol( U32 ModuleIndex );


//	void	NX_SSP_SetExternalClockSource( U32 ModuleIndex, CBOOL bExt );
//	CBOOL	NX_SSP_GetExternalClockSource( U32 ModuleIndex );
//	void	NX_SSP_SetDividerCount( U32 ModuleIndex, U32 dividerCNT );
//	U32		NX_SSP_GetDividerCount( U32 ModuleIndex );
//void	NX_SSP_SetByteSwap( U32 ModuleIndex, CBOOL bSwap );
//CBOOL	NX_SSP_GetByteSwap( U32 ModuleIndex );
//@}

//--------------------------------------------------------------------------
/// @name Operation Function
//--------------------------------------------------------------------------
//@{
// pl02212 Ssp�� ResetFIFO�� ����. ������ ��.
//void	NX_SSP_ResetFIFO( U32 ModuleIndex );

// pl02212 Ssp�� Burst��带 ���� �������� �ʴµ�.(2-26) DMA�� �켱 watermark level���� FIFO��
// �� ���� ���������� Burst�� ��û�Ѵٰ� �Ѵ�. ( RX/TX FIFO�� 4�� �̻��� �����Ͱ� �����ҽ� )
//void	NX_SSP_StartRxBurst( U32 ModuleIndex );
//void	NX_SSP_SetRxBurstEnable( U32 ModuleIndex, CBOOL bEnable );
//CBOOL	NX_SSP_GetRxBurstEnable( U32 ModuleIndex );
//void	NX_SSP_SetBurstRxSize( U32 ModuleIndex, U32 size );
//U32		NX_SSP_GetBurstRxSize( U32 ModuleIndex );

void	NX_SSP_SetEnable( U32 ModuleIndex, CBOOL bEnable );
CBOOL	NX_SSP_GetEnable( U32 ModuleIndex );
void 	NX_SSP_SetLoopBackMode( U32 ModuleIndex, CBOOL bEnable );
CBOOL 	NX_SSP_GetLoopBackMode( U32 ModuleIndex );

U8		NX_SSP_GetByte(U32 ModuleIndex);
U16		NX_SSP_GetHalfWord(U32 ModuleIndex);
void	NX_SSP_PutByte(U32 ModuleIndex, U8 ByteData);
void	NX_SSP_PutHalfWord(U32 ModuleIndex, U16 HalfWordData);

// 2120���� �����Ǿ� ���� ���� �Լ�.
//void	NX_SSP_SetHiZwhenTransmit( U32 ModuleIndex , CBOOL bEnable	);

//@}


//--------------------------------------------------------------------------
/// @name FIFO State Check Function
//--------------------------------------------------------------------------
//@{
//CBOOL	NX_SSP_IsTxShiftEmpty( U32 ModuleIndex );

CBOOL	NX_SSP_IsTxFIFOEmpty(U32 ModuleIndex);
CBOOL	NX_SSP_IsTxFIFOFull(U32 ModuleIndex);
CBOOL	NX_SSP_IsRxFIFOEmpty(U32 ModuleIndex);
CBOOL	NX_SSP_IsRxFIFOFull(U32 ModuleIndex);

CBOOL	NX_SSP_IsTxRxEnd( U32 ModuleIndex ); // CheckBusy�� ��ü.










//------------------------------------------------------------------------------
///	@name	Register Backup & Store Function
void	NX_SSP_Set_SSPCR0				( U32 ModuleIndex, U32 RegValue );
U32		NX_SSP_Get_SSPCR0				( U32 ModuleIndex );
void	NX_SSP_Set_SSPCR1				( U32 ModuleIndex, U32 RegValue );
U32		NX_SSP_Get_SSPCR1				( U32 ModuleIndex );
void	NX_SSP_Set_SSPDR				( U32 ModuleIndex, U32 RegValue );
U32		NX_SSP_Get_SSPDR				( U32 ModuleIndex );

U32		NX_SSP_Get_SSPSR				( U32 ModuleIndex );
void	NX_SSP_Set_SSPCPSR				( U32 ModuleIndex, U32 RegValue );
U32		NX_SSP_Get_SSPCPSR				( U32 ModuleIndex );
void	NX_SSP_Set_SSPIMSC				( U32 ModuleIndex, U32 RegValue );
U32		NX_SSP_Get_SSPIMSC				( U32 ModuleIndex );

U32		NX_SSP_Get_SSPRIS				( U32 ModuleIndex );

U32		NX_SSP_Get_SSPMIS				( U32 ModuleIndex );
void	NX_SSP_Set_SSPICR				( U32 ModuleIndex, U32 RegValue );

void	NX_SSP_Set_SSPDMACR				( U32 ModuleIndex, U32 RegValue );
U32		NX_SSP_Get_SSPDMACR				( U32 ModuleIndex );

U32		NX_SSP_Get_SSPPeriphID0			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPPeriphID1			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPPeriphID2			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPPeriphID3			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPCellID0			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPCellID1			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPCellID2			( U32 ModuleIndex );

U32		NX_SSP_Get_SSPCellID3			( U32 ModuleIndex );
//------------------------------------------------------------------------------



//---------- CLKGEN �� ���� prototype
U32 NX_SSP_GetClockNumber (U32 ModuleIndex);

//---------- RSTCON �� ���� prototype
U32	NX_SSP_GetResetNumber (U32 ModuleIndex, U32 ChannelIndex);

enum {
	NX_SSP_PRESETn = 0,
	NX_SSP_nSSPRST = 1,
};





//@}

#ifdef	__cplusplus
}
#endif

#endif // __NX_SSP_H__



