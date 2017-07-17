#ifndef __BSP_FIFO_H
#define __BSP_FIFO_H 	

#include "sys.h"

uint8_t FIFO_BUF[100];

typedef struct CanRing
{
	uint8_t *data;
	uint8_t len;
	struct CanRing * next;
}CanRingType;	





#endif
