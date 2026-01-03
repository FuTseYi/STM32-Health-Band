/**
 * @file    MAX30102.c
 * @brief   MAX30102心率血氧传感器驱动程序
 * @details 实现MAX30102传感器的初始化、数据读取、心率血氧算法处理
 * @author  Xieyi
 * @date    2025-06-25
 * @version 1.0.0
 * @note    支持心率检测、血氧饱和度检测、体温检测功能
 */

#include "MAX30102.h"
#include "sys.h"
#include "usart.h"

#define MAX30102_INT_PIN               GPIO_Pin_5
#define MAX30102_INT_GPIO_PORT         GPIOB
#define MAX30102_INT_EXTI_IRQn         EXTI9_5_IRQn
#define MAX30102_INT_GPIO_CLK          (RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO)

#define MAX30102_INT_EXTI_PORTSOURCE   GPIO_PortSourceGPIOB
#define MAX30102_INT_EXTI_PINSOURCE    GPIO_PinSource5
#define MAX30102_INT_EXTI_LINE         EXTI_Line5
#define MAX30102_INT_IRQHandler        EXTI9_5_IRQHandler
 /**
  * @brief  配置嵌套向量中断控制器NVIC
  * @details 配置MAX30102中断优先级
  * @param  无
  * @retval 无
  * @author Xieyi
  */
static void NVIC_Configuration(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
  /* ����NVICΪ���ȼ���1 */
	/* ��ʾ NVIC_PriorityGroupConfig() ����������ֻ��Ҫ����һ�����������ȼ�����*/
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
  
  /* �����ж�Դ������1 */
  NVIC_InitStructure.NVIC_IRQChannel = MAX30102_INT_EXTI_IRQn;
  /* ������ռ���ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  /* ���������ȼ� */
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  /* ʹ���ж�ͨ�� */
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}

 /**
  * @brief  配置 IO为EXTI中断口，设置中断优先级
  * @details 初始化MAX30102中断引脚GPIO配置
  * @param  无
  * @retval 无
  * @author Xieyi
  */
void max30102_int_gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	EXTI_InitTypeDef EXTI_InitStructure;

	/*��������GPIO�ڵ�ʱ��*/
	RCC_APB2PeriphClockCmd( MAX30102_INT_GPIO_CLK, ENABLE);
												
	/* ���� NVIC �ж�*/
	NVIC_Configuration();
	
/*--------------------------IO����-----------------------------*/
	/* ѡ�񰴼��õ���GPIO */	
  GPIO_InitStructure.GPIO_Pin = MAX30102_INT_PIN;
  /* ����Ϊ�������� */	
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
  GPIO_Init(MAX30102_INT_GPIO_PORT, &GPIO_InitStructure);

	/* ѡ��EXTI���ź�Դ */
  GPIO_EXTILineConfig(MAX30102_INT_EXTI_PORTSOURCE, MAX30102_INT_EXTI_PINSOURCE); 
  EXTI_InitStructure.EXTI_Line = MAX30102_INT_EXTI_LINE;
	
	/* EXTIΪ�ж�ģʽ */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	/* �������ж� */
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  /* ʹ���ж� */	
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
	

}
/******************************************************
 if(EXTI_GetITStatus(EXTI_Line8) != RESET)  //�ж��ж���
EXTI0_IRQHandler
EXTI9_5_IRQHandler
EXTI15_10_IRQHandler
		EXTI_ClearITPendingBit(EXTI_Line8);      //����жϱ�־λ
******************************************************/
//void MAX30102_INT_IRQHandler(void)   //�ⲿ�жϳ���
//{
//	if(EXTI_GetITStatus(MAX30102_INT_EXTI_LINE) != RESET)  //�ж��ж���
//	{
//		blood_dataget();
//		
//		EXTI_ClearITPendingBit(MAX30102_INT_EXTI_LINE);      //����жϱ�־λ
//	}
//}
void MAX30102_INT_IRQHandler(void)
{
  //ȷ���Ƿ������EXTI Line�ж�
	if(EXTI_GetITStatus(MAX30102_INT_EXTI_LINE) != RESET) 
	{		
		blood_dataget();
		//����жϱ�־λ
		EXTI_ClearITPendingBit(MAX30102_INT_EXTI_LINE);     
		
    
	}  
}

