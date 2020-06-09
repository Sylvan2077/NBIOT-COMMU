/******************** 鑫盛电子工作室 ********************
 * 文件名  ：adc.c
 * 描述    ：DMA方式读取ADC值应用函数库         
 * 实验平台：MINI STM32开发板 基于STM32F103C8T6
 * 硬件连接： PCA0 - ADC1 
 * 库版本  ：ST3.0.0  
 * 淘宝店：http://shop66177872.taobao.com
**********************************************************************************/

#include "adc.h"

#define ADC1_DR_Address    ((u32)0x4001244C)
#define CH_NUM  3           //????ADC???
__IO u16 ad_value[CH_NUM]={0}; 

/*配置采样通道端口 使能GPIO时钟	  设置ADC采样PA0端口信号*/
 void ADC1_GPIO_Config(void)
{ GPIO_InitTypeDef GPIO_InitStructure;    
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	  
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;		    //GPIO设置为模拟输入
  GPIO_Init(GPIOA, &GPIO_InitStructure);   
}
void adc_init(void)
{
    ADC_InitTypeDef adc;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
    RCC_ADCCLKConfig(RCC_PCLK2_Div6);                       //??ad???72/6=12M,ad???????14M
    
    ADC_DeInit(ADC1);
    adc.ADC_Mode                = ADC_Mode_Independent;     //???????
    adc.ADC_ContinuousConvMode  = ENABLE;                   //??????
    adc.ADC_DataAlign           = ADC_DataAlign_Right;      //?????
    adc.ADC_ExternalTrigConv    = ADC_ExternalTrigConv_None;//???????
    adc.ADC_NbrOfChannel        = CH_NUM ;                       //??3???
    adc.ADC_ScanConvMode        = ENABLE;                   //??????
    ADC_Init(ADC1,&adc);
    //?????ADC???,???????????
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_55Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_55Cycles5);
    //??adc?DMA
    ADC_DMACmd(ADC1,ENABLE);
    //??ADC1
    ADC_Cmd(ADC1, ENABLE);
    /* Enable ADC1 reset calibaration register */   
    ADC_ResetCalibration(ADC1);  //????
    /* Check the end of ADC1 reset calibration register */
    while(ADC_GetResetCalibrationStatus(ADC1));  //????????
    /* Start ADC1 calibaration */
    ADC_StartCalibration(ADC1); //????
    /* Check the end of ADC1 calibration */
    while(ADC_GetCalibrationStatus(ADC1));   //??????


}

void adc_dma_init()
{
    DMA_InitTypeDef dma;
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    
    DMA_DeInit(DMA1_Channel1);
    dma.DMA_PeripheralBaseAddr  = (u32)&(ADC1->DR);     //DMA????????
    dma.DMA_MemoryBaseAddr      = (u32)&ad_value;       //???????
    dma.DMA_DIR                 = DMA_DIR_PeripheralSRC;//DMA??????SRC??,????????
    dma.DMA_M2M                 = DMA_M2M_Disable;      //M2M????
    dma.DMA_PeripheralDataSize  = DMA_PeripheralDataSize_HalfWord;//?????????16?
    dma.DMA_MemoryDataSize      = DMA_MemoryDataSize_HalfWord;  //DMA??????,HalfWord???16?    
    dma.DMA_BufferSize          = CH_NUM;                       //DMA????CH_NUM?
    dma.DMA_MemoryInc           = DMA_MemoryInc_Enable;         //???????,????????
    dma.DMA_PeripheralInc       = DMA_PeripheralInc_Disable;    //???????,????????
    dma.DMA_Mode                = DMA_Mode_Circular;            //????,???????
    dma.DMA_Priority            = DMA_Priority_High;            //DMA????
    DMA_Init(DMA1_Channel1,&dma); 
}
void adc_start(void)
{
    /* Start ADC1 Software Conversion */ 
    ADC_SoftwareStartConvCmd(ADC1, ENABLE);  //??????,ADC??DMA???????RAM?? 
    DMA_Cmd(DMA1_Channel1,ENABLE);    
}

/*初始化ADC1 */
void ADC1_Init(void)
{
	ADC1_GPIO_Config();
	adc_dma_init();
	 adc_init();
	adc_start();
}




