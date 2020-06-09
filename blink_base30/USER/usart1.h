#ifndef UART_H
#define UART_H
#include "usart1.h"
//#include "Periph.h"
#include "misc.h"
void UART1_Config( unsigned long Baud );

void UART1_WriteByte( unsigned char Wchar );

void UART3_Config( unsigned long bound );

void UART2_Config( unsigned long Baud );

void UART2_WriteByte( unsigned char Wchar );
void UART2_WriteBytess(unsigned char  *cmd_buf,unsigned char  offset,unsigned char n );
 int CRC16(unsigned char *pDataIn,int pos, int iLenIn);
 unsigned char check_crc16(unsigned char *uart_rec_buffer2,unsigned char index_uart2);       
#endif

