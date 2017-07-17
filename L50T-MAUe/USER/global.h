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

#define  Versions    0x0011  //(����汾V1.0��Ӳ���汾V1.1)

#define  MAU_CAN_ID       0x03
//#define  BEEP           PCout(0)
#define  BEEPON           GPIO_SetBits(GPIOC,GPIO_Pin_0)//��������
#define  BEEPOFF          GPIO_ResetBits(GPIOC,GPIO_Pin_0)//�������ر�
#define  FLASHPASSWORD    0x0801F800						//����Ĵ��λ��127K����126ҳ


/* Private macro -------------------------------------------------------------*/

//LEDָʾ�ƿ���

#define RUNLEDON		   GPIO_ResetBits(GPIOD,GPIO_Pin_2)//����ָʾ�ƴ�
#define RUNLEDOFF		   GPIO_SetBits(GPIOD,GPIO_Pin_2)

#define ERRLEDON		   GPIO_ResetBits(GPIOC,GPIO_Pin_12)//����ָʾ�ƴ�
#define ERRLEDOFF		   GPIO_SetBits(GPIOC,GPIO_Pin_12)

//#define TUNONLEDON		 GPIO_ResetBits(GPIOC,GPIO_Pin_11)//����ָʾ�ƴ�
//#define TUNONLEDOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_11)

#define KEYLOCKON		   GPIO_ResetBits(GPIOC,GPIO_Pin_11)//����ָʾ�ƴ�
#define KEYLOCKOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_11)

//#define TESTON		   GPIO_ResetBits(GPIOB,GPIO_Pin_5)//����ָʾ�ƴ�
//#define TESTOFF		   GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define CENTREON		       GPIO_ResetBits(GPIOC,GPIO_Pin_10)//����ָʾ�ƴ�
#define CENTREOFF		     GPIO_SetBits(GPIOC,GPIO_Pin_10)

#define MANON		   GPIO_ResetBits(GPIOC,GPIO_Pin_14)//�ֿ�ָʾ�ƴ�
#define MANOFF		   GPIO_SetBits(GPIOC,GPIO_Pin_14)

//#define TIMEBASEON		 GPIO_ResetBits(GPIOB,GPIO_Pin_8)//ʱ��ָʾ�ƴ�
//#define TIMEBASEOFF		 GPIO_SetBits(GPIOB,GPIO_Pin_8)

#define AUTOFLSSON		 GPIO_ResetBits(GPIOB,GPIO_Pin_6)//�Զ���ָʾ�ƴ�
#define AUTOFLSSOFF	 GPIO_SetBits(GPIOB,GPIO_Pin_6)

//#define MANFLSSON		 GPIO_ResetBits(GPIOC,GPIO_Pin_13)//�ֿ���ָʾ�ƴ�
//#define MANFLSSOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_13)

//#define ERRFLSSON		 GPIO_ResetBits(GPIOC,GPIO_Pin_14)//������ָʾ�ƴ�
//#define ERRFLSSOFF		 GPIO_SetBits(GPIOC,GPIO_Pin_14)

//#define ANTCPFLSSON	 GPIO_ResetBits(GPIOC,GPIO_Pin_15)//������ָʾ�ƴ�
//#define ANTCPFLSSOFF	 GPIO_SetBits(GPIOC,GPIO_Pin_15)

#define LEDSTEPON		  GPIO_ResetBits(GPIOC,GPIO_Pin_15)//������
#define LEDSTEPOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_15)  //��������1���ر�ָʾ��

//ͨ��ָʾ�ƿ���
//Ϊ��ϱ����趨���������Ŷ����Ӧ�����
#define LEDALLREDON		  GPIO_ResetBits(GPIOA,GPIO_Pin_1)//ȫ��,Step4
#define LEDALLREDOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_1)

#define LEDYFLASHON		  GPIO_ResetBits(GPIOA,GPIO_Pin_4)//����
#define LEDYFLASHOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_4)

#define LEDFNON		  	  GPIO_ResetBits(GPIOA,GPIO_Pin_7)//FN
#define LEDFNOFF		      GPIO_SetBits(GPIOA,GPIO_Pin_7)

#define LEDONEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//������1,Step1
#define LEDONEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_6)

