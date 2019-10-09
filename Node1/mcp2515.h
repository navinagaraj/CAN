/*************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N  */
/* Date		    : 04-10-2019				 						     */
/* Filename	    : mcp2515.h			        						     */
/* Description	:  header file											 */
/*************************************************************************/

#ifndef MCP2515_H
#define MCP2515_H

#include <stdio.h>
#include "pic16f877a_spi.h"

#define MCP_DUMMY_BYTE	0x00

#define MCP_TX_BUF0_RTS 	0x01
#define MCP_TX_BUF1_RTS 	0x02
#define MCP_TX_BUF2_RTS 	0x04	

#define NORMAL_MODE		0x0
#define SLEEP_MODE		0x1
#define LOOPBACK_MODE	0x2
#define LISTEN_MODE		0x3
#define CONFIG_MODE		0x4

#define MCP_READ_INS		0x03
#define MCP_WRITE_INS		0x02
#define MCP_RESET_INS		0xC0
#define MCP_BITMODIFY_INS	0x05
#define MCP_RTS_INS			0x80
#define MCP_STAT_INS		0xA0

#define MCP_JUMP_TXB0SIDH_INS	0x40
#define MCP_JUMP_TXB0D0_INS		0x41
#define MCP_JUMP_TXB1SIDH_INS	0x42
#define MCP_JUMP_TXB1D0_INS		0x43
#define MCP_JUMP_TXB2SIDH_INS	0x44
#define MCP_JUMP_TXB2D0_INS		0x45
#define MCP_JUMP_RXB0SIDH_INS   0x90
#define MCP_JUMP_RXB1SIDH_INS   0x94

#define MCP_CANCTRL_ADDR 	0x0F
#define MCP_CANSTAT_ADDR 	0x0E
#define MCP_TXRTSCTRL_ADDR	0x0D
#define MCP_CNF1_ADDR		0x2A
#define MCP_CNF2_ADDR		0x29
#define MCP_CNF3_ADDR		0x28

#define MCP_TXB0CTRL_ADDR	0x30
#define MCP_TXB1CTRL_ADDR	0x40
#define MCP_TXB2CTRL_ADDR	0x50
#define MCP_BFPCTRL_ADDR	0x0C
#define MCP_RXB0CTRL_ADDR	0x60
#define MCP_RXB1CTRL_ADDR	0x70

#define MCP_CANINTF_ADDR	0x2C
#define MCP_CANINTE_ADDR	0x2B

struct mcp_buff_data{
	unsigned short int id;
	unsigned char dlc:4;
	unsigned char rtr:1;
	unsigned char res:3;
	unsigned char data[8];
};

void mcp_init(void);
char mcp_readrx(void* addr);
unsigned char mcp_reg_rx(unsigned char mcp_reg_addr);
char mcp_loadtx(const void* src);
void mcp_reg_tx(unsigned char mcp_reg_addr, unsigned char data);
char mcp_reset(void);
void mcp_bitmodify(unsigned char mcp_reg_addr, unsigned char mask, unsigned char data_byte);
void mcp_rts(unsigned char buf_name);
char mcp_status(void);
char mcp_mode_switch(unsigned char mode);
void mcp_test_read(void);

#endif // MCP2515_H