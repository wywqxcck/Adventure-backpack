#ifndef _GPS_//预编译
#define _GPS_
#include "sys.h"
#include "stdio.h"
#include "string.h"
#include "malloc.h"
#include "oled.h"
#include "LED.h"
#include "stdlib.h"
#include "stdbool.h"

//#define FALSE 0
//#define TRUE  1


typedef struct Data
{
	char GPS_Buffer[200];//数据接收	
	bool isGetData;//接收数据是否完成
	char *UTCTime;//时间戳
	char *latitude;//纬度
	char *N_S;//南北
	char *longitude;//经度
	char *E_W;//东西
	bool isParseData;//是否解析完成
	bool isUsefull;//是否为有效数据位
}Data;





void parseGpsBuffer(void);
void printfGpsBuffer(void);
#endif
