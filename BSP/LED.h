//=============================================================================
//文件名称：LED.h
//功能概要：LED驱动头文件
//更新时间：2013-12-31
//=============================================================================
#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

//#define LED_OFF                       GPIOA->BSRR = 0x04
//#define LED_ON                        GPIOA->BRR = 0x04
//#define LED_TURN                      GPIOA->ODR ^= 0x04
#define LED_GPIO_CLK   RCC_AHBPeriph_GPIOA 
#define LED_PORT   	   GPIOA
#define LED_PIN        GPIO_Pin_4

void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);

#endif
