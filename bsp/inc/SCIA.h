/*******************************************************
 * file_name  : SCIA.h
 * DIR:/28335_APP/bsp/inc/SCIA.h
 * Created on : 2018年1月20日-下午9:43:56
 * Author     : admin
 * Version    :
 *
 ******************************************************/

#ifndef BSP_INC_SCIA_H_
#define BSP_INC_SCIA_H_ 
#include "include.h"
#include <stdio.h>
#include <string.h>
void SCIA_GPIO_Init(void);
void SCIA_Init(void);
void SCIA_Send_data(unsigned short int data);
void SCIA_Send_string(unsigned char *str, int leng);
int fputc(int _c, register FILE *_fp);
int fputs(const char *_ptr, register FILE *_fp);
#endif /* BSP_INC_SCIA_H_ */
