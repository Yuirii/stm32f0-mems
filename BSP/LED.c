//=============================================================================
//�ļ����� LED.c
//���ܸ�Ҫ��LED�����ļ�
//����ʱ�䣺2014-01-04
//=============================================================================

#include "LED.h"

void LED_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;
  
  /* ʹ��GPIOAʱ�� */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  /* ����LED��Ӧ����PA4*/
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
//  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

void LED_ON(void)
{
		GPIO_ResetBits(LED_PORT, LED_PIN);
}

/****************************************************
�������ܣ�LED��
�����������
�����������
��    ע�����ô˺���ǰ����Ҫ��LED.h�޸ĺ궨��LED����
****************************************************/
void LED_OFF(void)
{
		GPIO_SetBits(LED_PORT, LED_PIN);
}





