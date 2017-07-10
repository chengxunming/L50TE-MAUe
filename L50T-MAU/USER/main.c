/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "global.h"
#include "SysTick.h"

/* Private variables ---------------------------------------------------------*/

//uint32_t TestCnt = 10000;

extern LED LEDStatus;

/*********************************************************************************************************
** 函数名称：myDelay
** 函数描述：软件约us延时，不精确
** 输入参数：ulTime
** 返回值  ：无
*********************************************************************************************************/
void myDelay (uint32_t  ulTime)
{
  uint32_t i;   

  while (ulTime--)
	{
    for (i = 0; i < 19192; i++);//120M，约1us不精确
  }
}



/**********************************************************************
* 名    称：IWDG_INIT()
* 功    能：延时
* 入口参数：cnt
* 出口参数：
-----------------------------------------------------------------------
* 说明：
***********************************************************************/
void IWDG_INIT(void)
{
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	IWDG_SetPrescaler(IWDG_Prescaler_64);
	IWDG_SetReload(0x4e2);
	IWDG_ReloadCounter();
	IWDG_Enable();
}


/**********************************************************************
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 ********************************************************************/
static void CAN_Filter_Config(void)
{
	CAN_FilterInitTypeDef  CAN_FilterInitStructure;

	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识列表模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(Heartbeat_ID<<21)&0xffff0000)>>16;				//要过滤的ID高位 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //要过滤的ID低位 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//过滤器高16位除心跳帧之外必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//过滤器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	
	//CAN过滤器初始化
	CAN_FilterInitStructure.CAN_FilterNumber=1;						//过滤器组0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识列表模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	// 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。

	CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(MAU_CAN_ID<<21)&0xffff0000)>>16;				//要过滤的ID高位
	CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //要过滤的ID低位
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//过滤器高16位除心跳帧之外必须匹配
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//过滤器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	//CAN通信中断使能
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}



int main(void)
{


	SystemInit();         //系统时钟初始化

	RCC_Configuration();  //外部时钟配置

	GPIOInit();           //GPIO口初始化


	NVIC_Configuration(); //中断配置

	CAN_Configuration();  //CAN1接口初始化

	CAN_Filter_Config();  //CAN1过滤器配置
	
	myDelay(100);
	
	
	TIM_Configuration();  //定时器初始化

	IWDG_INIT();          //独立看门狗设置

	while (1)
	{

		if (SendHeartbeatFlag)//发送心跳标志
		{
			Send_CAN_HeartbeatFrame();//CAN心跳帧发送
			SendHeartbeatFlag=0;
			IWDG_ReloadCounter();
		}
		if (passwordchangeFlag)//密码修改标志
		{
			flashstor(password);//密码存入Flash
			passwordchangeFlag=0;
		}
		RUNLED();
	}
}

/******************* (C) COPYRIGHT 2015 Lopu *****END OF FILE****/
