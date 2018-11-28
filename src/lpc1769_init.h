#include "LPC17xx.h"
#ifndef LPC1769_INIT_H_
#define LPC1769_INIT_H_

//オシレータ関連
#define OSC_EN        0x20
#define OSC_STAT      0x40
#define MAIN_OSC_SEL  0x1

//PLL0関連
#define PLL0ENA       0x1
#define PLL0CONNECT   0x3
#define PLL0MVAL      19
#define PLL0NVAL      0 << 16
#define CCLK_DIV4     3 //割る数-1に設定
#define USBCLK_DIV10  0x1 << 9 //割る数-1に設定
#define PLL0_UNLOCKED 0x1 << 26

//確認用クロック出力関連
#define CLK_OUTPUT    0x1 << 22
#define CLK_OUTEN     0x1 << 8
#define CLK_OUT_DIV5  0x1 << (2+4) //割る数-1に設定

void LPC_init(void);

#endif /* LPC1769_INIT_H_ */