u32 fifo_red;
u32 fifo_ir;
u8 max30102_flag;
int temp;

void max30102_iic_GPIO_INIT(void);
void max30102_iic_Start(void);
void max30102_iic_Stop(void);
void max30102_iic_Send_Byte(uint8_t byte);
void max30102_iic__Ack(uint8_t a);
uint8_t max30102_iic_Receive_Byte(void);

uint8_t max30102_iic_Read_Byte(uint8_t device_addr,uint8_t register_addr);
uint8_t max30102_iic_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data);

uint8_t max30102_iic_Read_Array(uint8_t device_addr,u16 register_addr,uint8_t *Data,u16 Num);
uint8_t max30102_iic_Write_Array(uint8_t device_addr,u16 register_addr,u8 *Data,u16 Num);

uint8_t ack;

void max30102_iic_GPIO_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC_APB2PeriphClockCmd(max30102_iic_SCL_CLK | max30102_iic_SDA_CLK,ENABLE);	
	GPIO_InitStruct.GPIO_Mode		=	GPIO_Mode_Out_OD ;
	GPIO_InitStruct.GPIO_Pin		=	max30102_iic_SCL_GPIO_Pin;
	GPIO_InitStruct.GPIO_Speed	=	GPIO_Speed_50MHz;
	GPIO_Init(max30102_iic_SCL_PORT,&GPIO_InitStruct);
	
	GPIO_InitStruct.GPIO_Pin		=	max30102_iic_SDA_GPIO_Pin;
	GPIO_Init(max30102_iic_SDA_PORT,&GPIO_InitStruct);
}



void max30102_iic_Delay(void)
{
//	uint8_t i=2; //i=10��ʱ1.5us//��������Ż��ٶ� ����������͵�5����д��
//   while(i--);
	delay_us(1);
}


void max30102_iic_Start(void)
{
	max30102_iic_SCL_1;
	max30102_iic_SDA_1;//��ʼ�źŽ���ʱ�� 0.6us 400KHz
	max30102_iic_Delay();
	max30102_iic_SDA_0;
	max30102_iic_Delay();//��ʼ�źű���ʱ��0.6us
	max30102_iic_SCL_0;
	max30102_iic_Delay();//ʱ�ӵ͵�ƽʱ��1.3us
}
 
void max30102_iic_Stop(void)
{	
	max30102_iic_SDA_0;
	max30102_iic_SCL_1;
	max30102_iic_Delay();//�����źŽ���ʱ��0.6us
	max30102_iic_SDA_1;
	max30102_iic_Delay();//���߿���ʱ��ʱ��1.3us
}

void max30102_iic_Send_Byte(uint8_t byte)
{
	uint8_t i;//�ȷ��͸�λ
	for(i=0;i<8;i++)
	{
		if(byte & 0x80)
		{
			max30102_iic_SDA_1;
		}
		else
		{
			max30102_iic_SDA_0;
		}
		max30102_iic_Delay();
		max30102_iic_SCL_1;
		max30102_iic_Delay();
		max30102_iic_SCL_0;
		max30102_iic_Delay();
		byte<<=1;
	}
	max30102_iic_SDA_1;  
	max30102_iic_Delay();
	max30102_iic_SCL_1;
	max30102_iic_Delay();
	if(max30102_iic_SDA_READ)
	{
		ack=1;
	}
	else
	{
		ack=0;
	}
	max30102_iic_SCL_0;
	max30102_iic_Delay();
}

