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
	if (xRxEnable)  //接收使能
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx5 = 1;//SCI_C_Rx
		//接收
#if USER_RS485_Port
		RS485_Pin = RX_EN;
#endif
	}
	else  //失能
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx5 = 0;//SCI_C_Rx
		//恢复发送
#if USER_RS485_Port
		RS485_Pin = TX_EN;
#endif
	}
	
	if (xTxEnable)  //发送使能
	{
		PieCtrlRegs.PIEACK.bit.ACK8   =1;
		PieCtrlRegs.PIEIER8.bit.INTx6 = 1;//SCI_C_Tx
#if USER_RS485_Port
		RS485_Pin = TX_EN;
#endif
	}
	else  //失能
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
	 * 关于串口波特率计算的问题
	 * BRR = ((SYS_CLK/4)/(BaudRate*8))-1
	 *********************************************/
	USHORT BaudRate_Reg_temp;
	(void)ucPORT;  //不修改串口号
	(void)ucDataBits;  //不修改数据位长度
	(void)eParity;  //不修改检验格式
	//计算波特率
	BaudRate_Reg_temp = (USHORT)((150000000/(4*8*ulBaudRate))-1);
	InitScicGpio();//使用SCI_C作为freemodbus的串口
	EALLOW;
		SysCtrlRegs.PCLKCR0.bit.SCICENCLK = 1;//使能时钟
	EDIS;
	ScicRegs.SCICCR.bit.ADDRIDLE_MODE = 0;//选择通信协议，选择空闲模式，此模式兼容RS232协议
	ScicRegs.SCICCR.bit.LOOPBKENA = 0;//自测试模式使能位:为1使能，为0禁用
	ScicRegs.SCICCR.bit.PARITY = 0;//奇偶校验位选择位:0为奇校验，1为偶校验
	ScicRegs.SCICCR.bit.PARITYENA = 0;//奇偶校验位使能:为1使能，为0禁用
	ScicRegs.SCICCR.bit.SCICHAR = 7;//该配置选择SCI的一个字符的长度可配置为1到8位(单位bit)
	ScicRegs.SCICCR.bit.STOPBITS = 0;//停止位的配置: 0 1位停止位  1 2位停止位
	//SCIC控制寄存器的配置
	ScicRegs.SCICTL1.bit.RXENA = 1;//SCI接收器使能位
	ScicRegs.SCICTL1.bit.RXERRINTENA = 0;//SCI接受中断错误使能
	ScicRegs.SCICTL1.bit.SLEEP = 0;//睡眠模式使能位
	ScicRegs.SCICTL1.bit.SWRESET = 0;//SCI软件复位
	ScicRegs.SCICTL1.bit.TXENA = 1;//SCI发送器使能位
	ScicRegs.SCICTL1.bit.TXWAKE = 0;//SCI发送器唤醒模式选择位，具体的不是太明白
	//-------------------------------------------------------------------
	ScicRegs.SCICTL2.bit.RXBKINTENA = 0;//接收中断使能
	ScicRegs.SCICTL2.bit.TXINTENA = 0;
	//波特率计算
	//ScicRegs.SCIHBAUD = 0x01;
	//ScicRegs.SCILBAUD = 0xE7;
	ScicRegs.SCIHBAUD = (BaudRate_Reg_temp>>8)&0xFF;
	ScicRegs.SCILBAUD = BaudRate_Reg_temp&0xFF;
	ScicRegs.SCICTL1.bit.SWRESET = 1;
	//以下代码是配置SCI的接收数据的FIFO和发送数据的FIFO
	//-------------------接收数据的FIFO配置
	ScicRegs.SCIFFRX.bit.RXFFIENA = 1;//使能接收中断
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 0;//复位接收器的FIFO
	ScicRegs.SCIFFRX.bit.RXFIFORESET = 1;
	ScicRegs.SCIFFRX.bit.RXFFIL = 0x01;//接受1个字节之后产生一个中断
	//---------------------发送数据的FIFO配置
	ScicRegs.SCIFFTX.bit.SCIRST = 0;//复位SCIC模块
	ScicRegs.SCIFFTX.bit.SCIRST = 1;//复位SCIC模块
	ScicRegs.SCIFFTX.bit.SCIFFENA = 1;//使能FIFO功能 
	ScicRegs.SCIFFTX.bit.TXFFIENA = 1;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=0;
	ScicRegs.SCIFFTX.bit.TXFIFOXRESET=1;
	ScicRegs.SCIFFTX.bit.TXFFIL = 0x01;//发送完一个字节产生中断
	return TRUE;
}
BOOL xMBPortSerialPutByte( CHAR ucByte )
{
	 //发送一个字节
	ScicRegs.SCITXBUF = ucByte;
	return TRUE;
}
BOOL xMBPortSerialGetByte( CHAR * pucByte )
{
	*pucByte= ScicRegs.SCIRXBUF.bit.RXDT;
	return TRUE;
}
/*******************************************************************************
* @Name   : SCIC中断服务函数
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
