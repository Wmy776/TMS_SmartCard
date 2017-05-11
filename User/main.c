#include "include.h"

unsigned char UIDbuffer[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char FPbuffer = 0x00;

void all_init(void)
{
	SystemInit();//系统初始化
	delay_init(72);//延迟初始化
	W25Q16_Init();//Epprom初始化
	#ifdef DEBUG_ON//如果开启调试初始化串口1
	usart1_config();
	#endif
	usart2_config();//串口2初始化
	usart3_config();//串口3初始化
	NVIC_Config();//中断初始化
	#ifdef DEBUG_ON//如果开启调试,开启串口1中断，否则关闭中断。
	USART1_IRQ(IRQ_ENABLE);
	#else
	USART1_IRQ(IRQ_DISABLE);
	#endif
	USART2_IRQ(IRQ_ENABLE);//串口2开中断
	USART3_IRQ(IRQ_ENABLE);//串口3开中断
	geted_order = (ORD *)malloc(sizeof(ORD));
	level = LV2;//将命令等级初始化为LV2
	userDataInit();//从epprom里获取绑定的用户ID
}

int main()
{
	short i;
	all_init();//初始化
	#ifdef DEBUG_ON//如果开启调试,开启串口1中断，否则关闭中断。
		STAT_DBG("Start\n");
	#endif
//	W25Q16_Write((u8*)UIDbuffer,0,sizeof(UIDbuffer));
//	STAT_DBG("OK1\n");
//	W25Q16_Write(&FPbuffer,6,sizeof(FPbuffer));
//	STAT_DBG("OK2\n");
//	Send_FPorder(FP_ask_unbind);
	for(i = 0;i < 6;i ++)
		USART2_Send(USER_ID[i]);
	delay_ms(1000);
		USART2_Send(FINGERPRINT_NUM);
	waitConnect();
	while(1)
	{
		anal_command();
	}	
	return 0;
}
