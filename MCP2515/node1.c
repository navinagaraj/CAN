/****************************************/
/* Author	    : Naveenkumar N         */
/* Date		    : 29-09-2019            */
/* Filename	    : node1.c               */
/* Description	: node1 file            */
/****************************************/
#include "header.h"
#include "define.h"

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & BOREN_ON & LVP_OFF);

unsigned char  message2[] ="UART initialized.....\n\r";
unsigned char  message1[] ="SPI Initializing ....\n\r";
volatile unsigned char  reciv;
unsigned char arr[30];

void delay();
void spi_init(void);
unsigned char spi_transfer(unsigned char data);
unsigned char SPI_data_ready();
void SPI_write(unsigned char);
unsigned char SPI_read();

void uart_init(void);
void print_uart(const unsigned char *str);
void uart_tx(unsigned char val);
unsigned char uart_rc(void);

void MCP_init();
void MCP_2515_write(unsigned char reg, unsigned char value);
unsigned char MCP_2515_read(unsigned char reg);
unsigned char MCP_BIT_MODIF(unsigned char address,unsigned char mask,unsigned char data);
unsigned char MCP_DATA_TX_BUFFER(unsigned char );
unsigned char MCP_DATA_RX_BUFFER(unsigned char);
unsigned char MCP_REQUEST_TO_SEND(unsigned char);


int main()
{
	GIE		=  0; /* GLOBAL INTERRUPT ENABLE */
	PEIE 	=  0; /* PHERIPHERAL INTERRUPT ENABLE */
	uart_init();  /* UART INITIALIZEING */
	spi_init();	  /* SPI  INITIALIZEING */
	delay();
	
	MCP_init();	/* MCP INITIALIZEING */
	
	reciv =  MCP_2515_read(MCP_CANSTAT_REG);
	
	if((0xE0 & reciv) == MCP_CONFIG_MODE){
			print_uart("MCP is initialized and Entered CONFIG mode\r\n");
	}
	else{
		print_uart("MCP is not initialized\r\n");
	}
	
	MCP_DATA_TX_BUFFER(0);
	reciv = MCP_DATA_RX_BUFFER(0);
	sprintf(arr,"Recive data =\n\r",reciv);
	print_uart(arr);
	
	while(1);
}
unsigned char SPI_read()
{
	delay();
	return SSPBUF;
}
void spi_init(void)
{
	unsigned char temp;
	SSPIE	= 0; /* SSPIF INTERRUPT ENABLE  */
	SSPIF	= 0; /* SSPIF INTERRUPT FLAG    */
	SSPBUF	= 0; /* SERIAL RECIVER AND TRANSMIT BUFFER */
	TRISC3	= 0; /* clock  */
	TRISC4	= 1; /* input  */
	TRISC5	= 0; /* output */
	TRISB1	= 0; /* chip select */
	
	SSPCON	= 0x00; /* MSSP CONTROL REGISTER1 */
	SSPSTAT	= 0x00;	/* MSSP STATUS REGISTER */
	
	CKP   = 0; /* Clock Polarity Select bit */
	CKE   = 1; /* SPI Clock Edge Select bit */
	SMP   = 1; /* Sample bit */
	SSPEN = 1; /*  Synchronous Serial Port Enable bit */
	
	print_uart(message1);
	sprintf(arr,"SSPCON 0X%.2X\n\r",SSPCON);
	print_uart(arr);
	sprintf(arr,"SSPSTAT 0X%.2X\n\r",SSPSTAT);
	print_uart(arr);
}
unsigned char spi_transfer(unsigned char data)
{
	SSPBUF	= data;
	while(SSPIF == 0);
	SSPIF	= 0;
	return SSPBUF;
}
void print_uart(const unsigned char *str)
{
	
	while(*str)	/* PRINTING SINGLE CHAR IN UART */
	{
		uart_tx(*str);
		delay();
		str++;
	}
}

