#ifndef __candata_H
#define __candata_H

#include "global.h"

/////
//////

//uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len);
//uint8_t Send_CAN_DataFrame_MAU(uint8_t *ptr);
uint8_t Send_CAN_HeartbeatFrame(void);
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num);
void USB_LP_CAN1_RX0_IRQHandler(void);

#endif
