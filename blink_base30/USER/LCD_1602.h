#ifndef LCD_Driver_H
#define LCD_Driver_H
#include "LCD_1602.h"
//#include "Periph.h"
 
#include "stm32f10x.h"
#include "ALL_Includes.h"
#include "bit.h"
/* 定义液晶硬件接口 */
#define RS  PAOUT(0)
#define RW  PAOUT(1)
#define CE  PAOUT(2)

#define LCDD0  PBOUT(9)
#define LCDD1  PBOUT(8)
#define LCDD2  PBOUT(7)
#define LCDD3  PBOUT(6)

#define LCDD4  PBOUT(5)
#define LCDD5  PBOUT(4)
#define LCDD6  PBOUT(3)
#define LCDD7  PAOUT(15)

#define Delay_Us1 Delay_Us(10)
	#define BIT(x)	(1 << (x))
/* 8位数据口写数据 */	
#define LCDD(x)	 	LCDD0=(((uchar)(x))&BIT(0));\
					LCDD1=(((uchar)(x))&BIT(1))>>1;\
					LCDD2=(((uchar)(x))&BIT(2))>>2;\
					LCDD3=(((uchar)(x))&BIT(3))>>3;\
					LCDD4=(((uchar)(x))&BIT(4))>>4;\
					LCDD5=(((uchar)(x))&BIT(5))>>5;\
					LCDD6=(((uchar)(x))&BIT(6))>>6;\
					LCDD7=(((uchar)(x))&BIT(7))>>7

/* 函数声明 */

unsigned char LCD_CheckBusy( void );
void LCD_WriteInstruct( unsigned char  Ins );
void LCD_WriteData( unsigned char WD );

#endif


