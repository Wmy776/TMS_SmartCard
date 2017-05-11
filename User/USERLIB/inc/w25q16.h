/*
	TMS智能卡用户信息库
		修订与2017,4,3
			库版本Ver.Alpha
				此库由正点原子W25QXX支持库修订而来，感谢正点原子
					任何问题请联系email：784485305@qq.com
*/
#ifndef W25Q16_H
#define W25Q16_H

#define	W25Q16_CS 		PBout(12)//w25q16的片选信号

//指令表
#define W25X_WriteEnable		0x06 
#define W25X_WriteDisable		0x04 
#define W25X_ReadStatusReg		0x05 
#define W25X_WriteStatusReg		0x01 
#define W25X_ReadData			0x03 
#define W25X_FastReadData		0x0B 
#define W25X_FastReadDual		0x3B 
#define W25X_PageProgram		0x02 
#define W25X_BlockErase			0xD8 
#define W25X_SectorErase		0x20 
#define W25X_ChipErase			0xC7 
#define W25X_PowerDown			0xB9 
#define W25X_ReleasePowerDown	0xAB 
#define W25X_DeviceID			0xAB 
#define W25X_ManufactDeviceID	0x90 
#define W25X_JedecDeviceID		0x9F 

void W25Q16_Init(void);
u8	 W25Q16_ReadSR(void);        		//读取状态寄存器 
void W25Q16_Write_SR(u8 sr);  			//写状体寄存器
void W25Q16_Write_Enable(void);  		//写使能
void W25Q16_Write_Disable(void);		//写保护
void W25Q16_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);
void W25Q16_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead);   //读取flash
void W25Q16_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite);//写入flash
void W25Q16_Erase_Chip(void);    	  	//整片擦除
void W25Q16_Erase_Sector(u32 Dst_Addr);	//扇区擦除
void W25Q16_Wait_Busy(void);           	//等待空闲
void W25Q16_PowerDown(void);        	//进入掉电模式
void W25Q16_WAKEUP(void);				//唤醒

#endif