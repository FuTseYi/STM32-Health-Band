#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define ESP8266_USART USART1

#define USART_DEBUG 	USART2

/*串口1 接收缓存区 */
#define USART1_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART1_MAX_SEND_LEN		400					//最大发送缓存字节数
extern u16 USART1_RX_STA;	
extern u8 USART1_RX_BUF[USART1_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
extern u8 USART1_TX_BUF[USART1_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节
/*串口2 接收缓存区 */
#define USART2_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART2_MAX_SEND_LEN		400					//最大发送缓存字节数
extern u16 USART2_RX_STA;	
extern u8 USART2_RX_BUF[USART2_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
extern u8 USART2_TX_BUF[USART2_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节
/*串口3 接收缓存区 */
#define USART3_MAX_RECV_LEN		400					//最大接收缓存字节数
#define USART3_MAX_SEND_LEN		400					//最大发送缓存字节数
extern u16 USART3_RX_STA;   						//接收数据状态
extern u8 USART3_RX_BUF[USART3_MAX_RECV_LEN]; 				//接收缓冲,最大USART2_MAX_RECV_LEN个字节.
extern u8 USART3_TX_BUF[USART3_MAX_SEND_LEN]; 			  //发送缓冲,最大USART2_MAX_SEND_LEN字节

void Esp8266_AP_Init(void);
void usart1_Init(u32 bound);
void usart2_Init(u32 bound);
void usart3_Init(u32 bound);
void UsartPrintf(USART_TypeDef *USARTx, char *fmt,...);
void UsartSendData(USART_TypeDef *USARTx, u8 data);
void Usart_SendString(USART_TypeDef *USARTx, unsigned char *str, unsigned short len);
#endif


