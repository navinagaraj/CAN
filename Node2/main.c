/*************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N  */
/* Date		    : 04-10-2019				 						     */
/* Filename	    : main.c	        								     */
/* Description	: mcp source file                                        */
/*************************************************************************/

#include "main.h"

__CONFIG(FOSC_HS & WDTE_OFF & PWRTE_OFF & BOREN_ON & LVP_OFF);

void init(void)
{
	uart_init();
	spi_init();
	mcp_init();
	can_init();
}



void main()
{
	unsigned char buffer[30]="";
	struct can_data txdata,rxdata;
	init();
	
	while(1)
	{
		txdata.id = 0x221;
		txdata.dlc = 8;
		txdata.rtr = 0;
		txdata.data[0] = 0x11;
		txdata.data[1] = 0x12;
		txdata.data[2] = 0x13;
		txdata.data[3] = 0x14;
		txdata.data[4] = 0x15;
		txdata.data[5] = 0x16;
		txdata.data[6] = 0x17;
		txdata.data[7] = 0x18;
		
		if(!can_tx(&txdata))
		{
			debug_print("main: message sending successfully\r\n");
		}
		else
		{
			debug_print("main: message sent failed\r\n");
		}
		
		__delay_ms(500);
		
		if(!can_rx(&rxdata))
		{
			debug_print("main: message sending successfully\r\n");
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.id);
			debug_print(buffer);
			
			sprintf(buffer,"rxdata.rtr = 0x%x\r\n",rxdata.rtr);
			debug_print(buffer);
			
			sprintf(buffer,"rxdata.dlc = 0x%x\r\n",rxdata.dlc);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[0]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[1]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[2]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[3]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[4]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[5]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[6]);
			debug_print(buffer);
			sprintf(buffer,"rxdata.id = 0x%x\r\n",rxdata.data[7]);
			debug_print(buffer);
		}
		else
		{
			debug_print("main: message sent failed\r\n");
			
		}
		__delay_ms(500);
		//while(1);
	}
}