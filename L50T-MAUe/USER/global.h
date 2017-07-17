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

//#define TUNONLEDON		 GPIO_ResetBits(GPIOC,GPIO_Pin_11)//开灯指示灯打开
//#define TUNONLEDOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_11)

#define KEYLOCKON		   GPIO_ResetBits(GPIOC,GPIO_Pin_11)//健锁指示灯打开
#define KEYLOCKOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_11)

//#define TESTON		   GPIO_ResetBits(GPIOB,GPIO_Pin_5)//测试指示灯打开
//#define TESTOFF		   GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define CENTREON		       GPIO_ResetBits(GPIOC,GPIO_Pin_10)//中心指示灯打开
#define CENTREOFF		     GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define MANON		   GPIO_ResetBits(GPIOC,GPIO_Pin_14)//手控指示灯打开
#define MANOFF		   GPIO_SetBits(GPIOC,GPIO_Pin_14)

//#define TIMEBASEON		 GPIO_ResetBits(GPIOB,GPIO_Pin_8)//时基指示灯打开
//#define TIMEBASEOFF		 GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define AUTOFLSSON		 GPIO_ResetBits(GPIOB,GPIO_Pin_6)//自动闪指示灯打开
#define AUTOFLSSOFF	 GPIO_SetBits(GPIOB,GPIO_Pin_6)

//#define MANFLSSON		 GPIO_ResetBits(GPIOC,GPIO_Pin_13)//手控闪指示灯打开
//#define MANFLSSOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_13)

//#define ERRFLSSON		 GPIO_ResetBits(GPIOC,GPIO_Pin_14)//故障闪指示灯打开
//#define ERRFLSSOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_14)

//#define ANTCPFLSSON	 GPIO_ResetBits(GPIOC,GPIO_Pin_15)//抢先闪指示灯打开
//#define ANTCPFLSSOFF	 GPIO_SetBits(GPIOC,GPIO_Pin_15)

#define LEDSTEPON		  GPIO_ResetBits(GPIOC,GPIO_Pin_15)//步进键
#define LEDSTEPOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_15)  //将引脚置1，关闭指示灯

//通道指示灯控制
//为配合背板设定，调整引脚定义对应输出口
#define LEDALLREDON		  GPIO_ResetBits(GPIOA,GPIO_Pin_1)//全红,Step4
#define LEDALLREDOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define LEDYFLASHON		  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//黄闪
#define LEDYFLASHOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LEDFNON		  	  GPIO_ResetBits(GPIOA,GPIO_Pin_7)//FN
#define LEDFNOFF		      GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LEDONEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//步进点1,Step1
#define LEDONEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LEDTWOON		  GPIO_ResetBits(GPIOA,GPIO_Pin_3)//步进点2,Step2
#define LEDTWOOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define LEDTHREEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_0)//步进点3,Step3
#define LEDTHREEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define LEDFOURON		  GPIO_ResetBits(GPIOC,GPIO_Pin_2)//步进点4,Step5
#define LEDFOUROFF		  GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define LEDFIVEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_5)//步进点5,Step6
#define LEDFIVEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LEDSIXON		  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//步进点6,Step7
#define LEDSIXOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define LEDSEVENON		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)//步进点7,Step9
#define LEDSEVENOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define LEDEIGHTON		  GPIO_ResetBits(GPIOC,GPIO_Pin_1)//步进点8,Step10
#define LEDEIGHTOFF		GPIO_SetBits(GPIOC,GPIO_Pin_1)





//#define LEDKEYAON		  GPIO_ResetBits(GPIOB,GPIO_Pin_1)// A 键,Step8
//#define LEDKEYAOFF		  GPIO_SetBits(GPIOB,GPIO_Pin_1)

//#define LEDTESTON		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)//灯测试,Step11
//#define LEDTESTOFF		GPIO_SetBits(GPIOC,GPIO_Pin_3)

//#define LEDKEYBON		  GPIO_ResetBits(GPIOB,GPIO_Pin_0)//B 键,Step12
//#define LEDKEYBOFF		GPIO_SetBits(GPIOB,GPIO_Pin_0)

//#define LEDKEYCON		  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//C 键,Step13
//#define LEDKEYCOFF		GPIO_SetBits(GPIOA,GPIO_Pin_6)

//#define LEDKEYDON		  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//D 键,Step14
//#define LEDKEYDOFF		GPIO_SetBits(GPIOA,GPIO_Pin_2)

//#define LEDKEYEON		  GPIO_ResetBits(GPIOC,GPIO_Pin_2)//E 键,Step15
//#define LEDKEYEOFF		GPIO_SetBits(GPIOC,GPIO_Pin_2)



//拨动开关状态读取（中，上）
#define SIGNAL1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//外灯开关
#define SIGNAL2	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//手控开关，0为关闭
#define SIGNAL3	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//闪灯开关，1为打开
#define SIGNAL4	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//遥控开关，默认为高，



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
	_Bool Open;
	_Bool Lock;
	_Bool Test;
	_Bool Center;
	_Bool Manual;
	_Bool Timebase;
	_Bool A_Flash;
	_Bool M_Flash;
	_Bool F_Flash;
	_Bool H_Flash;
	_Bool Step;
	_Bool One;
	_Bool Two;
	_Bool Three;
	_Bool Four;
	_Bool Five;
	_Bool Six;
	_Bool Seven;
	_Bool Eight;
	_Bool Nine;
	_Bool Allred;
	_Bool Ledtest;
	_Bool Key_A;
	_Bool Key_B;
	_Bool Key_C;
	_Bool Key_D;
	_Bool Key_E;
	
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
extern uint8_t passwordFlag,off_passwordFlag,password,passwordchangeFlag,KEYx,Keylednum,Keyled_state,Keyfree_state;//SignalSetFlag,StatechangFlag,

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
