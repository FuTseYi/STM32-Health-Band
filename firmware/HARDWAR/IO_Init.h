#ifndef __IO_Iint_H
#define __IO_Iint_H
#include "sys.h"
#include "delay.h"

/***************************
* 蜂鸣器IO宏定义
****************************/
//#define GPIO_RCC_Beep 	RCC_APB2Periph_GPIOB
//#define GPIO_Beep 			GPIOB
//#define GPIO_Pin_Beep 	GPIO_Pin_12
//#define Beep  					PBout(12) //蜂鸣器
#define GPIO_RCC_Beep 	RCC_APB2Periph_GPIOA
#define GPIO_Beep 			GPIOA
#define GPIO_Pin_Beep 	GPIO_Pin_8
#define Beep  					PAout(8) //蜂鸣器
/***************************
* 按键IO宏定义
****************************/
#define GPIO_RCC_K1 RCC_APB2Periph_GPIOB
#define GPIO_RCC_K2 RCC_APB2Periph_GPIOB
#define GPIO_RCC_K3 RCC_APB2Periph_GPIOB
#define GPIO_K1 		GPIOB
#define GPIO_K2 		GPIOB
#define GPIO_K3 		GPIOB
#define GPIO_Pin_K1 GPIO_Pin_13
#define GPIO_Pin_K2 GPIO_Pin_14
#define GPIO_Pin_K3 GPIO_Pin_15
#define K1  				PBin(13) //读取按键
#define K2  				PBin(14) //读取按键
#define K3  				PBin(15) //读取按键

/***************************
* 函数调用
****************************/
void Key_Init(void); //IO初始化
u8 Key_Scan(void); //按键扫描
void Relay_Init(void); //继电器
void Beep_Init(void); //蜂鸣器
void Fan_Init(void);//风扇
void Led_Init(void); //LED灯
#endif









