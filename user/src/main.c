#include "CAN_APP.h"
#include "LED.h"
#include "Flash.h"
#include "timer.h"
#include "os.h"
#include "mb.h"  //FreeModbus
#include "mbport.h"
#include "port.h"
#include "W25Q64BV.h"
#include "Fun_41.h"
int i = 0;
eMBErrorCode Code, error_code;
void Modbus_Data_Init(void);
void task_10ms(void);
void task_50ms(void);
void main(void)
{
	InitSysCtrl();
	Modbus_Data_Init();
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
	PieVectTable.ECAN1INTA = &Ecana_isr1;
	PieVectTable.XINT13    = &CPU_timer1_isr;
	PieVectTable.SCIRXINTC = &serial_Rx_isr;
	PieVectTable.SCITXINTC = &serial_Tx_isr;
	PieVectTable.TINT0     = &cpu_timer2_isr;
	EDIS;
	//LED_Timer_Config();
	CPU_timer1_init();
	CAN_Config(CANA);
	CAN_Rx_Config();
	CAN_Rx_IT_Concig();
	//------------------------------------
	SPIFLASH_Init();
	//-----------------------------------------------
	//以下函数是自己添加的自定义功能函数
	//主要是对保持寄存器的相关操作
	error_code = eMBRegisterCB(0x43, Fun_ReadHoldingRegister);
	error_code = eMBRegisterCB(0x41, Fun_WriteHoldingRegister);
	error_code = eMBRegisterCB(0x42, Fun_WriteMultipleHoldingRegister);
	eMBInit(MB_RTU, 0x0A, 1, 9600, MB_PAR_NONE);
	//------------------------------------
	//配置中断
	PieCtrlRegs.PIEIER9.bit.INTx6 = 1; //CANA的中断1
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;	//使能CPU定时器0的中断
	IER |= M_INT9;
	IER |= M_INT13;
	IER |= M_INT8;
	IER |= M_INT1;

	EINT;   // Enable Global interrupt INTM
	ERTM;   // Enable Global realtime interrupt DBGM
	eMBEnable();
   while(1)
   {
		for(i = 0;i<TASKNUM;i++)
		{
			if(tasktimer[i].flag != 0)
			{
				switch (i)
				{
					case TASK10MS:
					{
						task_10ms();
					}
					break;
					case TASK50MS:
					{
						/*
							if(tasktimer[TASK100MS].flag == 1)
							{
							GpioDataRegs.GPBTOGGLE.bit.GPIO34 = 1;
							GpioDataRegs.GPATOGGLE.bit.GPIO0 = 1;
							}
						 */
						task_50ms();
					}
					break;
				}
			}
		}
   }
}
void task_10ms(void)
{
	Code = eMBPoll();
}
void task_50ms(void)
{
	if(can_rx_msg.rx_update == UPDATE)
	{
		can_rx_msg.rx_update = NON_CHANGE;
		CAN_BOOT_ExecutiveCommand(&can_rx_msg);
	}
}
void Modbus_Data_Init(void)
{
	int cnt;
	for (cnt = 0; cnt < REG_INPUT_NREGS; cnt++)
	{
		usRegInputBuf[cnt] = cnt;
	}
	for (cnt = 0; cnt < REG_Coils_NREGS / 8; cnt++)
	{
		CoilsReg_Buf[cnt] = 0;
	}
	for (cnt = 0; cnt < REG_HOLDING_NREGS; cnt++)
	{
		usRegHoldingBuf[cnt] = cnt * 15;
	}
	for (cnt = 0; cnt < REG_Discrete_NREG / 8; cnt++)
	{
		DiscreteReg_Buf[cnt] = 0;
	}
}
