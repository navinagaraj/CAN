/*************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N  */
/* Date		    : 04-10-2019				 						     */
/* Filename	    : can.c		        								     */
/* Description	: source file                                            */
/*************************************************************************/



#include "can.h"

void can_init(void)
{
	debug_print("can_init: initialized\n\r");
}

char can_tx(struct can_data* data)
{
	if(mcp_loadtx(data) == (char)-1)
	{
		debug_print("can_tx: message sending failed\r\n");
		return -1;
	}
	else
	{
		debug_print("can_tx: message sent successfully\r\n");
		return 0;
	}
}

char can_rx(struct can_data* data)
{
	//mcp_test_read();
	if((char)-1 == mcp_readrx(data))
	{
		debug_print("can_rx: no message received\r\n");
		return -1;
	}
	else
	{
		debug_print("can_Rx: message received successfully\r\n");
		return 0;
	}
}

