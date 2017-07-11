#ifndef __BSP_KEY_H
#define __BSP_KEY_H 			   

#include "sys.h"
#include "global.h"

typedef enum
{
	EV_KEY_NONE = 0,		/* 0 表示无按键事件 */
	EV_KEY_DOWN,			/* 按下 */
	EV_KEY_UP,				/* 弹起 */
	EV_KEY_LONG,			/* 长按 */
}KeyEvent_DefType;
//按键对象结构体
typedef struct
{
	uint8_t 	FilterTime;		//滤波器时间
	uint16_t 	PressTime;		//长按时间
	uint8_t 	FilterCount;	//滤波器计数
	uint16_t 	PressCount;		//长按计数
	bool 		IsPress;		//按键状态(是否被按下)
	KeyEvent_DefType event;     //按键事件
	
	uint8_t (*IsKeyDownFunc)(void); //按键按下的判断函数,1表示按下
	void (*KeyEventDownFunc)(void);//按键按下事件函数
	void (*KeyEventDownLongFunc)(void);//按键长按事件函数
}KeyHandle_DefType;



#define KEY_RST_PIN		GPIO_Pin_3
#define KEY_MFU_PIN		GPIO_Pin_2
#define	KEY_GPIO		GPIOC
#define KEY_GPIO_CLK_ENABLE()      RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE )//PORTC时钟使能 

#define KEY_MFU_STATE		GPIO_ReadInputDataBit(KEY_GPIO,KEY_MFU_PIN)
#define KEY_RST_STATE		GPIO_ReadInputDataBit(KEY_GPIO,KEY_RST_PIN)

void KEY_Init(void);
void KEY_Scan(void);

void KEY_MFU_Down(void);
void KEY_MFU_DownLong(void);
void KEY_RST_Down(void);
void KEY_RST_DownLong(void);

#endif

