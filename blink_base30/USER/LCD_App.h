#ifndef LCD_APP_H
#define LCD_APP_H
#include "LCD_App.h"
#include "LCD_1602.h"

void LCD_Init( void );
void LCD_WriteNum( uchar LineX , uchar BitX , uchar WD );
void LCD_WriteFloat( uchar Linex , uchar StartBit , float Displayf );
void LCD_WriteString( uchar LineX , uchar BitX , void *WD );
void QP( void );
void LCD_SetAC( u8 LineX , u8 BitX );
void LCD_WriteInteger( u8 LineX , u8 BitX ,u8 YX ,u32 Dat );

#endif


