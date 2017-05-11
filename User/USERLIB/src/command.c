/*
	TMS智能卡公共命令库
		修订与2017,3,29
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
	注意：
	1、COMD_BIT1为指令功能位1，COMD_BIT2为指令功能位2。
	2、若回复指令的回复位为1位，则0xFF为真，0x00为假，
	   默认值为0xFE。命令发送发成后，相关函数有义务将
	   回复位重置为0xFE的默认值。
*/
#include "include.h"
/*
	静态命令定义
*/

int command_handle_state = NOCOM_HAND;//设置命令处理信号量，初值为为0时处理器没有处在理命令，为1时处理器正在处理命令
int command_get_state = NOCOM_GET;//设置命令获取信号量，初值为0时没有待处理的命令，为1时存在待处理的命令
int level;//命令权限，默认为lv.2
/////////////////////////////////////////////////////////////
ORD order_connect = {//上位机连接命令
	"t.connect\n",
	{0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_get_LV1 = {//上位机申请LV1(卡发送UID进行配对)
	"t.getlv1\n",
	{0xFE,0xFC,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_get_UID = {//上位机申请获取卡内绑定的UID
	"t.getuid\n",
	{0xFE,0xFA,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_want_bind = {//上位机申请绑定卡
	"t.Bind\n",
	{0xFE,0xF5,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_get_LV0 = {//上位机申请进行指纹识别
	"t.getlv0\n",
	{0xFF,0xFB,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_Verify_FP = {//上位机申请验证指纹是否存在
	"t.askfp\n",
	{0xFE,0xF9,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_ask_unbind = {//上位机申请解除绑定
	"t.unbind\n",
	{0xFE,0xF8,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_ask_add_FP = {//上位机申请增加指纹
	"t.addfp\n",
	{0xFF,0xF6,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD order_ask_del_FP = {//上位机申请删除指纹
	"t.delfp\n",
	{0xFE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};
/////////////////////////////////////////////////////////////
ORD reply_connect = {//回复上位机连接命令
	"r.connect\n",
	{0x00,0x00,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_busy = {//回复上位机忙
	"r.busy\n",
	{0x00,0x01,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_get_LV1 = {//回复上位机申请LV1(卡发送UID进行配对)
	"r.asklv1\n",
	{0x00,0x03,0xFE,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_want_bind = {//回复上位机申请绑定卡
	"r.Bind\n",
	{0x00,0x0A,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_get_LV0 = {//回复上位机申请进行指纹识别
	"r.getlv0\n",
	{0x00,0x04,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};


ORD reply_Verify_FP = {//回复上位机申请验证指纹是否存在
	"r.askfp\n",
	{0x00,0x06,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_ask_unbind = {//回复上位机申请解除绑定
	"r.unbind\n",
	{0x00,0x07,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_ask_add_FP = {//回复上位机申请增加指纹
	"r.addfp\n",
	{0x00,0x09,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD reply_ask_del_FP = {//回复上位机申请删除指纹
	"r.delfp\n",
	{0xFE,0xF7,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}
};

ORD *geted_order;
/////////////////////////////////////////////////////////////
/*
	函数定义
*/

void send_singleReply(const ORD *order,char value)//发送单字节回复命令
{
	short i;
	for(i = 0;i < 8;i ++)
	{
		if(i == 2)
			USART2_Send(value);
		else 
			USART2_Send(order->code[i]);
	}
}

void send_noneReply(const ORD *order)//发送无字节回复命令
{
	short i;
	for(i = 0;i < 8;i ++)
		USART2_Send(order->code[i]);
	#ifdef DEBUG_ON
	NRPL_DBG(order);
	#endif
}

//ORD *get_NoneDataOrder(void)//接受无有效位数据命令
//{
//	short i;
//	ORD *temp = (ORD *)malloc(sizeof(ORD));
//	temp->code[0] = USART2_Res;
//	for(i = 1;i < 8;i ++)
//		temp->code[i] = USART_ReceiveData(USART2);
//	command_handle_state = COM_HAND;//命令处理信号量置为1
//	command_get_state = COM_GET;//接收到命令
//	return temp;
//}

void anal_command()//指令分析
{
	short i;
	int mess;
	if(command_get_state == COM_GET)//如果串口2接收到命令
	{
		command_handle_state = COM_HAND;//正在处理命令，命令信号量置COM_HAND
		//如果接受到的指令为连接指令，重新启动智能卡
		if(geted_order->code[COMD_BIT1] == order_connect.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_connect.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Reboot\n");
			#endif
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			send_singleReply(&reply_busy,0xFF);//答忙
			__disable_fault_irq();//关全局中断
			NVIC_SystemReset();//重启
		}
		//如果接受到的指令为申请LV1指令
		if(geted_order->code[COMD_BIT1] == order_get_LV1.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_get_LV1.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Ask LV1\n");
			#endif
			for (i = 0;i < 6;i ++)
				GETED_ID[i] = geted_order->code[i+2];
			if(UID_cmp(GETED_ID,USER_ID))//如果匹配成功
			{
				send_singleReply(&reply_get_LV1,0xFF);
				level = LV1;
				#ifdef DEBUG_ON
				STAT_DBG("Get LV1!\n");
				#endif
			}	
			else if(UID_cmp(EMPTY_ID,USER_ID))//如果卡为空
			{	
				send_singleReply(&reply_get_LV1,0xFE);
				level = LV1;
				#ifdef DEBUG_ON
				STAT_DBG("Empty Card,Get LV1!\n");
				#endif
			}
			else//如果匹配失败
			{
				send_singleReply(&reply_get_LV1,0x00);
				#ifdef DEBUG_ON
				STAT_DBG("USER_ID not marry!\n");
				#endif
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请获取卡内绑定的UID
		if(geted_order->code[COMD_BIT1] == order_get_UID.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_get_UID.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Ask ID\n");
			#endif
			if(UID_cmp(EMPTY_ID,USER_ID))//如果为空卡
			{
				USART2_Send(0x00);
				USART2_Send(0x05);
				for(i = 0;i < 6;i ++)
					USART2_Send(0x00);
				#ifdef DEBUG_ON
				STAT_DBG("Empty card!\n");
				#endif
			
			}
			else if(level == LV2)//权限过低
			{
	
				USART2_Send(0x00);
				USART2_Send(0x05);
				for(i = 0;i < 6;i ++)
					USART2_Send(0xFF);
				#ifdef DEBUG_ON
				STAT_DBG("level too low!\n");
				#endif
			}
			else //发送ID
			{
				USART2_Send(0x00);
				USART2_Send(0x05);
				for(i = 0;i < 6;i ++)
					USART2_Send(USER_ID[i]);
				#ifdef DEBUG_ON
				STAT_DBG("Sended ID!\n");
				#endif
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请绑定卡
		if(geted_order->code[COMD_BIT1] == order_want_bind.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_want_bind.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Want Bind\n");
			#endif
			if(level == LV2)//权限过低
			{
				send_singleReply(&reply_want_bind,0xFE);
				#ifdef DEBUG_ON
				STAT_DBG("level too low!\n");
				#endif
			}
			else if(UID_cmp(EMPTY_ID,USER_ID) == 0)//已经绑定，不能重复绑定
			{
				send_singleReply(&reply_want_bind,0x00);
				#ifdef DEBUG_ON
				STAT_DBG("Card has been binded\n");
				#endif
			}
			else if(UID_cmp(EMPTY_ID,USER_ID))//未绑定状态，可以绑定
			{
				for(i = 0;i < 6;i ++)
					USER_ID[i] = geted_order->code[i+2];
				W25Q16_Write((u8*)USER_ID,USER_ID_ADDR,USER_ID_SIZE);
				send_singleReply(&reply_want_bind,0xFF);
				#ifdef DEBUG_ON
				STAT_DBG("Bind Success\n");
				#endif
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请验证指纹是否存在
		if(geted_order->code[COMD_BIT1] == order_Verify_FP.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_Verify_FP.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Verify fingerprints exist\n");
			#endif
			if(level == LV2)//权限低
			{
				send_singleReply(&reply_Verify_FP,0xFE);
				#ifdef DEBUG_ON
				STAT_DBG("Level too low!\n");
				#endif
			}
			else if(FINGERPRINT_NUM == 1)//存在指纹
			{
				send_singleReply(&reply_Verify_FP,0xFF);
				#ifdef DEBUG_ON
				STAT_DBG("Have a fingerprint!\n");
				#endif
			}
			else if(FINGERPRINT_NUM == 0)//不存在指纹
			{
				send_singleReply(&reply_Verify_FP,0x00);
				#ifdef DEBUG_ON
				STAT_DBG("Have no fingerprint!\n");
				#endif
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请进行指纹识别
		if(geted_order->code[COMD_BIT1] == order_get_LV0.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_get_LV0.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Ask for LV0\n");
			#endif
			if(level == LV2)//权限过低
			{
				send_singleReply(&reply_Verify_FP,0xFE);
				#ifdef DEBUG_ON
				STAT_DBG("Level too low!\n");
				#endif
			}
			else if(FINGERPRINT_NUM == 0)//没有录入指纹
			{
				send_singleReply(&reply_Verify_FP,0x01);
				#ifdef DEBUG_ON
				STAT_DBG("Don't have a fingerprint!\n");
				#endif
			}
			else
			{
				mess = Ask_for_FP();
				if(mess == 1)//指纹模块返回成功
				{
					send_singleReply(&reply_Verify_FP,0xFF);
					level = LV0;
					#ifdef DEBUG_ON
					STAT_DBG("Fingerprint marry success!\n");
					#endif
				}
				else if(mess == 0)//指纹模块返回失败
				{
					send_singleReply(&reply_Verify_FP,0x00);
					#ifdef DEBUG_ON
					STAT_DBG("Fingerprint marry fail!\n");
					#endif
				}
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请解除绑定
		if(geted_order->code[COMD_BIT1] == order_ask_unbind.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_ask_unbind.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Ask for unbind\n");
			#endif
			if(level != LV0)//权限低
			{
				send_singleReply(&reply_ask_unbind,0xFE);
				#ifdef DEBUG_ON
				STAT_DBG("Level too low!\n");
				#endif
			}
			else if(UID_cmp(EMPTY_ID,USER_ID) == 0)//如果处于绑定状态
			{
				mess = Ask_for_unbind();
				if(mess == 1)//成功
				{
					W25Q16_Write((u8*)EMPTY_ID,USER_ID_ADDR,USER_ID_SIZE);//epprom内的用户ID清零
					W25Q16_Write(&EMPTY_FINGERPRINT_NUM,6,sizeof(EMPTY_FINGERPRINT_NUM));//epprom内的指纹数量清零
					send_singleReply(&reply_Verify_FP,0xFF);
					#ifdef DEBUG_ON
					STAT_DBG("unbind success!\n");
					#endif
					__disable_fault_irq();//关全局中断
					NVIC_SystemReset();//重启
				}
				else if(mess == 0)//指纹模块返回失败
				{
					send_singleReply(&reply_Verify_FP,0x00);
					#ifdef DEBUG_ON
					STAT_DBG("fingerprint unbind fail!\n");
					#endif
				}
			}
			else if(UID_cmp(EMPTY_ID,USER_ID) == 1)//如果已经处于未绑定状态
			{
				send_singleReply(&reply_Verify_FP,0x01);
				#ifdef DEBUG_ON
				STAT_DBG("Drivce is already empty!\n");
				#endif
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请增加指纹
		if(geted_order->code[COMD_BIT1] == order_ask_add_FP.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == order_ask_add_FP.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Ask for addFP\n");
			#endif
			if(level == LV2)//权限低
			{
				send_singleReply(&reply_ask_add_FP,0xFE);
				#ifdef DEBUG_ON
				STAT_DBG("Level too low!\n");
				#endif
			}
			else if(FINGERPRINT_NUM == 1)//如果指纹存在
			{
				send_singleReply(&reply_ask_add_FP,0xFF);
				#ifdef DEBUG_ON
				STAT_DBG("Fingerprint already exists!\n");
				#endif
			}
			else
			{
				delay_ms(1000);
				//第一步
				mess = Ask_for_add_step1();//向指纹模块发送申请增加指纹第一步
				if(mess == 1)//成功
				{
					send_singleReply(&reply_ask_add_FP,0x01);
					#ifdef DEBUG_ON
					STAT_DBG("Add fingerprint step1 success!\n");
					#endif
				}
				else if(mess == 0)//指纹模块返回失败
				{
					send_singleReply(&reply_Verify_FP,0xF1);
					#ifdef DEBUG_ON
					STAT_DBG("Add fingerprint step1 fail!\n");
					#endif
					command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
					command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
					return;
				}
				delay_ms(1000);
				//第二步
				mess = Ask_for_add_step2();//向指纹模块发送申请增加指纹第二步
				if(mess == 1)//成功
				{
					send_singleReply(&reply_ask_add_FP,0x02);
					#ifdef DEBUG_ON
					STAT_DBG("Add fingerprint step2 success!\n");
					#endif
				}
				else if(mess == 0)//指纹模块返回失败
				{
					send_singleReply(&reply_Verify_FP,0xF2);
					#ifdef DEBUG_ON
					STAT_DBG("Add fingerprint step2 fail!\n");
					#endif
					command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
					command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
					return;
				}
				delay_ms(1000);
				//第三步
				mess = Ask_for_add_step3();//向指纹模块发送申请增加指纹第三步
				if(mess == 1)//成功
				{
					FINGERPRINT_NUM = 0x01;
					W25Q16_Write(&FULL_FINGERPRINT_NUM,6,sizeof(FULL_FINGERPRINT_NUM));//epprom内的指纹数量清零
					send_singleReply(&reply_ask_add_FP,0x00);
					#ifdef DEBUG_ON
					STAT_DBG("Add fingerprint step3 success!\n");
					#endif
				}
				else if(mess == 0)//指纹模块返回失败
				{
					send_singleReply(&reply_Verify_FP,0xF3);
					#ifdef DEBUG_ON
					STAT_DBG("Add fingerprint step3 fail!\n");
					#endif
					command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
					command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
					return;
				}
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		//上位机申请删除指纹
		if(geted_order->code[COMD_BIT1] == reply_ask_del_FP.code[COMD_BIT1]
				&& geted_order->code[COMD_BIT2] == reply_ask_del_FP.code[COMD_BIT2])
		{
			#ifdef DEBUG_ON
			STAT_DBG("Ask for del fingerprint！\n");
			#endif
			if(level != LV0)//权限低
			{
				send_singleReply(&reply_ask_del_FP,0xFE);
				#ifdef DEBUG_ON
				STAT_DBG("Level too low!\n");
				#endif
			}
			else if(FINGERPRINT_NUM == 1)//如果存在指纹
			{
				mess = Ask_for_unbind();
				if(mess == 1)//成功
				{
					FINGERPRINT_NUM = 0;//指纹数量清零
					W25Q16_Write(&EMPTY_FINGERPRINT_NUM,6,sizeof(EMPTY_FINGERPRINT_NUM));//epprom内的指纹数量清零
					send_singleReply(&reply_ask_del_FP,0xFF);
					#ifdef DEBUG_ON
					STAT_DBG("Ask for del fingerprint success!\n");
					#endif
				}
				else if(mess == 0)//指纹模块返回失败
				{
					send_singleReply(&reply_ask_del_FP,0x00);
					#ifdef DEBUG_ON
					STAT_DBG("Ask for del fingerprint fail!\n");
					#endif
				}
			}
			else if(FINGERPRINT_NUM == 0)//如果不存在指纹
			{
				send_singleReply(&reply_ask_del_FP,0x0C);
				#ifdef DEBUG_ON
				STAT_DBG("Fingerprint is already non-existent!\n");
				#endif
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
			return;
		}
		command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
		command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
	}
}




