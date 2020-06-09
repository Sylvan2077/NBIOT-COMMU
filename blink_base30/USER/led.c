/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 *           ----------------- 
 * 库版本  ：ST3.5.0

**********************************************************************************/
#include "led.h"
unsigned char testxx=0;
/*
 * 函数名：LED_GPIO_Config
 * 描述  ：配置LED用到的I/O口
 */
void LED_GPIO_Config(void)
{		
	/*定义一个GPIO_InitTypeDef类型的结构体*/
	GPIO_InitTypeDef GPIO_InitStructure;

	/*开启GPIOC的外设时钟*/
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA, ENABLE); 
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB, ENABLE); 
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC, ENABLE); 

	  
  GPIO_InitStructure.GPIO_Pin =GPIO_Pin_13;         //  CS
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
}





/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
