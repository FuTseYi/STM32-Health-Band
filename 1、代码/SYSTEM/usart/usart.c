#include "sys.h"
#include "usart.h"
#include "oled.h"

void Esp8266_AP_Init() //ESP8266局域网开热点等待手机APP链接
{
	OLED_printf(0,0,"1 AT+CWMODE ");
	UsartPrintf(ESP8266_USART,"AT+CWMODE=2\r\n"); 																	//设置路由器模式1 station,模式2 AP,模式3 station+AP混合模式   
	delay_ms(1000);OLED_Clear();
	UsartPrintf(ESP8266_USART,"AT+CWMODE=2\r\n"); 																	//设置路由器模式1 station,模式2 AP,模式3 station+AP混合模式   
	delay_ms(1000);OLED_Clear();
	delay_ms(1000);
	OLED_printf(0,0,"2 AT+CWSAP  ");
	UsartPrintf(ESP8266_USART,"AT+CWSAP=\"WIFI\",\"123456789\",11,4\r\n");  	//AT+CWSAP="wifi_yuan","123456789",11,4  设置模块SSID:WIFI, PWD:密码 及安全类型加密模式（WPA2-PSK） 
	delay_ms(1000);delay_ms(1000);delay_ms(1000);OLED_Clear();
	delay_ms(1000);
	OLED_printf(0,0,"3 AT+CIPMUX ");
	UsartPrintf(ESP8266_USART,"AT+CIPMUX=1\r\n");											//开启多连接模式，允许多个各客户端接入 
	delay_ms(1000);OLED_Clear();
	delay_ms(1000);
	OLED_printf(0,0,"4 AT+CIPSERVER ");
	UsartPrintf(ESP8266_USART,"AT+CIPSERVER=1,5000\r\n");  										//启动TCP/IP 实现基于网络//控制 
	delay_ms(1000);OLED_Clear();
	delay_ms(1000);
	OLED_printf(0,0,"5 AT+CIPSTO ");
	UsartPrintf(ESP8266_USART,"AT+CIPSTO=0\r\n"); 
	delay_ms(1000);OLED_Clear();
}


//串口接收缓存区 	
u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节
u16 USART1_RX_STA;   						//接收数据状态
/**************************************************************************
函数功能：串口1接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART1_IRQHandler(void)                	//串口1中断服务程序
{
	u8 res;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		res =USART1->DR;	//读取接收到的数据
		if(USART1_RX_STA<USART1_MAX_RECV_LEN)
		{
		
			USART1_RX_BUF[USART1_RX_STA++]=res;
		}
		else
			USART1_RX_STA=0;
  }
}



//串口接收缓存区 	
u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节
u16 USART2_RX_STA;   						//接收数据状态
//void USART2_IRQHandler(void)
//{
//	u8 res;	      
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
//	{	 
//		res =USART_ReceiveData(USART2);		 
//		if((USART2_RX_STA&(1<<15))==0)//接收完的一批数据,还没有被处理,则不再接收其他数据
//		{ 
//			if(USART2_RX_STA<USART2_MAX_RECV_LEN)	//还可以接收数据
//			{
//				TIM_SetCounter(TIM3,0);         				//计数器清空
//				if(USART2_RX_STA==0) 				//使能定时器3的中断 
//				{
//					TIM_Cmd(TIM3,ENABLE);//使能定时器3
//				}
//				USART2_RX_BUF[USART2_RX_STA++]=res;	//记录接收到的值	 
//			}
//			else 
//			{
//				USART2_RX_STA|=1<<15;				//强制标记接收完成
//			} 
//		}
//	}  				 											 
//}   
/**************************************************************************
函数功能：串口2接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART2_IRQHandler(void)
{
//	u8 res;
//	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)//接收到数据
//	{	 
//		res =USART2->DR;	
//		if(USART2_RX_STA<USART2_MAX_RECV_LEN) 
//		{
//			USART2_RX_STA=0;
//			 
//		}  	
//	}		
}   

//串口接收缓存区 	
u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节
u16 USART3_RX_STA=0;   						//接收数据状态
/**************************************************************************
函数功能：串口3接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{
	u8 res;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{
		res =USART3->DR;	//读取接收到的数据
		if(USART3_RX_STA<USART3_MAX_RECV_LEN)
		{
			
			USART3_RX_BUF[USART3_RX_STA++]=res;
		}
  }
}
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (unsigned char) ch);
	while (!(USART1->SR & USART_FLAG_TXE));
	return (ch);
}
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len)
{
	unsigned short count = 0;
	for(; count < len; count++)
	{
		USART_SendData(USARTx, *str++);									//发送数据
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);		//等待发送完成
	}
}
void UsartSendData(USART_TypeDef *USARTx, u8 data)
{
	USART_SendData(USARTx, data);
	while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...)
{
	unsigned char UsartPrintfBuf[296];
	va_list ap;
	unsigned char *pStr = UsartPrintfBuf;
	
	va_start(ap, fmt);
	vsnprintf((char *)UsartPrintfBuf, sizeof(UsartPrintfBuf), fmt, ap);							//格式化
	va_end(ap);
	
	while(*pStr != 0)
	{
		USART_SendData(USARTx, *pStr++);
		while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
	}
}
/**************************************************************************
函数功能：串口1初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void usart1_Init(u32 bound){
  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//使能USART1，GPIOA时钟
  
	//USART1_TX   GPIOA.9
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.9
   
  //USART1_RX	  GPIOA.10初始化
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA.10  

  //Usart1 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=2 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
  
   //USART 初始化设置

	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式

  USART_Init(USART1, &USART_InitStructure); //初始化串口1
  USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART1, ENABLE);                    //使能串口1 

}
/**************************************************************************
函数功能：串口2初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void usart2_Init(u32 bound)
{  

	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	// GPIOA时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE); //串口2时钟使能

 	USART_DeInit(USART2);  //复位串口2
		 //USART2_TX   PA2
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //PA2
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOA, &GPIO_InitStructure); //初始化PA2
   
    //USART2_RX	  PA3
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);  //初始化PA3
	
	USART_InitStructure.USART_BaudRate = bound;//波特率设置
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  
	USART_Init(USART2, &USART_InitStructure);     //初始化串口2
  
	USART_Cmd(USART2, ENABLE);                    //使能串口 
	
	//使能接收中断
  USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);//开启中断   
	
	//设置中断优先级
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
}
	

/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void usart3_Init(u32 bound)
{  	 
	  //GPIO端口设置
  GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//使能UGPIOB时钟
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//使能USART3时钟
	//USART3_TX  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB.10
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//复用推挽输出
  GPIO_Init(GPIOB, &GPIO_InitStructure);
   
  //USART3_RX	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//浮空输入
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  //Usart3 NVIC 配置
  NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器
   //USART 初始化设置
	USART_InitStructure.USART_BaudRate = bound;//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
  USART_Init(USART3, &USART_InitStructure);     //初始化串口3
  USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//开启串口接受中断
  USART_Cmd(USART3, ENABLE);                    //使能串口3 
}


