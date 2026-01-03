/**
 * @file    MAX30102.h
 * @brief   MAX30102心率血氧传感器驱动头文件
 * @details 定义MAX30102相关寄存器、结构体、函数声明
 * @author  Xieyi
 * @date    2025-06-25
 * @version 1.0.0
 */

#ifndef _MAX30102_H
#define _MAX30102_H
#include "stm32f10x.h"                  // Device header
#include "stdbool.h"
#include "delay.h"
#include "stm32f10x.h"                  // Device header
#include "MAX30102.h"
#include "math.h"
#define MAX30102_Device_address 			0xAE
//register addresses
#define REG_INTR_STATUS_1 	0x00
#define REG_INTR_STATUS_2 	0x01
#define REG_INTR_ENABLE_1 	0x02
#define REG_INTR_ENABLE_2 	0x03
#define REG_FIFO_WR_PTR 		0x04
#define REG_OVF_COUNTER 		0x05
#define REG_FIFO_RD_PTR 		0x06
#define REG_FIFO_DATA 			0x07
#define REG_FIFO_CONFIG 		0x08
#define REG_MODE_CONFIG 		0x09
#define REG_SPO2_CONFIG 		0x0A
#define REG_LED1_PA 				0x0C
#define REG_LED2_PA 				0x0D
#define REG_PILOT_PA 				0x10
#define REG_MULTI_LED_CTRL1 0x11
#define REG_MULTI_LED_CTRL2 0x12
#define REG_TEMP_INTR 			0x1F
#define REG_TEMP_FRAC 			0x20
#define REG_TEMP_CONFIG 		0x21
#define REG_PROX_INT_THRESH 0x30
#define REG_REV_ID 					0xFE
#define REG_PART_ID 				0xFF
#define SAMPLES_PER_SECOND 							100.0	//���Ƶ��

#define max30102_iic_SCL_CLK 						RCC_APB2Periph_GPIOB
#define max30102_iic_SCL_PORT 						GPIOB
#define max30102_iic_SCL_GPIO_Pin 				GPIO_Pin_8
#define max30102_iic_SDA_CLK 						RCC_APB2Periph_GPIOB
#define max30102_iic_SDA_PORT 						GPIOB
#define max30102_iic_SDA_GPIO_Pin 				GPIO_Pin_9

#define max30102_iic_SCL_1 			PBout(8) = 1
#define max30102_iic_SCL_0 			PBout(8) = 0
#define max30102_iic_SDA_1 			PBout(9) = 1
#define max30102_iic_SDA_0 			PBout(9) = 0
#define	max30102_iic_SDA_READ		GPIO_ReadInputDataBit(max30102_iic_SDA_PORT,max30102_iic_SDA_GPIO_Pin)


extern u32 fifo_red;
extern u32 fifo_ir;

extern int temp;
extern u8 max30102_flag;
/**
 * @brief  血液数据结构体定义
 * @author Xieyi
 */
typedef struct
{
	int 		heart;		//心率数值
	float 	SpO2;			//血氧饱和度
}BloodData;

extern float max30102_data[2],fir_output[2];			//ԭʼ���� fir�˲��������
extern BloodData g_blooddata;					//ѪҺ���ݴ洢

void blood_dataget(void);
void MAX30102_GPIO(void);
uint8_t Max30102_reset(void);
void MAX30102_Config(void);
void max30102_read_fifo(float *output_data);
int max30102_read_temp(void);//оƬ�¶ȹ��ܼ��
void max30102_init(void);	//��ʼ��




void blood_Loop(void);
#endif


