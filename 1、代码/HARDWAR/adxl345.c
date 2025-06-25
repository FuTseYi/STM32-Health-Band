/**
 * @file    adxl345.c
 * @brief   ADXL345三轴加速度传感器驱动程序
 * @details 实现ADXL345传感器的初始化、数据读取、计步算法、跌倒检测功能
 * @author  Xieyi
 * @date    2025-06-25
 * @version 1.0.0
 * @note    支持IIC通信、计步器、跌倒检测功能
 */

#include "stm32f10x.h"
#include "delay.h"
#include "adxl345.h"

#define SCL_RCC			RCC_APB2Periph_GPIOA
#define SCL_GPIO		GPIOA
#define SCL_GPIO_PIN	GPIO_Pin_4

#define SDA_RCC			RCC_APB2Periph_GPIOA
#define SDA_GPIO		GPIOA
#define SDA_GPIO_PIN	GPIO_Pin_5


#define SCL_OUT() SCL_Set_Output() //��λscl
#define SET_SCL() GPIO_SetBits(SCL_GPIO, SCL_GPIO_PIN) //��λscl
#define CLE_SCL() GPIO_ResetBits(SCL_GPIO, SCL_GPIO_PIN)//���scl
                    
#define SDA_OUT() SDA_Set_Output()
#define SDA_INT() SDA_Set_Input()
#define SET_SDA() GPIO_SetBits(SDA_GPIO, SDA_GPIO_PIN)//��λsda
#define CLE_SDA() GPIO_ResetBits(SDA_GPIO, SDA_GPIO_PIN)//���sda
#define SDA_VAL() GPIO_ReadInputDataBit(SDA_GPIO, SDA_GPIO_PIN)

#define	SlaveAddress   0xA6	  //����������IIC�����еĴӵ�ַ,����ALT  ADDRESS��ַ���Ų�ͬ�޸�                  //ALT  ADDRESS���Žӵ�ʱ��ַΪ0xA6���ӵ�Դʱ��ַΪ0x3A

unsigned char BUF[8];                         //�������ݻ�����      	
unsigned char ge,shi,bai,qian,wan;           //��ʾ����
unsigned char err;
float temp_X,temp_Y,temp_Z;

void SCL_Set_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SCL_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(SCL_GPIO, &GPIO_InitStructure);					 					
}	

void SDA_Set_Output(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(SDA_GPIO, &GPIO_InitStructure);					 					
}	

void SDA_Set_Input(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = SDA_GPIO_PIN;				 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 		 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
	GPIO_Init(SDA_GPIO, &GPIO_InitStructure);					 
}

/**************************************
��ʼ�ź�
**************************************/
void ADXL345_Start(void)
{
    SCL_OUT();
    SDA_OUT();
    SET_SDA();//SDA = 1;                    //����������
    SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();                 //��ʱ
    CLE_SDA();//SDA = 0;                    //�����½���
    delay_us(2);//Delay5us();                 //��ʱ
    CLE_SCL();//SCL = 0;                    //����ʱ����
}

/**************************************
ֹͣ�ź�
**************************************/
void ADXL345_Stop(void)
{
    SCL_OUT();
    SDA_OUT();
    CLE_SDA();//SDA = 0;                    //����������
    SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();                 //��ʱ
    SET_SDA();//SDA = 1;                    //����������
    delay_us(2);//Delay5us();                 //��ʱ
    CLE_SCL();
}

/**************************************
����Ӧ���ź�
��ڲ���:ack (0:ACK 1:NAK)
**************************************/
void ADXL345_SendACK(uchar ack)
{   
    SCL_OUT();
    SDA_OUT();
    if(ack==0)//SDA = ack;                  //дӦ���ź�
    {
      CLE_SDA();
    }
    else
    {
      SET_SDA();
    }
    SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//Delay5us();                 //��ʱ
    CLE_SCL();//SCL = 0;                    //����ʱ����
    delay_us(5);//Delay5us();                 //��ʱ
}

/**************************************
����Ӧ���ź�
**************************************/
uchar ADXL345_RecvACK(void)
{
    SDA_INT();
    SCL_OUT();
    SET_SCL();//SCL = 1;                    //����ʱ����
    delay_us(2);//    Delay5us();                 //��ʱ
    SET_SCL();
    if(SDA_VAL()== Bit_SET)   //CY = SDA;                   //��Ӧ���ź�
    {
      err = 1;
    }
    else
    {
      err = 0;
    }
 
    CLE_SCL() ;//SCL = 0;                    //����ʱ����
    delay_us(5);//    Delay5us();                 //��ʱ
    SDA_OUT();
    return err;
}

