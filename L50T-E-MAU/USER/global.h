#ifndef __global_H
#define __global_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "math.h"
#include "function.h"
#include "delay.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define HardV		13      //硬件版本号
#define SoftV		4       //软件版本号

#define Channel_Num  4
#define Sample_Num  1

#define PDVS    369    //版本号对象
#define MINCURRENT	5							//0.04A
#define MAXCURRENT	200						//2A

#define Heartbeat_ID 0x00




//外灯序号
#define CH1R				0x01
#define CH1Y				0x02
#define CH1G				0x03

#define CH2R				0x04
#define CH2Y				0x05
#define CH2G				0x06

#define CH3R				0x07
#define CH3Y				0x08
#define CH3G				0x09

#define CH4R				0x0a
#define CH4Y				0x0b
#define CH4G				0x0c




/* Private macro -------------------------------------------------------------*/
//LED控制

#define RUNLEDON		GPIO_ResetBits(GPIOC,GPIO_Pin_2)
#define RUNLEDOFF		GPIO_SetBits(GPIOC,GPIO_Pin_2)



//通道控制
//CH1
#define CH1RON		GPIO_ResetBits(GPIOA,GPIO_Pin_4)
#define CH1ROFF		GPIO_SetBits(GPIOA,GPIO_Pin_4)
#define CH1YON		GPIO_ResetBits(GPIOA,GPIO_Pin_6)
#define CH1YOFF		GPIO_SetBits(GPIOA,GPIO_Pin_6)
#define CH1GON		GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define CH1GOFF		GPIO_SetBits(GPIOB,GPIO_Pin_0)
//CH2
#define CH2RON		GPIO_ResetBits(GPIOB,GPIO_Pin_10)
#define CH2ROFF		GPIO_SetBits(GPIOB,GPIO_Pin_10)
#define CH2YON		GPIO_ResetBits(GPIOB,GPIO_Pin_12)
#define CH2YOFF		GPIO_SetBits(GPIOB,GPIO_Pin_12)
#define CH2GON		GPIO_ResetBits(GPIOB,GPIO_Pin_13)
#define CH2GOFF		GPIO_SetBits(GPIOB,GPIO_Pin_13)
//CH3
#define CH3RON		GPIO_ResetBits(GPIOB,GPIO_Pin_14)
#define CH3ROFF		GPIO_SetBits(GPIOB,GPIO_Pin_14)
#define CH3YON		GPIO_ResetBits(GPIOC,GPIO_Pin_6)
#define CH3YOFF		GPIO_SetBits(GPIOC,GPIO_Pin_6)
#define CH3GON		GPIO_ResetBits(GPIOC,GPIO_Pin_7)
#define CH3GOFF		GPIO_SetBits(GPIOC,GPIO_Pin_7)
//CH4
#define CH4RON		GPIO_ResetBits(GPIOC,GPIO_Pin_9)
#define CH4ROFF		GPIO_SetBits(GPIOC,GPIO_Pin_9)
#define CH4YON		GPIO_ResetBits(GPIOA,GPIO_Pin_8)
#define CH4YOFF		GPIO_SetBits(GPIOA,GPIO_Pin_8)
#define CH4GON		GPIO_ResetBits(GPIOB,GPIO_Pin_9)
#define CH4GOFF		GPIO_SetBits(GPIOB,GPIO_Pin_9)

//通道状态读取
#define CH1RCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)
#define CH1YCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_5)
#define CH1GCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_7)
#define CH2RCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define CH2YCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define CH2GCHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_11)
#define CH3RCHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_10)
#define CH3YCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10)
#define CH3GCHECK	GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9)
#define CH4RCHECK	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_12)
#define CH4YCHECK	GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)
#define CH4GCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_8)

#define PSSCHECK	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)




//外部变量定义
extern volatile uint8_t PDU_CAN_ID;
extern volatile int ADC_ConvertedValue[Channel_Num];


//结构体定义
typedef struct  //通道命令定义
{
	uint8_t Signal;
	uint8_t F_mode;
	uint8_t F_duty;
	uint8_t RStatusA;
	uint8_t RStatusB;
	uint8_t YStatusA;
	uint8_t YStatusB;
	uint8_t GStatusA;
	uint8_t GStatusB;
	uint8_t R_NovoltageCnt;
	uint8_t Y_NovoltageCnt;
	uint8_t G_NovoltageCnt;
	uint8_t FlashMode;
	
} CHANNEL1;

typedef struct  //通道命令定义
{
	uint8_t Signal;
	uint8_t F_mode;
	uint8_t F_duty;
	uint8_t RStatusA;
	uint8_t RStatusB;
	uint8_t YStatusA;
	uint8_t YStatusB;
	uint8_t GStatusA;
	uint8_t GStatusB;
	uint8_t R_NovoltageCnt;
	uint8_t Y_NovoltageCnt;
	uint8_t G_NovoltageCnt;
	uint8_t R_FuseErrCnt;
	uint8_t Y_FuseErrCnt;
	uint8_t G_FuseErrCnt;
	uint8_t FlashMode;
	
} CHANNEL2;

typedef struct  //通道命令定义
{
	uint8_t Signal;
	uint8_t F_mode;
	uint8_t F_duty;
	uint8_t RStatusA;
	uint8_t RStatusB;
	uint8_t YStatusA;
	uint8_t YStatusB;
	uint8_t GStatusA;
	uint8_t GStatusB;
	uint8_t R_NovoltageCnt;
	uint8_t Y_NovoltageCnt;
	uint8_t G_NovoltageCnt;
	uint8_t R_FuseErrCnt;
	uint8_t Y_FuseErrCnt;
	uint8_t G_FuseErrCnt;
	uint8_t FlashMode;
	
} CHANNEL3;

typedef struct  //通道命令定义
{
	uint8_t Signal;
	uint8_t F_mode;
	uint8_t F_duty;
	uint8_t RStatusA;
	uint8_t RStatusB;
	uint8_t YStatusA;
	uint8_t YStatusB;
	uint8_t GStatusA;
	uint8_t GStatusB;
	uint8_t R_NovoltageCnt;
	uint8_t Y_NovoltageCnt;
	uint8_t G_NovoltageCnt;
	uint8_t R_FuseErrCnt;
	uint8_t Y_FuseErrCnt;
	uint8_t G_FuseErrCnt;
	uint8_t FlashMode;
	
} CHANNEL4;


typedef struct  //通道命令定义
{
	uint8_t one[10];
	uint8_t two[10];
	uint8_t tre[10];
	uint8_t fou[10];
	uint8_t fiv[10];
	uint8_t six[10];
	uint8_t sev[10];
	uint8_t eig[10];
	uint8_t nine[10];
	uint8_t ten[10];
	
} TEST;


extern volatile uint8_t SendHeartbeatFlag,CalAverageFlag;
extern uint8_t Send_CAN_HeartbeatFrame();
extern void Cal_Curr_average();
extern void RUNLED_FLASH();
extern volatile uint8_t Allowcheckflag;


#endif
