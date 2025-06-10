#include "GPS.h"


double latitude = 113.2254,longitude = 34.77854;
char * pEnd;
const short int USART2_MAX_RECV_LEN = 200;
char USAR2_RX_BUF[USART2_MAX_RECV_LEN];
const short int GPS_Buffer_Length = 200;
struct Data Save_Data;

void parseGpsBuffer(void){
	char *subString;
	char *subStringNext;
	int i = 0;
	if(Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
//		printf("*****************\r\n");
//		printf("%s",Save_Data.GPS_Buffer);
		
		for(i = 0; i <= 6; i++){
			if(i == 0){
				if((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					printf("解析错误");
				}
				else{
					subString++;//到达解析数据中逗号的下一位
					if((subStringNext = strstr(subString, ",")) != NULL)
					{
						char usefullBuffer[2];
						switch(i){
							case 1: 
								//利用subStringNext和subString的首地址相减来确定指针开辟空间的大小，以防指针不合法。
								Save_Data.UTCTime = (char *)mymalloc(SRAMIN,(subStringNext - subString)*sizeof(char));
								memcpy(Save_Data.UTCTime, subString, subStringNext - subString); 
								break;
							case 2: memcpy(usefullBuffer, subString, subStringNext - subString); break;
							case 3: Save_Data.latitude = (char *)mymalloc(SRAMIN,(subStringNext - subString)*sizeof(char));
								memcpy(Save_Data.latitude, subString, subStringNext - subString); 
								break;
							case 4: Save_Data.N_S = (char *)mymalloc(SRAMIN,(subStringNext - subString)*sizeof(char));
								memcpy(Save_Data.N_S, subString, subStringNext - subString); 
								break;
							case 5: Save_Data.longitude = (char *)mymalloc(SRAMIN,(subStringNext - subString)*sizeof(char));
								memcpy(Save_Data.longitude, subString, subStringNext - subString); 
								break;
							case 6: Save_Data.E_W = (char *)mymalloc(SRAMIN,(subStringNext - subString)*sizeof(char));
								memcpy(Save_Data.E_W, subString, subStringNext - subString); 
								break;
							default: break;
						}
						subString = subStringNext;
						Save_Data.isParseData = true;
						if(usefullBuffer[0] == 'A')
							Save_Data.isUsefull = true;
						else if(usefullBuffer[0] == 'V')
							Save_Data.isUsefull = false;
					}
					else{
							printf("解析错误2");
					}
				}
			}
		}
}


void printfGpsBuffer(void){
	if(Save_Data.isParseData){
		Save_Data.isParseData = false;
//		printf("Save_Data.UTCTime = %s\r\n", Save_Data.UTCTime);//打印数据
		myfree(SRAMIN,Save_Data.UTCTime);//释放空间
		
		if(Save_Data.isUsefull){
			Save_Data.isUsefull = false;
//			printf("Save_Data.latitude = %s\r\n", Save_Data.latitude);
			latitude = strtod(Save_Data.latitude,&pEnd);
			myfree(SRAMIN,Save_Data.latitude);
//			printf("Save_Data.N_S = %s\r\n", Save_Data.N_S);
			myfree(SRAMIN,Save_Data.N_S);
//			printf("Save_Data.longitude = %s\r\n", Save_Data.longitude);
			longitude = strtod(Save_Data.longitude,&pEnd);
			myfree(SRAMIN,Save_Data.longitude);
//			printf("Save_Data.E_W = %s\r\n", Save_Data.E_W);
			myfree(SRAMIN,Save_Data.E_W);
		}
		else
		{
			printf("GPS DATA is not usefull!\r\n");
		}
	}
}

