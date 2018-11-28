#include "LPC17xx.h"
#ifndef PWM_H_
#define PWM_H_

#define PWMON        0x40
#define TCR_CNT_EN   0x1
#define TCR_PWM_EN   0x8

//高速化用関数マクロ
//duty 0-2000
#define  UPDATE_PWM_MR4(duty4) \
		do{\
			LPC_PWM1 -> MR4 = duty4;\
			LPC_PWM1 -> LER = 0x10;\
		}while(0)

//Latchレジスタ書き込み用変数
volatile static uint32_t LER_buf = 0;

void LER_write();
void Freq_write(int freq);
void update_duty(int addr,int duty);
void PWM_init(unsigned char pin);



#endif /* PWM_H_ */
