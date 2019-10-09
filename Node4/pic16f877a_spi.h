/**************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N   */
/* Date		    : 04-10-2019				 						      */
/* Filename	    : pic16f877a_spi.h			        					  */
/* Description	: header  file		    								  */
/**************************************************************************/

#ifndef PIC16F877A_SPI_H
#define PIC16F877A_SPI_H

#include <htc.h>

#define _XTAL_FREQ 20000000

#define  SPI_CLK_EN TRISC3=0
#define  SPI_MOSI_EN TRISC4=0
#define  SPI_MISO_EN TRISC5=1
#define  SPI_CS_EN TRISB1=0

#define  SPI_CS RB1

#define  SPI_INT_ENABLE 	SSPIE=1
#define  SPI_INT_DISABLE 	SSPIE=0
#define  SPI_INT_FLG_CLR    SSPIF=0
#define  WAIT_FOR_SPI_FLAG_SET	while(SSPIF==0)

#define SPI_DATA_BUF SSPBUF

#define SPI_CONFIG_SET 	{SSPCON = 0x00;\
						SSPSTAT	= 0x00;\
						CKP = 0;\
						CKE = 1;\
						SMP = 1;\
						SSPEN = 1;}

void spi_init(void);

//this function takes 1 byte to be tranferred and return one byte from the other end device
unsigned char spi_transfer(unsigned char data_byte); 

#endif //PIC16F877A_SPI_H