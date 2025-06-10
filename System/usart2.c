/********************************Copyright (c)**********************************\
**
**                   (c) Copyright 2019, temp_duan, China, DY.
**                           All Rights Reserved
**
**                           By(德阳科派科技有限公司)
**                             http://www.kpiot.top
**
**----------------------------------文件信息------------------------------------
** 文件名称: usart2.c
** 创建人员: temp_duan
** 创建日期: 2019-08-01
** 文档描述: 
**
**----------------------------------版本信息------------------------------------
** 版本代号: V0.1
** 版本说明: 初始版本
**
**------------------------------------------------------------------------------
\********************************End of Head************************************/

#include "usart2.h"
/*******************************************************************************
** 函数名称: usart2_init
** 功能描述: 
** 参数说明: bound: [输入/出] 
** 返回说明: None
** 创建人员: temp_duan
** 创建日期: 2019-08-01
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/
void usart2_init(u32 bound)
{

    NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	                       //GPIOA时钟
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);                         //串口2时钟使能

    //USART2_TX   PA2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;                                     //PA2
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	                               //复用推挽输出
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA2

    //USART2_RX	  PA3
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;                          //浮空输入
    GPIO_Init(GPIOA, &GPIO_InitStructure);                                         //初始化PA3

    USART_InitStructure.USART_BaudRate = bound;                                    //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;                    //字长为8位数据格式
    USART_InitStructure.USART_StopBits = USART_StopBits_1;                         //一个停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;                            //无奇偶校验位
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	               //收发模式

    USART_Init(USART2, &USART_InitStructure);       //初始化串口2


    USART_Cmd(USART2, ENABLE);                      //使能串口

    //使能接收中断
    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  //开启中断

    //设置中断优先级
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0 ; //抢占优先级0
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级1
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
    NVIC_Init(&NVIC_InitStructure);	//根据指定的参数初始化VIC寄存器

}

/*******************************************************************************
** 函数名称: USART2_IRQHandler
** 功能描述: 
** 参数说明: None
** 返回说明: None
** 创建人员: temp_duan
** 创建日期: 2019-08-01
**------------------------------------------------------------------------------
** 修改人员:
** 修改日期:
** 修改描述:
**------------------------------------------------------------------------------
********************************************************************************/
extern short int point1;
extern const short int USART2_MAX_RECV_LEN;
extern char USAR2_RX_BUF[200];
extern const short int GPS_Buffer_Length;
short int point1;
extern struct Data Save_Data;

void USART2_IRQHandler(void){
	u8 res;
	if(USART_GetITStatus(USART2, USART_IT_RXNE))
	{
			res = USART_ReceiveData(USART2);
			if(res == '$'){
				point1 = 0;
			}
			USAR2_RX_BUF[point1++] = res;
			if(USAR2_RX_BUF[0] == '$' && USAR2_RX_BUF[4] == 'M' && USAR2_RX_BUF[5] == 'C'){
				if(res == '\n'){
					memcpy(Save_Data.GPS_Buffer, USAR2_RX_BUF, point1);
					Save_Data.isGetData = true;
					point1 = 0;
					memset(USAR2_RX_BUF, 0, USART2_MAX_RECV_LEN);//????êy×é
					
					parseGpsBuffer();//?a??êy?Y
					printfGpsBuffer();//′òó?êy?Y
				}
			}
			if(point1 >= USART2_MAX_RECV_LEN)
			{
				point1 = USART2_MAX_RECV_LEN;
			}
	}
}

//int fputc(int ch,FILE *p)  //函数默认的，在使用printf函数时自动调用
//{
//	USART_SendData(USART2,(u8)ch);	
//	while(USART_GetFlagStatus(USART2,USART_FLAG_TXE)==RESET);
//	return ch;
//}
