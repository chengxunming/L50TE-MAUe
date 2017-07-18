/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "global.h"




/* Private variables ---------------------------------------------------------*/



/*时钟配置*/
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

   
		RCC_DeInit(); 
	//使能外部晶振
    RCC_HSEConfig(RCC_HSE_ON);
    //等待外部晶振稳定
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //如果外部晶振启动成功，则进行下一步操作
    if(HSEStartUpStatus==SUCCESS)
    {
        //设置HCLK（AHB时钟）=SYSCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //PCLK1(APB1) = HCLK/2
        RCC_PCLK1Config(RCC_HCLK_Div2);
        //PCLK2(APB2) = HCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //设置ADC时钟频率
        RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
        //FLASH时序控制
        //推荐值：SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        //PLL设置 SYSCLK/1 * 9 = 8*1*9 = 72MHz
        FLASH_SetLatency(FLASH_Latency_2);  //flash操作的延时   //Flash 2 wait state
				FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   //flash读取缓冲，加速 //Enable Prefetch Buffer
			
				RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        //启动PLL
        RCC_PLLCmd(ENABLE);
        //等待PLL稳定
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
				
        //系统时钟SYSCLK来自PLL输出
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        //切换时钟后等待系统时钟稳定
        while(RCC_GetSYSCLKSource()!=0x08);    
    }

		//启动GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
		//启动AFIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//启动ADC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		//启动 CAN1
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		//启动 timer1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		//启动 timer2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		//启动 timer3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		//启动 timer4
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

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
	IWDG_SetPrescaler(IWDG_Prescaler_32);
	IWDG_SetReload(0x4e2);
	IWDG_ReloadCounter();
	IWDG_Enable();
}




/**************************************************************************************************************
函数描述 : GPIO初始化
***************************************************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//上电设置所有GPIO为输入上拉
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
								   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
								   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
								   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7|
								   GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	

	/*设置 通道控制输出*/
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_4|GPIO_Pin_6|GPIO_Pin_8;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	

	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_12|GPIO_Pin_13|GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	

	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);

	ChannelAlloff();
	
	/*设置 通道状态检测输入*/
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10|GPIO_Pin_3|GPIO_Pin_5|GPIO_Pin_7|GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1|GPIO_Pin_8|GPIO_Pin_11|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_11|GPIO_Pin_10|GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD,&GPIO_InitStructure);
	
	//run LED
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//ADDR1~3
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
	//ADDR0
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPD;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	//curr1~4
	
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_4|GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AIN;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	//CAN GPIO CONFIG
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOA,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	
}

/**************************************************************************************************************
函数描述 : CAN初始化
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM = DISABLE; /*禁止时间触发通讯模式*/
	//CAN_InitStructure.CAN_ABOM = ENABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
  CAN_InitStructure.CAN_ABOM = DISABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
	CAN_InitStructure.CAN_AWUM = DISABLE; /*0-由软件通过清0唤醒，1-检测到报文时，自动唤醒*/
	CAN_InitStructure.CAN_NART = DISABLE; /*0-一直重复发送直到成功，1-不论成功以否只发送一次*///自动重传											
	CAN_InitStructure.CAN_RFLM = DISABLE; /*0-溢出时FIFO未锁定，新报文盖掉旧报文，1-锁定，溢出后新报文直接丢失*/
	CAN_InitStructure.CAN_TXFP = DISABLE; /*0-报文发送优先级由标志符决定，1-报文发送优先级由请求先后顺序决定*/
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;/*模式-测试模式-正常模式*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
/*-----------------------------------------------
//BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
//--------------------------------------------------------
//   速率   |  SJW  |  BS1  |  BS2  | prescaler | Sample |
//--------------------------------------------------------
// 1000kbps |  1tq  | 2tq   |  1tq  |     9     |  75.0% |
//--------------------------------------------------------
//  800kbps |  1tq  | 3tq   |  1tq  |     9     |  80.0% |
//--------------------------------------------------------
//  500kbps |  1tq  | 6tq   |  1tq  |     9     |  87.5% |
//--------------------------------------------------------
//  250kbps |  1tq  | 6tq   |  1tq  |     18    |  87.5% |
//--------------------------------------------------------
//  125kbps |  1tq  | 13tq  |  2tq  |     18    |  87.5% |
//--------------------------------------------------------
//  100kbps |  1tq  | 6tq   |  1tq  |     45    |  87.5% |  
//--------------------------------------------------------*/
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//重同步时间宽度
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;//CAN_BS1_5tq;//时间1宽度
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;//时间2宽度
	CAN_InitStructure.CAN_Prescaler = 9;//分频值(时间单元长度)
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//CAN_Filter_Reconfig(BOARD);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*
 * 函数名：CAN_Filter_Config
 * 描述  ：CAN的过滤器 配置
 * 输入  ：无
 * 输出  : 无
 * 调用  ：内部调用
 */
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

	/*CAN过滤器初始化*/
	CAN_FilterInitStructure.CAN_FilterNumber=1;						//过滤器组0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//工作在标识列表模式
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//过滤器位宽为单个32位。
	/* 使能报文标示符过滤器按照标示符的内容进行比对过滤，扩展ID不是如下的就抛弃掉，是的话，会存入FIFO0。 */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(PDU_CAN_ID<<21)&0xffff0000)>>16;				//要过滤的ID高位 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //要过滤的ID低位 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//过滤器高16位除心跳帧之外必须匹配
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//过滤器低16位每位必须匹配
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//过滤器被关联到FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//使能过滤器
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CAN通信中断使能*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}



