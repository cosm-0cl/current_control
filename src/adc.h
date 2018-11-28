#include "LPC17xx.h"
#ifndef ADC_H_
#define ADC_H_

#define ADCON       0x1 << 12
#define ADC_CLKSEL  0x2 << 24
#define ADC_CLKDIV5 0x4 << 8		//割る数-1に設定
#define ADC_OP      0x1 << 21
#define ADC_START   0x01000000

/*マクロ関数*/
//処理時間：209[ns]
#define ADC0READ() (LPC_ADC->ADDR0/16) & 0xFFF
//処理時間:525[ns]
#define ADC0TRIGGER() \
	do{\
		LPC_ADC->ADCR &= 0xF8FFFFFF;\
		LPC_ADC->ADCR |= ADC_START;\
	}while(0)
void ADC_init(unsigned char pin);
uint32_t ADC_Read(int pin);
uint32_t ADC_Read0();


#endif /* ADC_H_ */
