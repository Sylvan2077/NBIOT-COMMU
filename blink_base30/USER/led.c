/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 *           ----------------- 
 * ��汾  ��ST3.5.0

**********************************************************************************/
#include "led.h"
unsigned char testxx=0;
/*
 * ��������LED_GPIO_Config
 * ����  ������LED�õ���I/O��
 */
void LED_GPIO_Config(void)
{		
	/*����һ��GPIO_InitTypeDef���͵Ľṹ��*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*����GPIOC������ʱ��*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 

	  
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;         //  CS
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}





/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
