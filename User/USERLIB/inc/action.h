/*
	TMS智能卡公共动作库
		修订与2017,4,01
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
*/

#ifndef _ACTION_H_
#define _ACTION_H_

void actionConnect(void);//等待连接
void waitConnect(void);//等待连接，既开发板重启之后的第一次连接
void sendUserID(void);//发送用户ID


#endif