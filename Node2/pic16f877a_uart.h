/**************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N   */
/* Date		    : 04-10-2019				 						      */
/* Filename	    : pic16f877a_uart.h			        					  */
/* Description	: header file			   							      */
/**************************************************************************/

#ifndef PIC16F877A_UART_H
#define PIC16F877A_UART_H

#include <htc.h>
#define _XTAL_FREQ 20000000

#define debug_print uart_print

void uart_init(void);
unsigned char uart_recv(void);
void uart_send(unsigned char data_byte);
void uart_print(const unsigned char* str);


#endif // PIC16F877A_UART_H