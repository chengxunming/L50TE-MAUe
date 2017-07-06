#include "bsp_time.h"
#include "global.h"

//定时器初始化
void Time_Init(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

    //-----------------------------------------------------------------TIM2	  1ms  主定时器
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE); //时钟使能
	
	TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=(1000-1);		 					/* 自动重装载寄存器周期的值(计数值) */
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */					//定时公式：(TIM_Period + 1) * (TIM_Prescaler + 1) / TIMx Clock
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    	/* 时钟预分频数 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
    TIM_Cmd(TIM2, ENABLE);												/* 开启时钟 */
	
//	//-----------------------------------------------------------------TIM3		100ms 
//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //时钟使能
//	
//	TIM_DeInit(TIM3);
//    TIM_TimeBaseStructure.TIM_Period=(200-1);		 					/* 自动重装载寄存器周期的值(计数值) */
//	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
//    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    /* 时钟预分频数 72M/36000 */
//    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* 采样分频 */
//    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* 向上计数模式 */
//    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
//    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
//    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	 
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 4;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//    TIM_Cmd(TIM3, ENABLE);												/* 开启时钟 */
}

//定时器2中断回调函数 1ms
static void TIM2_ISR_Callback(void)
{	
	//CAN通信测试 200ms超时
	if(ev_CanRespone==CAN_EV_ResReady)
	{
		Time_CanRespone_Count++;
		if(Time_CanRespone_Count>=200)
		{
			Time_CanRespone_Count=0;
			ev_CanRespone=CAN_EV_ResTimeOut;
		}
	}
}
//定时器3中断回调函数 100ms
static void TIM3_ISR_Callback(void)
{

	
}
//定时器2中断服务程序
void TIM2_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update  );  //清除TIMx更新中断标志 
		TIM2_ISR_Callback();
	}
}
//定时器3中断服务程序
void TIM3_IRQHandler(void)   //TIM3中断
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)  //检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update  );  //清除TIMx更新中断标志 
		TIM3_ISR_Callback();
	}
}


