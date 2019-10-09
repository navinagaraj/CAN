/*************************************************************************/
/* Author	    : Vinoth R, Jeevanandham PS, Sugumaran A, Naveenkumar N  */
/* Date		    : 04-10-2019				 						     */
/* Filename	    : mcp2515.c			        						     */
/* Description	: mcp source file                                        */
/*************************************************************************/
#include "pic16f877a_spi.h"
#include "pic16f877a_uart.h"
#include "mcp2515.h"


void mcp_init(void)
{
	if(!mcp_reset())
	{
		debug_print("mcp_init: Reset success, MCP2515 is in config mode\r\n");
	}
	else
	{
		debug_print("mcp_init: Reset failed, MCP2515 is not in config mode\r\n");
		return;
	}
	mcp_reg_tx(MCP_CNF1_ADDR, 0x41);
	mcp_reg_tx(MCP_CNF2_ADDR, 0xE5);
	mcp_reg_tx(MCP_CNF3_ADDR, 0x05);
	
	mcp_reg_tx(MCP_TXRTSCTRL_ADDR, 0x00);
	mcp_reg_tx(MCP_TXB0CTRL_ADDR, 0x03);
	mcp_reg_tx(MCP_TXB1CTRL_ADDR, 0x03);
	mcp_reg_tx(MCP_TXB2CTRL_ADDR, 0x03);
	mcp_reg_tx(MCP_BFPCTRL_ADDR, 0x00);
	mcp_reg_tx(MCP_RXB0CTRL_ADDR, 0x60);
	mcp_reg_tx(MCP_RXB1CTRL_ADDR, 0x60);

	mcp_reg_tx(MCP_CANINTF_ADDR, 0x00);
	mcp_reg_tx(MCP_CANINTE_ADDR, 0xFF);
	
	if(!mcp_mode_switch(NORMAL_MODE))
	{
		debug_print("mcp_init: initialization successful\n\r");
	}
	else
	{
		debug_print("mcp_init: initialization failed\n\r");
	}
}

char mcp_readrx(void* addr)
{
	unsigned char rcaddr=0;
	unsigned char loop1;
	volatile unsigned char ret;
	struct mcp_buff_data* buff = (struct mcp_buff_data*)addr;
	
	ret =  mcp_reg_rx(MCP_CANINTF_ADDR);
	
	if(ret&0x01)
	{
		rcaddr = MCP_RXB0CTRL_ADDR;
	}
	else if(ret&0x02)
	{
		rcaddr = MCP_RXB1CTRL_ADDR;
	}
	else
	{
		debug_print("mcp_readrx: recv buffers empty\n\r");
		return -1;
	}

	SPI_CS = 0;
	ret = spi_transfer(MCP_READ_INS);
	ret = spi_transfer(rcaddr);
	ret = spi_transfer(MCP_DUMMY_BYTE);	//MCP_RXB0CTRL_ADDR or MCP_RXB1CTRL_ADDR
	if(ret&0x08)
	{
		buff->rtr = 1;
	}
	else
	{
		buff->rtr = 0;
	}
	buff->id = spi_transfer(MCP_DUMMY_BYTE);	//SIDH
	buff->id = buff->id <<3;
	ret = spi_transfer(MCP_DUMMY_BYTE);			//SIDL
	buff->id |= ret>>5;
	ret = spi_transfer(MCP_DUMMY_BYTE);			//EID8
	ret = spi_transfer(MCP_DUMMY_BYTE);			//EID0
	ret = spi_transfer(MCP_DUMMY_BYTE);			//DLC
	buff->dlc = ret & 0x0f;
	
	
	for(loop1=0; loop1<buff->dlc; loop1++)
	{
		buff->data[loop1] = spi_transfer(MCP_DUMMY_BYTE);
	}
	SPI_CS = 1;
	__delay_ms(3.36);
	if(rcaddr == MCP_RXB0CTRL_ADDR)
	{
		mcp_bitmodify(MCP_CANINTF_ADDR,0x01,1);
	}
	else if (rcaddr == MCP_RXB1CTRL_ADDR)
	{
		mcp_bitmodify(MCP_CANINTF_ADDR,0x02,2);
	}
	else {}
	
	return 0;
}

unsigned char mcp_reg_rx(unsigned char mcp_reg_addr)
{
	volatile unsigned char ret = 0xFF;
	if(mcp_reg_addr <= 0x7F)
	{
		SPI_CS = 0;
		ret = spi_transfer(MCP_READ_INS);
		ret = spi_transfer(mcp_reg_addr);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		SPI_CS = 1;
		__delay_ms(3.36); 
	}
	else
	{
		debug_print("mcp_reg_tx: invalid 'address'\n\r");
	}
	return ret;
}

