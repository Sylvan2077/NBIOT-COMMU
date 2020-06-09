/******************** STM32F103 .C File  keil 4.72 v********************************************************************
* Company			 : BDK
* File Name          : UART
* Author             : benten22  BDK
* firmware Version   : V3.5.0
* Date               : 2014/08/10
* Describe			 :

*************************************************************************************************************************/
#include "stm32f10x.h"
#include "usart1.h"
#include "stdio.h"


#define USARTy                   USART1
#define USARTy_GPIO              GPIOA
#define USARTy_CLK               RCC_APB2Periph_USART1
#define USARTy_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTy_RxPin             GPIO_Pin_10
#define USARTy_TxPin             GPIO_Pin_9
#define USARTy_Tx_DMA_Channel    DMA1_Channel4
#define USARTy_Tx_DMA_FLAG       DMA1_FLAG_TC4
#define USARTy_DR_Base           0x40013804

#if 1
#pragma import(__use_no_semihosting)             
                
struct __FILE 				//标准库需要的支持函数 
{ 
	int handle; 
	/* Whatever you require here. If the only file you are using is */ 
	/* standard output using printf() for debugging, no file handling */ 
	/* is required. */ 
}; 
/* FILE is typedef’ d in stdio.h. */ 
FILE __stdout;       
					  
_sys_exit(int x) 							//定义_sys_exit()以避免使用半主机模式  
{ 
	x = x; 
} 

int fputc( int ch , FILE *f )				//重定义fputc函数 
{      
	while( ( USART1->SR&0X40 ) == 0 );		//循环发送,直到发送完毕   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif

/* *******************************************************************************************************************/
/* ------------------------------------------------------------------------------------------------------------------*/
/* ********************************************* USART 1 *************************************************************/
/* ------------------------------------------------------------------------------------------------------------------*/
/* 1.配置基本参数*****************************************************************************************************/
void UART_RxInt( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}

void UART1_Config( unsigned long Baud )
{
	USART_InitTypeDef 		USART_InitStructure; 			//UART 初始化配置结构体
	GPIO_InitTypeDef 	  	GPIO_Structure;					//GPIO 结构体配置
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_USART1 , ENABLE ) ;	//使能时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE  );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE ); 

//引脚和时钟配置
	GPIO_Structure.GPIO_Pin = GPIO_Pin_9;
 	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_Init( GPIOA , &GPIO_Structure );						//Tx复用推挽式输出
	
	GPIO_Structure.GPIO_Pin = GPIO_Pin_10;
 	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init( GPIOA , &GPIO_Structure );						//Rx浮空输入

//UART配置
	USART_DeInit( USART1 ); 									//复位UART1
	/* UART功能设置 -----------------------------------------------------------------*/
	/*功能*/
	USART_InitStructure.USART_BaudRate = Baud; 						//设置波特率 BaudRate
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //设置传输数据长度 8位或者9位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 			//设置停止位 1 | 0.5 | 1.5 | 2 ；
	USART_InitStructure.USART_Parity = USART_Parity_No; 			//奇偶校验设置 Odd奇 No Even偶
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None; 								//RTS , CTC , 流控制设置
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //收发使能
	USART_Init(USART1, &USART_InitStructure);						//函数实规则
	
	USART_Cmd( USART1, ENABLE );	//使能UART1
	
	USART_ClearFlag( USART1 , USART_FLAG_TXE|USART_FLAG_RXNE );
	
	USART_ITConfig( USART1, USART_IT_RXNE, ENABLE );
	UART_RxInt();
}



void UART1_WriteByte( unsigned char Wchar )
{
	while ( USART_GetFlagStatus( USART1, USART_FLAG_TXE ) != SET );
	USART_ClearFlag( USART1 , USART_FLAG_TXE );
	USART_SendData( USART1 , Wchar ); 
}


#define uart_buffer_size   100
unsigned char uart_rec_buffer[uart_buffer_size];
unsigned char index_uart=0;
unsigned char uart_buffer_head_ok;
unsigned char cmd_in=0;


void USART1_IRQHandler(void)
{
	unsigned char  c;
	 if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART1->DR;
		  if(c=='@') 
			{
				uart_buffer_head_ok=1;
				index_uart=0;
			}
	 	  if(uart_buffer_head_ok==1)
		    {
		     uart_rec_buffer[index_uart++]=c;
				 if(c=='#') //end ok
				 {
					 cmd_in=1;
				 }					
		    }else
				{
				 index_uart=0;
				}
		  if(index_uart>=uart_buffer_size)  index_uart=0;
		  //delay_us(100);
		  //UART1_WriteByte(c+1);
		
		// UART1_WriteByte(c); UART1_WriteByte('>'); UART1_WriteByte(c+1); 	  
	}
	
	USART_ClearITPendingBit( USART1 , USART_IT_RXNE );
}
///////////////////////////
//  uart2  



