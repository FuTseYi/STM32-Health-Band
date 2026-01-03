/**
 * @file    OLED.h
 * @brief   OLED显示屏驱动头文件(SSD1306)
 * @details 定义OLED相关宏定义、函数声明
 * @author  Xieyi
 * @date    2025-06-25
 * @version 1.0.0
 */

#ifndef __OLED_H_
#define __OLED_H_  	 
  						  
//-----------------OLED IIC端口定义----------------  					   
#define GPIO_SCL GPIOB
#define GPIO_SDA GPIOB

#define GPIO_Pin_SCL GPIO_Pin_6
#define GPIO_Pin_SDA GPIO_Pin_7

#define OLED_RCC_SCL RCC_APB2Periph_GPIOB
#define OLED_RCC_SDA RCC_APB2Periph_GPIOB

/*  F6x8[][8]    F8X16[][16]     BMP1[]         Hzk[][32]*/
#include "stdlib.h"	  	 
#include "delay.h"
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#define OLED_MODE 0
#define SIZE 8
#define XLevelL		0x00
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF 
#define X_WIDTH 	128
#define Y_WIDTH 	64	  
#define OLED_SCLK_Clr() GPIO_ResetBits(GPIO_SCL,GPIO_Pin_SCL)//SCL
#define OLED_SCLK_Set() GPIO_SetBits(GPIO_SCL,GPIO_Pin_SCL)

#define OLED_SDIN_Clr() GPIO_ResetBits(GPIO_SDA,GPIO_Pin_SDA)//SDA
#define OLED_SDIN_Set() GPIO_SetBits(GPIO_SDA,GPIO_Pin_SDA)
				 
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����

void OLED_printf(u8 x,u8 y,char *fmt,...);
void OLED_ShowStr(u8 x,u8 y, u8 *p,u8 Char_Size); //�ַ���
void OLED_ShowCH(u8 x,u8 y,u8 no);                //��ʾ����
void OLED_ShowBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,unsigned char BMP[]);
void OLED_ShowNum(u8 x,u8 y,int num,u8 len,u8 size);//��ʾ����


//OLED�����ú���
void OLED_WR_Byte(unsigned dat,unsigned cmd);  
void OLED_Display_On(void);
void OLED_Display_Off(void);	   							   		    
void OLED_Init(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size);
void OLED_Set_Pos(unsigned char x, unsigned char y);

void Delay_50ms(unsigned int Del_50ms);
void Delay_1ms(unsigned int Del_1ms);
void fill_picture(unsigned char fill_Data);
void OLED_IIC_Start(void);
void OLED_IIC_Stop(void);
void Write_IIC_Command(unsigned char IIC_Command);
void Write_IIC_Data(unsigned char IIC_Data);
void Write_IIC_Byte(unsigned char IIC_Byte);
void IIC_Wait_Ack(void);


#endif  
	 



