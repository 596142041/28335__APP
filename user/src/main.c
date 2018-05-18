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
#include "CANB.h"
#include "SCIA.h"
#include  "stdio.h"
int i = 0;
eMBErrorCode Code, error_code;
void Modbus_Data_Init(void);
void task_10ms(void);
void task_50ms(void);
void task_5ms(void);
void main(void)
{
	InitSysCtrl();
	Modbus_Data_Init();
	LED_GPIO_Config();
	CAN_GPIO_Config(CANA);
	CAN_GPIO_Config(CANB);


	CAN_tx_msg_init();
	taskinit();
	DINT;
	DRTM;
	CsmUnlock();
	InitPieCtrl();
	IER = 0x0000;
	IFR = 0x0000;
	InitPieVectTable();
	MemCopy(&RamfuncsLoadStart,&RamfuncsLoadEnd,&RamfuncsRunStart);
	MemCopy(&Flash28_API_LoadStart, &Flash28_API_LoadEnd,&Flash28_API_RunStart);
	InitFlash();
	FlashAPI_Init();
	EALLOW;  // This is needed to write to EALLOW protected registers
	PieVectTable.XINT13    = &CPU_timer1_isr;
	PieVectTable.SCIRXINTC = &SCIC_Rx_isr;
	PieVectTable.SCITXINTC = &SCIC_Tx_isr;
	PieVectTable.TINT0     = &cpu_timer2_isr;
	EDIS;
	SCIA_Init();
	CPU_timer1_init();
	CAN_Config(CANA,500);
	CAN_Config(CANB,250);
	CAN_Rx_Config();
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
	PieCtrlRegs.PIEIER1.bit.INTx7 = 1;	//使能CPU定时器0的中断
	IER |= M_INT13;
	IER |= M_INT8;
	IER |= M_INT1;
	EINT;
	ERTM;
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
						task_50ms();
					}
					break;
					case TASK5MS:
					{
						task_5ms();
					}
					break;
					case TASK1000MS:
					{
						 SCIA_Send_string("TASK1000MS",10);
					//	 printf("TASK1000MS\r\n");
					}
				}
			}
			tasktimer[i].flag = 0;
		}
   }
}
void task_5ms(void)
{
	canbtransmit();
}
void task_10ms(void)
{
	usRegHoldingBuf[0] = speed_ref;
	usRegHoldingBuf[1] = speed_fbk;
	usRegHoldingBuf[2] = dir;
	Code = eMBPoll();
}
void task_50ms(void)
{
	if(ECanaRegs.CANRMP.bit.RMP31 == 1)
		{
		 ECanaRegs.CANRMP.bit.RMP31 = 1;
			CAN_Rx_Msg(&can_rx_msg);
			can_rx_msg.rx_update = UPDATE;
		}

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
		usRegInputBuf[cnt] = cnt*10+cnt%5;
	}
	for (cnt = 0; cnt < REG_Coils_NREGS / 8; cnt++)
	{
		if(i%2 == 0)
		{
			CoilsReg_Buf[cnt] = 0xAA;
		}
		else
		{
			CoilsReg_Buf[cnt] = 0x55;
		}
	}
	for (cnt = 0; cnt < REG_HOLDING_NREGS; cnt++)
	{
		usRegHoldingBuf[cnt] = (cnt+1) * 5;
	}
	for (cnt = 0; cnt < REG_Discrete_NREG /8; cnt++)
	{
		if(i%2 == 0)
		{
			DiscreteReg_Buf[cnt] = 0xAA;
		}
		else
		{
			DiscreteReg_Buf[cnt] = 0x55;
		}
	}
}
