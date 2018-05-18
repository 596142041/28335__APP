/*
 * CAN_APP.h
 *
 *  Created on: 2017年5月21日
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
#include "Flash.h"
#define APP_START_ADDR   ((uint32_t)0x310000)
#define APP_INFO_ADDR    ((uint32_t)0x310000)
#define Boot__START_ADDR ((uint32_t)0x33FFF6)
#define CAN_BL_APP       0xAAAAAA
#define CAN_BL_BOOT      0x555555
#define DEVICE_ADDR      0x134//设备地址
#define CMD_WIDTH        0x04
#define ADDR_WIDTH       0x0C
//--------------------------------------------------
//---以下宏定义是对芯片型号进行宏定义
#define TMS320F28335     0x01
#define TMS320F2808      0x02
#define STM32F407IGT6    0x03
//---------------------------------------------------------------------------------
//故障信息列表
#define DEVICE_ADDR_ERROR  0xA0
#define ERASE_ERROR        0xA1
#define WRITE_ERROR        0xA2
#define READ_LEN_ERROR     0xA3
#define MSG_DATA_LEN_ERROR 0xA4
//---------------------------------------------------
typedef  void (*pFunction)(void);
typedef struct _Device_INFO
{
	union
	{
		unsigned short int all;
		struct
		{
			unsigned short int Device_addr:	12;
			unsigned short int reserve:	4;
		}bits;//设备地址
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
	union
	{
		unsigned long  int all;
		struct
		{
			unsigned long  int Version:	7;//固件版本
			unsigned long  int date:	5;//日期
			unsigned long  int month:	4;//月
			unsigned long  int year:	16;//年
		}bits;
	}FW_Version;//固件版本
}Device_INFO;
typedef struct _bootloader_data
{
	union
	{
		u32 all;
		struct
		{
			u16 cmd :4; //命令
			u16 addr :12; //设备地址
			u16 reserve :16; //保留位
		} bit;
	} ExtId; //扩展帧ID
	unsigned char IDE;   //帧类型,可为：CAN_ID_STD(标准帧),CAN_ID_EXT(扩展帧)
	unsigned char DLC;  //数据长度，可为0到8;
	u8 data[8];
} bootloader_data;
typedef struct _Boot_CMD_LIST
{
	//Bootloader相关命令
	unsigned char Read;         //读取flash数据
	unsigned char Erase;        //擦出APP储存扇区数据
	unsigned char Write;        //以多字节形式写数据
	unsigned char Check;        //检测节点是否在线，同时返回固件信息
	unsigned char Excute;       //执行固件
	unsigned char WriteInfo;    //设置多字节写数据相关参数（写起始地址，数据量）
	unsigned char SetBaudRate;  //设置节点波特率
	//节点返回状态,关键
	unsigned char CmdFaild;     //命令执行失败
	unsigned char CmdSuccess;   //命令执行成功
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
