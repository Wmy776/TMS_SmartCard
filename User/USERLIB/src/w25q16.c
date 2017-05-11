/*
	TMS智能卡用户信息库
		修订与2017,4,3
			库版本Ver.Alpha
				此库由正点原子W25QXX支持库修订而来，感谢正点原子
					任何问题请联系email：784485305@qq.com
*/

#include "include.h"

u8 W25Q16_BUFFER[4096];	

void W25Q16_Init(void)
{	
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOB, ENABLE );//GPIOB使能

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;//PB12设置
 	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;//PB12设置推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOB, &GPIO_InitStructure);//使能PB12
 	GPIO_SetBits(GPIOB,GPIO_Pin_12);//上拉
	W25Q16_CS=1;//SPI FLASH不选中
	SPI2_Init();//初始化SPI2
	SPI2_SetSpeed(SPI_BaudRatePrescaler_2);//设置为18M时钟，高速模式
} 


/*
		读取W25Q16的状态寄存器
		BIT7  6   5   4   3   2   1   0
		SPR   RV  TB BP2 BP1 BP0 WEL BUSY
		SPR:默认0，章台寄存器保护位，配合WP使用
		TB,BP2,BP1,BP0:FLASH区域写保护设置
		WEL:写使能锁定
		BUSY：忙标记位(1忙;0空闲)
		默认:0x00
*/
u8 W25Q16_ReadSR(void)   
{  
	u8 byte=0;   
	W25Q16_CS=0;                            //使能器件
	SPI2_ReadWriteByte(W25X_ReadStatusReg); //发送读取状态寄存器指令    
	byte=SPI2_ReadWriteByte(0Xff);          //读取一个字节
	W25Q16_CS=1;                            //取消片选 
	return byte;
} 

/*
		写W25Q16的状态寄存器
		只有SPR,TB,BP2,BP1,BP0(bit 7,5,4,3,2)可写
*/


void W25Q16_Write_SR(u8 sr)   
{   
	W25Q16_CS=0;                            //使能器件 
	SPI2_ReadWriteByte(W25X_WriteStatusReg);//发送写取状态寄存器命令
	SPI2_ReadWriteByte(sr);               	//写入一个字节
	W25Q16_CS=1;                            //取消片选 	      
} 


/*
		W25Q16写使能
		将WEL置位
*/
void W25Q16_Write_Enable(void)   
{
	W25Q16_CS=0;                          	//使能器件
    SPI2_ReadWriteByte(W25X_WriteEnable); 	//发送写使能
	W25Q16_CS=1;                           	//取消片选   	      
} 

/*
		W25Q16禁止
		将WEL置位
*/
void W25Q16_Write_Disable(void)   
{
	W25Q16_CS=0;                          	//使能器件
    SPI2_ReadWriteByte(W25X_WriteDisable); 	//发送写使能
	W25Q16_CS=1;                           	//取消片选   	      
} 



/*
		读取SPI FLASH
		在指定地址开始读取指定长度的数据
		pBuffer:数据存储区
		ReadAddr:开始读取的地址(24bit)
		NumByteToRead:要读取的字节数(最大65535)
*/
void W25Q16_Read(u8* pBuffer,u32 ReadAddr,u16 NumByteToRead)   
{ 
 	u16 i;   										    
	W25Q16_CS=0;                            	//使能器件 
    SPI2_ReadWriteByte(W25X_ReadData);         	//发送读取命令 
    SPI2_ReadWriteByte((u8)((ReadAddr)>>16));  	//发送24bit地址   
    SPI2_ReadWriteByte((u8)((ReadAddr)>>8));   
    SPI2_ReadWriteByte((u8)ReadAddr);   
    for(i=0;i<NumByteToRead;i++)
	{ 
        pBuffer[i]=SPI2_ReadWriteByte(0XFF);   	//循环读数 
    }
	W25Q16_CS=1;  				    	      
}


/*
		SPI在一页(0~65535)内写入少于256个字节的数据
		在指定地址开始写入最大256字节的数据
		pBuffer：数据存储区
		WriteAddr：开始写入的地址(24bit)
		NumByteToWrite:要写入的字节数(最大256)，该数不应该超过该页的剩余字节数
*/
void W25Q16_Write_Page(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)
{
 	u16 i;  
    W25Q16_Write_Enable();                  	//SET WEL 
	W25Q16_CS=0;                            	//使能器件
    SPI2_ReadWriteByte(W25X_PageProgram);      	//发送写页指令  
    SPI2_ReadWriteByte((u8)((WriteAddr)>>16)); 	//发送24bit地址  
    SPI2_ReadWriteByte((u8)((WriteAddr)>>8));   
    SPI2_ReadWriteByte((u8)WriteAddr);   
    for(i=0;i<NumByteToWrite;i++)SPI2_ReadWriteByte(pBuffer[i]);//循环写数
	W25Q16_CS=1;                            	//取消片选
	W25Q16_Wait_Busy();					   		//等待写入结束
} 

