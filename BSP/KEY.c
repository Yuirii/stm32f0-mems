//=============================================================================
//文件名称：KEY.c
//功能概要：KEY驱动文件
//更新时间：2013-11-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

#include "KEY.h"
#include "USART1.h"   // USASRT1驱动模块头
#include "LED.h"

void usDelay1(volatile uint16_t delay);

void KEY_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
          
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
          
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
}


void KEY_EXTI_Init(void)
{
     GPIO_InitTypeDef GPIO_InitStruct; 
     EXTI_InitTypeDef EXTI_InitStruct;
     NVIC_InitTypeDef NVIC_InitStruct;

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
     RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
              
              
     GPIO_InitStruct.GPIO_Pin = GPIO_Pin_1; 
     GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
     GPIO_InitStruct.GPIO_Speed = GPIO_Speed_Level_2;
     GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_UP; 
     GPIO_Init(GPIOA, &GPIO_InitStruct);

     SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource1);

              
     EXTI_ClearITPendingBit(EXTI_Line1);

              
     EXTI_InitStruct.EXTI_Line = EXTI_Line1;
     EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
     EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
     EXTI_InitStruct.EXTI_LineCmd = ENABLE;
     EXTI_Init(&EXTI_InitStruct); 

     NVIC_InitStruct.NVIC_IRQChannel = EXTI0_1_IRQn;
     NVIC_InitStruct.NVIC_IRQChannelPriority = 0x01;
     NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
     NVIC_Init(&NVIC_InitStruct);
}



//=============================================================================
//文件名称：EXTI0_1_IRQHandler
//功能概要：按键/外部中断函数
//参数说明：无
//函数返回：无
//=============================================================================
void EXTI0_1_IRQHandler(void)
{
          LED_OFF();
          usDelay1(2000);
	if ( EXTI_GetITStatus(EXTI_Line1)!= RESET ) 
	{	

		printf("PA1响应一次下降沿触发中断\r\n");
		EXTI_ClearITPendingBit(EXTI_Line1);
	}		
	
	
}	
	
void usDelay1(volatile uint16_t delay)
{
  volatile uint16_t i;
  while(delay--)
  {
    i = 500;
    for(;i>0;i--);
  }
}
