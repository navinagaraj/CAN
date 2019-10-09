/*************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N  */
/* Date		    : 04-10-2019				 						     */
/* Filename	    : can.h		        								     */
/* Description	: header file                                            */
/*************************************************************************/

#ifndef CAN_H
#define CAN_H

#include "pic16f877a_uart.h"
#include "mcp2515.h"

#define	DATA_FRAME		0
#define	REMOTE_FRAME	1


struct can_data{
	unsigned short int id;
	unsigned char dlc:4;
	unsigned char rtr:1;
	unsigned char res:3;
	unsigned char data[8];
};

void can_init(void);
char can_tx(struct can_data* data);

//if return value is 0, valid data is recieved, else data is invalid.
char can_rx(struct can_data* data);

#endif // CAN_H