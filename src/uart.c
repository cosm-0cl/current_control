#include "uart.h"

//-------------------------
//UART1初期化関数
//-------------------------
//戻り値：なし
//引数   ：rate ボーレート
//-------------------------
void UART1_init(int rate)
{
	//UART1の電源を入れる(初期状態で入っている）
	LPC_SC -> PCONP |= UART1_START;
	//pclk = cclk  に設定
	LPC_SC -> PCLKSEL0 |= UART1_CLKDIV1;
	//８ビット長、１ストップビット、パリティなしで設定し、ボーレート設定のためにDLABを１にする
	LPC_UART1 -> LCR |= UART1_WL8 | UART1_DLAB;
	//ボーレートの設定
	BR_init(rate);
	//FIFOを有効にする
	LPC_UART1 -> FCR |= UART1_FIFOENA;
	//p0.15をTXD,p0.16をRXDに設定
	LPC_PINCON -> PINSEL0 |= UART1_TXDSET;
	LPC_PINCON -> PINSEL1 |= UART1_RXDSET;
	//DLABを0にする（THRにアクセスできないため）
	LPC_UART1 -> LCR &= ~UART1_DLAB;
	//送信有効にする
	LPC_UART1  -> TER |= UART1_TRENA;
}

//-----------------------
//ボーレート設定関数
//-----------------------
//戻り値：なし
//引数   :rate ボーレート
//-----------------------
void BR_init(int rate)
{
	switch(rate)
	{
	case 300:
		LPC_UART1 -> FDR |= 0x10;
		break;
	case 1200:
		LPC_UART1 -> FDR |= 0x10;
		break;
	case 2400:
		LPC_UART1 -> FDR |= 0x10;
		break;
	case 4800:
		LPC_UART1 -> FDR |= 0x20;
		DLM_DLL_init(1041);
		break;
	case 9600:
		LPC_UART1 -> FDR |= 0xF8;//計算値は0x21
		DLM_DLL_init(520);
		break;
	case 14400:
		LPC_UART1 -> FDR |= 0x20;
		DLM_DLL_init(347);
		break;
	case 19200:
		LPC_UART1 -> FDR |= 0x20;
		DLM_DLL_init(260);
		break;
	case 28800:
		LPC_UART1 -> FDR |= 0x20;
		DLM_DLL_init(173);
		break;
	case 38400:
		LPC_UART1 -> FDR |= 0xF7;
		DLM_DLL_init(133);
		break;
	case 57600:
		LPC_UART1 -> FDR |= 0x20;
		DLM_DLL_init(86);
		break;
	case 115200:
		LPC_UART1 -> FDR |= 0xF8;//計算値は0x21
		DLM_DLL_init(43);
		break;
	default:
		break;
	}
}

//-----------------------------
//DLM,DLL書き込み関数
//-----------------------------
//戻り値：なし
//引数  :DLest 計算したDLestの値
//-----------------------------
void DLM_DLL_init(uint32_t DLest)
{
	LPC_UART1 -> DLL = DLest & 0xFF;
	LPC_UART1 -> DLM = (DLest >> 8) & 0xFF;
}


void putc(unsigned char c) {
	if(c == '\n') c = '\r';
	while((LPC_UART1 -> LSR & UART_LSR_THRE) == 0);
	LPC_UART1 -> THR = c;
}

void puts(unsigned char *str) {
	while(*str) putc(*(str++));
}

unsigned char getc_nondisp(void) {
	unsigned char c = 0;

	while((LPC_UART1 -> LSR & UART_LSR_RDR) == 0); //データが来るまで待機
	c = LPC_UART1 -> RBR;

	return c;
}

unsigned char getc(void) {
	unsigned char c = 0;

	c = getc_nondisp();
	putc(c);

	return c;
}

int gets(unsigned char *buf){
	int i = 0;
	unsigned char c;

	buf_clean(buf);

	do{
    	c = getc();
    	if(c == '\n' || c == '\r') {
			c = '\0';
    		buf[i++] = c;
		}
		else if(c == 0x08 && i > 0) { //BackSpace
				i--;
				buf[i] = 0;
				putc(' ');
				puts(ESC_SEQ_1LEFT);
		}
		else if(c == 0x08 && i == 0) {
			puts(ESC_SEQ_1RIGHT);
		}
		else buf[i++] = c;
  	}
  	while(c || !strcmp(buf,ESC_SEQ_UP));

  	return i - 1;
}

int gets_nondisp(unsigned char *buf){
	int i = 0;
	unsigned char c;

	buf_clean(buf);

	do{
    	c = getc_nondisp();
    	if(c == '\n' || c == '\r') {
			putc(c);
			c = '\0';
		}
    	buf[i++] = c;
  	}
  	while(c);

  	return i - 1;
}