uint8_t max30102_iic_Receive_Byte(void)
{
   uint8_t receive=0;
   uint8_t i;//��������Ϊ���뷽ʽ
   for(i=0;i<8;i++)
   {     		
			receive<<=1;
			max30102_iic_SCL_1;//��ʱ����Ϊ��ʹ��������������Ч
			max30102_iic_Delay();
			if(max30102_iic_SDA_READ)
			{
				receive++;//������λ,���յ�����λ����retc��
			}
			max30102_iic_SCL_0;  
			max30102_iic_Delay();
   }
   return receive;
}

uint8_t max30102_iic_Write_Byte(uint8_t device_addr,uint8_t register_addr,uint8_t data)
{
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	max30102_iic_Send_Byte(register_addr);   
	if (ack == 1)return 0;
	max30102_iic_Send_Byte(data);
	if (ack == 1)return 0; 
	max30102_iic_Stop();
	return 1;
}

void max30102_iic__Ack(uint8_t a)
{ 
	if(a)
	{
		max30102_iic_SDA_1;            //��Ӧ��
		max30102_iic_Delay(); 
		max30102_iic_SCL_1;  	
		max30102_iic_Delay();  
		max30102_iic_SCL_0; 	
		max30102_iic_Delay();  		
	}
	else
	{
		max30102_iic_SDA_0; 						//Ӧ��
		max30102_iic_Delay(); 
		max30102_iic_SCL_1;  
		max30102_iic_Delay(); 
		max30102_iic_SCL_0; 
		max30102_iic_Delay(); 
		max30102_iic_SDA_1; 
	}
}

uint8_t max30102_iic_Read_Byte(uint8_t device_addr,uint8_t register_addr)
{
	uint8_t read_data;
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	max30102_iic_Send_Byte(register_addr);
	if (ack == 1)return 0;
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+1);
	if (ack == 1)return 0;
	read_data = max30102_iic_Receive_Byte();
	max30102_iic__Ack(1);
	max30102_iic_Stop();
	return read_data;
}

uint8_t max30102_iic_Write_Array(uint8_t device_addr,uint16_t register_addr,u8 *Data,uint16_t Num)
{
	uint16_t i;
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	max30102_iic_Send_Byte(register_addr);
	if (ack == 1)return 0;
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+1);
	if (ack == 1)return 0;
	for(i=0;i<Num;i++)
	{
		max30102_iic_Send_Byte(*Data++);
		if (ack == 1)return 0;
	}
	max30102_iic_Stop();
	return 1;
}

uint8_t max30102_iic_Read_Array(uint8_t device_addr,uint16_t register_addr,uint8_t *Data,uint16_t Num)
{
	uint16_t i;
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+0);
	if (ack == 1)return 0;
	max30102_iic_Send_Byte(register_addr);
	if (ack == 1)return 0;
	max30102_iic_Start();
	max30102_iic_Send_Byte(device_addr+1);
	if (ack == 1)return 0;
	for(i=0;i<Num;i++)
	{
		*Data++ = max30102_iic_Receive_Byte();
		if(i==Num-1)
			max30102_iic__Ack(1);
		else
			max30102_iic__Ack(0);
	}
	max30102_iic_Stop();
	return 1;
}

void max30102_init(void)	//��ʼ��
{
	max30102_iic_GPIO_INIT();
	max30102_int_gpio_init();
	Max30102_reset();
	MAX30102_Config();
}