void delay()
{
		TMR1CS  = 0; /*  Timer1 Clock Source Select bit */
		T1CKPS0 = 1; /*  Timer1 Input Clock Prescale Select bits */
		T1CKPS1 = 1; /*  Timer1 Input Clock Prescale Select bits */
		TMR1H   = 0XFA; /* Timer1 high value */
		TMR1L   = 0XB0;	/* Timer1 low value */
		TMR1ON  = 1; /* Timer1 On bit */
		while(!TMR1IF);
		TMR1IF  = 0; /* Timer1 interrupt bit */
		TMR1ON  = 0; /* Timer1 off bit */
}
void uart_tx(unsigned char val)
{

	TXREG = val;	/* TRANSFER DATA */
	while(TXIF == 0);
	TXIF = 0;
}

unsigned char uart_rc(void)
{
	while(RCIF == 0);/* RECIVING DATA */
	RCIF = 0;
	return RCREG;
}

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
	print_uart(message2);	/*	ACKNOWLEDGEMENT	   */
}

void MCP_init()
{
		MCP_BIT_MODIF(0x3F,0xFF,0x90); /* CANCTRL :CONTROL REGISTER */
		
		
		MCP_BIT_MODIF(0x0D,0xFF,0x07); /* TXRTSCTRL :TRANSMIT PIN CONTROL AND STATUS REGISTER */
		MCP_BIT_MODIF(0x30,0xFF,0x02); /* TXB0CTRL  :TRANSMIT BUFFER CONTROL REGISTER */
		MCP_BIT_MODIF(0x40,0xFF,0x02); /* TXB1CTRL  :TRANSMIT BUFFER CONTROL REGISTER */
		MCP_BIT_MODIF(0x50,0xFF,0x02); /* TXB2CTRL  :TRANSMIT BUFFER CONTROL REGISTER */
		
		
		MCP_BIT_MODIF(0x0C,0xFF,0x0F); /* BFPCTRL  :RECEIVE PIN CONTROL AND STATUS REGISTER */
		MCP_BIT_MODIF(0x60,0xFF,0x00); /* RXB0CTRL :RECEIVE BUFFER CONTROL REGISTER */
		MCP_BIT_MODIF(0x70,0xFF,0x00); /* RXB1CTRL :RECEIVE BUFFER CONTROL REGISTER */
		
		
		MCP_BIT_MODIF(0x2A,0xFF,0x01); /* CNF1 :CONFIGURATION REGISTER 1 */
		MCP_BIT_MODIF(0x29,0xFF,0x00); /* CNF2 :CONFIGURATION REGISTER 2 */
		MCP_BIT_MODIF(0x28,0xFF,0x00); /* CNF3 :CONFIGURATION REGISTER 3 */
		
		MCP_BIT_MODIF(0x2B,0xFF,0x00); /* CANINTE :CAN INTERRUPT ENABLE REGISTER */
		MCP_BIT_MODIF(0x2C,0xFF,0x00); /* CANINTF :CAN INTERRUPT FLAG REGISTER */

}
unsigned char MCP_2515_read(unsigned char reg)
{
		CS_pin	  = 0;  			/* chip select  */
		spi_transfer(MCP_READ_INS);	/* Instruction MCP_READ_INS  0x03 */
		spi_transfer(reg);			/* Address Byte	*/
		reciv = spi_transfer(0x00);
		CS_pin	  = 1;  			/* chip select  */
		__delay_ms(3.36);			/* Delay 		*/
		return reciv;				/* Return value	*/
}


void MCP_2515_write(unsigned char reg, unsigned char value)
{
		CS_pin	  = 0;  					/* 	chip select  */
		reciv = spi_transfer(MCP_WRITE_INS);/*	Instruction	MCP_WRITE_INS  0x02 */
		reciv = spi_transfer(reg);			/*	Address Byte */
		reciv = spi_transfer(value);		/*	Data Byte	 */
		__delay_ms(3.36);					/*  Delay  		 */
		CS_pin	  = 1;  					/* 	chip select  */
}

