/*
	TMS智能卡SPI支持库
		修订与2017,4,3
			库版本Ver.Alpha
				此库由正点原子SPI支持库修订而来，感谢正点原子
					任何问题请联系email：784485305@qq.com
*/
#ifndef SPI_H
#define SPI_H

void SPI2_Init(void);			 //初始化SPI2
void SPI2_SetSpeed(u8 SpeedSet); //设置SPI速度
u8 SPI2_ReadWriteByte(u8 TxData);//SPI总线读写一个字符

#endif