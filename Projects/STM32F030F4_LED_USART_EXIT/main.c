//=============================================================================
//�ļ����ƣ�main.h
//���ܸ�Ҫ��STM32F030F4���İ� EXIT����
//�汾���£�2013-12-20
//���Է�ʽ��J-Link OB ARM SWD
//=============================================================================

//ͷ�ļ�
#include "stm32f0xx.h"
#include "LED.h"
#include "USART1.h"
#include "KEY.h"      // KEY����ͷ�ļ�
#include "mems.h"
//��������

//=============================================================================
//�ļ����ƣ�Delay
//���ܸ�Ҫ����ʱ����
//����˵������
//�������أ���
//=============================================================================
void  Delay (uint32_t nCount)
{
  for(; nCount != 0; nCount--);
}

//=============================================================================
//�ļ����ƣ�main
//���ܸ�Ҫ��������
//����˵������
//�������أ�int
//=============================================================================
int main(void)
{
  /* LED��ʼ�� */
  LED_Init();
  /* USART1��ʼ�� */
  //  USART1_Init();	
  //  printf("��ӭʹ��Դ��YD-STM32F030F4���İ�\r\n"); 
  //  printf("�����ַhttp://vcc-gnd.taobao.com\r\n");
  //  printf("PA7��ʱ������Ϊ�ж��������ţ���PA7�ӵأ��½��أ������һ���ж�\r\n"); 
  KEY_EXTI_Init();
  MemsOpen();
  while(1)
  { 
    Delay(5000000);
    LED_ON();
  }
}

/*****END OF FILE****/