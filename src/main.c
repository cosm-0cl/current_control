//current control 02/12/2016
/*ADC:p0.23

 *DAC:p0.26
 *uart:TX:p0.15 RX:p0.16
 *PWM:p2.3 GPIO:p2.4
 *LED:p2.0,p2.1,p2.2
 */

#ifdef __USE_CMSIS
#include "LPC17xx.h"
#endif
#include <cr_section_macros.h>
#include <NXP/crp.h>
__CRP const unsigned int CRP_WORD = CRP_NO_CRP ;

#include "lpc1769_init.h"
#include "adc.h"
#include "pwm.h"
#include "qei.h"
#include "common.h"
#include "uart.h"
#include "dac.h"
#include "main.h"
#include "rit.h"

volatile int data[2];
volatile int cnt;
volatile int ei;
volatile int Kp,Ki,offset;
volatile int current_offset;
volatile int pos_control_counter;

//インピーダンス制御
volatile int Kp_im,Kd_im;
volatile int ei_im;
volatile int target_pos;


/*
 * kp=250,ki=30,kpim=40,kdim=50,target=20000
 */
void Param_clear()
{
	cnt = ei = ei_im = pos_control_counter = current_offset = 0;
}

void RIT_IRQHandler()
{
	int pos,vel,i;
	LPC_RIT -> RICTRL |= 0x1;
	ADC0TRIGGER();
	if(pos_control_counter % 60 == 0)
	{
		//インピーダンス制御
		pos = QEIPOSREAD();
		vel = QEIVELREAD();

		i = -(pos-target_pos)*Kp_im/1000 - vel*Kd_im/1000;

		if(i > 500)i = 500;
		else if(i < -500)i = -500;

		current_offset = i;
		/*
		write(i,10);
		putc(',');
		write(vel,10);
		putc('\n');
		*/
	}
	pos_control_counter++;
}

void ADC_IRQHandler()
{
	volatile int duty,data_t;
	if((cnt&0x1)==0)
	{
		data[0] = ADC0READ();
	}
	else
	{
		data[1] = ADC0READ();
		data_t = (data[0]+data[1])/2 - offset - 2048 + current_offset;

		duty = -Kp*data_t/1000 - Ki*ei/1000;
		MOTOR_DRIVE(duty);
		ei += data_t;
	}

	cnt++;
	DACWRITE((data_t+2048)>>2);
	return;
}

int main(void) {

	volatile char str[20];

	Kp = Ki = ei = 0;
	offset = 3;
	current_offset = 0;

	LPC_init();
	GPIO_init();
	UART1_init(115200);
	QEI_init();

	ADC_init(0x1);
	LPC_ADC -> ADCR |= 0x1;

	PWM_init(0x8);

	RIT_init();

	MOTOR_DRIVE(0);
	while(1) {
		puts(">>");
		gets(str);
		if(strcmp(str,"kp")==0)
		{
			puts("kp:");
			gets(str);
			Kp = atoi(str);
			write(Kp,10);
			putc('\n');
		}
		else if(strcmp(str,"ki")==0)
		{
			puts("ki:");
			gets(str);
			Ki = atoi(str);
			write(Ki,10);
			putc('\n');
		}
		else if(strcmp(str,"offset")==0)
		{
			puts("offset:");
			gets(str);
			offset = atoi(str);
			write(offset,10);
			putc('\n');
		}
		else if(strcmp(str,"current")==0)
		{
			puts("current:");
			gets(str);
			current_offset = atoi(str);
			write(current_offset,10);
			putc('\n');
		}
		else if(strcmp(str,"q")==0)
		{
			Param_clear();
			RIT_Enable();
		}
		else if(strcmp(str,"w")==0)
		{
			DACWRITE(0);
			RIT_Disable();
			usecWait(10);
			MOTOR_DRIVE(0);
		}
		else if(strcmp(str,"duty")==0)
		{
			puts("duty:");
			gets(str);
			MOTOR_DRIVE(atoi(str));
		}
		else if(strcmp(str,"kpim")==0)
		{
			puts("kpim:");
			gets(str);
			Kp_im = atoi(str);
			write(Kp_im,10);
			putc('\n');
		}
		else if(strcmp(str,"kdim")==0)
		{
			puts("kdim:");
			gets(str);
			Kd_im = atoi(str);
			write(Kd_im,10);
			putc('\n');
		}
		else if(strcmp(str,"target")==0)
		{
			puts("target:");
			gets(str);
			target_pos = atoi(str);
			write(target_pos,10);
			putc('\n');
		}
	}
	return 0 ;
}
