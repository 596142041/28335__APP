/*
 * LED.c
 *
 *  Created on: 2017��5��21��
 *      Author: admin
 */
#include "LED.h"
void LED_GPIO_Config(void)
{
	   EALLOW;
		   GpioCtrlRegs.GPBMUX1.bit.GPIO34 = 0;
		   GpioCtrlRegs.GPAMUX1.bit.GPIO0 = 0;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO58 = 0;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO59 = 0x00;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO60 = 0;
		   GpioCtrlRegs.GPBMUX2.bit.GPIO61 = 0;
		   GpioCtrlRegs.GPBDIR.bit.GPIO58 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO59 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO60 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO61 = 1;
		   GpioCtrlRegs.GPBDIR.bit.GPIO34 = 1;
		   GpioCtrlRegs.GPADIR.bit.GPIO0 = 1;
	   EDIS;
	   GpioDataRegs.GPBCLEAR.all = 0xFFFFFFFF;
	   GpioDataRegs.GPASET.bit.GPIO0 = 1;
}
void LED_Timer_Config(void)
{
	EALLOW;
		SysCtrlRegs.PCLKCR3.bit.CPUTIMER0ENCLK = 1;
	EDIS;
	InitCpuTimers();
	ConfigCpuTimer(&CpuTimer0, 150, 200000);
	CpuTimer0Regs.TCR.all = 0x4001; // Use write-only instruction to set TSS bit = 0

}
interrupt void cpu_timer0_isr(void)
{
   CpuTimer0.InterruptCount++;
   GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
   GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
   // Acknowledge this interrupt to receive more interrupts from group 1
   PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

