/*******************************************************
 * file_name  : SCIA.c
 * DIR:/28335_APP/bsp/src/SCIA.c
 * Created on : 2018年1月20日-下午9:43:47
 * Author     : admin
 * Version    :
 *
 ******************************************************/
#include "SCIA.h"
void SCIA_GPIO_Init(void)
{
	   EALLOW;
		GpioCtrlRegs.GPBPUD.bit.GPIO35 = 0;    // Enable pull-up for GPIO28 (SCIRXDA)
		GpioCtrlRegs.GPBPUD.bit.GPIO36 = 0;	   // Enable pull-up for GPIO29 (SCITXDA)
		GpioCtrlRegs.GPBQSEL1.bit.GPIO36 = 3;  // Asynch input GPIO36 (SCIRXDA)
		GpioCtrlRegs.GPBMUX1.bit.GPIO35 = 1;   // Configure GPIO28 for SCIRXDA operation
		GpioCtrlRegs.GPBMUX1.bit.GPIO36 = 1;   // Configure GPIO29 for SCITXDA operation
	    EDIS;
}
/**************************************************
 * 函数名：void SCIA_Init(void);
 * 输入参数：无；
 * 返回值：无；
 * 调用：供外部函数调用；
 * 备注：该函数是对SCIA进行配置：波特率 9600；8位数据位，无奇偶校验，1位停止位
 * */
void SCIA_Init(void)
{
	SCIA_GPIO_Init(); //初始化SCIC的GPIO
	/*-----------以下代码是初始化SCI外设接口-----------------*/
	//通信控制寄存器的配置
	EALLOW;
	SysCtrlRegs.PCLKCR0.bit.SCIAENCLK = 1;
	EDIS;

	SciaRegs.SCICCR.bit.ADDRIDLE_MODE = 0; //选择通信协议，选择空闲模式，此模式兼容RS232协议
	SciaRegs.SCICCR.bit.LOOPBKENA = 0; //自测试模式使能位:为1使能，为0禁用
	SciaRegs.SCICCR.bit.PARITY = 0; //奇偶校验位选择位:0为奇校验，1为偶校验
	SciaRegs.SCICCR.bit.PARITYENA = 0; //奇偶校验位使能:为1使能，为0禁用
	SciaRegs.SCICCR.bit.SCICHAR = 7; //该配置选择SCI的一个字符的长度可配置为1到8位(单位bit)
	SciaRegs.SCICCR.bit.STOPBITS = 0; //停止位的配置: 0 1位停止位  1 2位停止位
	//SCIC控制寄存器的配置
	SciaRegs.SCICTL1.bit.RXENA = 1;	//SCI接收器使能位
	SciaRegs.SCICTL1.bit.RXERRINTENA = 0;	//SCI接受中断错误使能
	SciaRegs.SCICTL1.bit.SLEEP = 0;	//睡眠模式使能位
	SciaRegs.SCICTL1.bit.SWRESET = 0;	//SCI软件复位
	SciaRegs.SCICTL1.bit.TXENA = 1;	//SCI发送器使能位
	SciaRegs.SCICTL1.bit.TXWAKE = 0;	//SCI发送器唤醒模式选择位，具体的不是太明白
	SciaRegs.SCICTL2.bit.RXBKINTENA = 0;	//接收中断使能
	SciaRegs.SCIHBAUD = 0x01;
	SciaRegs.SCILBAUD = 0xE7;
	SciaRegs.SCICTL1.bit.SWRESET = 1;
}

void SCIA_Send_data(unsigned short int data)
{
	SciaRegs.SCITXBUF = data;
	while (SciaRegs.SCICTL2.bit.TXRDY == 0)
		;
}
void SCIA_Send_string(unsigned char *str, int leng)
{
	int n = 0;
	for (n = 0; n < leng; n++)
	{
		SCIA_Send_data(*str);
		str++;
	}
}
int fputc(int _c, register FILE *_fp)
{
	  SCIA_Send_data((unsigned char)_c);
	  return((unsigned char)_c);
}
int fputs(const char *_ptr, register FILE *_fp)
{
	unsigned int i, len;

	  len = strlen(_ptr);

	  for(i=0 ; i<len ; i++)
	  {
		  SCIA_Send_data((unsigned char)_ptr[i]);
	  }

	  return len;
}
