#include "stm32f10x.h"
#include "global.h"
#include "bsp.h"
#include "bsp_can.h"
#include "bsp_time.h"
#include "bsp_io.h"
#include "bsp_led.h"
#include "delay.h"
/* Extern variables-----------------------------------------------------------*/
uint8_t Time_CanRespone_Count=0;
bool fault_canTest=false;
CAN_EV_TYPE ev_CanRespone=CAN_EV_None;

int main(void)
{	
	SystemInit();			 //系统初始化	 	
	NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	delay_init(72);	    	 //延时函数初始化
	
	Switch_GPIO_Init();
	BUZZER_Init();
	LED_Init();
	CAN_Configuration();
	Time_Init();
	
	delay_ms(1000);			//上电延迟1s，待单片机稳定
	
	MAU_CanTest();
	MAU_SwitchTest();
	MAU_BuzzerTest();
	LED_ALL_ON();
	MAU_TestEnd();
	while(1)
	{

	}
}

