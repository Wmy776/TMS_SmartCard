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

#include "include.h"

u8 USART1_Res,USART2_Res,USART3_Res;
NVIC_InitTypeDef NVIC_InitStructure_U1,NVIC_InitStructure_U2,NVIC_InitStructure_U3;

void usart1_config()
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//打开GPIO的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//打开USART1的时钟
	/*
	将USART1_TX的GPIO配置为推挽复用模式，配置TX为PA9
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	将USART1_RX的GPIO配置为浮空输入模式，配置RX为PA10
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	配置USART1的参数
		波特率：115200
		数据长度：8位
		停止位：1位
		禁止硬件流控
		使能接收和发送
	*/
	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure); 
	//使能USART1，配置完毕
	USART_Cmd(USART1, ENABLE);
	//USART1中断配置
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	//清空发送完成标志位，解决第一个字节无法发送的问题
	USART_ClearFlag(USART1, USART_FLAG_TC); 
}

void usart2_config()//串口2配置函数
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);//打开GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);//打开USART2的时钟
	/*
	将USART2_TX的GPIO配置为推挽复用模式，配置TX为PA2
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	将USART2_RX的GPIO配置为浮空输入模式，配置RX为PA3
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	配置USART2的参数
		波特率：115200
		数据长度：8位
		停止位：1位
		禁止硬件流控
		使能接收和发送
	*/
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART2, &USART_InitStructure);  
	//使能USART2，配置完毕
	USART_Cmd(USART2, ENABLE);
	//USART2中断配置
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
	//清空发送完成标志位，解决第一个字节无法发送的问题
	USART_ClearFlag(USART2, USART_FLAG_TC); 
}

void usart3_config()//串口3配置函数
{
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);//打开GPIO的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//打开USART3的时钟
	/*
	将USART3_TX的GPIO配置为推挽复用模式，配置TX为PB10
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*
	将USART3_RX的GPIO配置为浮空输入模式，配置RX为PB11
	*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	/*
	配置USART3的参数
		波特率：115200
		数据长度：8位
		停止位：1位
		禁止硬件流控
		使能接收和发送
	*/
	USART_InitStructure.USART_BaudRate = 19200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART3, &USART_InitStructure);  
	//使能USART3，配置完毕
	USART_Cmd(USART3, ENABLE);
	//USART3中断配置
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);
	//清空发送完成标志位，解决第一个字节无法发送的问题
	USART_ClearFlag(USART3, USART_FLAG_TC);
	USART3_Send(0xF5);
	//Send_FPorder(FP_empty);
}

void NVIC_Config(void)//串口中断配置
{
   	#ifdef VECT_TAB_RAM  
   	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
   	#else  
   	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
   	#endif 
   	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);
	/*
		串口1中断配置
	*/
   	NVIC_InitStructure_U1.NVIC_IRQChannel= USART1_IRQn;
  	NVIC_InitStructure_U1.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
  	NVIC_InitStructure_U1.NVIC_IRQChannelSubPriority = 1;//响应优先级1	
	NVIC_InitStructure_U1.NVIC_IRQChannelCmd = ENABLE;	
	/*
		串口2中断配置
	*/
   	NVIC_InitStructure_U2.NVIC_IRQChannel= USART2_IRQn;
  	NVIC_InitStructure_U2.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
  	NVIC_InitStructure_U2.NVIC_IRQChannelSubPriority = 2;//响应优先级2	
	NVIC_InitStructure_U2.NVIC_IRQChannelCmd = ENABLE;	
	/*
		串口3中断配置
	*/
   	NVIC_InitStructure_U3.NVIC_IRQChannel= USART3_IRQn;
  	NVIC_InitStructure_U3.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级0
  	NVIC_InitStructure_U3.NVIC_IRQChannelSubPriority = 3;//响应优先级3	
	NVIC_InitStructure_U3.NVIC_IRQChannelCmd = ENABLE;	
	/*
		中断使能
	*/
  	NVIC_Init(&NVIC_InitStructure_U1);
	NVIC_Init(&NVIC_InitStructure_U2);	 
	NVIC_Init(&NVIC_InitStructure_U3);	 
}

