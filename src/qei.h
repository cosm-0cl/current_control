#include "LPC17xx.h"

#ifndef QEI_H_
#define QEI_H_

#define QEI_PWRON  0x1 << 18
#define PCLK_QEI   0x3
#define QEI_MCI0   0x1 << 8
#define QEI_MCI1   0x1 << 14
#define QEI_MCI2   0x1 << 16
#define QEI_RESET  0xF
#define QEI_PCLK      120000000/8

//マクロ関数
#define QEIPOSREAD() LPC_QEI -> QEIPOS & 0xFFFFFFFF
#define QEIVELREAD() LPC_QEI->QEISTAT == 0 ? (LPC_QEI -> QEICAP & 0xFFFFFFFF) * 30 : -(LPC_QEI -> QEICAP & 0xFFFFFFFF) * 30

void QEI_init(void);
int QEI_Pos_Read();
uint32_t QEI_Vel_Read();



#endif /* QEI_H_ */
