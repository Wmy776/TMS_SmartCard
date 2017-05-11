/*
	TMS智能卡用户信息库
		修订与2017,4,3
			库版本Ver.Alpha
				任何问题请联系email：784485305@qq.com
	注意：
	1、用户信息均保存在板载flash内，若无绑定则为初始值
	机器启动时会从flash中获取相应信息。
*/

#include "include.h"

unsigned char USER_ID[6];//用户ID
unsigned char GETED_ID[6];//获取到的用户ID
const unsigned char EMPTY_ID[6] = {0x00,0x00,0x00,0x00,0x00,0x00};//空命令
unsigned char EMPTY_FINGERPRINT_NUM = 0x00;
unsigned char FULL_FINGERPRINT_NUM = 0x01;
unsigned char FINGERPRINT_NUM;//指纹数量

/*
	用户信息初始化
	从板载flash中获取保存的用户信息，其中有用户ID等等
*/
void userDataInit(void)//用户信息初始化
{				
	#ifdef DEBUG_ON
	STAT_DBG("UID began read.\n");
	#endif
	W25Q16_Read(USER_ID,USER_ID_ADDR,USER_ID_SIZE);
	#ifdef DEBUG_ON
	STAT_DBG("UID read success!\n");
	#endif
	#ifdef DEBUG_ON
	STAT_DBG("FingerPrint num began read.\n");
	#endif
	W25Q16_Read(&FINGERPRINT_NUM,FINGERPRINT_NUM_ADDR,FINGERPRINT_NUM_SIZE);
	#ifdef DEBUG_ON
	STAT_DBG("FingerPrint num read success!\n");
	#endif
}
/*
	用户信息比对
	用
*/
int UID_cmp(const char *UID1,const char *UID2)//UID比对
{
	short i;
	for(i = 0;i < 6;i ++)
	{
		if(UID1[i] != UID2[i])
			return 0;
	}
	return 1;
}