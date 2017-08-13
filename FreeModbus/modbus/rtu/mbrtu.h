#ifndef _MB_RTU_H
#define _MB_RTU_H

#ifdef __cplusplus
PR_BEGIN_EXTERN_C
#endif
    eMBErrorCode eMBRTUInit( UCHAR slaveAddress, UCHAR ucPort, ULONG ulBaudRate,
                             eMBParity eParity );
void            eMBRTUStart( void );
void            eMBRTUStop( void );
eMBErrorCode    eMBRTUReceive( UCHAR * pucRcvAddress, UCHAR ** pucFrame, USHORT * pusLength );
eMBErrorCode    eMBRTUSend( UCHAR slaveAddress, const UCHAR * pucFrame, USHORT usLength );
BOOL            xMBRTUReceiveFSM( void );
BOOL            xMBRTUTransmitFSM( void );
BOOL            xMBRTUTimerT15Expired( void );
BOOL            xMBRTUTimerT35Expired( void );

#ifdef __cplusplus
PR_END_EXTERN_C
#endif
#endif
