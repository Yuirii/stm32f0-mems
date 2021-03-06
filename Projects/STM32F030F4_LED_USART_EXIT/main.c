//=============================================================================
//文件名称：main.h
//功能概要：STM32F030F4核心板 EXIT测试
//版本更新：2013-12-20
//调试方式：J-Link OB ARM SWD
//=============================================================================

//头文件
#include "stm32f0xx.h"
#include "LED.h"
#include "USART1.h"
#include "KEY.h"      // KEY驱动头文件
#include "mems.h"
//函数声明

//=============================================================================
//文件名称：Delay
//功能概要：延时函数
//参数说明：无
//函数返回：无
//=============================================================================
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

//=============================================================================
//文件名称：main
//功能概要：主函数
//参数说明：无
//函数返回：int
//=============================================================================
int main(void)
{
  /* LED初始化 */
  LED_Init();
  /* USART1初始化 */
  //  USART1_Init();	
  //  printf("欢迎使用源地YD-STM32F030F4核心板\r\n"); 
  //  printf("网店地址http://vcc-gnd.taobao.com\r\n");
  //  printf("PA7这时被设置为中断输入引脚，将PA7接地（下降沿）会产生一个中断\r\n"); 
  KEY_EXTI_Init();
  MemsOpen();
  while(1)
  { 
    Delay(5000000);
    LED_ON();
  }
}

/*****END OF FILE****/
