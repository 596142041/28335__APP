/*
 * CAN_APP.h
 *
 *  Created on: 2017��5��21��
 *      Author: admin
 */

#ifndef CAN_APP_INC_CAN_APP_H_
#define CAN_APP_INC_CAN_APP_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
#include "CANA.h"
#define APP_START_ADDR   ((uint32_t)0x310000)
#define Boot__START_ADDR ((uint32_t)0x33FFF6)
#define CAN_BL_APP      0xAAAAAAAA
#define CAN_BL_BOOT     0x55555555
#define DEVICE_ADDR     0x134//�豸��ַ
#define CMD_WIDTH       4
typedef struct _Device_INFO
{
	union
	{
		unsigned short int all;
		struct
		{
			unsigned short int Device_addr:	12;
			unsigned short int reserve:	4;
		}bits;//�豸��ַ
	}Device_addr;
	union
	{
		unsigned long int all;
		struct
		{
			unsigned long int FW_type:24;
			unsigned long int Chip_Value:8;
		}bits;
	}FW_TYPE;
	//unsigned long int FW_TYPE;//�̼�����
	unsigned long int FW_Version;//�̼��汾
}Device_INFO;
typedef struct _bootloader_data
{
	union
	{
		u32 all;
		struct
		{
			u16 cmd :4; //����
			u16 addr :12; //�豸��ַ
			u16 reserve :16; //����λ
		} bit;
	} ExtId; //��չ֡ID
	unsigned char IDE;   //֡����,��Ϊ��CAN_ID_STD(��׼֡),CAN_ID_EXT(��չ֡)
	unsigned char DLC;  //���ݳ��ȣ���Ϊ0��8;
	u8 data[8];
} bootloader_data;
typedef struct _Boot_CMD_LIST
{
	//Bootloader�������
	unsigned char Erase;        //����APP������������
	unsigned char Write;        //�Զ��ֽ���ʽд����
	unsigned char Check;        //���ڵ��Ƿ����ߣ�ͬʱ���ع̼���Ϣ
	unsigned char Excute;       //ִ�й̼�
	unsigned char WriteInfo;    //���ö��ֽ�д������ز�����д��ʼ��ַ����������
	unsigned char SetBaudRate;  //���ýڵ㲨����
	//�ڵ㷵��״̬,�ؼ�
	unsigned char CmdFaild;     //����ִ��ʧ��
	unsigned char CmdSuccess;   //����ִ�гɹ�
} Boot_CMD_LIST;
extern Boot_CMD_LIST cmd_list;
extern bootloader_data Bootloader_data;
extern Device_INFO DEVICE_INFO;
void __disable_irq(void);
void __enable_irq(void);
void __set_PRIMASK(u8 state);

unsigned short int CRCcalc16 (unsigned char *data,unsigned short int len);
void CAN_BOOT_JumpToApplication(uint32_t Addr);
void CAN_BOOT_ExecutiveCommand(CanRxMsg *pRxMessage);


#endif /* CAN_APP_INC_CAN_APP_H_ */
