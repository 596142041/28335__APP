#include "W25Q64BV.h"
SPI_FLASH W25Q64;
static u8 buffer[4096];
static void SPIFLAH_GPIO_Config(void)
{
	// GPIO_InitTypeDef GPIO_init;
	// RCC_AHB1PeriphClockCmd(W25Q64_SPI_GPIO_RCC|W25Q64_GPIO_RCC,ENABLE);
	// GPIO_PinAFConfig(W25Q64_SPI_GPIO,GPIO_PinSource3,GPIO_AF_SPI3);
	// GPIO_PinAFConfig(W25Q64_SPI_GPIO,GPIO_PinSource4,GPIO_AF_SPI3);
	// GPIO_PinAFConfig(W25Q64_SPI_GPIO,GPIO_PinSource5,GPIO_AF_SPI3);
	// GPIO_init.GPIO_Mode = GPIO_Mode_OUT;
	// GPIO_init.GPIO_OType = GPIO_OType_PP;
	// GPIO_init.GPIO_PuPd = GPIO_PuPd_UP;
	// GPIO_init.GPIO_Speed = GPIO_Speed_50MHz;
	// GPIO_init.GPIO_Pin = W25Q64_CS_Pin;
	// GPIO_Init(W25Q64_CS_GPIO,&GPIO_init);
	// GPIO_init.GPIO_Mode = GPIO_Mode_AF;
	// GPIO_init.GPIO_Pin = W25Q64_MISO_Pin|W25Q64_MOSI_Pin|W25Q64_SCK_Pin;
	//GPIO_Init(W25Q64_SPI_GPIO,&GPIO_init);
}
static void SPIFLASH_SPI_Config(void)
{
	// SPI_InitTypeDef SPI_init;
	// RCC_APB1PeriphClockCmd(W25Q64SPI_CLK,ENABLE);
	// SPI_init.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_32;
	// SPI_init.SPI_CPHA = SPI_CPHA_1Edge;
	// SPI_init.SPI_CPOL = SPI_CPOL_Low;
	// SPI_init.SPI_CRCPolynomial = 7;
	// SPI_init.SPI_DataSize  = SPI_DataSize_8b;
	// SPI_init.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	// SPI_init.SPI_FirstBit  = SPI_FirstBit_MSB;
	// SPI_init.SPI_Mode = SPI_Mode_Master;
	// SPI_init.SPI_NSS  = SPI_NSS_Soft;
	// SPI_Init(W25Q64_SPI,&SPI_init);
	// SPI_Cmd(W25Q64_SPI,ENABLE);
}
void SPIFLASH_Init(void)
{
	SPIFLAH_GPIO_Config();
	SPIFLASH_SPI_Config();
	SPI_FLASH_ReadID(&W25Q64);
	SPI_FLASH_ReadDevice_ID(&W25Q64);
	if(W25Q64.ChipID == W25Q64BV_ID)
	{
		W25Q64.PageSize = 4*1024;
	//	W25Q64.TotalSize = 8*1024*1024;
	}
	Write_Status(0xFF);
}
static u8 SPI_send_data(u8 data)
{
	u8 read;
	read=data;
	// while(SPI_I2S_GetFlagStatus(W25Q64_SPI, SPI_I2S_FLAG_TXE) == RESET);
	// SPI_I2S_SendData(W25Q64_SPI, data);
	// while(SPI_I2S_GetFlagStatus(W25Q64_SPI, SPI_I2S_FLAG_RXNE) == RESET);
	// read = SPI_I2S_ReceiveData(W25Q64_SPI);
	return read;
}
void SPI_FLASH_ReadID(SPI_FLASH *SPI_flash)
{
	u8 id_temp1,id_temp2,id_temp3;
	W25Q64_CS_CLR;
	SPI_send_data(CMD_RDID);
	id_temp1 = SPI_send_data(DUMMY_BYTE);
	id_temp2 = SPI_send_data(DUMMY_BYTE);
	id_temp3 = SPI_send_data(DUMMY_BYTE);
	W25Q64_CS_SET;
	SPI_flash->ChipID = ((uint32_t)id_temp1 << 16) | ((uint32_t)id_temp2 << 8) | id_temp3;
}
void SPI_FLASH_ReadDevice_ID(SPI_FLASH *SPI_flash)
{
	u8 temp1,temp2;
	W25Q64_CS_CLR;
	SPI_send_data(0x90);
	SPI_send_data(0x00);
	SPI_send_data(0x00);
	SPI_send_data(0x00);
	temp1 = SPI_send_data(DUMMY_BYTE);
	temp2 = SPI_send_data(DUMMY_BYTE);
	W25Q64_CS_SET;
	SPI_flash->device_id = (uint16_t)(temp1<<8|temp2);
}
static void SPI_FLASH_Write_Enable(void)
{
	W25Q64_CS_CLR;
	SPI_send_data(CMD_WREN);
	W25Q64_CS_SET;
}
static void SPI_FLASH_Write_Disable(void)
{
	W25Q64_CS_CLR;
	SPI_send_data(CMD_DISWR);
	W25Q64_CS_SET;
}
static void WaiForWriteEnd(void)
{
	u8 temp = 0x01;
	W25Q64_CS_CLR;
	SPI_send_data(0x05);//��ȡ״̬�Ĵ���1
	while(temp&WIP_FLAG)
	{
		temp = SPI_send_data(DUMMY_BYTE);
	}
	W25Q64_CS_SET;
	
}
static void Write_Status(u8 status_data)
{
	W25Q64_CS_CLR;
	SPI_send_data(0x05);//д״̬�Ĵ���
	SPI_send_data(status_data);
	W25Q64_CS_SET;
}
void Sector_4kb_Erase(u32 addr)
{
	SPI_FLASH_Write_Enable();
	W25Q64_CS_CLR;
	SPI_send_data(CMD_SE4k);//��������
	SPI_send_data((addr&0xFF0000)>>16);//�����ֽڸ߰�λ
	SPI_send_data((addr&0xFF00)>>8);//�����ֽ��м�8λ
	SPI_send_data((addr&0xFF));//�����ֽڸ߰�λ
	W25Q64_CS_SET;
	WaiForWriteEnd();
}
void Sector_32kb_Erase(u32 addr)
{
	SPI_FLASH_Write_Enable();
	W25Q64_CS_CLR;
	SPI_send_data(CMD_SE32k);//��������
	SPI_send_data((addr&0xFF0000)>>16);//�����ֽڸ߰�λ
	SPI_send_data((addr&0xFF00)>>8);//�����ֽ��м�8λ
	SPI_send_data((addr&0xFF));//�����ֽڸ߰�λ
	W25Q64_CS_SET;
	WaiForWriteEnd();
}
void Sector_64kb_Erase(u32 addr)
{
	SPI_FLASH_Write_Enable();
	W25Q64_CS_CLR;
	SPI_send_data(CMD_SE64k);//��������
	SPI_send_data((addr&0xFF0000)>>16);//�����ֽڸ߰�λ
	SPI_send_data((addr&0xFF00)>>8);//�����ֽ��м�8λ
	SPI_send_data((addr&0xFF));//�����ֽڸ߰�λ
	W25Q64_CS_SET;
	WaiForWriteEnd();
}
void Chip_Erase(void)
{
	SPI_FLASH_Write_Enable();
	W25Q64_CS_CLR;
	SPI_send_data(CMD_Chip);//��������
	W25Q64_CS_SET;
	WaiForWriteEnd();
}
void Write_Page(u32 addr,u8 *data,u16 len)
{
	u16 page_temp;
	u16 i,j;
	page_temp=len/256;//�ж�д�����ݳ����м�ҳ
	for(j = 0; j < page_temp; j++)
	{
		SPI_FLASH_Write_Enable();
		W25Q64_CS_CLR;
		SPI_send_data(CMD_PAGE_W);
		SPI_send_data((addr&0xFF0000)>>16);//�����ֽڸ߰�λ
		SPI_send_data((addr&0xFF00)>>8);//�����ֽ��м�8λ
		SPI_send_data((addr&0xFF));//�����ֽڸ߰�λ
		for(i = 0;i < 256;i++)
		{
			SPI_send_data(*data);
			data++;
		}
			W25Q64_CS_SET;
			WaiForWriteEnd();
		addr+=256;
	}
	SPI_FLASH_Write_Disable();
}
void Read_Buffer(u32 addr,u8 *data,u16 len)
{
	if((len == 0)||(len > W25Q64.TotalSize))
	{
		return;
	}
	W25Q64_CS_CLR;
	SPI_send_data(CMD_READ);//��������
	SPI_send_data((addr&0xFF0000)>>16);//�����ֽڸ߰�λ
	SPI_send_data((addr&0xFF00)>>8);//�����ֽ��м�8λ
	SPI_send_data((addr&0xFF));//�����ֽڸ߰�λ
	while(len)
	{
		*data = SPI_send_data(DUMMY_BYTE);
		len--;
		data++;
	}
	W25Q64_CS_SET;
}
void SPI_Flash_WR(unsigned long int addr,unsigned long int leng,unsigned char *data)
{
	/*
		���ṩһ��˼·
		��һ��,�жϵ�ַ�Ƿ���;
		�ڶ���,�ж���ʼ��ַ���ڵ�����
	*/
	  
	SECTO_INF start,end;
	
	unsigned short int cnt;
	start.sector_num = addr/W25Q64.PageSize;
	start.sector_offset = addr%W25Q64.PageSize;
	end.sector_num = (addr+leng)/W25Q64.PageSize;
	end.sector_offset = (addr+leng)%W25Q64.PageSize;
	if(start.sector_num == end.sector_num)
	{
		Read_Buffer((start.sector_num*W25Q64.PageSize),buffer,W25Q64.PageSize);//��ȡ��ǰ������
		for(cnt = start.sector_offset; cnt < end.sector_offset;cnt++)//�ı䵱ǰ������
		{
			buffer[cnt] = *data;
			data++;
		}
		//׼��������ǰ������
		Sector_4kb_Erase((start.sector_num*W25Q64.PageSize));//������ǰ���ڵ�����
		//��������������ȫ��д��������
		Write_Page((start.sector_num*W25Q64.PageSize),buffer,4096);
		Read_Buffer((start.sector_num*W25Q64.PageSize),buffer,W25Q64.PageSize);//��ȡ��ǰ������
	}
	else
	{
		
	}
	for(cnt = 0;cnt < 4096;cnt++)
	{
		buffer[cnt] = 0;
	}
}
