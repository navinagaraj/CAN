/**************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N   */
/* Date		    : 04-10-2019				 						      */
/* Filename	    : pic16f877a_uart.c			        					  */
/* Description	: uart source file										  */
/**************************************************************************/

#include "pic16f877a_uart.h"


void uart_init(void)
{
	TRISC6 = 0;				/* MAKING TX AS OUTPUT 	*/
	TRISC7 = 1;				/* MAKING RX AS INPUT 	*/
	TXSTA  = 0x00;			/* ASYNCHRONOUS MODE AND 8-BIT LOW SPEED SELECTION    */
	RCSTA  = 0x00;			/* ENABLE UART TO USE RC7 AND RC6 AS UART MODULE PINS */
	SPBRG  = 31;			/* TO GENERATE 9600 BAUDRATE */
	TXEN   = 1;				/* TRANSMIT ENABLE BIT */	
	SPEN   = 1;				/* SERIAL PORT ENABLE  */
	TXIF   = 0;				/* MAKING TXIF  ZERO   */
	RCIF   = 0;				/* MAKING RXIF  ZERO   */
	debug_print("Uart initialized...\r\n");	/*	ACKNOWLEDGEMENT	   */
}
unsigned char uart_recv(void)
{
	while(RCIF == 0);/* RECIVING DATA */
	RCIF = 0;
	return RCREG;
}
void uart_send(unsigned char data_byte)
{

	TXREG = data_byte;	/* TRANSFER DATA */
	while(TXIF == 0);
	TXIF = 0;
}


void uart_print(const unsigned char* str)
{
	while(*str)	/* PRINTING SINGLE CHAR IN UART */
	{
		uart_send(*str);
		str++;
	}
}