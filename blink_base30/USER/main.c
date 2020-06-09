
#include "stm32f10x.h"    
#include "bit.h"
#include "led.h"
 
 extern __IO u16 ad_value[ ];
extern unsigned char cmd_in;
extern unsigned char index_uart;
unsigned char msg[50];
extern unsigned char uart_rec_buffer[];
int main(void)
{   
	int j,i,k,m;

  /* 配置系统时钟为72M */      
  SystemInit();	
  LED_GPIO_Config();
	UART1_Config(9600);
	UART3_Config(9600);
	
	
	TIM2_Configuration();
	TIM2_NVIC_Configuration();
	Delay_Init();
	ADC1_Init();
	
		LCD_Init();
	
	//TIM4_PWM_Init(999,7199);
//TIM4 PWM?????
 // TIM3_Interrupt_Init(999,7199);
//TIM3????????

 //  pwmval= 600;  
//???
	LCD_WriteString( 1 , 7 , "SP=");	LCD_WriteString( 2 , 0 , "SF=");
	 printf("a\r\n");delay_ms(1);
	printf("b\r\n");
	
  while (1)
  {
		  if(cmd_in==1)
			{
			  // printf("get a cmd!\r\n");
				uart_rec_buffer[index_uart]='\0';
				sprintf(msg,"%s",uart_rec_buffer );
				printf("cmd is: %s\r\n",uart_rec_buffer);
				
				
				cmd_in=0;
			
			}				
	  // printf("aa\r\n",
	//	ad_value[0],
//		ad_value[1],
//		ad_value[2]);
		//led1=1;
		//	UART3_WriteByte('a');
		delay_ms(500);
		//led1=0;
		led1=!led1;
	}
}