void USART1_Send(const unsigned char ch)//串口1发送
{
	USART_SendData(USART1,ch);
  	while(USART_GetFlagStatus(USART1,USART_FLAG_TC) == RESET);
}

void USART2_Send(const unsigned char ch)//串口2发送
{
	USART_SendData(USART2,ch);
  	while(USART_GetFlagStatus(USART2,USART_FLAG_TC) == RESET);
}

void USART3_Send(const unsigned char ch)//串口3发送
{
	USART_SendData(USART3,ch);
  	while(USART_GetFlagStatus(USART3,USART_FLAG_TC) == RESET);
}


void USART1_IRQHandler(void)
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART1_Res = USART_ReceiveData(USART1);
		if(USART1_Res == 0xFF)
			USART1_Send(0x01);	
	} 
}

void USART2_IRQHandler(void)
{
	short i;
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{
//		#ifdef DEBUG_H
//		STAT_DBG("IRQ2_get!\n");
//		#endif
		if(command_handle_state == COM_HAND || command_get_state == COM_GET)//处理器正在处理指令或已经存在指令，不进行接收
		{
			#ifdef DEBUG_H
			STAT_DBG("device busy\n");
			#endif
			send_singleReply(&reply_busy,0xFF);//向上位机发送忙指令
		}
		else
		{
			geted_order->code[0] = USART_ReceiveData(USART2);
			for(i = 1;i < 8;i ++)
			{
				while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == RESET);
				geted_order->code[i] = USART_ReceiveData(USART2);
			}
			command_get_state = COM_GET;//获取到数据，数据获取信号量置COM_GET
		}
	} 
}

void USART3_IRQHandler(void)
{
	short i;
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)
	{
		if(fingerprint_command_get_state == COM_GET)//如果已接受到指纹模块发送来的数据且未发送完毕，回复忙。
		{
			#ifdef DEBUG_H
			STAT_DBG("FingerPrint driveve busy\n");
			#endif
			send_singleReply(&reply_busy,0xFE);//向上位机发送忙指令
		}
		else
		{
			FP_Geted_order[0] = USART_ReceiveData(USART3);
			for(i = 1;i < 8;i ++)
			{
				while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == RESET);
				FP_Geted_order[i] = USART_ReceiveData(USART3);
			}
			fingerprint_command_get_state = COM_GET;//指纹模块信息接受信号量
		}
	} 
}



void USART1_IRQ(int swi)//串口1中断开关
{
	if(swi == IRQ_ENABLE)
		USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);	
	else if(swi == IRQ_DISABLE)
		USART_ITConfig(USART1,USART_IT_RXNE,DISABLE);
}

void USART2_IRQ(int swi)//串口2中断开关
{
	if(swi == IRQ_ENABLE)
		USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);	
	else if(swi == IRQ_DISABLE)
		USART_ITConfig(USART2,USART_IT_RXNE,DISABLE);
}

void USART3_IRQ(int swi)//串口3中断开关
{
	if(swi == IRQ_ENABLE)
		USART_ITConfig(USART3,USART_IT_RXNE,ENABLE);	
	else if(swi == IRQ_DISABLE)
		USART_ITConfig(USART3,USART_IT_RXNE,DISABLE);
}

unsigned char USART1_Recive(void)//串口1接收
{
	unsigned char ch;
	while(USART_GetFlagStatus(USART1,USART_FLAG_RXNE) == SET);
	ch = USART_ReceiveData(USART1);
	return ch;
}

unsigned char USART2_Recive(void)//串口2接收
{
	unsigned char ch;
	while(USART_GetFlagStatus(USART2,USART_FLAG_RXNE) == SET);
	ch = USART_ReceiveData(USART2);
	return ch;
}

unsigned char USART3_Recive(void)//串口3接收
{
	unsigned char ch;
	while(USART_GetFlagStatus(USART3,USART_FLAG_RXNE) == SET);
	ch = USART_ReceiveData(USART3);
	return ch;
}
