/*
 * os.h
 *
 *  Created on: 2014-3-25
 *      Author: ThinkPad
 */

#ifndef OS_H_
#define OS_H_
#include "DSP2833x_Device.h"
#include "DSP2833x_Examples.h"
#include "data.h"
#include "delay.h"
#include "stdint.h"

#define TASKNUM                     10

#define MaxUnfinishedNum            10

#define TASK500US		            0
#define TASK1MS		                1
#define TASK1300US	                2
#define TASK2MS                     3
#define TASK5MS		                4
#define TASK10MS		            5
#define TASK20MS		            6
#define TASK50MS		            7
#define TASK100MS		            8
#define TASK1000MS		            9

//  10KHZ
#define TASK500US_CYCLE		        5
#define TASK1MS_CYCLE		        10
#define TASK1300US_CYCLE		    13
#define TASK2MS_CYCLE               20
#define TASK5MS_CYCLE		        50
#define TASK10MS_CYCLE		        100
#define TASK20MS_CYCLE		        200
#define TASK50MS_CYCLE		        500
#define TASK100MS_CYCLE		        1000
#define TASK1000MS_CYCLE		    10000

#define TIME_CONST_100US	        1
#define TIME_CONST_200US	        2
#define TIME_CONST_300US	        3
#define TIME_CONST_400US	        4
#define TIME_CONST_500US	        5
#define TIME_CONST_600US	        6
#define TIME_CONST_700US	        7
#define TIME_CONST_800US	        8
#define TIME_CONST_900US	        9

/*
//8KHZ
#define TASK500US_CYCLE		        4
#define TASK1MS_CYCLE		        8
#define TASK1300US_CYCLE		    11
#define TASK2MS_CYCLE               16
#define TASK5MS_CYCLE		        40
#define TASK10MS_CYCLE		        80
#define TASK20MS_CYCLE		        160
#define TASK50MS_CYCLE		        400
#define TASK100MS_CYCLE		        800
#define TASK1000MS_CYCLE		    8000
*/
/*  6.25KHZ
#define TASK500US_CYCLE		        4      //640
#define TASK1MS_CYCLE		        8      //1280
#define TASK1300US_CYCLE		    11     //1.76
#define TASK2MS_CYCLE               16     //2.56
#define TASK5MS_CYCLE		        40     //6.4
#define TASK10MS_CYCLE		        80     //12.8
#define TASK20MS_CYCLE		        125    //20
#define TASK50MS_CYCLE		        400    //64
#define TASK100MS_CYCLE		        800    //100
#define TASK1000MS_CYCLE		    8000   //1000
*/

#define OS_DISABLE 0
#define OS_ENABLE  1


typedef struct
{
	Uint16 cnt;
	Uint16 flag;
	Uint16 period;
	Uint16 enable;
}tag_tasktimer;

#ifdef _OS_C

tag_tasktimer tasktimer[TASKNUM];
void taskinit(void);
void taskclock(void);
void taskdelayctrl(void);

#else
//===========================================================================
// 外部引用接口声明
//===========================================================================
extern tag_tasktimer tasktimer[TASKNUM];
extern void taskinit(void);
extern void taskclock(void);

#endif	// end of #ifdef _OS_C

#endif /* OS_H_ */
