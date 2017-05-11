/*
	TMS智能卡公共调试库
		修订与2017,3,29
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
	注意：
	1、若要启动调试，请将DEBUG_ON宏开启
	2、所有调试信息通过USART1发送
*/

#include "include.h"

void SRPL_DBG(const ORD *order,const unsigned char value)//单字节回复指令调试
{
	char text1[10] = "order:\n";
	char text2[10] = "code:\n";
	short i = 0;
	while(text1[i] != '\n')
	{
		USART1_Send(text1[i]);
		i ++;
	}
	i = 0;
	while(order->name[i] != '\n')
	{
		USART1_Send(order->name[i]);
		i ++;
	}
	USART1_Send(' ');
	USART1_Send(' ');
	USART1_Send(' ');
	i = 0;
	while(text2[i] != '\n')
	{
		USART1_Send(text2[i]);
		i ++;
	}
	for(i = 0;i < 8;i ++)
	{
		if(i == 2)
			USART1_Send(value);
		else
			USART1_Send(order->code[i]);
	}		
	USART1_Send('\r');
	USART1_Send('\n');
}


void NRPL_DBG(const ORD *order)//无字节回复指令调试
{
	char text1[10] = "order:\n";
	char text2[10] = "code:\n";
	short i = 0;
	while(text1[i] != '\n')
	{
		USART1_Send(text1[i]);
		i ++;
	}
	i = 0;
	while(order->name[i] != '\n')
	{
		USART1_Send(order->name[i]);
		i ++;
	}
	USART1_Send(' ');
	USART1_Send(' ');
	USART1_Send(' ');
	i = 0;
	while(text2[i] != '\n')
	{
		USART1_Send(text2[i]);
		i ++;
	}
	for(i = 0;i < 8;i ++)
		USART1_Send(order->code[i]);	
	USART1_Send('\r');
	USART1_Send('\n');
}

void ERR_DBG(const char *type,const unsigned char value)//出错信息调试
{
	;
}

void STAT_DBG(const char str[])//状态信息调试
{
	short i;
	char text[] = "debugmessage:";
	for(i = 0;i < 13;i ++)
		USART1_Send(text[i]);
	i = 0;
	while(str[i] != '\n')
	{
		USART1_Send(str[i]);
		i ++;
	}
	USART1_Send('\r');
	USART1_Send('\n');
}
