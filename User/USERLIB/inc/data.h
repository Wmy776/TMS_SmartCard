/*
	TMS智能卡系统自定义库
		修订与2017,3,29
			库版本Ver.Alpha
				部分代码修订自正点原子若干内容，感谢正点原子
					任何问题请联系email：784485305@qq.com
*/
#ifndef _DATA_H_
#define _DATA_H_
/*
	DEBUG错误类型定义
*/
#define GET_TYPE_ERR  0xFF

/*
	LEVEL权限定义
*/
#define LV2 0x02
#define LV1 0x01
#define LV0 0x00
/*
	中断定义
*/
#define IRQ_ENABLE	0xFF //开子中断宏
#define IRQ_DISABLE	0x00 //关子中断宏
#define irq_allEnable()		__set_PRIMASK(1) //全局中断开  
#define irq_allDisable()	__set_PRIMASK(0) //全局中断关

/*
	信号量真假值定义
*/

#define COM_GET 	0xFF	//接收到命令
#define NOCOM_GET 	0x00	//没有接收到命令
#define COM_HAND    0xFF	//正在处理指令
#define NOCOM_HAND	0x00	//现在没有指令正在被处理


//IO口操作，只针对单一IO口！
//确保n的值小于16
#define PAout(n)   BIT_ADDR(GPIOA_ODR_Addr,n)  //输入
#define PAin(n)    BIT_ADDR(GPIOA_IDR_Addr,n)  //输出

#define PBout(n)   BIT_ADDR(GPIOB_ODR_Addr,n)  //输入
#define PBin(n)    BIT_ADDR(GPIOB_IDR_Addr,n)  //输出

#define PCout(n)   BIT_ADDR(GPIOC_ODR_Addr,n)  //输入
#define PCin(n)    BIT_ADDR(GPIOC_IDR_Addr,n)  //输出

#define PDout(n)   BIT_ADDR(GPIOD_ODR_Addr,n)  //输入
#define PDin(n)    BIT_ADDR(GPIOD_IDR_Addr,n)  //输出

#define PEout(n)   BIT_ADDR(GPIOE_ODR_Addr,n)  //输入
#define PEin(n)    BIT_ADDR(GPIOE_IDR_Addr,n)  //输出

#define PFout(n)   BIT_ADDR(GPIOF_ODR_Addr,n)  //输入
#define PFin(n)    BIT_ADDR(GPIOF_IDR_Addr,n)  //输出

#define PGout(n)   BIT_ADDR(GPIOG_ODR_Addr,n)  //输入
#define PGin(n)    BIT_ADDR(GPIOG_IDR_Addr,n)  //输出

//IO口地址映射
#define GPIOA_ODR_Addr    (GPIOA_BASE+12) //0x4001080C 
#define GPIOB_ODR_Addr    (GPIOB_BASE+12) //0x40010C0C 
#define GPIOC_ODR_Addr    (GPIOC_BASE+12) //0x4001100C 
#define GPIOD_ODR_Addr    (GPIOD_BASE+12) //0x4001140C 
#define GPIOE_ODR_Addr    (GPIOE_BASE+12) //0x4001180C 
#define GPIOF_ODR_Addr    (GPIOF_BASE+12) //0x40011A0C    
#define GPIOG_ODR_Addr    (GPIOG_BASE+12) //0x40011E0C    

#define GPIOA_IDR_Addr    (GPIOA_BASE+8) //0x40010808 
#define GPIOB_IDR_Addr    (GPIOB_BASE+8) //0x40010C08 
#define GPIOC_IDR_Addr    (GPIOC_BASE+8) //0x40011008 
#define GPIOD_IDR_Addr    (GPIOD_BASE+8) //0x40011408 
#define GPIOE_IDR_Addr    (GPIOE_BASE+8) //0x40011808 
#define GPIOF_IDR_Addr    (GPIOF_BASE+8) //0x40011A08 
#define GPIOG_IDR_Addr    (GPIOG_BASE+8) //0x40011E08 

//IO口操作宏定义，具体思想请参考《CM3权威指南》第五章(87~92页)
#define BITBAND(addr, bitnum) ((addr & 0xF0000000)+0x2000000+((addr &0xFFFFF)<<5)+(bitnum<<2)) 
#define MEM_ADDR(addr)  *((volatile unsigned long  *)(addr)) 
#define BIT_ADDR(addr, bitnum)   MEM_ADDR(BITBAND(addr, bitnum)) 


#endif