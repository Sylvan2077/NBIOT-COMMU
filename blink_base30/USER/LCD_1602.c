/***************************************************************************
Title : LCD_Driver
Time  ��20140205
author��benten22 BDK

	��Ȩ���У�δ��������ͬ���Ͻ�������ҵӦ�ã�������ѧϰ������
****************************************************************************/

#include "LCD_1602.h"


#define Delay_Us1  delay_us(5);
void LCD_WriteData( unsigned char WD )
{
	//while( LCD_CheckBusy() != 0 );
	//Delay_Ms(5);
	CE = 0;
	RS = 1;			//ѡ������
	RW = 0; 		//ѡ��д����
	Delay_Us1;
	CE = 1; 
	Delay_Us1;
	
	LCDD( WD );		//����
		
	CE = 0;	
}

void LCD_WriteInstruct( unsigned char Ins )
{
	//while( LCD_CheckBusy() != 0 );
	//Delay_Ms(5);
	CE = 0;
	RS = 0;			//ѡ��ָ��
	RW = 0; 		//ѡ��д����
	Delay_Us1;
	CE = 1; 
	Delay_Us1;
	
	LCDD( Ins );		//����
		
	CE = 0;	
}


u8 LCD_CheckBusy( void )
{
	LCDD(0xff);			//����������
	CE = 0;
	RS = 0;
	RW = 1;
	Delay_Us1;
	CE = 1;
	Delay_Us1;
	return(LCDD7);
}