#define USARTy2                   USART1
#define USARTy2_GPIO              GPIOA
#define USARTy2_CLK               RCC_APB2Periph_USART2
#define USARTy2_GPIO_CLK          RCC_APB2Periph_GPIOA
#define USARTy2_RxPin             GPIO_Pin_3
#define USARTy2_TxPin             GPIO_Pin_2
#define USARTy2_Tx_DMA_Channel    DMA1_Channel4
#define USARTy2_Tx_DMA_FLAG       DMA1_FLAG_TC4
#define USARTy2_DR_Base           0x40013804
 

/* *******************************************************************************************************************/
/* ------------------------------------------------------------------------------------------------------------------*/
/* ********************************************* USART 1 *************************************************************/
/* ------------------------------------------------------------------------------------------------------------------*/
/* 1.配置基本参数*****************************************************************************************************/
void UART2_RxInt( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTy2 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}

void UART2_Config( unsigned long Baud )
{
	USART_InitTypeDef 		USART_InitStructure; 			//UART 初始化配置结构体
	GPIO_InitTypeDef 	  	GPIO_Structure;					//GPIO 结构体配置
	
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_USART2, ENABLE); 	//使能时钟
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE  );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE ); 

//引脚和时钟配置
	GPIO_Structure.GPIO_Pin = GPIO_Pin_2;
 	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Structure.GPIO_Mode = GPIO_Mode_AF_PP;
 	GPIO_Init( GPIOA , &GPIO_Structure );						//Tx复用推挽式输出
	
	GPIO_Structure.GPIO_Pin = GPIO_Pin_3;
 	GPIO_Structure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Structure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
 	GPIO_Init( GPIOA , &GPIO_Structure );						//Rx浮空输入

//UART配置
	USART_DeInit( USART2 ); 									//复位UART1
	/* UART功能设置 -----------------------------------------------------------------*/
	/*功能*/
	USART_InitStructure.USART_BaudRate = Baud; 						//设置波特率 BaudRate
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //设置传输数据长度 8位或者9位
	USART_InitStructure.USART_StopBits = USART_StopBits_1; 			//设置停止位 1 | 0.5 | 1.5 | 2 ；
	USART_InitStructure.USART_Parity = USART_Parity_No; 			//奇偶校验设置 Odd奇 No Even偶
	USART_InitStructure.USART_HardwareFlowControl = 
	USART_HardwareFlowControl_None; 								//RTS , CTC , 流控制设置
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx; //收发使能
	USART_Init(USART2, &USART_InitStructure);						//函数实规则
	
	USART_Cmd( USART2, ENABLE );	//使能UART1
	
	USART_ClearFlag( USART2 , USART_FLAG_TXE|USART_FLAG_RXNE );
	
	USART_ITConfig( USART2, USART_IT_RXNE, ENABLE );
	 UART2_RxInt();
}



void UART2_WriteByte( unsigned char Wchar )
{
	while ( USART_GetFlagStatus( USART2, USART_FLAG_TXE ) != SET );
	USART_ClearFlag( USART2 , USART_FLAG_TXE );
	USART_SendData( USART2 , Wchar ); 
}
void UART2_WriteBytes(unsigned char  *cmd_buf,unsigned char  offset,unsigned char n )
{
	int i=0,end;
	end=n+offset;
 for(i=offset;i<end;i++)
	{
	  while ( USART_GetFlagStatus( USART2, USART_FLAG_TXE ) != SET );
	  USART_ClearFlag( USART2 , USART_FLAG_TXE );
	  USART_SendData( USART2 , cmd_buf[i] ); 
	}
}

#define uart_buffer_size2   100
unsigned char uart_rec_buffer2[uart_buffer_size2];
unsigned char index_uart2=0;
unsigned char uart_buffer_head_ok2;
unsigned char cmd_in2=0;
unsigned char bq_cmd_type=0;
unsigned char max_board_num=0;
unsigned char board_addr_rd_ok=0;
unsigned char connet_sta=0,ip_sig=0,ip_ok=0;
  char my_IP[30];
