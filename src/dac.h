#include "LPC17xx.h"
#ifndef DAC_H_
#define DAC_H_

//マクロ関数
#define DACWRITE(val) \
	do{\
		LPC_DAC -> DACR = val << 6;\
	}while(0)\

void DAC_init(void);
void DAC_write(uint32_t val);


#endif /* DAC_H_ */
