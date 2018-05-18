/*******************************************************
 * file_name  : CANB.c
 * DIR:/28335_APP/bsp/src/CANB.c
 * Created on : 2017年11月15日-下午8:25:38
 * Author     : admin
 * Version    :
 *
 ******************************************************/
#include "CANB.h"
tag_mbxCfg CANB_tx_msg[12];
u8 dir = 0;
unsigned short int speed_fbk = 0;
unsigned short int speed_ref = 0;
void CAN_tx_msg_init(void)
{
	int i;
	unsigned long int ID = 0x4185;
	for(i = 0;i<12;i++)
	{
		CANB_tx_msg[i].Enable = 1;
		CANB_tx_msg[i].cnt = 0;
		CANB_tx_msg[i].flag = 0;
		CANB_tx_msg[i].txEnable = 1;
		CANB_tx_msg[i].can_tx_msg.CAN_num =	CANB;
		CANB_tx_msg[i].can_tx_msg.DLC = 8;
		CANB_tx_msg[i].can_tx_msg.IDE =CAN_ID_EXT;
		CANB_tx_msg[i].can_tx_msg.ExtId.all = 0x00;
		CANB_tx_msg[i].can_tx_msg.ExtId.bit.ExtId = ID+i;
		CANB_tx_msg[i].can_tx_msg.MBox_num = i+1;

		CANB_tx_msg[i].can_tx_msg.StdId.all = 0x00;
		CANB_tx_msg[i].can_tx_msg.tx_update = NON_CHANGE;
		CANB_tx_msg[i].can_tx_msg.SAE_J1939_Flag = 0;

	}
	CANB_tx_msg[0].prd  =10;
	CANB_tx_msg[1].prd  =20;
	CANB_tx_msg[2].prd  =20;
	CANB_tx_msg[3].prd  =20;
	CANB_tx_msg[4].prd  =10;
	CANB_tx_msg[5].prd  =10;
	CANB_tx_msg[6].prd  =20;
	CANB_tx_msg[7].prd  =20;
	CANB_tx_msg[8].prd  =20;
	CANB_tx_msg[9].prd  =20;
	CANB_tx_msg[10].prd =20;
	CANB_tx_msg[11].prd =20;
}

