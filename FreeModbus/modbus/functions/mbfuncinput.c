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
#define MB_PDU_FUNC_READ_ADDR_OFF           ( MB_PDU_DATA_OFF )
#define MB_PDU_FUNC_READ_REGCNT_OFF         ( MB_PDU_DATA_OFF + 2 )
#define MB_PDU_FUNC_READ_SIZE               ( 4 )
#define MB_PDU_FUNC_READ_REGCNT_MAX         ( 0x007D )
#define MB_PDU_FUNC_READ_RSP_BYTECNT_OFF    ( MB_PDU_DATA_OFF )
/* ----------------------- Static functions ---------------------------------*/
eMBException    prveMBError2Exception( eMBErrorCode eErrorCode );
/* ----------------------- Start implementation -----------------------------*/
#if MB_FUNC_READ_INPUT_ENABLED > 0
eMBException eMBFuncReadInputRegister( UCHAR * pucFrame, USHORT * usLen )
{
    USHORT          usRegAddress;
    USHORT          usRegCount;
    UCHAR          *pucFrameCur;
    eMBException    eStatus = MB_EX_NONE;
    eMBErrorCode    eRegStatus;
    if( *usLen == ( MB_PDU_FUNC_READ_SIZE + MB_PDU_SIZE_MIN ) )
    {
        usRegAddress = ( USHORT )( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF] << 8 );
        usRegAddress |= ( USHORT )( pucFrame[MB_PDU_FUNC_READ_ADDR_OFF + 1] ); 
        usRegCount = ( USHORT )( pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF] << 8 );
        usRegCount |= ( USHORT )( pucFrame[MB_PDU_FUNC_READ_REGCNT_OFF + 1] ); 
        /* Check if the number of registers to read is valid. If not return Modbus illegal data value exception. */
        if( ( usRegCount >= 1 ) && ( usRegCount < MB_PDU_FUNC_READ_REGCNT_MAX ) )
        {
            /* Set the current PDU data pointer to the beginning. */
            pucFrameCur = &pucFrame[MB_PDU_FUNC_OFF];
            *usLen = MB_PDU_FUNC_OFF; 
            /* First byte contains the function code. */
            *pucFrameCur++ = MB_FUNC_READ_INPUT_REGISTER;
            *usLen += 1; 
            /* Second byte in the response contain the number of bytes. */
            *pucFrameCur++ = ( UCHAR )( usRegCount * 2 );
            *usLen += 1; 
            eRegStatus = eMBRegInputCB( pucFrameCur, usRegAddress, usRegCount ); 
            /* If an error occured convert it into a Modbus exception. */
            if( eRegStatus != MB_ENOERR )
            {
                eStatus = prveMBError2Exception( eRegStatus );
            }
            else
            {
                *usLen += usRegCount * 2;
            }
        }
        else
        {
            eStatus = MB_EX_ILLEGAL_DATA_VALUE;
        }
    }
    else
    {
        /* Can't be a valid read input register request because the length is incorrect*/ 
        eStatus = MB_EX_ILLEGAL_DATA_VALUE;
    }
    return eStatus;
}

#endif