uint8_t Max30102_reset(void)
{
	if(max30102_iic_Write_Byte(MAX30102_Device_address,REG_MODE_CONFIG, 0x40))
        return 1;
    else
        return 0;    
}
int max30102_read_temp()//оƬ�¶ȹ��ܼ��
{
	static int temp1,temp2;
//	tt= max30102_iic_Read_Byte(MAX30102_Device_address,REG_TEMP_INTR); 
	temp1=max30102_iic_Read_Byte(MAX30102_Device_address,REG_TEMP_INTR);       // �����¶��ź�
	temp2=max30102_iic_Read_Byte(MAX30102_Device_address,REG_TEMP_FRAC);       // �����¶�С����������
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_TEMP_CONFIG,0x01);       // 0X21��ַB0λTEMP_EN��1
	return temp1*10+((temp2)*0.625);     // �����¶ȣ�С��������Сֵ0.625;
}
void MAX30102_Config(void)
{
	delay_ms(5);
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_INTR_ENABLE_1,0xE0);//
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_INTR_ENABLE_2,0x00);//interrupt enable: FIFO almost full flag, new FIFO Data Ready,
																						 	//                   ambient light cancellation overflow, power ready flag, 
																							//						    		internal temperature ready flag
	
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_FIFO_WR_PTR,0x00);//FIFO_WR_PTR[4:0]
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_OVF_COUNTER,0x00);//OVF_COUNTER[4:0]
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_FIFO_RD_PTR,0x00);//FIFO_RD_PTR[4:0]
	
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_FIFO_CONFIG,0x4f);//FIFO configuration: sample averaging(4),FIFO rolls on full(0), FIFO almost full value(15 empty data samples when interrupt is issued)  
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_MODE_CONFIG,0x03);//0x02 for Red only, 0x03 for SpO2 mode 0x07 multimode LED
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_SPO2_CONFIG,0x2A);//SpO2 configuration:ACD resolution:15.63pA,sample rate control:200Hz, LED pulse width:215 us 
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_LED1_PA,0x2F);//Choose value for ~ 10mA for LED1
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_LED2_PA,0x2F);// Choose value for ~ 10mA for LED2
	//max30102_iic_Write_Byte(MAX30102_Device_address,REG_PILOT_PA,0x25);// Choose value for ~ 25mA for Pilot LED
	max30102_iic_Write_Byte(MAX30102_Device_address,REG_TEMP_CONFIG,0x01);       // 0X21��ַB0λTEMP_EN��1
	max30102_iic_Read_Byte(MAX30102_Device_address,REG_INTR_STATUS_1);
	max30102_iic_Read_Byte(MAX30102_Device_address,REG_INTR_STATUS_2);
}
void max30102_read_fifo(float *output_data)
{
    uint8_t receive_data[6];
		uint32_t data[2];
		max30102_iic_Read_Array(MAX30102_Device_address,REG_FIFO_DATA,receive_data,6);
    data[0] = ((receive_data[0]<<16 | receive_data[1]<<8 | receive_data[2]) & 0x03ffff);
    data[1] = ((receive_data[3]<<16 | receive_data[4]<<8 | receive_data[5]) & 0x03ffff);
		*output_data = data[0];
		*(output_data+1) = data[1];
}

