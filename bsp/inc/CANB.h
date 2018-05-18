/*******************************************************
 * file_name  : CANB.h
 * DIR:/28335_APP/bsp/inc/CANB.h
 * Created on : 2017年11月15日-下午8:25:28
 * Author     : admin
 * Version    :
 *
 ******************************************************/

#ifndef BSP_INC_CANB_H_
#define BSP_INC_CANB_H_ 
#include "CANA.h"
typedef struct _tag_mbxCfg
{
	unsigned short int  cnt;
	unsigned short int  prd;
	unsigned char  flag;
	unsigned char  txEnable;
	unsigned short int  txBufFul;
	unsigned char  Enable;
	CanTxMsg can_tx_msg;
}tag_mbxCfg;
extern tag_mbxCfg CANB_tx_msg[12];
void CAN_tx_msg_init(void);
void packcanbMbx0(void);
void packcanbMbx1(void);
void packcanbMbx2(void);
void packcanbMbx3(void);
void packcanbMbx4(void);
void packcanbMbx5(void);
void packcanbMbx6(void);
void packcanbMbx7(void);
void packcanbMbx8(void);
void packcanbMbx9(void);
void packcanbMbx10(void);
void packcanbMbx11(void);
void canbMbxSlotCtrl(void);
void canbtransmit(void);
extern unsigned short int speed_fbk;
extern unsigned short int speed_ref;
extern u8 dir;
#endif /* BSP_INC_CANB_H_ */
