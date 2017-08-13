//定时器配置部分相关函数,需要产生20KHz的时钟信号
/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mbport.h"
USHORT temp;
/* ----------------------- Start implementation -----------------------------*/
BOOL xMBPortTimersInit( USHORT usTim1Timerout50us )
{
	    //使能时钟
		temp = usTim1Timerout50us*50;
		EALLOW;
		 SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1; // CPU Timer2
		 EDIS;
	    /**--------以下代码是初始化CPU定时器0---------**/

	#if CPU_FRQ_150MHZ
	    ConfigCpuTimer(&CpuTimer0,150,temp);//1.2ms的周期
	#endif
	    CpuTimer0Regs.TCR.bit.FREE = 0x01;//仿真控制位，当仿真暂停时，定时器的工作状态
	    CpuTimer0Regs.TCR.bit.SOFT = 0x00;//FREE和SOFT位配置仿真控制
	    CpuTimer0Regs.TCR.bit.TRB  = 0x01;//定时器重新装载位,写1表示将发生预分频寄存器和周期寄存器重新装载事件
	    CpuTimer0Regs.TCR.bit.TIF  = 0x01;//清除中断标志位
	    CpuTimer0Regs.TCR.bit.TIE  = 0x01;//使能定时器中断
	    CpuTimer0Regs.TCR.bit.TSS  = 0x01;//定时器的启动和停止位，写1停止定时器，写0启动定时器
	    CpuTimer0Regs.TIM.all      = 0;   //计数器清零
	return TRUE;
}
void vMBPortTimersEnable(void)
{
   PieCtrlRegs.PIEACK.bit.ACK1 = 1;
   CpuTimer0Regs.TIM.all      = 0;   //计数器清零
   CpuTimer0Regs.TCR.bit.TIF  = 0x01;//清除中断标志位
   CpuTimer0Regs.TCR.bit.TIE  = 0x01;//使能定时器中断
   ReloadCpuTimer0();
   StartCpuTimer0();
}

void vMBPortTimersDisable(void)
{
    /* Disable any pending timers. */
	CpuTimer0Regs.TIM.all      = 0;   //计数器清零
	CpuTimer0Regs.TCR.bit.TIE  = 0;//使能定时器中断
	CpuTimer0Regs.TCR.bit.TIF  = 1;//清除中断标志位
	StopCpuTimer0();
}

/*******************************************************************************
* @Name   : CPU定时器2中断函数
*
* @Brief  : none
*
* @Input  : none
*
* @Output : none
*
* @Return : none
*********************************************************************************/
__interrupt void cpu_timer2_isr(void)
{
	pxMBPortCBTimerExpired(  );
	  CpuTimer0Regs.TCR.bit.TIF  = 0x01;//清除中断标志位
	PieCtrlRegs.PIEACK.bit.ACK1 = 1;
}
