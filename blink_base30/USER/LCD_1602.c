/***************************************************************************
Title : LCD_Driver
Time  ：20140205
author：benten22 BDK

	版权所有：未经本作者同意严禁用于商业应用，仅用于学习交流！
****************************************************************************/

#include "LCD_1602.h"


#define Delay_Us1  delay_us(5);
void LCD_WriteData( unsigned char WD )
{
	//while( LCD_CheckBusy() != 0 );
	//Delay_Ms(5);
	CE = 0;
	RS = 1;			//选择数据
	RW = 0; 		//选择写数据
	Delay_Us1;
	CE = 1; 
	Delay_Us1;
	
	LCDD( WD );		//数据
		
	CE = 0;	
}

void LCD_WriteInstruct( unsigned char Ins )
{
	//while( LCD_CheckBusy() != 0 );
	//Delay_Ms(5);
	CE = 0;
	RS = 0;			//选择指令
	RW = 0; 		//选择写数据
	Delay_Us1;
	CE = 1; 
	Delay_Us1;
	
	LCDD( Ins );		//数据
		
	CE = 0;	
}


u8 LCD_CheckBusy( void )
{
	LCDD(0xff);			//拉高做输入
	CE = 0;
	RS = 0;
	RW = 1;
	Delay_Us1;
	CE = 1;
	Delay_Us1;
	return(LCDD7);
}



