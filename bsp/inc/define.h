/*
 * define.h
 *
 *  Created on: 2015年9月4日
 *      Author: admin
 */

#ifndef USER_INC_DEFINE_H_
#define USER_INC_DEFINE_H_
typedef enum
{
	ERROR = 0,

	///TRUE = 1
}ERROR_FLAG;
typedef struct
{
	int quot;//表示商
	int rem;//表示余数
}DIV_t;
typedef enum
{
  XINT_Trigger_Falling = 0x00,
  XINT_Trigger_Rising = 0x01,
  XINT_Trigger_Rising_Falling = 0x03
}XINTTrigger_TypeDef;
typedef enum
{
	DISABLE = 0,
	ENABLE = !DISABLE
} FunctionalState;
typedef enum
{
	SCI_Rx_IT = 1,
	SCI_Tx_IT = 1<<1,
}SCI_IT;
#endif /* USER_INC_DEFINE_H_ */
