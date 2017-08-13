#include "port.h"
#include "mbcrc.h"
USHORT  usMBCRC16( UCHAR * pucFrame, USHORT usLen )
{
	int i;
	unsigned short int crc_res =  0xFFFF;
	while(usLen--)
	{
		crc_res^=*pucFrame++;
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
