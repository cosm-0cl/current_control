#include "LPC17xx.h"
#include "pwm.h"
#include "dac.h"
#ifndef MAIN_H_
#define MAIN_H_

#define P026_GPIO (0x04000000)//(0x1 << 26)
#define P204_GPIO (0x10)//0x1 << 4
#define DUTY_LIMIT (1000)
#define PWM_LIMIT_MINUS(val) (val < -DUTY_LIMIT ? DUTY_LIMIT : -val)
#define PWM_LIMIT_PLUS(val)  (val > DUTY_LIMIT ? 2000-DUTY_LIMIT : 2000-val)

//モータドライバ関数マクロ
//実行時間およそ400[ns] 48clocks
//do whileを外せば速くなるかも
#define MOTOR_DRIVE(duty)\
do{\
	if(duty >= 0)\
	{\
		UPDATE_PWM_MR4(PWM_LIMIT_PLUS(duty));\
		LPC_GPIO2 -> FIOCLR = P204_GPIO;\
	}\
	else\
	{\
		UPDATE_PWM_MR4(PWM_LIMIT_MINUS(duty));\
		LPC_GPIO2 -> FIOSET = P204_GPIO;\
	}\
}while(0)

#define SIG_OUT_ONE() LPC_GPIO0 -> FIOSET = P026_GPIO
#define SIG_OUT_ZERO() LPC_GPIO0 -> FIOCLR = P026_GPIO

#define DAC_DEBUG

void GPIO_init()
{
	LPC_GPIO2 -> FIODIR |= 0x17;//p2.0,p2.1,p2.2,p2.4

#ifndef DAC_DEBUG
	LPC_GPIO0 -> FIODIR |= P026_GPIO;
#else
	DAC_init();
#endif


}

//処理時間計測用出力
void SIG_OUT(char val)
{
	if(val == 1)LPC_GPIO0 -> FIOSET = P026_GPIO;
	else LPC_GPIO0 -> FIOCLR = P026_GPIO;
}

void LED_GREEN(char val)
{
	if(val == 1)LPC_GPIO2 -> FIOSET = 0x1;
	else LPC_GPIO2 -> FIOCLR = 0x1;
}

void LED_BLUE(char val)
{
	if(val == 1)LPC_GPIO2 -> FIOSET = 0x2;
	else LPC_GPIO2 -> FIOCLR = 0x2;
}

void LED_RED(char val)
{
	if(val == 1)LPC_GPIO2 -> FIOSET = 0x4;
	else LPC_GPIO2 -> FIOCLR = 0x4;
}

#endif /* MAIN_H_ */
