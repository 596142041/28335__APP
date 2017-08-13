#ifndef __FUN_41_H__
#define __FUN_41_H__
/* ----------------------- System includes ----------------------------------*/
#include "stdlib.h"
#include "string.h"

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbframe.h"
#include "mbproto.h"
#include "mbconfig.h"
/* ----------------------- Defines ------------------------------------------*/
#define Fun_READ_ADDR_OFF               ( MB_PDU_DATA_OFF + 0)
#define Fun_READ_REGCNT_OFF             ( MB_PDU_DATA_OFF + 2 )
#define Fun_READ_SIZE                   ( 4 )
#define Fun_READ_REGCNT_MAX             ( 0x007D )

#define Fun_WRITE_ADDR_OFF              ( MB_PDU_DATA_OFF + 0)
#define Fun_WRITE_VALUE_OFF             ( MB_PDU_DATA_OFF + 2 )
#define Fun_WRITE_SIZE                  ( 4 )

#define Fun_WRITE_MUL_ADDR_OFF          ( MB_PDU_DATA_OFF + 0 )
#define Fun_WRITE_MUL_REGCNT_OFF        ( MB_PDU_DATA_OFF + 2 )
#define Fun_WRITE_MUL_BYTECNT_OFF       ( MB_PDU_DATA_OFF + 4 )
#define Fun_WRITE_MUL_VALUES_OFF        ( MB_PDU_DATA_OFF + 5 )
#define Fun_WRITE_MUL_SIZE_MIN          ( 5 )
#define Fun_WRITE_MUL_REGCNT_MAX        ( 0x0078 )
/************************************************************** 
* 0x41 写单个保持寄存器,保存至SPI_FLASH
* 0x42 写多个寄存器保存至SPI_FLASH
* 0x43 从SPI_FLASH读取数据
****************************************************************/
/**---------------新的功能函数使能----------------------------**/
#define FUN_READ_HOLDING_ENABLED    (1)
#define FUN_WRITE_HOLDING_ENABLED   (1)
#define FUN_WRITE_MULTIPLE_HOLDING_ENABLED (1)
static eMBErrorCode  Fun_RegHolding( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode );
#if FUN_READ_HOLDING_ENABLED > 0
eMBException Fun_ReadHoldingRegister( UCHAR * pucFrame, USHORT * usLen );
#endif

#if FUN_WRITE_HOLDING_ENABLED > 0
eMBException Fun_WriteHoldingRegister( UCHAR * pucFrame, USHORT * usLen );
#endif

#if FUN_WRITE_MULTIPLE_HOLDING_ENABLED > 0
eMBException Fun_WriteMultipleHoldingRegister( UCHAR * pucFrame, USHORT * usLen );
#endif 
#endif
