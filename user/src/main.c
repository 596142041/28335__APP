#include "CAN_APP.h"
#include "LED.h"
#include "Flash.h"
#include "timer.h"
#include "os.h"
int i = 0;
void main(void)
{
	InitSysCtrl();
	LED_GPIO_Config();
	CAN_GPIO_Config(CANA);
	taskinit();
	DINT;
	DRTM;
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
	InitFlash();
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.TINT0 = &cpu_timer0_isr;
	PieVectTable.ECAN1INTA = &Ecana_isr1;
	PieVectTable.XINT13 = & CPU_timer1_isr;
	EDIS;
	//LED_Timer_Config();
	CPU_timer1_init();
	CAN_Config(CANA);
	CAN_Rx_Config();
	CAN_Rx_IT_Concig();
	//------------------------------------
	//≈‰÷√÷–∂œ
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1; //CANAµƒ÷–∂œ1
	IER |= M_INT9;
	IER |= M_INT13;
	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM
   while(1)
   {
	   for(i = 0;i<TASKNUM;i++)
	   {
		   if(tasktimer[i].flag != 0)
		   {
			   switch (i) {
				case TASK500US:
				{
					   if(can_rx_msg.rx_update == UPDATE)
					   		{
					   			can_rx_msg.rx_update = NON_CHANGE;
					   			CAN_BOOT_ExecutiveCommand(&can_rx_msg);
					   		}
				}
					break;
				case TASK100MS:
				{
					if(tasktimer[TASK100MS].flag == 1)
					{
						GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
					    GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
					}

				}
					break;
			}
		   }
	   }

   }
}