extern unsigned char message[];
void USART2_IRQHandler(void)
{
	unsigned char  c,i;
	 if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)
	{ 	
	    c=USART2->DR; 
	if(ip_ok==0)
		{
		 if(c=='"'&&ip_sig==0) 
			{
				uart_buffer_head_ok2=1;
				index_uart2=0;
				
			
			}
			if(uart_buffer_head_ok2==1)
		    {
					if(c=='.') //end ok
				 {
					 ip_sig=1;
				 }
				}
	 	  if(uart_buffer_head_ok2==1)
		    {
		     uart_rec_buffer2[index_uart2++]=c;
				 if(c=='\r'&&ip_sig==1) //end ok
				 {	 
	
					 cmd_in2=1;
					 if(ip_sig==1&&ip_ok==0)
					 {ip_ok=1;
						 for(i=0;;i++)
						 {
					    my_IP[i]=uart_rec_buffer2[i+1];
							// UART1_WriteByte(my_IP[i]);
							 if(my_IP[i]=='"')
							 {								 
								 my_IP[i]='\0';
								 break;
							 }
							 
							 if(i>25) break;
						 }
					 }
				 }					
		    }else
				{
				 index_uart2=0;
				}
		   if(index_uart2>=uart_buffer_size)  index_uart2=0;
			}  
		if(ip_ok==1)
		{
		 if(c=='@' ) 
			{
				uart_buffer_head_ok2=1;
				index_uart2=0;
				
			
			}
			 
	 	  if(uart_buffer_head_ok2==1)
		    {
		     uart_rec_buffer2[index_uart2++]=c;
				 if(c=='#' ) //end ok
				 {	 
	
					 cmd_in2=1;
					   
				 }					
		    }else
				{
				 index_uart2=0;
				}
		   if(index_uart2>=uart_buffer_size)  index_uart2=0;
			}   
		    UART1_WriteByte(c);
		 //  UART2_WriteByte(c); UART2_WriteByte('>'); UART2_WriteByte(c+1); 	  
	}
	
	USART_ClearITPendingBit( USART2 , USART_IT_RXNE );
}


void UART3_RxInt( void )
{
	NVIC_InitTypeDef NVIC_InitStructure;

	/* Enable the USARTy Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init( &NVIC_InitStructure );
}


 void UART3_Config( unsigned long bound )
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;

  /* Enable the USART3 Pins Software Remapping */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB| RCC_APB2Periph_AFIO ,ENABLE);
          GPIO_PinRemapConfig(GPIO_FullRemap_USART3, ENABLE); 
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);


/* Configure USART3 Rx (PB.11) as input floating */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; 
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
GPIO_Init(GPIOB, &GPIO_InitStructure);

/* Configure USART3 Tx (PB.10) as alternate function push-pull */
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
GPIO_Init(GPIOB, &GPIO_InitStructure);

/* Enable the USART3 Interrupt */
NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); 
NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
NVIC_Init(&NVIC_InitStructure); 

USART_InitStructure.USART_BaudRate = bound;
USART_InitStructure.USART_WordLength = USART_WordLength_8b;
USART_InitStructure.USART_StopBits = USART_StopBits_1; 
USART_InitStructure.USART_Parity = USART_Parity_No;
USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;        

USART_Init(USART3, &USART_InitStructure);
USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);


USART_Cmd(USART3, ENABLE);
USART_ClearFlag(USART3, USART_FLAG_TXE);
UART3_RxInt();
}

// USART3DATA[20]=0;
 void USART3_IRQHandler(void)                             //串口中断输入
{ 
        static u8 i = 0; 
        u8 data = 0;
      if(USART_GetFlagStatus(USART3, USART_FLAG_RXNE) == SET)
      {
       data = USART_ReceiveData(USART3); 
                                //i++;
                                //if(i == 9)
                                 //{
                                        //i = 0;
                                        //temp1 = 1;                                        
         
       // }
        USART_SendData(USART3, data);
                         }
}
				
	

void UART3_WriteByte( unsigned char Wchar )
{
	while ( USART_GetFlagStatus( USART3, USART_FLAG_TXE ) != SET );
	USART_ClearFlag( USART3 , USART_FLAG_TXE );
	USART_SendData( USART3 , Wchar ); 
}
void UART3_WriteBytes(unsigned char  *cmd_buf,unsigned char  offset,unsigned char n )
{
	int i=0,end;
	end=n+offset;
 for(i=offset;i<end;i++)
	{
	  while ( USART_GetFlagStatus( USART3, USART_FLAG_TXE ) != SET );
	  USART_ClearFlag( USART3 , USART_FLAG_TXE );
	  USART_SendData( USART3 , cmd_buf[i] ); 
	}
}			