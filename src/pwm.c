#include "pwm.h"

//-----------------------------------------------
//PWM初期化ルーチン
//-----------------------------------------------
//戻り値:　なし
//引数   : pin PWMを使用するピンの番号-1ビット目に1を立てる
//-----------------------------------------------
void PWM_init(unsigned char pin)
{
	volatile int i;

	//PWM1の電源を入れる
	LPC_SC -> PCONP |= PWMON;
	//PCLK=CCLKに設定
	LPC_SC -> PCLKSEL0 |= 0x1000;
	//ピンモードをPWMに設定
	for(i = 0;i < 6;i++)
	{
		if(pin & 0x1 << i)
		{
			LPC_PINCON -> PINSEL4 |= 0x1 << (i*2);
		}
	}
	LPC_PWM1->MCR = 1 << 1;
	//周波数とデューティ比の設定
	LPC_PWM1 -> PR  = 2;
	LPC_PWM1 -> MR0 = 2000;//40000000/20000;//4kHzに設定

	//各ピンのデューティ比の初期値は全て0にする
	LPC_PWM1 -> MR1 = 0;
	LPC_PWM1 -> MR2 = 0;
	LPC_PWM1 -> MR3 = 0;
	LPC_PWM1 -> MR4 = 0;
	LPC_PWM1 -> MR5 = 0;
	LPC_PWM1 -> MR6 = 0;

	//MRレジスタの設定の変更を反映する
	LPC_PWM1 -> LER |=0x7F;
	//PWMのモード設定(シングルエッジモード)と出力を有効にする
	for(i = 0;i < 6;i++)
	{
		if(pin & 0x1 << i)
		{
			LPC_PWM1 -> PCR |= 0x1 << (i+9);
		}
	}
	//カウンタとPWMの許可
	LPC_PWM1 -> TCR |= TCR_CNT_EN | TCR_PWM_EN;

}

//----------------------------------------
//PWM周波数変更用関数
//----------------------------------------
//戻り値:Latchレジスタの番地
//引数   :freq 周波数[Hz] 1Hz～1MHz
//----------------------------------------
void Freq_write(int freq)
{

	LPC_PWM1 -> MR0 = 40000000 / freq;
	LER_buf |= 0x1;
}

//----------------------------------------
//PWM1.1デューティ比設定用関数
//----------------------------------------
//戻り値:Latchレジスタの番地
//引数　: addr 変更するPWMの場所
//       duty デューティ比
//----------------------------------------
void update_duty(int addr,int duty)
{
	switch(addr){
	case 1:
		LPC_PWM1 -> MR1 = LPC_PWM1 -> MR0 * duty / 100;
		LER_buf |= 0x2;
		break;
	case 2:
		LPC_PWM1 -> MR2 = LPC_PWM1 -> MR0 * duty / 100;
		LER_buf |= 0x4;
		break;
	case 3:
		LPC_PWM1 -> MR3 = LPC_PWM1 -> MR0 * duty / 100;
		LER_buf |= 0x8;
		break;
	case 4:
		LPC_PWM1 -> MR4 = LPC_PWM1 -> MR0 * duty / 100;
		LER_buf |= 0x10;
		break;
	case 5:
		LPC_PWM1 -> MR5 = LPC_PWM1 -> MR0 * duty / 100;
		LER_buf |= 0x20;
		break;
	case 6:
		LPC_PWM1 -> MR6 = LPC_PWM1 -> MR0 * duty / 100;
		LER_buf |= 0x40;
		break;
	}
}

//-----------------------------------------
//PWM1Latchレジスタ書き込み関数
//-----------------------------------------
//戻り値:なし
//引数　:なし
//-----------------------------------------
void LER_write()
{
	//なんかこれ無いと動かない
	//usecWait(5);
	LPC_PWM1 -> LER = LER_buf;
	LER_buf = 0;
}