char mcp_loadtx(const void* src)
{
	unsigned char buff_free=0;
	unsigned char ins=0;
	unsigned char buf_num=0;
	unsigned char rts_val=0;

	
	unsigned char loop1;
	volatile unsigned char ret;
	
	struct mcp_buff_data* user_data = (struct mcp_buff_data*) src;
	
	ret =  mcp_reg_rx(MCP_TXB0CTRL_ADDR);
	if(!(ret&0x08))
	{
		buff_free |= 0x01;
	}
	
	ret =  mcp_reg_rx(MCP_TXB1CTRL_ADDR);
	if(!(ret&0x08))
	{
		buff_free |= 0x02;
	}
	
	ret =  mcp_reg_rx(MCP_TXB2CTRL_ADDR);
	if(!(ret&0x08))
	{
		buff_free |= 0x04;
	}
	
	if(buff_free&0x01)
	{
		ins = MCP_JUMP_TXB0SIDH_INS;
		buf_num = 0;
		rts_val = MCP_TX_BUF0_RTS;
	}
	else if(buff_free&0x02)
	{
		ins = MCP_JUMP_TXB1SIDH_INS;
		buf_num = 1;
		rts_val = MCP_TX_BUF1_RTS;
	}
	else if(buff_free&0x04)
	{
		ins = MCP_JUMP_TXB2SIDH_INS;
		buf_num = 2;
		rts_val = MCP_TX_BUF2_RTS;
	}
	else
	{
		debug_print("mcp_loadtx: TX Buffers are busy\r\n");
		return -1;
	}
	

	SPI_CS = 0;
	ret = spi_transfer(ins);
	ret = spi_transfer((unsigned char)(user_data->id >> 3));	//sending SIDH
	ret = spi_transfer((unsigned char)(user_data->id << 5));	//sending SIDL
	ret = spi_transfer(MCP_DUMMY_BYTE);	//sending EID8
	ret = spi_transfer(MCP_DUMMY_BYTE);	//sending EID0
	ret = spi_transfer((user_data->rtr==1 ? 0x40 : 0x00) | (user_data->dlc <= 8 ? user_data->dlc : 8)); //DLC and RTR is sent
	for(loop1=0; loop1<user_data->dlc; loop1++)
	{
		ret = spi_transfer(user_data->data[loop1]);
	}
	
	for(; loop1<8; loop1++)
	{
		ret = spi_transfer(MCP_DUMMY_BYTE);
	}
	SPI_CS = 1;
	__delay_ms(3.36);
	
	mcp_rts(rts_val);
	return 0;
}

void mcp_reg_tx(unsigned char mcp_reg_addr, unsigned char data)
{
	volatile unsigned char ret;
	if(mcp_reg_addr <= 0x7F)
	{
		SPI_CS = 0;
		ret = spi_transfer(MCP_WRITE_INS);
		ret = spi_transfer(mcp_reg_addr);
		ret = spi_transfer(data);
		SPI_CS = 1;
		__delay_ms(3.36);
	}
	else
	{
		debug_print("mcp_reg_tx: invalid 'address'\n\r");
	}
}

char mcp_reset(void)
{
	volatile unsigned char ret;
	SPI_CS = 0;
	ret = spi_transfer(MCP_RESET_INS);
	SPI_CS = 1;
	__delay_ms(3.36);
	
	ret = mcp_reg_rx(MCP_CANSTAT_ADDR);
	if((ret>>5) == CONFIG_MODE)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void mcp_bitmodify(unsigned char mcp_reg_addr, unsigned char mask, unsigned char data_byte)
{
	volatile unsigned char ret;
	
	if(mcp_reg_addr <= 0x7F)
	{
		SPI_CS = 0;
		ret = spi_transfer(MCP_BITMODIFY_INS);
		ret = spi_transfer(mcp_reg_addr);
		ret = spi_transfer(mask);
		ret = spi_transfer(data_byte);
		SPI_CS = 1;
		__delay_ms(3.36);
	}
	else
	{
		debug_print("mcp_bitmodify: invalid address \n\r");
	}
}

void mcp_rts(unsigned char buf_name)
{
	volatile unsigned char ret;
	SPI_CS = 0;
	ret = spi_transfer(MCP_RTS_INS | buf_name);
	SPI_CS = 1;
	__delay_ms(3.36);
}

unsigned char mcp_status(void)
{
	volatile unsigned char ret;
	SPI_CS = 0;
	ret = spi_transfer(MCP_STAT_INS);
	ret = spi_transfer(MCP_DUMMY_BYTE);
	SPI_CS = 1;
	__delay_ms(3.36);
	return ret;
}

char mcp_mode_switch(unsigned char mode)
{
	volatile unsigned char ret;
	mcp_bitmodify(MCP_CANCTRL_ADDR,0xE0,mode<<5);
	
	ret = mcp_reg_rx(MCP_CANSTAT_ADDR);
	if((ret>>5) == mode)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

void mcp_test_read(void)
{
		volatile unsigned char ret;
		unsigned char buffer[30]="";
		SPI_CS = 0;
		ret = spi_transfer(MCP_JUMP_RXB0SIDH_INS);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"SIDH = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"SIDL = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"EID8 = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"EID0 = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"DLC  = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D0   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D1   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D2   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D3   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D4   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D5   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D6   = 0x%x\r\n",ret);
		debug_print(buffer);
		ret = spi_transfer(MCP_DUMMY_BYTE);
		sprintf(buffer,"D7   = 0x%x\r\n",ret);
		SPI_CS = 1;
}