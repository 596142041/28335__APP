#ifndef _OS_C
#define _OS_C
#include "os.h"
// 配置下列函数在RAM运行
#pragma CODE_SECTION(taskclock, 		"ramfuncs");

void taskinit(void)
{
	int i;
	for(i=0;i<TASKNUM;i++)
	{
		tasktimer[i].cnt = 0;
		tasktimer[i].flag = 0;
		tasktimer[i].enable = OS_DISABLE;
	}
	tasktimer[0].period=TASK500US_CYCLE;
	tasktimer[1].period=TASK1MS_CYCLE;
	tasktimer[2].period=TASK1300US_CYCLE;
	tasktimer[3].period=TASK2MS_CYCLE;
	tasktimer[4].period=TASK5MS_CYCLE;
	tasktimer[5].period=TASK10MS_CYCLE;
	tasktimer[6].period=TASK20MS_CYCLE;
	tasktimer[7].period=TASK50MS_CYCLE;
	tasktimer[8].period=TASK100MS_CYCLE;
	tasktimer[9].period=TASK1000MS_CYCLE;

	tasktimer[0].enable=OS_ENABLE;
}
void taskclock(void)
{
	int i;
	for(i=0;i<TASKNUM;i++)
	{

		if(tasktimer[i].enable==OS_ENABLE)
		{
			tasktimer[i].cnt++;
		}else{
			taskdelayctrl();
		}

		if(tasktimer[i].cnt == tasktimer[i].period)
		{
			tasktimer[i].cnt = 0;
			tasktimer[i].flag++;
			if(tasktimer[i].flag>MaxUnfinishedNum)
			{
				tasktimer[i].flag=0;
				//error
			}
		}
	}
}

void taskdelayctrl(void)
{
	if((tasktimer[0].cnt == TIME_CONST_200US)&&(tasktimer[1].enable == OS_DISABLE))//500US的定时延迟100us执行
	{
		tasktimer[1].enable=OS_ENABLE;
	}

	if((tasktimer[0].cnt == TIME_CONST_300US)&&(tasktimer[2].enable == OS_DISABLE))
	{
		tasktimer[2].enable=OS_ENABLE;
	}

	if((tasktimer[0].cnt == TIME_CONST_400US)&&(tasktimer[3].enable == OS_DISABLE))
	{
		tasktimer[3].enable=OS_ENABLE;
	}

	if((tasktimer[1].cnt == TIME_CONST_400US)&&(tasktimer[4].enable == OS_DISABLE))
	{
		tasktimer[4].enable=OS_ENABLE;
	}

	if((tasktimer[1].cnt == TIME_CONST_500US)&&(tasktimer[5].enable == OS_DISABLE))
	{
		tasktimer[5].enable=OS_ENABLE;
	}

	if((tasktimer[1].cnt == TIME_CONST_600US)&&(tasktimer[6].enable == OS_DISABLE))
	{
		tasktimer[6].enable=OS_ENABLE;
	}

	if((tasktimer[1].cnt == TIME_CONST_700US)&&(tasktimer[7].enable == OS_DISABLE))
	{
		tasktimer[7].enable=OS_ENABLE;
	}

	if((tasktimer[1].cnt == TIME_CONST_800US)&&(tasktimer[8].enable == OS_DISABLE))
	{
		tasktimer[8].enable=OS_ENABLE;
	}

	if((tasktimer[1].cnt == TIME_CONST_900US)&&(tasktimer[9].enable == OS_DISABLE))
	{
		tasktimer[9].enable=OS_ENABLE;
	}
}

#endif


