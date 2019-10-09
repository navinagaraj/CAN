/****************************************/
/* Author	    : Naveenkumar N         */
/* Date		    : 04-10-2019            */
/* Filename	    : main.h              */
/* Description	: header file           */
/****************************************/

#ifndef MAIN_H
#define MAIN_H

#include <htc.h>
#include <stdio.h>
#include "pic16f877a_uart.h"
#include "pic16f877a_spi.h"
#include "can.h"

#define _XTAL_FREQ 20000000

void init(void);

#endif //MAIN_H