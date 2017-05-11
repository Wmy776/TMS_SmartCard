/*
	TMS智能卡用户信息库
		修订与2017,4,3
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
	注意：
	1、用户信息均保存在板载flash内，若无绑定则为初始值
	机器启动时会从flash中获取相应信息。
*/

#ifndef _USER_H_
#define _USER_H_

#define USER_ID_ADDR 0x00000//用户ID在epprom中的地址
#define FINGERPRINT_NUM_ADDR 0x00006//指纹数量在epprom中的地址
#define FINGERPRINT_NUM_SIZE sizeof(unsigned char)
#define USER_ID_SIZE sizeof(USER_ID)//用户ID的大小

extern unsigned char USER_ID[6];//用户ID
extern unsigned char GETED_ID[6];//获取到的用户ID
extern unsigned char FINGERPRINT_NUM;//指纹数量
extern const unsigned char EMPTY_ID[6];//空用户
extern unsigned char EMPTY_FINGERPRINT_NUM;//空指纹数量
extern unsigned char FULL_FINGERPRINT_NUM;//满指纹数量

void userDataInit(void);//用户信息初始化
int UID_cmp(const char *UID1,const char *UID2);//UID比对

#endif