#define LEDTWOON		  GPIO_ResetBits(GPIOA,GPIO_Pin_3)//������2,Step2
#define LEDTWOOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_3)

#define LEDTHREEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_0)//������3,Step3
#define LEDTHREEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_0)

#define LEDFOURON		  GPIO_ResetBits(GPIOC,GPIO_Pin_2)//������4,Step5
#define LEDFOUROFF		  GPIO_SetBits(GPIOC,GPIO_Pin_2)

#define LEDFIVEON		  GPIO_ResetBits(GPIOA,GPIO_Pin_5)//������5,Step6
#define LEDFIVEOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_5)

#define LEDSIXON		  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//������6,Step7
#define LEDSIXOFF		  GPIO_SetBits(GPIOA,GPIO_Pin_2)

#define LEDSEVENON		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)//������7,Step9
#define LEDSEVENOFF		  GPIO_SetBits(GPIOC,GPIO_Pin_3)

#define LEDEIGHTON		  GPIO_ResetBits(GPIOC,GPIO_Pin_1)//������8,Step10
#define LEDEIGHTOFF		GPIO_SetBits(GPIOC,GPIO_Pin_1)





//#define LEDKEYAON		  GPIO_ResetBits(GPIOB,GPIO_Pin_1)// A ��,Step8
//#define LEDKEYAOFF		  GPIO_SetBits(GPIOB,GPIO_Pin_1)

//#define LEDTESTON		  GPIO_ResetBits(GPIOC,GPIO_Pin_3)//�Ʋ���,Step11
//#define LEDTESTOFF		GPIO_SetBits(GPIOC,GPIO_Pin_3)

//#define LEDKEYBON		  GPIO_ResetBits(GPIOB,GPIO_Pin_0)//B ��,Step12
//#define LEDKEYBOFF		GPIO_SetBits(GPIOB,GPIO_Pin_0)

//#define LEDKEYCON		  GPIO_ResetBits(GPIOA,GPIO_Pin_6)//C ��,Step13
//#define LEDKEYCOFF		GPIO_SetBits(GPIOA,GPIO_Pin_6)

//#define LEDKEYDON		  GPIO_ResetBits(GPIOA,GPIO_Pin_2)//D ��,Step14
//#define LEDKEYDOFF		GPIO_SetBits(GPIOA,GPIO_Pin_2)

//#define LEDKEYEON		  GPIO_ResetBits(GPIOC,GPIO_Pin_2)//E ��,Step15
//#define LEDKEYEOFF		GPIO_SetBits(GPIOC,GPIO_Pin_2)



//��������״̬��ȡ���У��ϣ�
#define SIGNAL1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_12)//��ƿ���
#define SIGNAL2	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_13)//�ֿؿ��أ�0Ϊ�ر�
#define SIGNAL3	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_14)//���ƿ��أ�1Ϊ��
#define SIGNAL4	GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_1)//ң�ؿ��أ�Ĭ��Ϊ�ߣ�



#define Keyrelease 16


//�ṹ�嶨��
typedef struct  
{
	uint8_t Status0;//key led״̬
	uint8_t Status1;//key led״̬
	
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

//�ⲿ��������
extern volatile uint8_t TestFlag,SendHeartbeatFlag,GetHeartbaet,MAUFunFlag,ringflag,ErrFlag,FATAFlag,WarningErrFlag,SeriousErrFlag;
extern uint8_t passwordFlag,off_passwordFlag,password,passwordchangeFlag,KEYx,Keylednum,Keyled_state,Keyfree_state;//SignalSetFlag,StatechangFlag,

extern uint8_t SwitchMode[4],Address[1],TxBuff[10],Receive_Buf[10];
extern KEYLEDSTATUS KEYLED[16];
extern volatile uint8_t CPUOFFLINEFLAG,GetHeartCnt;
extern uint8_t RUNledstatus;

//extern CHANNEL CH[8];


//�ⲿ��������

extern void CAN_Configuration(void);
extern void TIM_Configuration(void);
extern uint8_t Send_CAN_HeartbeatFrame(void);
extern uint8_t Send_CAN_DataFrame_MAU(uint8_t *ptr);
extern uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len);
extern void RUNLED(void);
extern void GetLedStatus(void);

#endif