#define BLOCK_SIZE         	1     				//����һ�δ����Ĳ��������
#define NUM_TAPS           	29     				//�˲���ϵ������
#define CACHE_NUMS 					150						//������
#define PPG_DATA_THRESHOLD 	50000 				//�����ֵ
float ppg_data_cache_ir[CACHE_NUMS]={0};  //������
float ppg_data_cache_red[CACHE_NUMS]={0};  //������
float max30102_data[2],fir_output[2];			//ԭʼ���� fir�˲��������
static float firStateF32_ir[BLOCK_SIZE + NUM_TAPS - 1];        /* ״̬���棬��СnumTaps + blockSize - 1*/
static float firStateF32_red[BLOCK_SIZE + NUM_TAPS - 1];        /* ״̬���棬��СnumTaps + blockSize - 1*/
int cache_counter=0;
//��ͨ�˲���ϵ�� ͨ��fadtool��ȡ
const float firCoeffs32LP[NUM_TAPS] = {
  -0.001542701735,-0.002211477375,-0.003286228748, -0.00442651147,-0.004758632276,
  -0.003007677384, 0.002192312852,  0.01188309677,  0.02637642808,  0.04498152807,
    0.06596207619,   0.0867607221,   0.1044560149,   0.1163498312,   0.1205424443,
     0.1163498312,   0.1044560149,   0.0867607221,  0.06596207619,  0.04498152807,
    0.02637642808,  0.01188309677, 0.002192312852,-0.003007677384,-0.004758632276,
   -0.00442651147,-0.003286228748,-0.002211477375,-0.001542701735
};
void ir_max30102_fir(float *input,float *output,int N)
{
	int i = 0,j = 0,k=0;
	float temp; 
	for (k = 0; k < N; k++) 
	{
		firStateF32_ir[0] = input[k];
		for (i = 0, temp = 0; i < NUM_TAPS; i++)
			temp += firCoeffs32LP[i] * firStateF32_ir[i];
		output[k] = temp;
		for (j = NUM_TAPS - 1; j > -1 ; j--)
			firStateF32_ir[j+1] = firStateF32_ir[j];
	}
}
void red_max30102_fir(float *input,float *output,int N)
{
	int i = 0,j = 0,k=0;
	float temp; 
	for (k = 0; k < N; k++) 
	{
		firStateF32_ir[0] = input[k];
		for (i = 0, temp = 0; i < NUM_TAPS; i++)
			temp += firCoeffs32LP[i] * firStateF32_red[i];
		output[k] = temp;
		for (j = NUM_TAPS - 1; j > -1 ; j--)
			firStateF32_red[j+1] = firStateF32_red[j];
	}
}
uint16_t max30102_getHeartRate(float *input_data,uint16_t cache_nums)
{
	float input_data_sum_aver = 0;
	uint16_t i,temp;
	for(i=0;i<cache_nums;i++)
	{
		input_data_sum_aver += *(input_data+i);
	}
	input_data_sum_aver = input_data_sum_aver/cache_nums;
	for(i=0;i<cache_nums;i++)
	{
		if((*(input_data+i)>input_data_sum_aver)&&(*(input_data+i+1)<input_data_sum_aver))
		{
			temp = i;
			break;
		}
	}
	i++;
	for(;i<cache_nums;i++)
	{
		if((*(input_data+i)>input_data_sum_aver)&&(*(input_data+i+1)<input_data_sum_aver))
		{
			temp = i - temp;
			break;
		}
	}
	if((temp>20)&&(temp<100))
	{
		//SpO2����Ƶ��200Hz��FIFO����ƽ������ֵΪ4���������ղ���Ƶ����Ϊ50Hz
		return 3000/temp;//����һ�ε�������0.02s��temp��һ�����������еĲ����������һ����������temp/50��,һ��������������60/(temp/50)
	}
	else
	{
		return 0;
	}
//	if((temp>40)&&(temp<120))
//		return 60*100/temp;
//	else
//		return 0;
}


float max30102_getSpO2(float *ir_input_data,float *red_input_data,uint16_t cache_nums)
{
			float ir_max=*ir_input_data,ir_min=*ir_input_data;
			float red_max=*red_input_data,red_min=*red_input_data;
			float R;
			uint16_t i;
			for(i=1;i<cache_nums;i++)
			{
				if(ir_max<*(ir_input_data+i))
				{
					ir_max=*(ir_input_data+i);
				}
				if(ir_min>*(ir_input_data+i))
				{
					ir_min=*(ir_input_data+i);
				}
				if(red_max<*(red_input_data+i))
				{
					red_max=*(red_input_data+i);
				}
				if(red_min>*(red_input_data+i))
				{
					red_min=*(red_input_data+i);
				}
			}
//			printf("%.0f,%.0f,%.0f,%.0f\r\n",(ir_max-ir_min),red_min,(red_max-red_min),ir_min);
			R=((ir_max-ir_min)*red_min)/((red_max-red_min)*ir_min);
//			 R=((ir_max+ir_min)*(red_max-red_min))/((red_max+red_min)*(ir_max-ir_min));
			return ((-45.060)*R*R + 30.354*R + 94.845);
}
int nofinger_cnt=0,Flag;




#define DATA_CNT 6