unsigned char MCP_BIT_MODIF(unsigned char add,unsigned char mask,unsigned char data)
{
		CS_pin	  = 0;				/* chip select  */
		spi_transfer(BIT_MODIF_INS);/* Instruction BIT_MODIF_INS  0x05 */
		spi_transfer(add);			/* Address Byte */
		spi_transfer(mask);			/* Mask Byte 	*/
		spi_transfer(data);			/* Data Byte 	*/
		CS_pin	  = 1;				/* chip select  */
		return 0;
}
unsigned char MCP_REQUEST_TO_SEND(unsigned char data)
{
	if(data == 0|| data == 3){
		CS_pin	  = 0;/* chip select  */
		spi_transfer(RTS0);	/* Message Request-to-Send ( 0b1000 0000 )=>LSB 8=0,4=Buffer2,2=Buffer1,1=Buffer0*/
		CS_pin	  = 0;/* chip select  */
	}
	if(data == 1|| data == 3){
		CS_pin	  = 0;/* chip select  */
		spi_transfer(RTS1);	/* Message Request-to-Send ( 0b1000 0000 )=>LSB 8=0,4=Buffer2,2=Buffer1,1=Buffer0*/
		CS_pin	  = 0;/* chip select  */
	}
	if(data == 2|| data == 3){
		CS_pin	  = 0;/* chip select  */
		spi_transfer(RTS2);	/* Message Request-to-Send ( 0b1000 0000 )=>LSB 8=0,4=Buffer2,2=Buffer1,1=Buffer0*/
		CS_pin	  = 0;/* chip select  */
	}
	return 0;
}
unsigned char MCP_DATA_TX_BUFFER(unsigned char data)
{
	if(data == 0)
	{
	MCP_2515_write(0x31, 0xFF);	/* TRANSMIT BUFFER 0 STANDARD IDENTIFIER REGISTER HIGH */
	MCP_2515_write(0x32, 0xE0);	/* TRANSMIT BUFFER 0 STANDARD IDENTIFIER REGISTER LOW */
	MCP_2515_write(0x33, 0x00);	/* TRANSMIT BUFFER 0 EXTENDED IDENTIFIER 8 REGISTER HIGH */
	MCP_2515_write(0x34, 0x00);	/* TRANSMIT BUFFER 0 EXTENDED IDENTIFIER 0 REGISTER LOW */
	MCP_2515_write(0x35, 0x00);	/* TRANSMIT BUFFER 0 DATA LENGTH CODE REGISTER */
	MCP_2515_write(0x36, 0); /* TRANSMIT BUFFER 0 DATA BYTE m REGISTER */
	MCP_2515_write(0x37, 0); /* TRANSMIT BUFFER 1 DATA BYTE m REGISTER */
	MCP_2515_write(0x38, 0); /* TRANSMIT BUFFER 2 DATA BYTE m REGISTER */
	MCP_2515_write(0x39, 1); /* TRANSMIT BUFFER 3 DATA BYTE m REGISTER */
	MCP_2515_write(0x3A, 1); /* TRANSMIT BUFFER 4 DATA BYTE m REGISTER */
	MCP_2515_write(0x3B, 0); /* TRANSMIT BUFFER 5 DATA BYTE m REGISTER */
	MCP_2515_write(0x3C, 0); /* TRANSMIT BUFFER 6 DATA BYTE m REGISTER */
	MCP_2515_write(0x3D, 0); /* TRANSMIT BUFFER 7 DATA BYTE m REGISTER */
	MCP_REQUEST_TO_SEND(0);/* Message Request-to-Send */
	}
	if(data == 1){
	MCP_2515_write(0x41, 0xFF);	/* TRANSMIT BUFFER 1 STANDARD IDENTIFIER REGISTER HIGH */
	MCP_2515_write(0x42, 0xE0);	/* TRANSMIT BUFFER 1 STANDARD IDENTIFIER REGISTER LOW */
	MCP_2515_write(0x43, 0x00);	/* TRANSMIT BUFFER 1 EXTENDED IDENTIFIER 8 REGISTER HIGH */
	MCP_2515_write(0x44, 0x00);	/* TRANSMIT BUFFER 1 EXTENDED IDENTIFIER 0 REGISTER LOW */
	MCP_2515_write(0x45, 0x00);	/* TRANSMIT BUFFER 1 DATA LENGTH CODE REGISTER */
	MCP_2515_write(0x46, 0); /* TRANSMIT BUFFER 0 DATA BYTE m REGISTER */
	MCP_2515_write(0x47, 0); /* TRANSMIT BUFFER 1 DATA BYTE m REGISTER */
	MCP_2515_write(0x48, 0); /* TRANSMIT BUFFER 2 DATA BYTE m REGISTER */
	MCP_2515_write(0x49, 1); /* TRANSMIT BUFFER 3 DATA BYTE m REGISTER */
	MCP_2515_write(0x4A, 1); /* TRANSMIT BUFFER 4 DATA BYTE m REGISTER */
	MCP_2515_write(0x4B, 0); /* TRANSMIT BUFFER 5 DATA BYTE m REGISTER */
	MCP_2515_write(0x4C, 0); /* TRANSMIT BUFFER 6 DATA BYTE m REGISTER */
	MCP_2515_write(0x4D, 0); /* TRANSMIT BUFFER 7 DATA BYTE m REGISTER */
	MCP_REQUEST_TO_SEND(0);/* Message Request-to-Send */
	}
	if(data == 2){
	MCP_2515_write(0x51, 0xFF);	/* TRANSMIT BUFFER 2 STANDARD IDENTIFIER REGISTER HIGH */
	MCP_2515_write(0x52, 0xE0);	/* TRANSMIT BUFFER 2 STANDARD IDENTIFIER REGISTER LOW */
	MCP_2515_write(0x53, 0x00);	/* TRANSMIT BUFFER 2 EXTENDED IDENTIFIER 8 REGISTER HIGH */
	MCP_2515_write(0x54, 0x00);	/* TRANSMIT BUFFER 2 EXTENDED IDENTIFIER 0 REGISTER LOW */
	MCP_2515_write(0x55, 0x00);	/* TRANSMIT BUFFER 2 DATA LENGTH CODE REGISTER */
	MCP_2515_write(0x56, 0); /* TRANSMIT BUFFER 0 DATA BYTE m REGISTER */
	MCP_2515_write(0x57, 0); /* TRANSMIT BUFFER 1 DATA BYTE m REGISTER */
	MCP_2515_write(0x58, 0); /* TRANSMIT BUFFER 2 DATA BYTE m REGISTER */
	MCP_2515_write(0x59, 1); /* TRANSMIT BUFFER 3 DATA BYTE m REGISTER */
	MCP_2515_write(0x5A, 1); /* TRANSMIT BUFFER 4 DATA BYTE m REGISTER */
	MCP_2515_write(0x5B, 0); /* TRANSMIT BUFFER 5 DATA BYTE m REGISTER */
	MCP_2515_write(0x5C, 0); /* TRANSMIT BUFFER 6 DATA BYTE m REGISTER */
	MCP_2515_write(0x5D, 0); /* TRANSMIT BUFFER 7 DATA BYTE m REGISTER */
	MCP_REQUEST_TO_SEND(0);/* Message Request-to-Send */
	}
	return 0;
}

unsigned char MCP_DATA_RX_BUFFER(unsigned char data)
{
	if(data == 0){
	unsigned int add_rx_buffer[]={0x66,0x67,0x68,0x69,0x6A,0x6B,0x6C,0x6D};
	unsigned char val= 0x00,rte;
	for(int i=7 ;i>=0; i--){
		rte = MCP_2515_read(add_rx_buffer[i]);
		val = ((val+rte)<<1);
	}
	return val;
	}
	if(data == 1){
	unsigned int add_rx_buffer[]={0x76,0x77,0x78,0x79,0x7A,0x7B,0x7C,0x7D};
	unsigned char val= 0x00,rte;
	for(int i=7 ;i>=0; i--){
		rte = MCP_2515_read(add_rx_buffer[i]);
		val = ((val+rte)<<1);
	}
	return val;
	}
	else
		return 0;
}
