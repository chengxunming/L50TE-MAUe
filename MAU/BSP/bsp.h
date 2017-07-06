#ifndef __BSP_H
#define __BSP_H 			   

#include "sys.h"
#include "global.h"

void MAU_CanTest(void);
void MAU_SwitchTest(void);
void MAU_BuzzerTest(void);
void MAU_TestEnd(void);
void MAU_FunCan_Respone_Deal(Frame_DefType * data);
uint8_t DataCheckSum(uint8_t *data,uint8_t len);

#endif

