#ifndef _USART_H_
#define _USART_H_

/*
	TMS智能卡公共串口库
		修订与2017,3,28
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
				
	注意：
	1：USART1用于调试，USART2用于连接蓝牙模块，USART3用于连接指纹模块。
	2：USART1的TX为PA9，RX为PA10。USART2的TX为PA2，RX为PA3。USART3的TX
	   为PB10，USART3的RX为PB11。	
*/

extern unsigned char USART1_Res;
extern unsigned char USART2_Res;
extern unsigned char USART3_Res;


void usart1_config(void);//串口1配置函数
void usart2_config(void);//串口2配置函数
void usart3_config(void);//串口3配置函数
void NVIC_Config(void);//串口中断配置
void USART1_Send(const unsigned char ch);//串口1发送
void USART2_Send(const unsigned char ch);//串口2发送
void USART3_Send(const unsigned char ch);//串口3发送
unsigned char USART1_Recive(void);//串口1接收
unsigned char USART2_Recive(void);//串口2接收
unsigned char USART3_Recive(void);//串口3接收
void USART1_IRQ(int swi);//串口1中断开关
void USART2_IRQ(int swi);//串口2中断开关
void USART3_IRQ(int swi);//串口3中断开关

#endif


