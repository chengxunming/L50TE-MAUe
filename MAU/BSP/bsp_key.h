#ifndef __BSP_KEY_H
#define __BSP_KEY_H 			   

#include "sys.h"
#include "global.h"

typedef enum
{
	EV_KEY_NONE = 0,		/* 0 ��ʾ�ް����¼� */
	EV_KEY_DOWN,			/* ���� */
	EV_KEY_UP,				/* ���� */
	EV_KEY_LONG,			/* ���� */
}KeyEvent_DefType;
//��������ṹ��
typedef struct
{
	uint8_t 	FilterTime;		//�˲���ʱ��
	uint16_t 	PressTime;		//����ʱ��
	uint8_t 	FilterCount;	//�˲�������
	uint16_t 	PressCount;		//��������
	bool 		IsPress;		//����״̬(�Ƿ񱻰���)
	KeyEvent_DefType event;     //�����¼�
	
	uint8_t (*IsKeyDownFunc)(void); //�������µ��жϺ���,1��ʾ����
	void (*KeyEventDownFunc)(void);//���������¼�����
	void (*KeyEventDownLongFunc)(void);//���������¼�����
}KeyHandle_DefType;



#define KEY_RST_PIN		GPIO_Pin_3
#define KEY_MFU_PIN		GPIO_Pin_2
#define	KEY_GPIO		GPIOC
#define KEY_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE )//PORTCʱ��ʹ�� 

#define KEY_MFU_STATE		GPIO_ReadInputDataBit(KEY_GPIO,KEY_MFU_PIN)
#define KEY_RST_STATE		GPIO_ReadInputDataBit(KEY_GPIO,KEY_RST_PIN)

void KEY_Init(void);
void KEY_Scan(void);

void KEY_MFU_Down(void);
void KEY_MFU_DownLong(void);
void KEY_RST_Down(void);
void KEY_RST_DownLong(void);

#endif

