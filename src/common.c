#include "common.h"
#include "uart.h"

const unsigned char array[] = "0123456789ABCDEF";

//--------------------------------
//Watch Dog Timer 初期化関数
//--------------------------------
//戻り値:なし
//引数   :なし
//--------------------------------
void WDT_init(void)
{
	//WDTCLK = CCLK に設定
	LPC_SC -> PCLKSEL0  |= WDT_CCLK_SEL;

	//クロックのもとをPCLKに設定
	LPC_WDT -> WDCLKSEL = WDT_PCLK_SEL | CLKSELLOCKED;

	//カウンタの値を最大にする
	LPC_WDT -> WDTC   = 0xFFFFFFFF;

	//ウォッチドッグタイマを起動
	LPC_WDT ->WDMOD = WDT_START;
}

//--------------------------------
//Watch dog Timer リセット関数
//--------------------------------
//戻り値:なし
//引数   :なし
//--------------------------------
void WDT_Reset(void)
{
	//FEEDレジスタに0xAAを書き込んでから、再度0x55を書き込むとWDTがリセットされる
	LPC_WDT -> WDFEED = 0xAA;
	LPC_WDT -> WDFEED = 0X55;
}
//---------------------------------
//delay関数
//---------------------------------
//戻り値：なし
//引数　：usec 時間[us]
//---------------------------------
void usecWait (uint32_t usec)
{
	volatile uint32_t loop = 0;
	volatile uint32_t usec_cnt;
	for (usec_cnt=0; usec_cnt<usec; usec_cnt++)
	{
		while(loop<10)//CCLK=120MHz,最適化最大でおよそ0.934[us]になるよう設定
		{
			loop++;
		}
		loop=0;
	}
}

int hex2str(int  hexval, unsigned char *str, int len) {
	int i = 0;
	unsigned char hex[] = "0123456789ABCDEF";
	buf_clean(str);

	if(hexval < 0)
	{
		str[0] = '-';
		hexval *= -1;
		for(i=len-1; i>=1; i--) {
			str[i] = hex[hexval & 0x0F];
			hexval = hexval >> 4;
		}
	}
	else
	{
		for(i=len-1; i>=0; i--) {
			str[i] = hex[hexval & 0x0F];
			hexval = hexval >> 4;
		}
	}
	return 0;
}

int atoi(unsigned char *ch)
{
	int a = 0,i;
	int num = 0;
	int len;

	len = strlen(ch);

	for(i = len-1;i >= 0 ;i--)
	{
		num = *ch & 0x0F;
		a += pow(10,i) *  num;
		ch++;
	}
	return a;
}
int pow(int x,int y)
{
	int a = 1,i;
	for(i = 0;i < y;i++)
	{
		a = a*x;
	}
	return a;
}

int buf_clean(unsigned char *buf) {
	int i;
	for(i=0; i<strlen(buf); i++) buf[i] = '\0';

	return 0;
}

int strlen(unsigned char *str) {
	int i = 0;
	while(str[i] != 0) i++;

	return i;
}

int strcmp(unsigned char *str1, unsigned char *str2) {
	while(*str1 == *str2 && *str1 != 0 && *str2 != 0) {
		str1++;
		str2++;
	}

	if(*str1 == 0 && *str2 == 0) return 0;
	else if(*str1 == 0 && *str2 != 0) return -1;
	else return 1;
}

//リングバッファ
void enqueue(QUEUE *x,uint32_t data,int len)
{
	x -> lingbuf[x -> rear] = data;
	x -> rear = next(x -> rear,len);
}



int next(int i,int len)
{
	return (i+1) % len;
}

//----------------------------------------
//移動平均フィルタ
//----------------------------------------
//戻り値: 平滑した値
//引数   x 配列先頭のポインタ
//     max 配列の大きさ
//----------------------------------------
uint32_t average(uint32_t *x,int max)
{
	int i;
	uint32_t sum = 0;
	for(i = 0;i < max;i++)
	{
		sum += x[i];
	}
	return sum/max;
}

//----------------------------------------
//クイックソート
//----------------------------------------
//戻り値: なし
//引数   　 a 配列先頭のポインタ
//      left 配列の左端の要素番号
//　　　　　right 配列の右端の要素番号
//----------------------------------------
void quick(uint32_t *a,int left,int right)
{
	int pl = left; 					//グループの左端に左カーソルを合わせる
	int pr = right;					//グループの右端に右カーソルを合わせる
	int x = a[(pl+pr)/2];			//枢軸は中央
	do{
		while(a[pl] < x) pl++;		//左カーソルを右にずらす
		while(a[pr] > x) pr--;		//右カーソルを左にずらす
		if(pl <= pr)				//二つのカーソルが交差していないなら
		{
			swap(uint32_t,a[pl],a[pr]);	//二つのカーソル同士の値を入れ替える
			pl++;					//左カーソルを右にずらす
			pr--;					//右カーソルを左にずらす
		}
	}while(pl <= pr);				//二つのカーソルが交差するまで続ける
	if(left < pr) quick(a,left,pr);
	if(pl < right) quick(a,pl,right);
}

//----------------------------------------
//メディアンフィルタ
//----------------------------------------
//戻り値　 中央値
//引数 　   a　配列先頭のポインタ
//    　 last 最後の要素番号
//----------------------------------------
uint32_t median(uint32_t *a,int last)
{
	quick(a,0,last);
	return a[last/2];
}

//----------------------------------------
//シリアル整数出力関数
//----------------------------------------
//戻り値　なし
//引数  d 整数
//    base n進数
//----------------------------------------
void write(int d,int base)
{
	int m;

	if(d < 0)
	{
		d *= -1;
		putc('-');
	}

	m = d % base;
	d /=  base;

	if(d != 0)
	{
		write(d,base);

	}
	putc(array[m]);

}
/*****************************************************************************
** Function name:		delayMs
**
** Descriptions:		Start the timer delay in milo seconds
**						until elapsed
**
** parameters:			timer number, Delay value in milo second
**
** Returned value:		None
**
*****************************************************************************/
void delayMs(uint8_t timer_num, uint32_t delayInMs)
{
  if ( timer_num == 0 )
  {
	LPC_TIM0->TCR = 0x02;		/* reset timer */
	LPC_TIM0->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM0->MR0 = delayInMs * (9000000 / 1000-1);
	LPC_TIM0->IR  = 0xff;		/* reset all interrrupts */
	LPC_TIM0->MCR = 0x04;		/* stop timer on match */
	LPC_TIM0->TCR = 0x01;		/* start timer */

	/* wait until delay time has elapsed */
	while (LPC_TIM0->TCR & 0x01);
  }
  else if ( timer_num == 1 )
  {
	LPC_TIM1->TCR = 0x02;		/* reset timer */
	LPC_TIM1->PR  = 0x00;		/* set prescaler to zero */
	LPC_TIM1->MR0 = delayInMs * (9000000 / 1000-1);
	LPC_TIM1->IR  = 0xff;		/* reset all interrrupts */
	LPC_TIM1->MCR = 0x04;		/* stop timer on match */
	LPC_TIM1->TCR = 0x01;		/* start timer */

	/* wait until delay time has elapsed */
	while (LPC_TIM1->TCR & 0x01);
  }
  return;
}


