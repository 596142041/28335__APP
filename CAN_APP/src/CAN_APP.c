/*
 * CAN_APP.c
 *
 *  Created on: 2017年5月21日
 *      Author: admin
 */
#include "CAN_APP.h"
#define DATA_LEN  520
#define READ_MAX  256
bootloader_data Boot_ID_info;
u8  can_cmd         = (u8 )0x00;//ID的bit0~bit3位为命令码
u16 can_addr        = (u16)0x00;;//ID的bit4~bit15位为节点地址
u32 exe_type        = (u32)0x00;
u32 read_addr       = (u32)0x00;//读取数据起始地址
u32 read_len        = (u32)0x00;//读取数据长度
u32 data_index      = (u32)0x00;//数据指针
Uint16 app_check[2] = {0xF2F2,0xF3F3};
u8	   data_temp[DATA_LEN*2];
Uint16 read_temp[READ_MAX];
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
 .FW_Version                    = 0x0011001,
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
	Boot_ID_info.ExtId.all     = pRxMessage->ExtId.bit.ExtId;
	can_cmd                    = Boot_ID_info.ExtId.bit.cmd;
	can_addr                   = Boot_ID_info.ExtId.bit.addr;
	if((can_addr!=DEVICE_ADDR))
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
	//cmd_list.read,读取flash数据,
	//该命令是用于读取内部存储器数据
	//该命令在Bootloader和APP程序中国必须实现
	if(can_cmd == cmd_list.Read)
	{
		if(pRxMessage->DLC != 8)
		{
			TxMessage.DLC                              = 0x02;
			TxMessage.ExtId.bit.ExtId                  = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdFaild;
			TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = (u8)cmd_list.Read;//主版本号，两字节
			TxMessage.CAN_Tx_msg_data.msg_byte.data[1] = (u8)MSG_DATA_LEN_ERROR;
			CAN_Tx_Msg(&TxMessage);
			return;
		}
		read_addr =  (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[0])&0xFFFFFFFF)<<0x18)|\
					 (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[1])&0x00FFFFFF)<<0x10)|\
					 (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[2])&0x0000FFFF)<<0x08)|\
					 (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[3])&0x000000FF)<<0x00);
		read_len  =  (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[4])&0xFFFFFFFF)<<0x18)|\
					 (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[5])&0x00FFFFFF)<<0x10)|\
				     (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[6])&0x0000FFFF)<<0x08)|\
				     (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[7])&0x000000FF)<<0x00);
		if(read_len > READ_MAX)
		{
			TxMessage.DLC                              = 0x02;
			TxMessage.ExtId.bit.ExtId                  = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdFaild;
			TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = (u8)cmd_list.Read;//主版本号，两字节
			TxMessage.CAN_Tx_msg_data.msg_byte.data[1] = (u8)READ_LEN_ERROR;
			CAN_Tx_Msg(&TxMessage);
			return;
		}
		u16 read_len_temp  = 0;
		if(read_len%2 == 0)//因为每次只能读取N个字
		{
			read_len_temp = read_len;
		}
		else
		{
			read_len_temp = read_len+1;
		}
		read_len_temp = read_len_temp>>1;
		__set_PRIMASK(1);
		Flash_RD(read_addr,read_temp,read_len_temp);
		__set_PRIMASK(0);
		for(i = 0;i <read_len_temp;i++)
		{
			data_temp[i*2+0] = (read_temp[i]>>8)&0xFF;
			data_temp[i*2+1] = read_temp[i]&0xFF;
		}
		data_index = 0;
		i = 0;
		while(data_index < read_len)
		{
			int temp;
			temp = read_len - data_index;
			if (temp >= 8)
			{
				TxMessage.DLC             = 0x08;
				TxMessage.ExtId.bit.ExtId = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdSuccess;
				for (i = 0; i < TxMessage.DLC; i++)
				{
					TxMessage.CAN_Tx_msg_data.msg_byte.data[i] = data_temp[data_index];
					data_index++;
				}

			}
			else
			{
				TxMessage.DLC             = temp;
				TxMessage.ExtId.bit.ExtId = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdSuccess;
				for (i = 0; i < TxMessage.DLC; i++)
				{
					TxMessage.CAN_Tx_msg_data.msg_byte.data[i] = data_temp[data_index];
					data_index++;
				}
			}
			CAN_Tx_Msg(&TxMessage);
		}
		data_index = 0;
		return;
	}
	//CMD_List.Excute，控制程序跳转到指定地址执行
	//该命令在Bootloader和APP程序中都必须实现
	if(can_cmd == cmd_list.Excute)//该命令在DSP中已经实现
	{
		__set_PRIMASK(1);
		Flash_WR(APP_INFO_ADDR,app_check,2);
		__set_PRIMASK(0);
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


