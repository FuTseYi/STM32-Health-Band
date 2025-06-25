/**
 * @file    main.c
 * @brief   Xieyi智能手环主程序
 * @details 基于STM32F103C8T6的智能手环项目，集成心率检测、血氧检测、
 *          体温检测、计步、跌倒检测等功能
 * @author  Xieyi
 * @date    2025-06-25
 * @version 1.0.0
 * @note    本项目采用MIT开源许可证
 */

#include "stm32f10x.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "MAX30102.h"
#include "oled.h"
#include "timer.h"
#include "IO_Init.h"
#include "adxl345.h"

u8 mode;
u8 history[3][128]; //����,Ѫ�����¶���ʷ����
BloodData g_blooddata = {0};		//ѪҺ���ݴ洢

/**
 * @brief  OLED显示数据函数
 * @details 显示心率、血氧、温度等健康数据，跌倒时显示重启提示
 * @author  Xieyi
 */
void OLED_show_data()
{
	if(fall == 0)
	{
		OLED_printf(0,2,"Heart Spo2  Temp");  //������ʾ
		OLED_printf(0,4,"%03d   %02.1f  %02.1f ",g_blooddata.heart,g_blooddata.SpO2,temp/10.0f); 
		OLED_printf(0,6,"Step:%d   ",Steps);
	}
	else
	{
		OLED_Clear();
		OLED_ShowStr(0,0,(u8 *)"RESET SYS",16);
	}
}
/**
 * @brief  报警处理函数
 * @details 跌倒检测时控制蜂鸣器报警
 * @author  Xieyi
 */
void alarm_deal()  //��������
{
		if(fall == 1) //����
		{
			Beep =!Beep;
		}
		else
		{
			Beep = 1;
		}
}

/**
 * @brief  主函数 - Xieyi智能手环核心控制程序
 * @details 系统初始化、外设配置、主循环控制
 *          实现心率血氧检测、计步、跌倒检测、WiFi数据传输等功能
 * @author  Xieyi
 * @date    2025-06-25
 * @retval  int 程序永不返回
 */
int main(void)
{
	u8 str[40],i;
	static u8 send_flag;
	SystemInit();   //ϵͳʱ�ӳ�ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //   2���жϷ���
  delay_init();
	Beep_Init();
	Beep = 0;
	usart1_Init(115200);
	OLED_Init();
	Beep = 1;
	Esp8266_AP_Init(); 			//wifi
	max30102_init();
	Init_ADXL345();
	TIM3_Int_Init(9999,719); //100ms
////	Key_Init();
	OLED_show_data();
  while(1)
	{
		blood_Loop();
		if(mode==0)
		{
			if(max30102_flag == 1)  //���ݲɼ����,��������
			{
				max30102_flag = 0;
				OLED_show_data();
			}
			if(time_100ms>1)  //200msһ��
			{
				time_100ms = 0;
				send_flag ++;
				if(send_flag==10)
				{
					i=sprintf((char *)str,"%dR/mW%.1f%%W%.1fCW%dW%dW/r/n",g_blooddata.heart,g_blooddata.SpO2,temp/10.0f,Steps,fall);
					UsartPrintf(ESP8266_USART,"AT+CIPSEND=0,%d\r\n",i);
				}
				if(send_flag==15)
				{
					send_flag = 0;
					UsartPrintf(ESP8266_USART,"%s",str);
				}
				alarm_deal();
			}
		}
		else
		{
			Beep = 1;
		}
	}
}
