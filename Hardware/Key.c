#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "stdio.h"

void Key_Init(void)
{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOB, &GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(GPIOA, &GPIO_InitStructure);
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0);
		Delay_ms(20);
		KeyNum = 1;
		printf("1\r\n");
	}
	if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0)
	{
		Delay_ms(20);
		while (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_1) == 0);
		Delay_ms(20);
		KeyNum = 2;
		printf("2\r\n");
	}

	return KeyNum;
}
