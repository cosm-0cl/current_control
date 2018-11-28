#include "adc.h"

//----------------------------------------
//ADコンバータ初期化関
//----------------------------------------
//戻り値:なし
//引数   :pin 読み取るピンの場所のビットに1を立てる
//----------------------------------------
void ADC_init(unsigned char pin)
{
	int i;
	//ピンモードの設定
	for(i = 0;i < 4;i++)
	{
		LPC_PINCON -> PINSEL1 |= ((pin & 0x1 << i) ? 1 : 0) << (14 + i*2);
	}
	for(i = 4;i < 6;i++)
	{
		LPC_PINCON -> PINSEL3 |= ((pin & 0x1 << i) ? 1 : 0)  << (28 + i*2);
	}
	//先にADCの電源を入れる
	LPC_SC -> PCONP |= ADCON;
	//PCLKは13MHz以下に設定されなければならない  12MHz
	LPC_SC  -> PCLKSEL0 |= ADC_CLKSEL;
	LPC_ADC -> ADCR |= ADC_CLKDIV5;


	//PDNに1を書き込むとリセットされる
	LPC_ADC -> ADCR |= ADC_OP;

	LPC_ADC->ADINTEN |= 0x1;
	LPC_ADC->ADINTEN &=~(0x1<<8);
	NVIC_EnableIRQ(ADC_IRQn);

}

//---------------------------
//ADコンバータ読み取り用関数
//---------------------------
//戻り値：電圧(12ビット)
//引数　：pin　読み取るADCの場所を整数で
//---------------------------
uint32_t ADC_Read(int Channel)
{
	volatile uint32_t *CheckChannel;
	//初期値0x1のため0にしてから読み込むピンの場所に書き込む
	LPC_ADC -> ADCR &= 0xFFFFFE00;
	LPC_ADC -> ADCR |= 1 << Channel;

	//STARTを000にしてから001にして変換を開始
	LPC_ADC->ADCR &= 0xF8FFFFFF;
	LPC_ADC->ADCR |= ADC_START;

	//データを読み取る
	CheckChannel = (uint32_t *)(&(LPC_ADC->ADDR0) + Channel);
	//変換が終わるまで待つ(DONE == 1になるまで待つ)
	while ((*CheckChannel & 0x80000000) == 0);

	//変換が終わったらSTARTを000にする
	LPC_ADC->ADCR &= 0xF8FFFFFF;
	//マスクをかけて値を返す
	return (*CheckChannel>>4) & 0xFFF;
}

//回転方向+でマイナス電流
//処理時間7[us] 内ポーリング5.5[us] @12MHz
uint32_t ADC_Read0()
{
	//volatile uint32_t *CheckChannel;
	//初期値0x1のため0にしてから読み込むピンの場所に書き込む
	//LPC_ADC -> ADCR &= 0xFFFFFE00;
	//LPC_ADC -> ADCR |= 0x1;

	//STARTを000にしてから001にして変換を開始
	LPC_ADC->ADCR &= 0xF8FFFFFF;
	LPC_ADC->ADCR |= ADC_START;

	//データを読み取る
	//CheckChannel = (uint32_t *)(&(LPC_ADC->ADDR0));
	//変換が終わるまで待つ(DONE == 1になるまで待つ)
	while ((LPC_ADC->ADDR0 & 0x80000000) == 0);
	//変換が終わったらSTARTを000にする
	LPC_ADC->ADCR &= 0xF8FFFFFF;
	//マスクをかけて値を返す
	return (LPC_ADC->ADDR0/16) & 0xFFF;
}