/**************************************
��IIC���߷���һ���ֽ�����
**************************************/
void ADXL345_SendByte(unsigned char dat)
{
    unsigned char i;
    SCL_OUT();
    SDA_OUT();
    for (i=0; i<8; i++)         //8λ������
    {
        delay_us(5);             //��ʱ
        if(dat&0x80)  //SDA = CY;               //�����ݿ�
        {SET_SDA();}
        else
        {CLE_SDA();}       
        delay_us(5);             //��ʱ
        SET_SCL();//SCL = 1;                //����ʱ����
        delay_us(5);             //��ʱ
        CLE_SCL();//SCL = 0;                //����ʱ����
        dat <<= 1;              //�Ƴ����ݵ����λ
    }
    ADXL345_RecvACK();
}

/**************************************
��IIC���߽���һ���ֽ�����
**************************************/
unsigned char ADXL345_RecvByte(void)
{
    unsigned char i;
    unsigned char Mid;
    unsigned char dat = 0;
    SDA_INT();
    SCL_OUT();

    for (i=0; i<8; i++)         //8λ������
    {
        dat <<= 1;
        delay_us(5);            //��ʱ
        SET_SCL();

			if(SDA_VAL()== Bit_SET)   //CY = SDA;                   //��Ӧ���ź�
			{
				Mid = 1;
			}
			else
			{
				Mid = 0;
			}
		
//        Mid=SDA_VAL();
        if(Mid) dat++;
        delay_us(5);     
        CLE_SCL();//SCL = 0;                //����ʱ����
    }
    return dat;
}

//******���ֽ�д��*******************************************

void Single_Write_ADXL345(uchar REG_Address,uchar REG_data)
{
    ADXL345_Start();                  //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);   //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);    //�ڲ��Ĵ�����ַ����ο�����pdf22ҳ 
    ADXL345_SendByte(REG_data);       //�ڲ��Ĵ������ݣ���ο�����pdf22ҳ 
    ADXL345_Stop();                   //����ֹͣ�ź�
}

//********���ֽڶ�ȡ*****************************************
uchar Single_Read_ADXL345(uchar REG_Address)
{  	uchar REG_data;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(REG_Address);            //���ʹ洢��Ԫ��ַ����0��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
    REG_data=ADXL345_RecvByte();              //�����Ĵ�������
		ADXL345_SendACK(1);   
		ADXL345_Stop();                           //ֹͣ�ź�
    return REG_data; 
}
//*********************************************************
//
//��������ADXL345�ڲ����ٶ����ݣ���ַ��Χ0x32~0x37
//
//*********************************************************
void Multiple_Read_ADXL345(void)
{   uchar i;
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress);           //�����豸��ַ+д�ź�
    ADXL345_SendByte(0x32);                   //���ʹ洢��Ԫ��ַ����0x32��ʼ	
    ADXL345_Start();                          //��ʼ�ź�
    ADXL345_SendByte(SlaveAddress+1);         //�����豸��ַ+���ź�
	 for (i=0; i<6; i++)                      //������ȡ6����ַ���ݣ��洢��BUF
    {
        BUF[i] = ADXL345_RecvByte();          //BUF[0]�洢0x32��ַ�е�����
        if (i == 5)
        {
           ADXL345_SendACK(1);                //���һ��������Ҫ��NOACK
        }
        else
        {
          ADXL345_SendACK(0);                //��ӦACK
       }
   }
    ADXL345_Stop();                          //ֹͣ�ź�
//    delay_ms(5);
}


//*****************************************************************

//��ʼ��ADXL345
void Init_ADXL345(void)
{
   Single_Write_ADXL345(0x31,0x0B);   //������Χ,����16g��13λģʽ
   Single_Write_ADXL345(0x2C,0x08);   //�����趨Ϊ12.5 
   Single_Write_ADXL345(0x2D,0x08);   //ѡ���Դģʽ  
   Single_Write_ADXL345(0x2E,0x80);   //ʹ�� DATA_READY �ж�
   Single_Write_ADXL345(0x1E,0x00);   //X ƫ���� 
   Single_Write_ADXL345(0x1F,0x00);   //Y ƫ���� 
   Single_Write_ADXL345(0x20,0x05);   //Z ƫ���� 
//		while(Single_Read_ADXL345(0X00)!=0xe5)
//		delay_ms(50);       
}
//***********************************************************************
//��ʾx��
void ReadData_x(void)
{   
  long int  dis_data;                       //����
  Multiple_Read_ADXL345();       	//�����������ݣ��洢��BUF��
//  dis_data=(BUF[1]<<8)+BUF[0];  //�ϳ�����   
//  if(dis_data<0)
//  {
//    dis_data=-dis_data;
//  }
//  temp_X=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
  dis_data=(BUF[3]<<8)+BUF[2];  //�ϳ�����   
  if(dis_data<0)
  {
    dis_data=-dis_data;
  }
  temp_Y=dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
//  dis_data=(BUF[5]<<8)+BUF[4];    //�ϳ�����   
//  if(dis_data<0)
//  {
//    dis_data=-dis_data;
//  }
//  temp_Z=(float)dis_data*3.9;  //�������ݺ���ʾ,�鿼ADXL345�������ŵ�4ҳ
}


