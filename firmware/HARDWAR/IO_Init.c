#include "IO_Init.h"
int key_num=0; //按键值




void Beep_Init() //蜂鸣器
{
	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(GPIO_RCC_Beep,ENABLE);//使能PORTA,PORTB时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_Beep;//IO口配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	  //速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;    //设置成上拉输入
 	GPIO_Init(GPIO_Beep, &GPIO_InitStructure);         //初始化GPIOB
	Beep=1;
}

void Key_Init(void) //IO初始化
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	RCC_APB2PeriphClockCmd(GPIO_RCC_K1|GPIO_RCC_K2|GPIO_RCC_K3,ENABLE);//使能PORTA,PORTB时钟
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_K1;//IO口配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //设置成上拉输入
 	GPIO_Init(GPIO_K1, &GPIO_InitStructure);                 //初始化GPIOB4,3
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_K2;//IO口配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //设置成上拉输入
 	GPIO_Init(GPIO_K2, &GPIO_InitStructure);                 //初始化GPIOB4,3
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_K3;//IO口配置
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	     //速度为50MHz
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;          //设置成上拉输入
 	GPIO_Init(GPIO_K3, &GPIO_InitStructure);                 //初始化GPIOB4,3
}

u8 Key_Scan()
{
	static u8 key_up,key_num;		//松手检测
	key_num=0;
	if(((K1==0)||(K2==0)||(K3==0))&&(key_up==1) )
	{
		key_up=0;
		delay_ms(10);		//软件消抖
		if(K1==0)
			key_num = 1;
		else if(K2==0)
			key_num = 2;
		else if(K3==0)
			key_num = 3;
	}
	else if((K1==1)&&(K2==1)&&(K3==1))
	{
		key_up=1;
	}
	return key_num;
}





