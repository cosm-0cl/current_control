#include "qei.h"

void QEI_init(void)
{
	//QEIの電源を入れる
	LPC_SC -> PCONP |= QEI_PWRON;
	//PCLKの設定
	LPC_SC -> PCLKSEL1 |= PCLK_QEI;
	//ピンのファンクション設定
	LPC_PINCON -> PINSEL3 |= QEI_MCI0 | QEI_MCI1 | QEI_MCI2;

	//カウンタをリセット
	LPC_QEI -> QEICON |= QEI_RESET;

	//分解能を4倍にする
	LPC_QEI -> QEICONF |= 0x1 << 2;
	//LPC_QEI -> QEICONF |= 0x1 << 1;
	//位置の最大値を指定
	LPC_QEI -> QEIMAXPOS |= 2000 *43; //ギヤ1:26
	//時間周期の設定
	LPC_QEI -> QEILOAD = 120000/8;
}

int QEI_Pos_Read()
{
	return LPC_QEI -> QEIPOS & 0xFFFFFFFF;
}

uint32_t QEI_Vel_Read()
{
	//uint32_t cap = LPC_QEI -> QEICAP & 0xFFFFFFFF;
	//uint32_t load = LPC_QEI -> QEILOAD & 0xFFFFFFFF;
	return (LPC_QEI -> QEICAP & 0xFFFFFFFF) * 30;
//	return (QEI_PCLK * cap * 60)/ (load * 500 * 4);
}

