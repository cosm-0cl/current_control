/*
 * rit.c
 *
 *  Created on: 2016/12/07
 *      Author: spica
 */
#include "rit.h"
void RIT_init()
{
	LPC_SC->PCONP |= (0x1 << 16);
	LPC_SC->PCLKSEL1 |= (0x1 << 26);
	LPC_RIT -> RICOUNTER = 0;
	LPC_RIT -> RICOMPVAL = 3000;
	LPC_RIT -> RICTRL |= (0x1 << 1);
}

void RIT_Enable()
{
	LPC_RIT -> RICOUNTER = 0;
	LPC_RIT -> RICTRL |= 0x1 << 3;
	NVIC_EnableIRQ(RIT_IRQn);
}

void RIT_Disable()
{
	LPC_RIT -> RICTRL &= ~(0x1<<3);
}


