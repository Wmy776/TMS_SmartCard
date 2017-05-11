/*
	TMS智能卡用指纹模块驱动库
		修订与2017,4,6
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
*/
#ifndef FPC1020_H
#define FPC1020_H

#define MESSAGE_BIT 4
extern unsigned char fingerprint_command_get_state;//指纹模块消息接受信号量
///////////////////////////////////////////////////////////////
//返回消息信息位含义
#define ACK_SUCCESS			0x00//操作成功
#define ACK_FAIL			0x01//操作失败
#define ACK_FULL			0x04//指纹数据库已满
#define ACK_NOUSER			0x05//无此用户
#define ACK_USER_OCCUPIED	0x06//此ID用户已存在
#define ACK_USER_EXIST		0x07//用户已存在
#define ACK_TIMEOUT			0x08//采集超时
///////////////////////////////////////////////////////////////
extern unsigned char FP_ask_LV0[8];//请求指纹识别指令
extern unsigned char FP_ask_unbind[8];//解除绑定指令
extern unsigned char FP_ask_addfp_step1[8];//请求增加指纹第一步
extern unsigned char FP_ask_addfp_step2[8];//请求增加指纹第二步
extern unsigned char FP_ask_addfp_step3[8];//请求增加指纹第三步
extern unsigned char FP_Geted_order[8];//收到的指令
extern unsigned char FP_empty[8];//空命令
////////////////////////////////////////////////////////////////
extern unsigned char FP_ask_LV0_reply[8];//回复请求指纹识别指令
extern unsigned char FP_ask_unbind_reply[8];//回复请求解除绑定指令

int Ask_for_FP(void);//向指纹模块发送申请指纹识别
int Ask_for_unbind(void);//向指纹模块发送申请解除绑定
int Ask_for_add_step1(void);//向指纹模块发送申请增加指纹第一步
int Ask_for_add_step2(void);//向指纹模块发送申请增加指纹第二步
int Ask_for_add_step3(void);//向指纹模块发送申请增加指纹第三步
int Ask_for_del(void);//向指纹模块发送申请删除指纹
void Send_FPorder(const unsigned char *order);//发送指纹模块命令
#endif 