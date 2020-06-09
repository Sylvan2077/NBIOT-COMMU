/**
  ******************************************************************************
  * @file    bsp_TimBase.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   TIM2 1ms ��ʱӦ��bsp
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ�� ISO-MINI STM32 ������ 
  * ��̳    :http://www.chuxue123.com
  * �Ա�    :http://firestm32.taobao.com
  *
  ******************************************************************************
  */ 
#include "stm32f10x_it.h"
#include "stm32f10x_tim.h"
#include "Timer.h" 
#include "misc.h"
#include "led.h"
/// TIM2�ж����ȼ�����
void TIM2_NVIC_Configuration(void)
{
    NVIC_InitTypeDef NVIC_InitStructure; 
    
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);  													
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	  
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;	
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

/*
 * TIM_Period / Auto Reload Register(ARR) = 1000   TIM_Prescaler--71 
 * �ж�����Ϊ = 1/(72MHZ /72) * 1000 = 1ms
 *
 * TIMxCLK/CK_PSC --> TIMxCNT --> TIM_Period(ARR) --> �ж� ��TIMxCNT����Ϊ0���¼��� 
 */
void TIM2_Configuration(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
		
		/* ����TIM2CLK Ϊ 72MHZ */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);
    //TIM_DeInit(TIM2);
	
	/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
    TIM_TimeBaseStructure.TIM_Period=1000;
	
    /* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	  /* ʱ��Ԥ��Ƶ��Ϊ72 */
    TIM_TimeBaseStructure.TIM_Prescaler= 71;
	
		/* ���ⲿʱ�ӽ��в�����ʱ�ӷ�Ƶ,����û���õ� */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;
	
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; 
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
		
    TIM_Cmd(TIM2, ENABLE);																		
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , DISABLE);		/*�ȹرյȴ�ʹ��*/    
 			 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2 , ENABLE);          //��ʱ����ʼ��ʱ
}

unsigned int ms_cnt=0;
void TIM2_IRQHandler(void)
{
if ( TIM_GetITStatus(TIM2 , TIM_IT_Update) != RESET ) 
	{	
		CLI();//
   
		ms_cnt++;
	 
		 //led1=!led1;
			if(ms_cnt>=1000)
		  {
		    
		    ms_cnt=0;
		    
			
		   }// 1 sec		
		 
		
		
		SEI();
		TIM_ClearITPendingBit(TIM2 , TIM_FLAG_Update);  		 
	}		 	
}


///////////////////////////////////////////////////
void TIM4_PWM_Init(u16 arr,u16 psc) 
//TIM4 ???PWM?????????
{   
GPIO_InitTypeDef GPIO_InitStructure;
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;


RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  | RCC_APB2Periph_AFIO, ENABLE);//??GPIO???AFIO??????????
                                                                         
GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_8 | GPIO_Pin_9;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
//??????
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_Init(GPIOB, &GPIO_InitStructure);


TIM_TimeBaseStructure.TIM_Period = arr; 
//??????????????????????????? 80K
TIM_TimeBaseStructure.TIM_Prescaler =psc;
//??????TIMx???????????  ???
TIM_TimeBaseStructure.TIM_ClockDivision = 1;
//??????:TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//TIM??????
TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
//??TIM_TimeBaseInitStruct?????????TIMx???????

TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
//PWM??2
TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
//??????
TIM_OCInitStructure.TIM_OutputNState = TIM_OutputNState_Disable;
//?????? 
TIM_OCInitStructure.TIM_Pulse = 0;
//????

// TIM_OC1Init(TIM4,&TIM_OCInitStructure);        //??1
// TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
// TIM_OC2Init(TIM4,&TIM_OCInitStructure);        //??2
// TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
TIM_OC3Init(TIM4, &TIM_OCInitStructure);
//????3
TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
TIM_OC4Init(TIM4, &TIM_OCInitStructure);
//????4 
TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);


TIM_CtrlPWMOutputs(TIM4,ENABLE);


TIM_ARRPreloadConfig(TIM4, ENABLE);
//??TIMx?ARR????????
TIM_Cmd(TIM4, ENABLE);
//??TIMx??
}

void TIM3_Interrupt_Init(u16 arr,u16 psc) 
//TIM3????????????
{
TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
NVIC_InitTypeDef NVIC_InitStructure;


RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
//????
  
TIM_TimeBaseStructure.TIM_Period = arr;
//??????????????????????????? ???5000?500ms
TIM_TimeBaseStructure.TIM_Prescaler =psc;
//??????TIMx???????????  10Khz?????  
TIM_TimeBaseStructure.TIM_ClockDivision = 0;
//??????:TDTS = Tck_tim
TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
//TIM??????
TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//??TIM_TimeBaseInitStruct?????????TIMx???????
 
TIM_ITConfig(TIM3,TIM_IT_Update | TIM_IT_Trigger,ENABLE);


NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
//TIM3??
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//?????0?
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//?????3?
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//IRQ?????
NVIC_Init(&NVIC_InitStructure);
//??NVIC_InitStruct???????????NVIC???


TIM_Cmd(TIM3, ENABLE);
//??TIMx??
}

void TIM3_IRQHandler(void) 
//TIM3??????
{ 
if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //?????TIM??????:TIM ??? 
{ 
	
//++Event;
//if(Event == 1)
//{ 
//TIM_SetCompare3(TIM4,(led0pwmval*80)/100);

//}
//if(Event == 2)
//{
//Event = 0;

//TIM_SetCompare3(TIM4,(led0pwmval*20)/100);

//}


TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //??TIMx???????:TIM ??? 
}
}




















/*********************************************END OF FILE**********************/
