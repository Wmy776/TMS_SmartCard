/*
	TMS智能卡公共调试库
		修订与2017,3,29
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
	注意：
	1、若要启动调试，请将DEBUG_ON宏开启
	2、所有调试信息通过USART1发送
*/
#ifndef _DEBUG_H_
#define _DEBUG_H_
//若要开启串口调试，请将DEBUG的注释去掉
#define DEBUG_ON

void SRPL_DBG(const ORD *order,const unsigned char value);//单字节回复指令调试
void NRPL_DBG(const ORD *order);//无字节回复指令调试
void ERR_DBG(const char *type,const unsigned char value);//出错信息调试
void STAT_DBG(const char str[]);//状态信息调试

#endif