void packcanbMbx0(void)//0x4185
{
	CANB_tx_msg[0].can_tx_msg.ExtId.bit.ExtId = 0x4185;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[0].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx1(void)//0x4186
{
	CANB_tx_msg[1].can_tx_msg.ExtId.bit.ExtId = 0x4186;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[1].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx2(void)//0x4187
{
	CANB_tx_msg[2].can_tx_msg.ExtId.bit.ExtId = 0x4187;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = (speed_ref)&0xFF;;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = (speed_ref>>8)&0xFF;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = (speed_fbk)&0xFF;
	CANB_tx_msg[2].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = (speed_fbk>>8)&0xFF;
}
void packcanbMbx3(void)//0x4188
{
	CANB_tx_msg[3].can_tx_msg.ExtId.bit.ExtId = 0x4188;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[3].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx4(void)//0x4189
{
	CANB_tx_msg[1].can_tx_msg.ExtId.bit.ExtId = 0x4189;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[4].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx5(void)//0x418A
{
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[5].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx6(void)//0x418B
{
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[6].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx7(void)//0x418C
{
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[7].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx8(void)//0x418D
{
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[8].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx9(void)//0x418E
{
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[9].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx10(void)//0x418F
{
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[10].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void packcanbMbx11(void)//0x4190
{
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte0 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte1 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte2 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte3 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte4 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte5 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte6 = 0;
	CANB_tx_msg[11].can_tx_msg.CAN_Tx_msg_data.msg_Byte.byte7 = 0;
}
void canbMbxSlotCtrl(void)//0x4191
{
    int i;
    if(dir == 0)
    {
    	speed_fbk++;
    	speed_ref = 0x4190-speed_fbk;
    	if(speed_fbk > 0x4190)
    	{
    		dir = 1;
    	}
    }
    else if(dir == 1)
    {
    	if(speed_fbk >5)
    	{
    		speed_fbk--;
    		speed_ref = 0x4190-speed_fbk;
    	}
    	else
    	{
    		dir = 0;
    	}
    }
	for(i=0;i<12;i++)
	{
		CANB_tx_msg[i].cnt++;
		if(CANB_tx_msg[i].cnt>=CANB_tx_msg[i].prd)
		{
			CANB_tx_msg[i].flag=1;
			CANB_tx_msg[i].cnt=0;
		}
	}
}
void canbtransmit(void)
{
	canbMbxSlotCtrl();
	if((CANB_tx_msg[0].txEnable) && (CANB_tx_msg[0].flag) &&CANB_tx_msg[0].Enable)//4185
	{
		packcanbMbx0();
		CAN_Tx_Msg(&CANB_tx_msg[0].can_tx_msg);
		CANB_tx_msg[0].flag=0;
	}

	if((CANB_tx_msg[1].txEnable) && (CANB_tx_msg[1].flag)&&CANB_tx_msg[1].Enable)//4186
	{
		packcanbMbx1();
		CAN_Tx_Msg(&CANB_tx_msg[1].can_tx_msg);
		CANB_tx_msg[1].flag=0;
	}
	if((CANB_tx_msg[2].txEnable) && (CANB_tx_msg[2].flag)&&CANB_tx_msg[2].Enable)//4187
	{
		packcanbMbx2();
		CAN_Tx_Msg(&CANB_tx_msg[2].can_tx_msg);
		CANB_tx_msg[2].flag=0;
	}

	if((CANB_tx_msg[3].txEnable) && (CANB_tx_msg[3].flag)&&CANB_tx_msg[3].Enable)//4188
	{
		packcanbMbx3();
		CAN_Tx_Msg(&CANB_tx_msg[3].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 8, &ECanbMBoxData[8]);
		CANB_tx_msg[3].flag=0;
	}
	if((CANB_tx_msg[4].txEnable) && (CANB_tx_msg[4].flag)&&CANB_tx_msg[4].Enable)//4189
	{
		packcanbMbx4();
		CAN_Tx_Msg(&CANB_tx_msg[4].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 9, &ECanbMBoxData[9]);
		CANB_tx_msg[4].flag=0;
	}

	if((CANB_tx_msg[5].txEnable) && (CANB_tx_msg[5].flag)&&CANB_tx_msg[5].Enable)//418A
	{
		packcanbMbx5();
		CAN_Tx_Msg(&CANB_tx_msg[5].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 10, &ECanbMBoxData[10]);
		CANB_tx_msg[5].flag=0;
	}
	if((CANB_tx_msg[6].txEnable) && (CANB_tx_msg[6].flag)&&CANB_tx_msg[6].Enable)//418B
	{
		packcanbMbx6();
		CAN_Tx_Msg(&CANB_tx_msg[6].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 11, &ECanbMBoxData[11]);
		CANB_tx_msg[6].flag=0;
	}
	if((CANB_tx_msg[7].txEnable) && (CANB_tx_msg[7].flag)&&CANB_tx_msg[7].Enable)//418C
	{
		packcanbMbx7();
		CAN_Tx_Msg(&CANB_tx_msg[7].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 12, &ECanbMBoxData[12]);
		CANB_tx_msg[7].flag=0;
	}
	if((CANB_tx_msg[8].txEnable) && (CANB_tx_msg[8].flag)&&CANB_tx_msg[8].Enable)//418D
	{
		packcanbMbx8();
		CAN_Tx_Msg(&CANB_tx_msg[8].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 13, &ECanbMBoxData[13]);
		CANB_tx_msg[8].flag=0;
	}
	if((CANB_tx_msg[9].txEnable) && (CANB_tx_msg[9].flag)&&CANB_tx_msg[9].Enable)//418E
	{
		packcanbMbx9();
		CAN_Tx_Msg(&CANB_tx_msg[9].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 14, &ECanbMBoxData[14]);
		CANB_tx_msg[9].flag=0;
	}
	if((CANB_tx_msg[10].txEnable) && (CANB_tx_msg[10].flag)&&CANB_tx_msg[10].Enable)//418F
	{
		packcanbMbx10();
		CAN_Tx_Msg(&CANB_tx_msg[10].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 15, &ECanbMBoxData[15]);
		CANB_tx_msg[10].flag=0;
	}
	if((CANB_tx_msg[11].txEnable) && (CANB_tx_msg[11].flag)&&CANB_tx_msg[11].Enable)//4190
	{
		packcanbMbx11();
		CAN_Tx_Msg(&CANB_tx_msg[11].can_tx_msg);
		//canTransmitDriver(CAN_B_INDEX, 16, &ECanbMBoxData[16]);
		CANB_tx_msg[11].flag=0;
	}
}
