#include "lpc1769_init.h"

void LPC_init(void)
{
	//(1)オシレータの設定
	//12MHz外付けクリスタル使用
	LPC_SC -> SCS |= OSC_EN;
	//オシレータが安定するまで待つ
	while((LPC_SC -> SCS & OSC_STAT) == 0){}

	//(2)PLL0の設定
	//PLL0を無効にしておく
	LPC_SC -> PLL0CON = 0;
	//FEEDレジスタに0xAAを書き込んでから、再度0x55を書き込むと設定が反映される
	LPC_SC -> PLL0FEED = 0xAA;
	LPC_SC -> PLL0FEED = 0x55;

	//クロックの源を外部オシレータに設定
	LPC_SC -> CLKSRCSEL |= MAIN_OSC_SEL;

	//Fcco(PLLCLK) = 480MHz , M = 20 , N = 1 (M-1,N-1を書き込む)
	LPC_SC -> PLL0CFG = PLL0MVAL | PLL0NVAL;
	//FEEDレジスタに0xAAを書き込んでから、再度0x55を書き込むと設定が反映される
	LPC_SC -> PLL0FEED = 0xAA;
	LPC_SC -> PLL0FEED = 0x55;

	//PLL0の使用を許可する
	LPC_SC -> PLL0CON = PLL0ENA;
	//FEEDレジスタに0xAAを書き込んでから、再度0x55を書き込むと設定が反映される
	LPC_SC -> PLL0FEED = 0xAA;
	LPC_SC -> PLL0FEED = 0x55;

	//CCLKをPLLCLK/4に設定する(CCLK = 120MHz)
	LPC_SC -> CCLKCFG = CCLK_DIV4;

	// PLL0が安定するまで待つ。
	while ((LPC_SC->PLL0STAT & PLL0_UNLOCKED)==0);

	//PLL0を接続する
	LPC_SC -> PLL0CON = PLL0CONNECT;
	//FEEDレジスタに0xAAを書き込んでから、再度0x55を書き込むと設定が反映される
	LPC_SC -> PLL0FEED = 0xAA;
	LPC_SC -> PLL0FEED = 0x55;

	//USB用クロックはPLLCLK/10に設定する
	LPC_SC -> USBCLKCFG = USBCLK_DIV10;
	//FEEDレジスタに0xAAを書き込んでから、再度0x55を書き込むと設定が反映される
	LPC_SC -> PLL0FEED = 0xAA;
	LPC_SC -> PLL0FEED = 0x55;

	//確認用のクロックを1/5でP1.27から出力
	LPC_PINCON->PINSEL3 = CLK_OUTPUT;
	LPC_SC->CLKOUTCFG = CLK_OUTEN | CLK_OUT_DIV5;

}