void blood_dataget(void)
{
	max30102_read_fifo(max30102_data);  //ԭʼ���ݻ�ȡ
	if((max30102_data[0]>PPG_DATA_THRESHOLD)&&(max30102_data[1]>PPG_DATA_THRESHOLD))   //������ֵ��˵���������нӴ�
	{
		ir_max30102_fir(&max30102_data[0],&fir_output[0],1); //Fir�˲�
//		red_max30102_fir(&max30102_data[1],&fir_output[1],1); //�˲���Ѫ�����ݲ���
		ppg_data_cache_ir[cache_counter]=fir_output[0];   //Fir�˲�����ת�ƣ�δ�������ݼ�����׼��
		ppg_data_cache_red[cache_counter]=max30102_data[1];
		cache_counter++;
		nofinger_cnt=0;
	}
	else				//С����ֵ ˵��û����ָ
	{
		cache_counter=0; //��������
		if(nofinger_cnt++>CACHE_NUMS) //����150�� �˳���ѭ��
		{
			nofinger_cnt= 0;
			max30102_flag = 1;
			g_blooddata.heart=0;
			g_blooddata.SpO2=0;
		}
		return;
	}
}

void blood_Loop(void)
{
	static u8 cnt=0,heart[DATA_CNT]={0},heart2[DATA_CNT]={0},hr_data;
	static float spo2[DATA_CNT]={0},spo22[DATA_CNT]={0},spo2_data;
	u8 i,j;
	
	if(cache_counter>=CACHE_NUMS)  //�ռ���������
	{
//		printf("1\r\n");
		max30102_flag = 1;
		temp = max30102_read_temp();
		hr_data = max30102_getHeartRate(ppg_data_cache_ir,CACHE_NUMS); 										//���ʼ���
		spo2_data = max30102_getSpO2(ppg_data_cache_ir,ppg_data_cache_red,CACHE_NUMS);  	//Ѫ������
		if(spo2_data>99.9)spo2_data =  99.9; 
		
		
//		printf("%d\t%d,%d,%d,%d,%d,%d\r\n",hr_data,heart2[0],heart2[1],heart2[2],heart2[3],heart2[4],heart2[5]);
//		printf("%.1f\t%.1f,%.1f,%.1f,%.1f,%.1f,%.1f\r\n",spo2_data,spo22[0],spo22[1],spo22[2],spo22[3],spo22[4],spo22[5]);
		if((hr_data>0)&&(spo2_data<101)&&(spo2_data>70))  //������Ч
		{
			heart2[cnt] = hr_data;
			spo22[cnt] = spo2_data;
			cnt++;
			if(cnt<DATA_CNT)
			{
				g_blooddata.heart = hr_data;
				g_blooddata.SpO2 = spo2_data;
			}
		}
		cache_counter=0;
		
		if(cnt >= DATA_CNT)   //�ɼ���������
		{
			cnt = DATA_CNT-1;
			for(i=0;i<DATA_CNT;i++)  //����ת��
			{
				heart[i] = heart2[i];
				spo2[i] = spo22[i];
			}
			for(i=0;i<DATA_CNT-1;i++)  //����ǰ�ƣ��ڳ���λ����
			{
				heart2[i] = heart2[i+1];
				spo22[i] = spo22[i+1];
			}
			
			//����
			for (i = 0; i < DATA_CNT -1 ; i++) {  
        for (j = 0; j < DATA_CNT - 1 - i; j++) {
            if (heart[j] > heart[j+1]) {        // ����Ԫ�������Ա�
                hr_data = heart[j+1];        // Ԫ�ؽ���
                heart[j+1] = heart[j];
                heart[j] = hr_data;
            }
						if (spo2[j] > spo2[j+1]) {        // ����Ԫ�������Ա�
                spo2_data = spo2[j+1];        // Ԫ�ؽ���
                spo2[j+1] = spo2[j];
                spo2[j] = spo2_data;
            }
        }
			}
			g_blooddata.heart = (heart[2]+heart[3])/2;
			g_blooddata.SpO2 =	(spo2[2]+spo2[3])/2;
		}

	}
}

