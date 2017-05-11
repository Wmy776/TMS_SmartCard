/*
	TMS智能卡公共动作库
		修订与2017,4,01
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
*/

#include "include.h"

//void waitConnect() //等待连接，既开发板重启之后的第一次连接
//{
//	while(1)
//	{
//		if(command_get_state == COM_GET)//如果串口2接收到命令
//		{
//			command_handle_state = COM_HAND;//正在处理命令，命令信号量置COM_HAND
//			if(geted_order->code[COMD_BIT1] == order_connect.code[COMD_BIT1]
//					&& geted_order->code[COMD_BIT2] == order_connect.code[COMD_BIT2])
//			//如果接受到的指令为连接指令，既第一字节第二字节都为0xFF则为真
//			{
//				send_singleReply(&reply_connect,0xFF);
//				#ifdef DEBUG_ON
//				STAT_DBG("connect successful!");
//				#endif
//				command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
//				command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
//				break;
//			}
//			else
//			{
//				#ifdef DEBUG_ON
//				STAT_DBG("wrong order,device are not connected!\n");
//				#endif
//				send_singleReply(&reply_connect,0x00);
//			}
//			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
//			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
//		}
//	}
//}

void waitConnect()//等待连接，既开发板重启之后的第一次连接
{
	while(1)
	{
		if(command_get_state == COM_GET)//如果串口2接收到命令
		{
			command_handle_state = COM_HAND;//正在处理命令，命令信号量置COM_HAND
			if(geted_order->code[COMD_BIT1] == order_connect.code[COMD_BIT1]
					&& geted_order->code[COMD_BIT2] == order_connect.code[COMD_BIT2])
			//如果接受到的指令为连接指令，既第一字节第二字节都为0xFF则为真
			{
				send_singleReply(&reply_connect,0xFF);
				#ifdef DEBUG_ON
				STAT_DBG("connect successful!");
				#endif
				goto over;
			}
			if(geted_order->code[COMD_BIT1] != order_connect.code[COMD_BIT1]
					|| geted_order->code[COMD_BIT2] != order_connect.code[COMD_BIT2])
			{
				#ifdef DEBUG_ON
				STAT_DBG("wrong order,device are not connected!\n");
				#endif
				send_singleReply(&reply_connect,0x00);
			}
			command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
			command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
		}
	}
	over:;
	command_handle_state = NOCOM_HAND;//没有正在处理的命令，命令处理信号量置NOCOM_HAND
	command_get_state = NOCOM_GET;//接受到的命令已经处理完毕，没有待处理的命令，命令获取信号量置NOCOM_GET
}

void sendUserID()//发送用户ID
{
	
}

void actionConnect() //连接事件，既开发板没有重启但App重新要求连接
{
	send_singleReply(&reply_connect,0x00);
	#ifdef DEBUG_ON
	STAT_DBG("connect successful!");
	#endif
}