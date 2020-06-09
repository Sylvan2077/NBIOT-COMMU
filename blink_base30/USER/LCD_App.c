/***************************************************************************
Title : LCD_App
Time  £º20130205
author£ºbenten22 BDK

****************************************************************************/

#include "LCD_App.h"
 
#define Delay_Us1  delay_us(5);

void QP( void )
{
 	LCD_WriteInstruct( 0x01 );
	delay_ms(2);
	LCD_WriteInstruct( 0x02 );
	delay_ms(2);
} 
void LCD_Init( void ) 						//³õÊ¼»¯LCD
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_AFIO , ENABLE );
	GPIO_PinRemapConfig( GPIO_Remap_SWJ_JTAGDisable , ENABLE );   //only enable swd debug
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA , ENABLE );
	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB , ENABLE );
	
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;   //LCD RS RW E
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
	
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;   //SPI1_CS_N USB_CN LCD_D7
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6
								 |GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_12;   //
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
		
		
	LCD_WriteInstruct( 0x38 );
	delay_us(50);
	LCD_WriteInstruct( 0x06 );
	delay_us(50);
	LCD_WriteInstruct( 0x0f );
	delay_us(50);
	LCD_WriteInstruct( 0x01 );
	delay_ms(2);
	LCD_WriteInstruct( 0x02 );
	delay_ms(5);
}

void LCD_SetAC( u8 LineX , u8 BitX )
{
	if( LineX==1 )
	{
		LCD_WriteInstruct( 0x80+BitX );
		delay_us(50);
		
	}
	else
	{
		LCD_WriteInstruct( 0x80+0x40+BitX );
		delay_us(50);	
	}
}

void LCD_WriteNum( uchar LineX , uchar BitX , uchar WD )		//Ð´Êý×Ö
{
	if( LineX==1 )
	{
		LCD_WriteInstruct( 0x80+BitX );
		delay_us(50);
		LCD_WriteData( 0x30+WD );
		delay_us(50);
	}
	else
	{
		LCD_WriteInstruct( 0x80+0x40+BitX );
		delay_us(50);
		LCD_WriteData( 0x30+WD );
		delay_us(50);		
	}
}

void LCD_WriteChar( uchar LineX , uchar BitX , uchar WD )
{
	if( LineX==1 )
	{
		LCD_WriteInstruct( 0x80+BitX );
		delay_us(50);
		LCD_WriteData( WD );
		delay_us(50);
	}
	else
	{
		LCD_WriteInstruct( 0x80+0x40+BitX );
		delay_us(50);
		LCD_WriteData( WD );
		delay_us(50);		
	}
}

void LCD_WriteString( uchar LineX , uchar BitX , void *WD )  		//Ð´×Ö·û´®
{
	u8 *s;
	s = WD;
	while( *s != '\0' )
	{
		LCD_WriteChar( LineX , BitX++ , *s );
		s++;
	}
}


void LCD_WriteFloat( uchar Linex , uchar StartBit , float Displayf ) //Ð´Ò»¸öfloat
{
	unsigned long int Temp;
	uchar k;
	uchar Flag=0;
	uchar DW;

	for( k=0; k<16 ; k++ )
	{
		LCD_WriteChar( Linex , k , ' ' );
	}
	
	if( Displayf < 0 )
	{
		LCD_WriteChar( Linex , StartBit , '-' );
		StartBit++;	
		Displayf = -Displayf;	
	}

	if( Displayf <=9999.0 )
	{
		Temp = (unsigned long)(Displayf*1000);
		DW='V';	
	}
	else
	{
		if( 9999.0<Displayf && Displayf<=9999999.0)
		{
			Temp = Displayf;
			DW='K';
		}	
	}
	
	if( 9999999.0<Displayf )
	{
		LCD_WriteString(  Linex , StartBit ,"Out Rang!");
		return;	
	}	

		k =	Temp/1000000%10;
		if( (Flag==2)||(k!=0) )
		{
			LCD_WriteNum( Linex , StartBit , k );
			Flag = 2;
			StartBit++;	
		}

		k =	Temp/100000%10;
		if( (Flag==2)||(k!=0))
		{
			LCD_WriteNum( Linex , StartBit , k );	
			Flag = 2;
			StartBit++;	
		}

		k =	Temp/10000%10;
		if( (Flag==2)||(k!=0))
		{
			LCD_WriteNum( Linex , StartBit , k );	
			Flag = 2;
			StartBit++;	
		}

		Flag = 2;
		k =	Temp/1000%10;
		if( (Flag==2)||(k!=0))
		{
			LCD_WriteNum( Linex , StartBit , k );
			StartBit++;		
		}
		
		//Delay_Ms(5);
		LCD_WriteChar( Linex , StartBit++ , '.' );
		//Delay_Ms(5);
		k =	Temp/100%10;
		LCD_WriteNum( Linex , StartBit++ , k );	
		//Delay_Ms(5);
		k =	Temp/10%10;
		LCD_WriteNum( Linex , StartBit++ , k );	
		//Delay_Ms(5);
		k =	Temp%10;
		LCD_WriteNum( Linex , StartBit++ , k );	
		//Delay_Ms(5);
		LCD_WriteChar( Linex , StartBit , DW );
		//Delay_Ms(5);
}

void LCD_WriteInteger( u8 LineX , u8 BitX , u8 YX, u32 Dat )
{
	u8 i;
	for( i=YX;  i>0 ; i--)
	{
		LCD_WriteNum( LineX , BitX+i-1 , Dat%10 ); 
		if( Dat != 0 )
			Dat = Dat /10;
	}
}