/*
		无检验写SPI FLASH
		必须确保所写的地址范围内的数据全部为0xFF，否则在非0xFF处写入的数据将失败！
		具有自动换页的功能
		在指定地址开始雪茹指定长度的数据，但是要确保地址不越界
		pBuffer：数据存储区
		WriteAddress：开始写入的地址(24bit)
		NumByteToWrite:要写入的字节数(最大65535)
		CHECK OK
*/
void W25Q16_Write_NoCheck(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 			 		 
	u16 pageremain;	   
	pageremain=256-WriteAddr%256; //单页剩余的字节数	 	    
	if(NumByteToWrite<=pageremain)pageremain=NumByteToWrite;//不大于256个字节
	while(1)
	{	   
		W25Q16_Write_Page(pBuffer,WriteAddr,pageremain);
		if(NumByteToWrite==pageremain)break;//写入结束了
	 	else //NumByteToWrite>pageremain
		{
			pBuffer+=pageremain;
			WriteAddr+=pageremain;	

			NumByteToWrite-=pageremain;			  //减去已经写入的字节数
			if(NumByteToWrite>256)pageremain=256; //一次可以写入256个字节
			else pageremain=NumByteToWrite; 	  //不够256个字节了
		}
	}	    
}

/*
		写SPI FLASH
		在指定地址开始写入指定长度的数据
		该函数带擦除操作！
		pBuffer：数据存储区
		WriteAddress：开始写入的地址(24bit)
		NumByteToWrite:要写入的字节数(最大65535)
*/
void W25Q16_Write(u8* pBuffer,u32 WriteAddr,u16 NumByteToWrite)   
{ 
	u32 secpos;
	u16 secoff;
	u16 secremain;	   
 	u16 i;    
	u8 * W25QXX_BUF;	  
   	W25QXX_BUF=W25Q16_BUFFER;	     
 	secpos=WriteAddr/4096;//扇区地址
	secoff=WriteAddr%4096;//在扇区内的偏移
	secremain=4096-secoff;//扇区剩余空间大小 
 	//printf("ad:%X,nb:%X\r\n",WriteAddr,NumByteToWrite);//测试用
 	if(NumByteToWrite<=secremain)secremain=NumByteToWrite;//不大于4096个字节
	while(1) 
	{	
		W25Q16_Read(W25QXX_BUF,secpos*4096,4096);//读出整个扇区内容
		for(i=0;i<secremain;i++)//校验数据
		{
			if(W25QXX_BUF[secoff+i]!=0XFF)break;//需要擦除 
		}
		if(i<secremain)//需要擦除
		{
			W25Q16_Erase_Sector(secpos);		//擦除这个扇区
			for(i=0;i<secremain;i++)	   		//复制
			{
				W25QXX_BUF[i+secoff]=pBuffer[i];	  
			}
			W25Q16_Write_NoCheck(W25QXX_BUF,secpos*4096,4096);//写入整个扇区 

			}else W25Q16_Write_NoCheck(pBuffer,WriteAddr,secremain);//写入已经擦出的，直接写入扇区剩余区间				   
		if(NumByteToWrite==secremain)break;//写入结束
		else//写入未结束
		{
			secpos++;//扇区地址增1
			secoff=0;//偏移位置为0 

		   	pBuffer+=secremain;  				//指针偏移
			WriteAddr+=secremain;				//写地址偏移	   
		   	NumByteToWrite-=secremain;			//字节数递减
			if(NumByteToWrite>4096)secremain=4096;//下一个扇区还是写不完
			else secremain=NumByteToWrite;		//下一个扇区可以写完了
		}	 
	}	 
}
/*
		擦除整个芯片，等待时间很长很长
*/
void W25Q16_Erase_Chip(void)   
{                                   
    W25Q16_Write_Enable();                 	 	//SET WEL 
    W25Q16_Wait_Busy();   
  	W25Q16_CS=0;                            	//使能器件 
    SPI2_ReadWriteByte(W25X_ChipErase);        	//发送片擦除命令 
	W25Q16_CS=1;                            	//取消片选    	      
	W25Q16_Wait_Busy();   				   		//等待芯片擦除完毕
} 


/*
		擦除一个扇区
		Dst_Addr:扇区地址 根据实际容量设置
		擦除一个扇区的最招时间:150ms
*/
void W25Q16_Erase_Sector(u32 Dst_Addr)   
{  
 	//printf("fe:%x\r\n",Dst_Addr);//监视flash擦除情况，测试用	  
 	Dst_Addr*=4096;
    W25Q16_Write_Enable();                  	//SET WEL 	 
    W25Q16_Wait_Busy();   
  	W25Q16_CS=0;                            	//使能器件
    SPI2_ReadWriteByte(W25X_SectorErase);      	//发送扇区擦除指令
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>16));  	//发送24bit地址	
    SPI2_ReadWriteByte((u8)((Dst_Addr)>>8));   
    SPI2_ReadWriteByte((u8)Dst_Addr);  
	W25Q16_CS=1;                            	//取消片选   	      
    W25Q16_Wait_Busy();   				   		//等待擦除完成
}


//等待空闲
void W25Q16_Wait_Busy(void)   
{   
	while((W25Q16_ReadSR()&0x01)==0x01);  		//等待BUSY为自动清空
}

//进入掉电模式
void W25Q16_PowerDown(void)   
{ 
  	W25Q16_CS=0;                           	 	//使能器件
    SPI2_ReadWriteByte(W25X_PowerDown);        //发送掉电命令
	W25Q16_CS=1;                            	//取消片选    	      
    delay_us(3);                               //等待TPD
} 


//唤醒
void W25Q16_WAKEUP(void)   
{ 
  	W25Q16_CS=0;                           	 	//使能器件
    SPI2_ReadWriteByte(W25X_ReleasePowerDown);  //发送掉电命令
	W25Q16_CS=1;                            	//取消片选    	      
    delay_us(3);                                //等待TPD
} 





