#include "port.h"
#include "mb.h"
#include "mbport.h"
#include "mbutils.h"
//����Ĵ���
USHORT usRegInputStart = REG_INPUT_START;
USHORT usRegInputBuf[REG_INPUT_NREGS] = {0x00};
//���ּĴ���
USHORT usRegHoldingStart = REG_HOLDING_START;
USHORT usRegHoldingBuf[REG_HOLDING_NREGS] = {0x00};
//��Ȧ�Ĵ���
USHORT CoilsReg_Start = REG_Coils_START;
UCHAR CoilsReg_Buf[3] = {0x00};
//��ɢ�Ĵ���
USHORT Discrete_Start = REG_Discrete_START;
UCHAR DiscreteReg_Buf[4];
void __assert(const char * x1, const char * x2, int x3)
{
	(void)x3;
}
//���´����ǹ���ȫ���ж�ʹ�ܺ͹ر�
void EnterCriticalSection(  )//�ر�ȫ���ж�
{
	asm(" setc DBGM");
	asm(" setc INTM");
}

void ExitCriticalSection(  )//ʹ��ȫ���ж�
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
* @Name   : ��������Ĵ���
*
* @Brief  : ��Ӧ������0x04 -> eMBFuncReadInputRegister
*
* @Input  : *pucRegBuffer:���ݻ���������Ӧ������
*           usAddress:     �Ĵ�����ַ
*           usNRegs:       �����Ĵ�������
*
* @Output : none
*
* @Return : Modbus״̬��Ϣ
*******************************************************************************/
eMBErrorCode eMBRegInputCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs )
{
	eMBErrorCode eStatus = MB_ENOERR;
	int          iRegIndex = 0;
	//
	//�жϵ�ַ�Ϸ���
	//
	if ((usAddress >= REG_INPUT_START) && (usAddress + usNRegs <= REG_INPUT_START + REG_INPUT_NREGS))
	{
		iRegIndex = (int)(usAddress - usRegInputStart);
		while (usNRegs > 0)
		{
			*pucRegBuffer++ = (UCHAR)( usRegInputBuf[iRegIndex] >> 8);  //��8λ�ֽ�
			*pucRegBuffer++ = (UCHAR)( usRegInputBuf[iRegIndex] & 0xFF); //��8λ�ֽ�
			iRegIndex++;
			usNRegs--;
		}
	}
	else  //�����ַ
	{
		eStatus = MB_ENOREG;
	}
	
	return eStatus;
}

/******************************************************************************
* @Name   : �������ּĴ���
*
* @Brief  : ��Ӧ������0x06 -> eMBFuncWriteHoldingRegister
*                    0x16 -> eMBFuncWriteMultipleHoldingRegister
*                    0x03 -> eMBFuncReadHoldingRegister
*                    0x23 -> eMBFuncReadWriteMultipleHoldingRegister
*
* @Input  : *pucRegBuffer:���ݻ���������Ӧ������
*           usAddress:     �Ĵ�����ַ
*           usNRegs:       �����Ĵ�������
*           eMode:         ������
*
* @Output : none
*
* @Return : Modbus״̬��Ϣ
******************************************************************************/
eMBErrorCode eMBRegHoldingCB( UCHAR * pucRegBuffer, USHORT usAddress, USHORT usNRegs, eMBRegisterMode eMode )
{
	eMBErrorCode eStatus = MB_ENOERR;
	int          iRegIndex = 0;
	//
	//�жϵ�ַ�Ƿ�Ϸ�
	//
	if((usAddress >= REG_HOLDING_START) && ((usAddress + usNRegs) <= (REG_HOLDING_START + REG_HOLDING_NREGS)))
	{
		iRegIndex = (int)(usAddress - usRegHoldingStart);
		//
		//���ݹ�������в���
		//
		switch(eMode)
		{
			case MB_REG_READ:  //�����ּĴ���
					while(usNRegs > 0)
					{
						*pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] >> 8);  //��8λ�ֽ�
						*pucRegBuffer++ = (UCHAR)(usRegHoldingBuf[iRegIndex] & 0xFF); //��8λ�ֽ�
						iRegIndex++;
						usNRegs--;
					}                            
					break;
					
			case MB_REG_WRITE:  //д���ּĴ���
					while(usNRegs > 0)
					{
						usRegHoldingBuf[iRegIndex] = *pucRegBuffer++ << 8;  //��8λ�ֽ�
						usRegHoldingBuf[iRegIndex] |= *pucRegBuffer++;  //��8λ�ֽ�
						iRegIndex++;
						usNRegs--;
					}
					break;
		}
	}
	else  //�����ַ
	{
		eStatus = MB_ENOREG;
	}
	
	return eStatus;
}

/*******************************************************************************
* @Name   : ������Ȧ
*
* @Brief  : ��Ӧ������0x01 -> eMBFuncReadCoils
*                    0x05 ->д������Ȧ eMBFuncWriteCoil
*                    0x15 -> д�����Ȧ eMBFuncWriteMultipleCoils
*
* @Input  : *pucRegBuffer: ���ݻ���������Ӧ������
*           usAddress:     �Ĵ�����ַ
*           usNRegs:       �����Ĵ�������
*           eMode:         ������
*
* @Output : none
*
* @Return : Modbus״̬��Ϣ
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
* @Name   : ������ɢ�Ĵ���
*
* @Brief  : ��Ӧ������0x02 -> eMBFuncReadDiscreteInputs
*
* @Input  : *pucRegBuffer: ���ݻ���������Ӧ������
*           usAddress:     �Ĵ�����ַ
*           usNRegs:       �����Ĵ�������
*
* @Output : none
*
* @Return : Modbus״̬��Ϣ
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
