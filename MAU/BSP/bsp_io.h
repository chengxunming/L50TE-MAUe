#ifndef __BSP_IO_H
#define __BSP_IO_H 			   

#include "sys.h"


#define  BEEPON           GPIO_SetBits(GPIOC,GPIO_Pin_0)//蜂鸣器打开
#define  BEEPOFF          GPIO_ResetBits(GPIOC,GPIO_Pin_0)//蜂鸣器关闭

//拨动开关状态读取（中，上）
#define SIGNAL1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//外灯开关
#define SIGNAL2	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//手控开关，0为关闭
#define SIGNAL3	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//闪灯开关，1为打开
#define SIGNAL4	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//遥控开关，默认为高

void BUZZER_Init(void);
void Switch_GPIO_Init(void);

#endif

