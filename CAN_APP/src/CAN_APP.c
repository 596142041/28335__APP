/*
 * CAN_APP.c
 *
 *  Created on: 2017年5月21日
 *      Author: admin
 */
#include "CAN_APP.h"
typedef  void (*pFunction)(void);
bootloader_data Boot_ID_info;
u8  can_cmd       = (u8 )0x00;//ID的bit0~bit3位为命令码
u16 crc_data      = (u16)0x00;
u16 can_addr      = (u16)0x00;;//ID的bit4~bit15位为节点地址
u32 start_addr    = (u32)0x00;//每一包数据的起始地址
u32 data_size     = (u32)0x00;//数据包的大小
u32 data_index    = (u32)0x00;//数据指针
u32 exe_type      = (u32)0x00;
Boot_CMD_LIST cmd_list =
{
	.Read        = 0x0A, //读取flash数据
	.Erase       = 0x00, //擦除APP区域数据
	.Write       = 0x02,//以多字节形式写数据
	.Check       = 0x03,//检测节点是否在线，同时返回固件信息
	.Excute      = 0x05,//执行固件
	.CmdFaild    = 0x09,//命令执行失败
	.WriteInfo   = 0x01,//设置多字节写数据相关参数(写起始地址,数据量)
	.CmdSuccess  = 0x08,//命令执行成功
	.SetBaudRate = 0x04,//设置节点波特率
};
Device_INFO DEVICE_INFO =
{
 .FW_Version                    = 0x0011121,
 .Device_addr.bits.Device_addr  = DEVICE_ADDR,
 .Device_addr.bits.reserve      = 0x00,
 .FW_TYPE.bits.FW_type          = CAN_BL_APP,
 .FW_TYPE.bits.Chip_Value       = TMS320F28335
};
void __disable_irq(void)
{
	DINT;
	DRTM;
}
void __enable_irq(void)
{
	EINT;
	ERTM;
}
void __set_PRIMASK(u8 state)
{
	if(state == 1)
	{
		__disable_irq();
	}
	else if(state == 0)
	{
		__enable_irq();
	}
	else
	{
		return;
	}
}
unsigned short int CRCcalc16 (unsigned char *data,unsigned short int len)
{
	int i;
	unsigned short int crc_res =  0xFFFF;
	while(len--)
	{
		crc_res^=*data++;
		for(i = 0;i < 8;i++)
		{
			if(crc_res&0x01)
			{
				crc_res = (crc_res>>1)^0xa001;
			}
			else
			{
				crc_res = (crc_res>>1);
			}
		}
	}
	return crc_res;
}
void CAN_BOOT_JumpToApplication(uint32_t Addr)
{
	(*((void(*)(void))(Addr)))();
}
#pragma CODE_SECTION(CAN_BOOT_ExecutiveCommand,"ramfuncs");
void CAN_BOOT_ExecutiveCommand(CanRxMsg *pRxMessage)
{
	u8 i;
	CanTxMsg TxMessage;//发送对应消息
	for(i = 0;i <8;i++)
	{
		TxMessage.CAN_Tx_msg_data.msg_byte.data[i] = 0x00;
	}
	TxMessage.DLC              = 0x01;
	TxMessage.CAN_num          = CANA;
	TxMessage.MBox_num         = 0x03;
	TxMessage.Tx_timeout_cnt   = 0x00;
	TxMessage.SAE_J1939_Flag   = 0x00;
	TxMessage.ExtId.bit.resved = 0x00;
	TxMessage.IDE              = CAN_ID_EXT;
	//获取地址信息
	Boot_ID_info.ExtId.all     = pRxMessage->ExtId;
	can_cmd                    = Boot_ID_info.ExtId.bit.cmd;
	can_addr                   = Boot_ID_info.ExtId.bit.addr;
	if((can_addr!=DEVICE_ADDR)&&(can_addr!=0))
	{
		TxMessage.DLC                              = 0x01;
		TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = 0x01;
		TxMessage.ExtId.bit.ExtId                  = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdFaild;
		CAN_Tx_Msg(&TxMessage);
		return;
	}
	//CMD_List.Check，节点在线检测
	//节点收到该命令后返回固件版本信息和固件类型，
	//该命令在Bootloader程序和APP程序都必须实现
	if(can_cmd == cmd_list.Check)//DSP中尚未实现,相对比较容易实现,主要是为实现APP再次更新应用程序
	{
		if(can_addr != 0x00)
		{
			TxMessage.DLC                              = 8;
			TxMessage.ExtId.bit.ExtId                  = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdSuccess;
			TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = (u8)(DEVICE_INFO.FW_Version>>24);;//主版本号，两字节
			TxMessage.CAN_Tx_msg_data.msg_byte.data[1] = (u8)(DEVICE_INFO.FW_Version>>16);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[2] = (u8)(DEVICE_INFO.FW_Version>>8);//次版本号，两字节
			TxMessage.CAN_Tx_msg_data.msg_byte.data[3] = (u8)(DEVICE_INFO.FW_Version>>0);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[4] = (u8)(DEVICE_INFO.FW_TYPE.bits.FW_type>>16);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[5] = (u8)(DEVICE_INFO.FW_TYPE.bits.FW_type>>8);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[6] = (u8)(DEVICE_INFO.FW_TYPE.bits.FW_type>>0);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[7] = (u8)(DEVICE_INFO.FW_TYPE.bits.Chip_Value>>0);
			CAN_Tx_Msg(&TxMessage);
		}
	}
	//CMD_List.Excute，控制程序跳转到指定地址执行
	//该命令在Bootloader和APP程序中都必须实现
	if(can_cmd == cmd_list.Excute)//该命令在DSP中已经实现
	{
		exe_type = (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[0])&0xFFFFFFFF)<<0x10)|\
				   (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[1])&0x00FFFFFF)<<0x08)|\
				   (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[2])&0x0000FFFF)<<0x00);
		if(exe_type == CAN_BL_BOOT)
		{
			if((*((uint32_t*)Boot__START_ADDR)!=0xFFFFFFFF))
			{
				CAN_BOOT_JumpToApplication(Boot__START_ADDR);
			}
		}
		return;
	}
	return;

}


