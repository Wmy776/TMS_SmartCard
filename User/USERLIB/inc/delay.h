#ifndef _DELAY_H_
#define _DELAY_H_

/*
	TMS智能卡公共延迟库
		修订与2017,3,28
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
*/

void delay_init(unsigned char SYSCLK);//延迟初始化
void delay_ms(unsigned short nms);//毫秒延迟
void delay_us(unsigned int nus);//微妙延迟

#endif


