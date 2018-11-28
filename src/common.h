#include "LPC17xx.h"
#ifndef COMMON_H_
#define COMMON_H_

//ウォッチドッグタイマ関連
#define WDT_PCLK_SEL  0x1
#define WDT_CCLK_SEL  0x1
#define WDT_START     0x1
#define CLKSELLOCKED  0x80000000

//リングバッファ関連
#define BUFSIZE  10

//クイックソート関連
#define swap(type,x,y) do{type t = x;x = y;y = t;}while(0)

typedef struct
{
	uint32_t lingbuf[BUFSIZE];
	int front;
	int rear;
}QUEUE;

typedef struct
{
	int a;
	long vel;
	long x;
}ACCE;

void LPC_init(void);
void WDT_init(void);
void WDT_Reset(void);
void usecWait (uint32_t usec);
int hex2str(int hexval, unsigned char *str, int len);
int atoi(unsigned char *ch);
int pow(int x,int y);
int buf_clean(unsigned char *buf);
int strlen(unsigned char *str);
int strcmp(unsigned char *str1, unsigned char *str2);
void enqueue(QUEUE *x,uint32_t data,int len);
int next(int data,int len);
uint32_t average(uint32_t *x,int max);
void quick(uint32_t *a,int left,int right);
uint32_t median(uint32_t *a,int max);
void write(int d,int base);
void delayMs(uint8_t timer_num, uint32_t delayInMs);



#endif /* COMMON_H_ */
