#include "dac.h"

void DAC_init(void)
{
	LPC_SC -> PCLKSEL0 |= 0x1 << 22;
	LPC_PINCON -> PINSEL1 |= 0x2 << 20;
}

//立ち上がり＆立下り時間およそ800[ns]
void DAC_write(uint32_t val)
{
	LPC_DAC -> DACR = val << 6;
}
