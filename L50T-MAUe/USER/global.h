#ifndef __global_H
#define __global_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_can.h"
#include "string.h"
#include "gpioint.h"
#include "RCCANDNVIC.h"
#include "math.h"
#include "function.h"
#include "SysTick.h"
#include "candata.h"
#include "TIMIRQHandler.h"


/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/

#define  ON       1
#define  OFF       0

#define Heartbeat_ID 0x00

#define  Versions    0x0011  //(软件版本V1.0，硬件版本V1.1)

#define  MAU_CAN_ID       0x03
//#define  BEEP           PCout(0)
#define  BEEPON           GPIO_SetBits(GPIOC,GPIO_Pin_0)//蜂鸣器打开
#define  BEEPOFF          GPIO_ResetBits(GPIOC,GPIO_Pin_0)//蜂鸣器关闭
#define  FLASHPASSWORD    0x0801F800						//密码的存放位置127K处即126页


/* Private macro -------------------------------------------------------------*/

//LED指示灯控制
#define RUNLEDON		   GPIO_ResetBits(GPIOD,GPIO_Pin_2)//运行指示灯打开
#define RUNLEDOFF		   GPIO_SetBits(GPIOD,GPIO_Pin_2)

#define ERRLEDON		   GPIO_ResetBits(GPIOC,GPIO_Pin_12)//故障指示灯打开
#define ERRLEDOFF		   GPIO_SetBits(GPIOC,GPIO_Pin_12)

#define KEYLOCKON		   GPIO_ResetBits(GPIOC,GPIO_Pin_11)//健锁指示灯打开
#define KEYLOCKOFF		   GPIO_SetBits(GPIOC,GPIO_Pin_11)

#define CENTREON		   GPIO_ResetBits(GPIOC,GPIO_Pin_10)//中心指示灯打开
#define CENTREOFF		   GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define AUTOON		 GPIO_ResetBits(GPIOB,GPIO_Pin_6)//自动指示灯打开
#define AUTOOFF	 	 GPIO_SetBits(GPIOB,GPIO_Pin_6)

#define TUNONLEDON		 GPIO_ResetBits(GPIOA,GPIO_Pin_8)//开灯指示灯打开
#define TUNONLEDOFF		 GPIO_SetBits(GPIOA,GPIO_Pin_8)

#define MANON		   	 GPIO_ResetBits(GPIOC,GPIO_Pin_14)//手控指示灯打开
#define MANOFF		     GPIO_SetBits(GPIOC,GPIO_Pin_14)

#define LEDSTEPON		  GPIO_ResetBits(GPIOC,GPIO_Pin_15)//步进键
#define LEDSTEPOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_15)  //将引脚置1，关闭指示灯

//通道指示灯控制
//为配合背板设定，调整引脚定义对应输出口
#define LEDALLREDON		  GPIO_ResetBits(GPIOA,GPIO_Pin_1)//全红,Step4
#define LEDALLREDOFF	  GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define LEDYFLASHON		  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//黄闪
#define LEDYFLASHOFF	  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LEDFNON		  	  GPIO_ResetBits(GPIOA,GPIO_Pin_7)//FN
#define LEDFNOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LEDONEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//步进点1,Step1
#define LEDONEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LEDTWOON		  GPIO_ResetBits(GPIOA,GPIO_Pin_3)//步进点2,Step2
#define LEDTWOOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define LEDTHREEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_0)//步进点3,Step3
#define LEDTHREEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define LEDFOURON		  GPIO_ResetBits(GPIOC,GPIO_Pin_2)//步进点4,Step4
#define LEDFOUROFF		  GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define LEDFIVEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_5)//步进点5,Step5
#define LEDFIVEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LEDSIXON		  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//步进点6,Step6
#define LEDSIXOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define LEDSEVENON		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)//步进点7,Step7
#define LEDSEVENOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define LEDEIGHTON		  GPIO_ResetBits(GPIOC,GPIO_Pin_1)//步进点8,Step8
#define LEDEIGHTOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_1)


//拨动开关状态读取（中，上）
#define SIGNAL1	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_4)//外灯开关
#define SIGNAL2	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)//手控开关，0为关闭
#define SIGNAL3	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)//步进开关，1为打开


#define KEY_NUMFN		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)
#define KEY_NUMYF		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_10)
#define KEY_NUMAR		GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_11)
#define KEY_NUM1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)
#define KEY_NUM2	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)
#define KEY_NUM3	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)
#define KEY_NUM4	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15)
#define KEY_NUM5	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6)
#define KEY_NUM6	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7)
#define KEY_NUM7	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8)
#define KEY_NUM8	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9)



#define Keyrelease 16


//结构体定义
typedef struct  
{
	uint8_t Status0;//key led状态
	uint8_t Status1;//key led状态
	
} KEYLEDSTATUS;



typedef struct  
{
	uint8_t Fault;
	_Bool Lock;
	_Bool Center;
	_Bool Auto;
	
	_Bool Open;
	_Bool Manual;
	_Bool Step;
	
	_Bool One;
	_Bool Two;
	_Bool Three;
	_Bool Four;
	_Bool Five;
	_Bool Six;
	_Bool Seven;
	_Bool Eight;
//	_Bool Nine;
	_Bool Allred;
	_Bool Y_Flash;
	_Bool Fn;
//	_Bool Ledtest;	
} LED;
/*
typedef enum
{ 
	Bit_RESET = 0,
  Bit_SET
	
}BitAction;

*/

//外部变量定义
extern volatile uint8_t TestFlag,SendHeartbeatFlag,GetHeartbaet,MAUFunFlag,ringflag,ErrFlag,FATAFlag,WarningErrFlag,SeriousErrFlag;
extern uint8_t passwordFlag,off_passwordFlag,password,passwordchangeFlag,Keylednum,Keyled_state,Keyfree_state;//SignalSetFlag,StatechangFlag,KEYx
extern uint16_t KEYx;


extern uint8_t SwitchMode[4],Address[1],TxBuff[10],Receive_Buf[10];
extern KEYLEDSTATUS KEYLED[16];
extern volatile uint8_t CPUOFFLINEFLAG,GetHeartCnt;
extern uint8_t RUNledstatus;

//extern CHANNEL CH[8];


//外部函数定义

extern void CAN_Configuration(void);
extern void TIM_Configuration(void);
extern uint8_t Send_CAN_HeartbeatFrame(void);
extern uint8_t Send_CAN_DataFrame_MAU(uint8_t *ptr);
extern uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len);
extern void RUNLED(void);
extern void GetLedStatus(void);

#endif
