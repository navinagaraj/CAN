/**************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N   */
/* Date		    : 04-10-2019				 						      */
/* Filename	    : pic16f877a_spi.c			        					  */
/* Description	: spi source file										  */
/**************************************************************************/


#include "pic16f877a_spi.h"
#include "pic16f877a_uart.h"

void spi_init(void)
{

	SSPIE=0;
	SSPIF=0;
	SSPBUF = 0;
	TRISC3=0;
	TRISC4=1;
	TRISC5=0;
	TRISB1 = 0;

	SSPCON = 0x00;
	SSPSTAT	= 0x00;
	CKP = 0;
	CKE = 1;
	SMP = 1;
	SSPEN = 1;
	
	debug_print("SPI initialized\r\n");
}

unsigned char spi_transfer(unsigned char data_byte)
{
	volatile unsigned char ret;
	SSPBUF = data_byte;
	while(SSPIF==0);
	SSPIF=0;
	ret = SSPBUF;
	return ret;
} 
