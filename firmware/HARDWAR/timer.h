#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);
void TIM2_Int_Init(u16 arr,u16 psc);
extern u32 time_100ms;

extern u8 fall;							//跌倒
extern u32 Steps;						//步数
extern u32 Normal_num;			//正常次数
extern u32 Error_num;				//倾斜次数
#endif
