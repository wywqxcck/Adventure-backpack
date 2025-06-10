#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include "Delay.h"
#include "AD.h"
#include "Buzzer.h"
#include "DHT11.h"
#include "usart.h"
#include "Key.h"
#include "stdbool.h"
#include "malloc.h"
#include "usart2.h"
#include "GPS.h"
#include "math.h"
#include "gizwits_protocol.h"
#include "gizwits_product.h"
#include "common.h"
#include "usart3.h"
#include "Timer.h"
#include "max30102.h"

uint8_t oledID = 0;
uint8_t humidity = 0;
uint8_t tempreture = 0;
uint8_t temp_limit=5;
bool model = true;
float ppm;

extern double latitude,longitude;
extern u8 dis_hr_upload,dis_spo2_upload;

void oled_show(void);
void mq2_deal(void);
void key_deal(void);
void dht11_get(void);

void app_init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
		
		uart_init(115200);
		usart3_init(9600);
		usart2_init(9600);
		TIM1_Int_Init(9,7199);
	
		userInit();
    gizwitsInit();//机智云初始化

//		Delay_ms(2000);
//		gizwitsSetMode(WIFI_AIRLINK_MODE);
		my_mem_init(SRAMIN);

		MAX30102_Init();
		max_init();
		AD_Init();
		Buzzer_Init();
		DHT11_Init();
		Key_Init();
		OLED_Init();
		Heater_OFF();
}

void app_run(void)
{
		max_show();
		mq2_deal();
		oled_show();
		key_deal();

		userHandle();//数据上行
		gizwitsHandle((dataPoint_t *)&currentDataPoint);//机智云包
}

void mq2_deal(void)
{
		uint16_t temp = AD_GetValue();
		float vol = (float)temp*(3.3/4096);
    float R0=6.64;;
		float RS=(5-vol)/vol*0.5;
		
		ppm=pow(11.5428*R0/RS,0.6549f)*100;        //放大100倍
		
		if(temp >= 1100)
			Buzzer_ON();
		else
			Buzzer_OFF();			
		if(!model)
		{
				if(tempreture <= temp_limit)
						Heater_ON();
				else
						Heater_OFF();
		}
}

void key_deal(void)
{
		uint8_t temp;
		temp = Key_GetNum();

		switch(temp)
		{
			case 0:break;
			case 1:model = !model;break;
			case 2:if(model) Heater_Turn();break;
		}
}

void oled_show(void)
{
	DHT11_Read_Data(&tempreture, &humidity);
	OLED_ShowString(0,0,"T-H:",16);
	OLED_ShowNum(30,0,tempreture,2,16);
	OLED_ShowString(45,0,"-",16);
	OLED_ShowNum(55,0,humidity,2,16);
	OLED_ShowString(80,0,"P:",16);
	OLED_ShowNum(95,0,ppm,4,16);
	
	OLED_ShowString(0,2,"HR:",16);
	OLED_ShowString(65,2,"Sp:",16);
	OLED_ShowNum(30,2,dis_hr_upload,3,16);
	OLED_ShowNum(90,2,dis_spo2_upload,3,16);

	
	char arr[100];
	sprintf(arr,"%.2f",latitude);
	OLED_ShowString(0,4,"lati:",16);
	OLED_ShowString(40,4,(char*)arr,16);
	sprintf(arr,"%.2f",longitude);
	OLED_ShowString(0,6,"long:",16);	
	OLED_ShowString(40,6,(char*)arr,16);	

	if(model)
		OLED_ShowString(110,6,"Y",16);
	else
		OLED_ShowString(110,6,"N",16);
	
}
