#include "LPC17xx.h"
#include "common.h"

#ifndef UART_H_
#define UART_H_

#define UART1_START    0x10
#define UART1_CLKDIV1  0x1 <<8
#define UART1_WL8      0x3
#define UART1_DLAB     0x80
#define UART1_TXDSET   0x40000000
#define UART1_RXDSET   0x1
#define UART1_FIFOENA  0x1
#define UART1_TRENA    0x8


#define UART_LSR_RDR        1 << 0 //Receive Data Ready（0の時データなし、1の時データあり）
#define UART_LSR_THRE       1 << 5
#define UART_LSR_RDR        1 << 0


//エスケープシーケンス
#define ESC_SEQ_1LEFT       "\x1b[1D"	//1つ左にカーソルを戻す
#define ESC_SEQ_1RIGHT      "\x1b[1C"	//1つ右にカーソルを進める
#define ESC_SEQ_UP          "\x1b[A"	//上キー

void UART1_init(int rate);
void BR_init(int rate);
void DLM_DLL_init(uint32_t DLest);
void putc(unsigned char c);
void puts(unsigned char *str);
unsigned char getc_nondisp(void);
unsigned char getc_nondisp(void);
int gets(unsigned char *buf);
int gets_nondisp(unsigned char *buf);



#endif /* UART_H_ */
