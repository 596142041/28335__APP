/*
 * CAN_APP.c
 *
 *  Created on: 2017��5��21��
 *      Author: admin
 */
#include "CAN_APP.h"
typedef  void (*pFunction)(void);
bootloader_data Boot_ID_info;
u8	   data_temp[128];
Uint16 write_temp[128];
u8 can_cmd    = 0x00;//ID��bit0~bit3λΪ������
u16 can_addr  = 0x00;//ID��bit4~bit15λΪ�ڵ��ַ
u32 start_addr = 0x0000;//ÿһ�����ݵ���ʼ��ַ
u32 data_size=0;//���ݰ��Ĵ�С
u32 data_index=0;//����ָ��
u16 crc_data;
uint32_t exe_type = 0x00;
Boot_CMD_LIST cmd_list =
{
	.Erase       = 0x00, //����APP��������
	.Write       = 0x02,//�Զ��ֽ���ʽд����
	.Check       = 0x03,//���ڵ��Ƿ����ߣ�ͬʱ���ع̼���Ϣ
	.Excute      = 0x05,//ִ�й̼�
	.CmdFaild    = 0x09,//����ִ��ʧ��
	.WriteInfo   = 0x01,//���ö��ֽ�д������ز���(д��ʼ��ַ,������)
	.CmdSuccess  = 0x08,//����ִ�гɹ�
	.SetBaudRate = 0x04,//���ýڵ㲨����
};
Device_INFO DEVICE_INFO =
{
//.FW_TYPE = CAN_BL_APP,
.FW_Version = 0x0010001,
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
	CanTxMsg TxMessage;//���Ͷ�Ӧ��Ϣ
	DEVICE_INFO.Device_addr.bits.Device_addr  = DEVICE_ADDR;
	DEVICE_INFO.Device_addr.bits.reserve = 0x00;
	DEVICE_INFO.FW_TYPE.bits.FW_type = 0xAAAAAA;
	DEVICE_INFO.FW_TYPE.bits.Chip_Value = 0xAA;
	for(i = 0;i <8;i++)
	{
		TxMessage.CAN_Tx_msg_data.msg_byte.data[i] = 0x00;
	}
	TxMessage.CAN_num = CANA;
	TxMessage.DLC = 1;
	TxMessage.ExtId.bit.resved = 0x00;
	TxMessage.IDE = CAN_ID_EXT;
	TxMessage.MBox_num = 0x03;
	TxMessage.Tx_timeout_cnt = 0x00;
	TxMessage.SAE_J1939_Flag = 0;
	//��ȡ��ַ��Ϣ
	Boot_ID_info.ExtId.all = pRxMessage->ExtId;
	can_cmd  = Boot_ID_info.ExtId.bit.cmd;
	can_addr = Boot_ID_info.ExtId.bit.addr;
	if((can_addr!=DEVICE_ADDR)&&(can_addr!=0))
	{
		TxMessage.ExtId.bit.ExtId = (DEVICE_ADDR<<CMD_WIDTH)|cmd_list.CmdFaild;
		TxMessage.DLC = 1;
		TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = 0x01;
		CAN_Tx_Msg(&TxMessage);
		return;
	}

	//CMD_List.Check���ڵ����߼��
	//�ڵ��յ�������󷵻ع̼��汾��Ϣ�͹̼����ͣ�
	//��������Bootloader�����APP���򶼱���ʵ��
	if(can_cmd == cmd_list.Check)//DSP����δʵ��,��ԱȽ�����ʵ��,��Ҫ��Ϊʵ��APP�ٴθ���Ӧ�ó���
	{
		if(can_addr != 0x00)
		{
			DEVICE_INFO.Device_addr.bits.Device_addr = DEVICE_ADDR;
			TxMessage.ExtId.bit.ExtId = (DEVICE_INFO.Device_addr.bits.Device_addr<<CMD_WIDTH)|cmd_list.CmdSuccess;
			TxMessage.CAN_Tx_msg_data.msg_byte.data[0] = (u8)(DEVICE_INFO.FW_Version>>24);;//���汾�ţ����ֽ�
			TxMessage.CAN_Tx_msg_data.msg_byte.data[1] = (u8)(DEVICE_INFO.FW_Version>>16);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[2] = (u8)(DEVICE_INFO.FW_Version>>8);//�ΰ汾�ţ����ֽ�
			TxMessage.CAN_Tx_msg_data.msg_byte.data[3] = (u8)(DEVICE_INFO.FW_Version>>0);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[4] = (u8)(DEVICE_INFO.FW_TYPE.bits.FW_type>>16);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[5] = (u8)(DEVICE_INFO.FW_TYPE.bits.FW_type>>8);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[6] = (u8)(DEVICE_INFO.FW_TYPE.bits.FW_type>>0);
			TxMessage.CAN_Tx_msg_data.msg_byte.data[7] = (u8)(DEVICE_INFO.FW_TYPE.bits.Chip_Value>>0);
			TxMessage.DLC = 8;
			CAN_Tx_Msg(&TxMessage);
		}
	}
	//CMD_List.Excute�����Ƴ�����ת��ָ����ִַ��
	//��������Bootloader��APP�����ж�����ʵ��
	if(can_cmd == cmd_list.Excute)//��������DSP���Ѿ�ʵ��
	{
		exe_type  = (((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[0])&0xFFFFFFFF)<<24)|\
					(((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[1])&0x00FFFFFF)<<16)|\
					(((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[2])&0x0000FFFF)<<8)|\
					(((u32)(pRxMessage->CAN_Rx_msg_data.msg_byte.data[3])&0x000000FF)<<0);
		if(exe_type == CAN_BL_BOOT)
		{
			if((*((uint32_t *)Boot__START_ADDR)!=0xFFFFFFFF))
			{
				CAN_BOOT_JumpToApplication(Boot__START_ADDR);
			}
		}
		return;
	}
	return;

}


