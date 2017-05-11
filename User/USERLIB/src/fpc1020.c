/*
	TMS智能卡用指纹模块驱动库
		修订与2017,4,6
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
*/
#include "include.h"
unsigned char fingerprint_command_get_state = NOCOM_GET;//指纹模块消息接受信号量
unsigned char FP_ask_LV0[8] = //请求指纹识别指令
{
	0xF5,0x0B,0x00,0x01,0x00,0x00,0x0A,0xF5
};

unsigned char FP_empty[8] = //空命令
{
	0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF
};
unsigned char FP_ask_unbind[8] = //解除绑定指令
{
	0xF5,0x04,0x00,0x01,0x00,0x00,0x05,0xF5
};
unsigned char FP_ask_addfp_step1[8] = //请求增加指纹第一步
{
	0xF5,0x01,0x00,0x01,0x01,0x00,0x01,0xF5
};
unsigned char FP_ask_addfp_step2[8] = //请求增加指纹第二步
{
	0xF5,0x02,0x00,0x01,0x01,0x00,0x02,0xF5
};
unsigned char FP_ask_addfp_step3[8] = //请求增加指纹第三步
{
	0xF5,0x03,0x00,0x01,0x01,0x00,0x03,0xF5
};
///////////////////////////////////////////////////////////////
unsigned char FP_Geted_order[8];//收到的指令

unsigned char FP_ask_LV0_reply[8] = //回复请求指纹识别指令
{
	0xF5,0x0B,0x00,0x00,0x00,0x00,0x0B,0xF5
};

unsigned char FP_ask_unbind_reply[8] = //回复请求解除绑定指令
{
	0xF5,0x04,0x00,0x00,0x00,0x00,0x04,0xF5
};

unsigned char FP_ask_addfp_step1_reply[8] = //回复请求增加指纹第一步
{
	0xF5,0x01,0x00,0x01,0x01,0x00,0x01,0xF5
};
unsigned char FP_ask_addfp_step2_reply[8] = //回复请求增加指纹第二步
{
	0xF5,0x02,0x00,0x01,0x01,0x00,0x02,0xF5
};
unsigned char FP_ask_addfp_step3_reply[8] = //回复请求增加指纹第三步
{
	0xF5,0x03,0x00,0x01,0x01,0x00,0x00,0xF5
};

int Ask_for_FP(void)//向指纹模块发送申请指纹识别，成功回复1失败回复0。
{
	short i;
	Send_FPorder(FP_ask_LV0);//向指纹模块发起要求接受指纹
	while(fingerprint_command_get_state == NOCOM_GET);
	if(FP_Geted_order[0] == FP_ask_LV0_reply[0] && FP_Geted_order[1] == FP_ask_LV0_reply[1])
		switch(FP_Geted_order[MESSAGE_BIT])
		{
			case ACK_SUCCESS:
				fingerprint_command_get_state = NOCOM_GET;
				return 1;
				break;
			case ACK_FAIL:
				fingerprint_command_get_state = NOCOM_GET;
				return 0;
				break;
		}
	else
	{
		#ifdef DEBUG_ON
		STAT_DBG("Fingerprint marry error!\n");
		#endif
		fingerprint_command_get_state = NOCOM_GET;
		return -1;
	}
}

int Ask_for_unbind()//向指纹模块发送申请解除绑定，成功回复1失败回复0。
{
	short i;
	Send_FPorder(FP_ask_unbind);//向指纹模块发起要求接受指纹
	while(fingerprint_command_get_state == NOCOM_GET);
	if(FP_Geted_order[0] == FP_ask_unbind_reply[0] && FP_Geted_order[1] == FP_ask_unbind_reply[1])
		switch(FP_Geted_order[MESSAGE_BIT])
		{
			case ACK_SUCCESS:
				fingerprint_command_get_state = NOCOM_GET;
				return 1;
				break;
			case ACK_FAIL:
				fingerprint_command_get_state = NOCOM_GET;
				return 0;
				break;
		}
	else
	{
		#ifdef DEBUG_ON
		STAT_DBG("FP reply message error!\n");
		#endif
		fingerprint_command_get_state = NOCOM_GET;
		return -1;
	}
}

void Send_FPorder(const unsigned char *order)//发送指纹模块命令
{
	short i;
	for(i = 0;i < 8;i ++)
		USART3_Send(order[i]);
	#ifdef DEBUG_ON
	STAT_DBG("Send order to FP\n");
	#endif
}

int Ask_for_add_step1(void)//向指纹模块发送申请增加指纹第一步
{
	short i;
	Send_FPorder(FP_ask_addfp_step1);//向指纹模块发起申请增加指纹第一步
	while(fingerprint_command_get_state == NOCOM_GET);
	if(FP_Geted_order[0] == FP_ask_addfp_step1_reply[0] && FP_Geted_order[1] == FP_ask_addfp_step1_reply[1])
		switch(FP_Geted_order[MESSAGE_BIT])
		{
			case ACK_SUCCESS:
				fingerprint_command_get_state = NOCOM_GET;
				return 1;
				break;
			case ACK_FAIL:
				fingerprint_command_get_state = NOCOM_GET;
				return 0;
				break;
		}
	else
	{
		#ifdef DEBUG_ON
		STAT_DBG("FP reply message error!\n");
		#endif
		fingerprint_command_get_state = NOCOM_GET;
		return -1;
	}
}

int Ask_for_add_step2(void)//向指纹模块发送申请增加指纹第二步
{
	short i;
	Send_FPorder(FP_ask_addfp_step2);//向指纹模块发起申请增加指纹第二步
	while(fingerprint_command_get_state == NOCOM_GET);
	if(FP_Geted_order[0] == FP_ask_addfp_step2_reply[0] && FP_Geted_order[1] == FP_ask_addfp_step2_reply[1])
		switch(FP_Geted_order[MESSAGE_BIT])
		{
			case ACK_SUCCESS:
				fingerprint_command_get_state = NOCOM_GET;
				return 1;
				break;
			case ACK_FAIL:
				fingerprint_command_get_state = NOCOM_GET;
				return 0;
				break;
		}
	else
	{
		#ifdef DEBUG_ON
		STAT_DBG("FP reply message error!\n");
		#endif
		fingerprint_command_get_state = NOCOM_GET;
		return -1;
	}
}

int Ask_for_add_step3()//向指纹模块发送申请增加指纹第三步
{
	short i;
	Send_FPorder(FP_ask_addfp_step3);//向指纹模块发起申请增加指纹第三步
	while(fingerprint_command_get_state == NOCOM_GET);
	if(FP_Geted_order[0] == FP_ask_addfp_step3_reply[0] && FP_Geted_order[1] == FP_ask_addfp_step3_reply[1])
		switch(FP_Geted_order[MESSAGE_BIT])
		{
			case ACK_SUCCESS:
				fingerprint_command_get_state = NOCOM_GET;
				return 1;
				break;
			case ACK_FAIL:
				fingerprint_command_get_state = NOCOM_GET;
				return 0;
				break;
		}
	else
	{
		#ifdef DEBUG_ON
		STAT_DBG("FP reply message error!\n");
		#endif
		fingerprint_command_get_state = NOCOM_GET;
		return -1;
	}
}