#ifndef __BSP_CAN_H
#define __BSP_CAN_H 			   

#include "sys.h"

#define CAN1_CLK_ENABLE()			RCC_APB1PeriphClockCmd(	RCC_APB1Periph_CAN1,  ENABLE )//CAN1时钟使能 

#define CAN1_TX_PIN		GPIO_Pin_12
#define CAN1_RX_PIN		GPIO_Pin_11
#define	CAN1_GPIO		GPIOA
#define CAN1_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE )//PORTA时钟使能 

void CAN_Configuration(void);
void CAN_Filter_Config(void);
uint8_t Send_CAN_DataFrame(uint8_t *ptr,uint8_t len);

#endif

