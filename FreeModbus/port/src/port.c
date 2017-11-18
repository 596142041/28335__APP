#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"
//输入寄存器
USHORT usRegInputStart = REG_INPUT_START;
USHORT usRegInputBuf[REG_INPUT_NREGS] = {0x00};
//保持寄存器
USHORT usRegHoldingStart = REG_HOLDING_START;
USHORT usRegHoldingBuf[REG_HOLDING_NREGS] = {0x00};
//线圈寄存器
USHORT CoilsReg_Start = REG_Coils_START;
UCHAR CoilsReg_Buf[3] = {0x00};
//离散寄存器
USHORT Discrete_Start = REG_Discrete_START;
UCHAR DiscreteReg_Buf[4];
void __assert(const char * x1, const char * x2, int x3)
{
	(void)x3;
}
//以下代码是关于全局中断使能和关闭
void EnterCriticalSection(  )//关闭全局中断
{
	asm(" setc DBGM");
	asm(" setc INTM");
}

void ExitCriticalSection(  )//使能全局中断
{
	asm(" clrc INTM");
	asm(" clrc DBGM");
}
void INTX_DISABLE(void)
{
	asm(" setc DBGM");
 	asm(" setc INTM");
}
void  INTX_ENABLE(void)
{
	asm(" clrc INTM");
	asm(" clrc DBGM");
}
/*******************************************************************************
* @Name   : 操作输入寄存器
*
* @Brief  : 对应功能码0x04 -> eMBFuncReadInputRegister
*
* @Input  : *pucRegBuffer:数据缓冲区，响应主机用
*           usAddress:     寄存器地址
*           usNRegs:       操作寄存器个数
*
* @Output : none
*
* @Return : Modbus状态信息
*******************************************************************************/
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode eStatus = MB_ENOERR;
	int          iRegIndex = 0;
	//
	//判断地址合法性
	//
	if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
	{
		iRegIndex = (int)(usAddress - usRegInputStart);
		while (usNRegs > 0)
		{
			*pucRegBuffer++ = (UCHAR)( usRegInputBuf[iRegIndex] >> 8);  //高8位字节
			*pucRegBuffer++ = (UCHAR)( usRegInputBuf[iRegIndex] & 0xFF); //低8位字节
			iRegIndex++;
			usNRegs--;
		}
	}
	else  //错误地址
	{
		eStatus = MB_ENOREG;
	}
	
	return eStatus;
}

/******************************************************************************
* @Name   : 操作保持寄存器
*
* @Brief  : 对应功能码0x06 -> eMBFuncWriteHoldingRegister
*                    0x16 -> eMBFuncWriteMultipleHoldingRegister
*                    0x03 -> eMBFuncReadHoldingRegister
*                    0x23 -> eMBFuncReadWriteMultipleHoldingRegister
*
* @Input  : *pucRegBuffer:数据缓冲区，响应主机用
*           usAddress:     寄存器地址
*           usNRegs:       操作寄存器个数
*           eMode:         功能码
*
* @Output : none
*
* @Return : Modbus状态信息
******************************************************************************/
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode eStatus = MB_ENOERR;
	int          iRegIndex = 0;
	//
	//判断地址是否合法
	//
	if((usAddress >= REG_HOLDING_START) && ((usAddress + usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		//
		//根据功能码进行操作
		//
		switch(eMode)
		{
			case MB_REG_READ:  //读保持寄存器
					while(usNRegs > 0)
					{
						*pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] >> 8);  //高8位字节
						*pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] & 0xFF); //低8位字节
						iRegIndex++;
						usNRegs--;
					}                            
					break;
					
			case MB_REG_WRITE:  //写保持寄存器
					while(usNRegs > 0)
					{
						usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;  //高8位字节
						usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;  //低8位字节
						iRegIndex++;
						usNRegs--;
					}
					break;
		}
	}
	else  //错误地址
	{
		eStatus = MB_ENOREG;
	}
	
	return eStatus;
}

/*******************************************************************************
* @Name   : 操作线圈
*
* @Brief  : 对应功能码0x01 -> eMBFuncReadCoils
*                    0x05 ->写单个线圈 eMBFuncWriteCoil
*                    0x15 -> 写多个线圈 eMBFuncWriteMultipleCoils
*
* @Input  : *pucRegBuffer: 数据缓冲区，响应主机用
*           usAddress:     寄存器地址
*           usNRegs:       操作寄存器个数
*           eMode:         功能码
*
* @Output : none
*
* @Return : Modbus状态信息
*******************************************************************************/
eMBErrorCode eMBRegCoilsCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNCoils, eMBRegisterMode eMode )
{
 eMBErrorCode    eStatus = MB_ENOERR;
    short           iNCoils = ( short )usNCoils;
    unsigned short  usBitOffset;
    /* Check if we have registers mapped at this block. */
    if( ( usAddress >= REG_Coils_START ) &&
        ( usAddress + usNCoils <= REG_Coils_START + REG_Coils_NREGS ) )
    {
        usBitOffset = ( unsigned short )( usAddress - REG_Coils_START );
        switch ( eMode )
        {
                /* Read current values and pass to protocol stack. */
            case MB_REG_READ:
                while( iNCoils > 0 )
                {
                    *pucRegBuffer++ =
                    xMBUtilGetBits( CoilsReg_Buf,usBitOffset,( unsigned char )( iNCoils >8 ? 8 :iNCoils ) );
                    iNCoils -= 8;
                    usBitOffset += 8;
                }
                break;

                /* Update current register values. */
            case MB_REG_WRITE:
                while( iNCoils > 0 )
                {
                    xMBUtilSetBits( CoilsReg_Buf, usBitOffset,
                                    ( unsigned char )( iNCoils > 8 ? 8 : iNCoils ),
                                    *pucRegBuffer++ );
                    iNCoils -= 8;
                }
                break;
        }

    }
    else
    {
        eStatus = MB_ENOREG;
    }
    return eStatus;
}
/*******************************************************************************
* @Name   : 操作离散寄存器
*
* @Brief  : 对应功能码0x02 -> eMBFuncReadDiscreteInputs
*
* @Input  : *pucRegBuffer: 数据缓冲区，响应主机用
*           usAddress:     寄存器地址
*           usNRegs:       操作寄存器个数
*
* @Output : none
*
* @Return : Modbus状态信息
*******************************************************************************/
eMBErrorCode eMBRegDiscreteCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNDiscrete )
{
   	eMBErrorCode    eStatus = MB_ENOERR;
    short   int     iNDiscrete = ( short int)usNDiscrete;
    unsigned short int usBitOffset;
		if((usAddress >= REG_Discrete_START)&&(usAddress+usNDiscrete <= REG_Discrete_START+REG_Discrete_NREG))
		{
			  usBitOffset = ( unsigned short )( usAddress - REG_Discrete_START );
				while(iNDiscrete > 0)
				{
					*pucRegBuffer = xMBUtilGetBits( DiscreteReg_Buf, usBitOffset,( unsigned char )(iNDiscrete > 8?8:iNDiscrete));
					iNDiscrete -= 8;
					usBitOffset += 8;
					pucRegBuffer++;
				}
		}
		else
		{
			 eStatus = MB_ENOREG;
		}
    return eStatus;
}
