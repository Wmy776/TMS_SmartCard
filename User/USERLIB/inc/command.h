/*
	TMS智能卡公共命令库
		修订与2017,3,29
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
*/

#ifndef _COMMAND_H_
#define _COMMAND_H_

#define COMD_BIT1 0
#define COMD_BIT2 1
#define REPL_BIT  2

typedef struct order
{
	char name[10];
	char code[8];
}ORD;
///////////////////////////////////////////////////////////////////
extern int command_handle_state;
extern int command_get_state;
extern int level;//命令权限，默认为lv.2
extern ORD *geted_order;
///////////////////////////////////////////////////////////////////
extern ORD order_connect;//上位机连接命令
extern ORD order_get_LV1;//上位机申请LV1(卡发送UID进行配对)
extern ORD order_get_UID;//上位机申请获取卡内绑定的UID
extern ORD order_want_bind;//上位机申请绑定卡
extern ORD order_get_LV0;//上位机申请进行指纹识别
extern ORD order_Verify_FP;//上位机申请验证指纹是否存在
extern ORD order_ask_unbind;//上位机申请解除绑定
extern ORD order_ask_add_FP;//上位机申请增加指纹
extern ORD order_ask_del_FP;//上位机申请删除指纹
///////////////////////////////////////////////////////////////////
extern ORD reply_connect;//回复上位机连接命令
extern ORD reply_get_LV1;//回复上位机申请LV1(发送UID进行配对)
extern ORD reply_want_bind;//回复上位机申请绑定卡
extern ORD reply_busy;//回复上位机忙
extern ORD reply_get_LV0;//回复上位机申请进行指纹识别
extern ORD reply_Verify_FP;//回复上位机申请验证指纹是否存在
extern ORD reply_ask_unbind;//回复上位机申请解除绑定
extern ORD reply_ask_add_FP;//回复上位机申请增加指纹
extern ORD reply_ask_del_FP;//回复上位机申请删除指纹
///////////////////////////////////////////////////////////////////
void send_singleReply(const ORD *order,char value);//发送单字节回复命令
void send_noneReply(const ORD *order);//发送无字节回复命令
void anal_command(void);//指令分析
ORD *get_NoneDataOrder(void);//接受无有效位数据命令



#endif