/**************************************************************************************************************
函数描述 : 中断优先级设置
***************************************************************************************************************/	

/* ============================================================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  | Description
  ============================================================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             |   0 bits for pre-emption priority
                         |                                   |                             |   4 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------
   NVIC_PriorityGroup_1  |                0-1                |            0-7              |   1 bits for pre-emption priority
                         |                                   |                             |   3 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_2  |                0-3                |            0-3              |   2 bits for pre-emption priority
                         |                                   |                             |   2 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_3  |                0-7                |            0-1              |   3 bits for pre-emption priority
                         |                                   |                             |   1 bits for subpriority
  ----------------------------------------------------------------------------------------------------------------------------    
   NVIC_PriorityGroup_4  |                0-15               |            0                |   4 bits for pre-emption priority
                         |                                   |                             |   0 bits for subpriority                       
  ============================================================================================================================*/


void NVIC_Configuration(void)
{
	NVIC_InitTypeDef NVIC_InitStructure; 
	/* Configure the NVIC Preemption Priority Bits */  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	/* Enable the CAN Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;	 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	

	
}


/**************************************************************************************************************
函数描述 : 定时器初始化
***************************************************************************************************************/	
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //-----------------------------------------------------------------TIM2	  50ms    主程序推动定时器
	TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=(100);		 						/* 自动重装载寄存器周期的值(计数值) */
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */					//定时公式：(TIM_Period + 1) * (TIM_Prescaler + 1) / TIMx Clock
    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    	/* 时钟预分频数 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* 向上计数模式 */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);												/* 开启时钟 */
	
	//-----------------------------------------------------------------TIM3		100ms     计算心跳包间隔及控制命令接收间隔
	TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period=(200);		 						/* 自动重装载寄存器周期的值(计数值) */
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    	/* 时钟预分频数 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* 向上计数模式 */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);												/* 开启时钟 */
	
	//-----------------------------------------------------------------TIM4		1ms    ADC启动
	TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period=(1000);		 						/* 自动重装载寄存器周期的值(计数值) */
	/* 累计 TIM_Period个频率后产生一个更新或者中断 */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    	/* 时钟预分频数 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* 采样分频 */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* 向上计数模式 */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);							    /* 清除溢出中断标志 */
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    //TIM_Cmd(TIM4, ENABLE);												/* 开启时钟 */

	
}


/**************************************************************************************************************
函数描述 : ADC初始化
***************************************************************************************************************/	
void ADC_DMA_Config(void)
{
	DMA_InitTypeDef DMA_InitStructure;
    
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel1);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC1->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&ADC_ConvertedValue;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = Channel_Num;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不变
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址递增
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	//DMA_Cmd(DMA1_Channel1,ENABLE);
}

void ADC1_Config(void)
{

	ADC_InitTypeDef ADC_InitStructure; 

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //设置ADC分频因子6 72M/6=12,ADC最大时间不能超过14M
	

	ADC_DeInit(ADC1);  //复位ADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1, &ADC_InitStructure);	//根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器   

  
	ADC_Cmd(ADC1, ENABLE);	//使能指定的ADC1
	
	ADC_ResetCalibration(ADC1);	//使能复位校准  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//等待复位校准结束
	
	ADC_StartCalibration(ADC1);	 //开启AD校准
 
	while(ADC_GetCalibrationStatus(ADC1));	 //等待校准结束
 	
	

}


void Exti_Configuration(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource9);
	EXTI_InitStructure.EXTI_Line = EXTI_Line9;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	

}


int main(void)
{
  /*!< At this stage the microcontroller clock setting is already configured, 
       this is done through SystemInit() function which is called from startup
       file (startup_stm32f10x_xx.s) before to branch to application main.
       To reconfigure the default setting of SystemInit() function, refer to
       system_stm32f10x.c file
     */    


	SystemInit();				//系统初始化
   
	RCC_Configuration();	//系统RCC时钟初始化
	
	GPIOInit();				//IO初始化
	
	
	NVIC_Configuration();	//中断优先级设置
	
	delay_init(72);			//延迟初始化，使用systick做延迟
	
	delay_ms(500);			//上电延迟500ms，待单片机稳定
	
	Cal_PDU_CANID();		//CAN ID 计算
	CAN_Configuration();	//CAN配置，速率125K
	CAN_Filter_Config();    //CAN过滤器设置，只有心跳包，命令数据可以传送进来
	
	TIM_Configuration();	//定时器初始化
	
	//ADC_DMA_Config();			//DMA初始化
	ADC1_Config();			//ADC初始化
	
	//IWDG_INIT();				//看门狗初始化
	
	
	
	
   
	
	while (1)
	{
		if(SendHeartbeatFlag)   //心跳发送
		{
			Send_CAN_HeartbeatFrame();
			SendHeartbeatFlag=0;
			IWDG_ReloadCounter();
		}
		if(CalAverageFlag)		//电流平均值计算
		{
			Cal_Curr_average();
			CalAverageFlag=0;
		}
		RUNLED_FLASH();	//运行灯闪烁
		
		Get_Flash_CurrentValue();
		
		
	}
}

/******************* (C) COPYRIGHT 2015 Lopu *****END OF FILE****/
