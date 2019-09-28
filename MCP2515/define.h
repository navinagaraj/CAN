/****************************************/
/* Author	    : Naveenkumar N         */
/* Date		    : 25-09-2019            */
/* Filename	    : define.h              */
/* Description	: define file           */
/****************************************/


#ifndef define_H
#define define_H

#define CS_pin RB1
#define _XTAL_FREQ 20000000

#define MCP_CANSTAT_REG 0X0E
#define MCP_CANCTRL_REG 0X0F




#define MCP_RESET_INS  0xc0
#define MCP_READ_INS   0x03
#define MCP_WRITE_INS  0x02
#define BIT_MODIF_INS  0x05


#define MCP_NORMAL_MODE			0x00
#define MCP_CONFIG_MODE			0x80
#define MCP_LISTENONLY_MODE		0x60
#define MCP_LOOPBACK_MODE		0x40
#define MCP_SLEEP_MODE			0x20

#define RTS0 0x81
#define RTS1 0x82
#define RTS2 0x84


#endif