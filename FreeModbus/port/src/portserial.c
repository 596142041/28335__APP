#include "port.h"
/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
unsigned char test[8];
unsigned char test_cnt = 0x00;
unsigned char tx_temp[10];
unsigned char tx_cnt = 0x00;  
/* ----------------------- Start implementation -----------------------------*/
void vMBPortSerialEnable( BOOL xRxEnable, BOOL xTxEnable )
{ 
	if (xRxEnable)  //����ʹ��
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx5 = 1;//SCI_C_Rx
		//����
#if USER_RS485_Port
		RS485_Pin = RX_EN;
#endif
	}
	else  //ʧ��
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx5 = 0;//SCI_C_Rx
		//�ָ�����
#if USER_RS485_Port
		RS485_Pin = TX_EN;
#endif
	}
	
	if (xTxEnable)  //����ʹ��
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx6 = 1;//SCI_C_Tx
#if USER_RS485_Port
		RS485_Pin = TX_EN;
#endif
	}
	else  //ʧ��
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx6 = 0;//SCI_C_Tx
#if USER_RS485_Port
		RS485_Pin = RX_EN;
#endif
	}
}

BOOL xMBPortSerialInit( UCHAR ucPORT, ULONG ulBaudRate, UCHAR ucDataBits, eMBParity eParity )
{
	//*USHORT temp;
	/*********************************************
	 * ���ڴ��ڲ����ʼ��������
	 * BRR = ((SYS_CLK/4)/(BaudRate*8))-1
	 *********************************************/
	USHORT BaudRate_Reg_temp;
	(void)ucPORT;  //���޸Ĵ��ں�
	(void)ucDataBits;  //���޸�����λ����
	(void)eParity;  //���޸ļ����ʽ
	//���㲨����
	BaudRate_Reg_temp = (USHORT)((150000000/(4*8*ulBaudRate))-1);
	InitScicGpio();//ʹ��SCI_C��Ϊfreemodbus�Ĵ���
	EALLOW;
		SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;//ʹ��ʱ��
	EDIS;
	ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;//ѡ��ͨ��Э�飬ѡ�����ģʽ����ģʽ����RS232Э��
	ScicRegs.SCICCR.bit.LOOPBKENA = 0;//�Բ���ģʽʹ��λ:Ϊ1ʹ�ܣ�Ϊ0����
	ScicRegs.SCICCR.bit.PARITY = 0;//��żУ��λѡ��λ:0Ϊ��У�飬1ΪżУ��
	ScicRegs.SCICCR.bit.PARITYENA = 0;//��żУ��λʹ��:Ϊ1ʹ�ܣ�Ϊ0����
	ScicRegs.SCICCR.bit.SCICHAR = 7;//������ѡ��SCI��һ���ַ��ĳ��ȿ�����Ϊ1��8λ(��λbit)
	ScicRegs.SCICCR.bit.STOPBITS = 0;//ֹͣλ������: 0 1λֹͣλ  1 2λֹͣλ
	//SCIC���ƼĴ���������
	ScicRegs.SCICTL1.bit.RXENA = 1;//SCI������ʹ��λ
	ScicRegs.SCICTL1.bit.RXERRINTENA = 0;//SCI�����жϴ���ʹ��
	ScicRegs.SCICTL1.bit.SLEEP = 0;//˯��ģʽʹ��λ
	ScicRegs.SCICTL1.bit.SWRESET = 0;//SCI�����λ
	ScicRegs.SCICTL1.bit.TXENA = 1;//SCI������ʹ��λ
	ScicRegs.SCICTL1.bit.TXWAKE = 0;//SCI����������ģʽѡ��λ������Ĳ���̫����
	//-------------------------------------------------------------------
	ScicRegs.SCICTL2.bit.RXBKINTENA = 0;//�����ж�ʹ��
	ScicRegs.SCICTL2.bit.TXINTENA = 0;
	//�����ʼ���
	//ScicRegs.SCIHBAUD = 0x01;
	//ScicRegs.SCILBAUD = 0xE7;
	ScicRegs.SCIHBAUD = (BaudRate_Reg_temp>>8)&0xFF;
	ScicRegs.SCILBAUD = BaudRate_Reg_temp&0xFF;
	ScicRegs.SCICTL1.bit.SWRESET = 1;
	//���´���������SCI�Ľ������ݵ�FIFO�ͷ������ݵ�FIFO
	//-------------------�������ݵ�FIFO����
	ScicRegs.SCIFFRX.bit.RXFFIENA = 1;//ʹ�ܽ����ж�
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;//��λ��������FIFO
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
	ScicRegs.SCIFFRX.bit.RXFFIL = 0x01;//����1���ֽ�֮�����һ���ж�
	//---------------------�������ݵ�FIFO����
	ScicRegs.SCIFFTX.bit.SCIRST = 0;//��λSCICģ��
	ScicRegs.SCIFFTX.bit.SCIRST = 1;//��λSCICģ��
	ScicRegs.SCIFFTX.bit.SCIFFENA = 1;//ʹ��FIFO���� 
	ScicRegs.SCIFFTX.bit.TXFFIENA = 1;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=0;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScicRegs.SCIFFTX.bit.TXFFIL = 0x01;//������һ���ֽڲ����ж�
	return TRUE;
}
BOOL xMBPortSerialPutByte( CHAR ucByte )
{
	 //����һ���ֽ�
	ScicRegs.SCITXBUF = ucByte;
	return TRUE;
}
BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
	*pucByte= ScicRegs.SCIRXBUF.bit.RXDT;
	return TRUE;
}
/*******************************************************************************
* @Name   : SCIC�жϷ�����
*
* @Brief  : none
*
* @Input  : none
*
* @Output : none
*
* @Return : none
*******************************************************************************/
__interrupt void serial_Rx_isr(void)
{
	if(ScicRegs.SCIFFRX.bit.RXFFINT == 1)
	{
		pxMBFrameCBByteReceived();
		ScicRegs.SCIFFRX.bit.RXFFINTCLR = 1;
		ScicRegs.SCIFFRX.bit.RXFFOVRCLR = 1;
	}
	PieCtrlRegs.PIEACK.bit.ACK8 =1;
}
__interrupt void serial_Tx_isr(void)
{
	if(ScicRegs.SCIFFTX.bit.TXFFINT == 1)
	{
		pxMBFrameCBTransmitterEmpty();
		ScicRegs.SCIFFTX.bit.TXFFINTCLR = 1;
	}
	PieCtrlRegs.PIEACK.bit.ACK8 = 1;
}
