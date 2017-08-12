/*
 * timer.h
 *
 *  Created on: 2017Äê8ÔÂ11ÈÕ
 *      Author: admin
 */

#ifndef BSP_INC_TIMER_H_
#define BSP_INC_TIMER_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"
#include "os.h"
void CPU_timer1_init(void);
__interrupt void CPU_timer1_isr(void);



#endif /* BSP_INC_TIMER_H_ */
