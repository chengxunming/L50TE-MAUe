/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"
#include <stdio.h>
#include "global.h"




/* Private variables ---------------------------------------------------------*/



/*ʱ������*/
void RCC_Configuration(void)
{
    ErrorStatus HSEStartUpStatus;

   
		RCC_DeInit(); 
	//ʹ���ⲿ����
    RCC_HSEConfig(RCC_HSE_ON);
    //�ȴ��ⲿ�����ȶ�
    HSEStartUpStatus = RCC_WaitForHSEStartUp();
    //����ⲿ���������ɹ����������һ������
    if(HSEStartUpStatus==SUCCESS)
    {
        //����HCLK��AHBʱ�ӣ�=SYSCLK
        RCC_HCLKConfig(RCC_SYSCLK_Div1);
        //PCLK1(APB1) = HCLK/2
        RCC_PCLK1Config(RCC_HCLK_Div2);
        //PCLK2(APB2) = HCLK
        RCC_PCLK2Config(RCC_HCLK_Div1);
        //����ADCʱ��Ƶ��
        RCC_ADCCLKConfig(RCC_PCLK2_Div8); 
        //FLASHʱ�����
        //�Ƽ�ֵ��SYSCLK = 0~24MHz   Latency=0
        //        SYSCLK = 24~48MHz  Latency=1
        //        SYSCLK = 48~72MHz  Latency=2
        //PLL���� SYSCLK/1 * 9 = 8*1*9 = 72MHz
        FLASH_SetLatency(FLASH_Latency_2);  //flash��������ʱ   //Flash 2 wait state
				FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   //flash��ȡ���壬���� //Enable Prefetch Buffer
			
				RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);
        //����PLL
        RCC_PLLCmd(ENABLE);
        //�ȴ�PLL�ȶ�
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);
				
        //ϵͳʱ��SYSCLK����PLL���
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        //�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�
        while(RCC_GetSYSCLKSource()!=0x08);    
    }

		//����GPIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
		//����AFIO
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
		//����ADC
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
		//���� CAN1
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
		//���� timer1
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
		//���� timer2
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
		//���� timer3
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
		//���� timer4
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

}



/**********************************************************************
* ��    �ƣ�IWDG_INIT()
* ��    �ܣ���ʱ
* ��ڲ�����cnt
* ���ڲ�����
-----------------------------------------------------------------------
* ˵����
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
�������� : GPIO��ʼ��
***************************************************************************************************************/
void GPIOInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	//�ϵ���������GPIOΪ��������
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
	

	/*���� ͨ���������*/
	
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
	
	/*���� ͨ��״̬�������*/
	
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
�������� : CAN��ʼ��
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_InitStructure.CAN_TTCM = DISABLE; /*��ֹʱ�䴥��ͨѶģʽ*/
	//CAN_InitStructure.CAN_ABOM = ENABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
  CAN_InitStructure.CAN_ABOM = DISABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
	CAN_InitStructure.CAN_AWUM = DISABLE; /*0-�����ͨ����0���ѣ�1-��⵽����ʱ���Զ�����*/
	CAN_InitStructure.CAN_NART = DISABLE; /*0-һֱ�ظ�����ֱ���ɹ���1-���۳ɹ��Է�ֻ����һ��*///�Զ��ش�											
	CAN_InitStructure.CAN_RFLM = DISABLE; /*0-���ʱFIFOδ�������±��ĸǵ��ɱ��ģ�1-������������±���ֱ�Ӷ�ʧ*/
	CAN_InitStructure.CAN_TXFP = DISABLE; /*0-���ķ������ȼ��ɱ�־��������1-���ķ������ȼ��������Ⱥ�˳�����*/
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;/*ģʽ-����ģʽ-����ģʽ*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
/*-----------------------------------------------
//BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
//--------------------------------------------------------
//   ����   |  SJW  |  BS1  |  BS2  | prescaler | Sample |
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
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//��ͬ��ʱ����
	CAN_InitStructure.CAN_BS1 = CAN_BS1_6tq;//CAN_BS1_5tq;//ʱ��1���
	CAN_InitStructure.CAN_BS2 = CAN_BS2_1tq;//ʱ��2���
	CAN_InitStructure.CAN_Prescaler = 9;//��Ƶֵ(ʱ�䵥Ԫ����)
	CAN_Init(CAN1, &CAN_InitStructure);
	
	//CAN_Filter_Reconfig(BOARD);
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
static void CAN_Filter_Config(void)
{
   CAN_FilterInitTypeDef  CAN_FilterInitStructure;
   
   
   /*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=0;						//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ�б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(Heartbeat_ID<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//��������16λ������֮֡�����ƥ��
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);

	/*CAN��������ʼ��*/
	CAN_FilterInitStructure.CAN_FilterNumber=1;						//��������0
    CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ�б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	/* ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0�� */

    CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(PDU_CAN_ID<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //Ҫ���˵�ID��λ 
    CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//��������16λ������֮֡�����ƥ��
    CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	/*CANͨ���ж�ʹ��*/
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}



/**************************************************************************************************************
�������� : �ж����ȼ�����
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
�������� : ��ʱ����ʼ��
***************************************************************************************************************/	
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

    //-----------------------------------------------------------------TIM2	  50ms    �������ƶ���ʱ��
	TIM_DeInit(TIM2);
    TIM_TimeBaseStructure.TIM_Period=(100);		 						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */					//��ʱ��ʽ��(TIM_Period + 1) * (TIM_Prescaler + 1) / TIMx Clock
    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* �������жϱ�־ */
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM2, ENABLE);												/* ����ʱ�� */
	
	//-----------------------------------------------------------------TIM3		100ms     �������������������������ռ��
	TIM_DeInit(TIM3);
    TIM_TimeBaseStructure.TIM_Period=(200);		 						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    /* �������жϱ�־ */
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
    TIM_Cmd(TIM3, ENABLE);												/* ����ʱ�� */
	
	//-----------------------------------------------------------------TIM4		1ms    ADC����
	TIM_DeInit(TIM4);
    TIM_TimeBaseStructure.TIM_Period=(1000);		 						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
    TIM_TimeBaseStructure.TIM_Prescaler= (72 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36000 */
    TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
    TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    TIM_ClearFlag(TIM4, TIM_FLAG_Update);							    /* �������жϱ�־ */
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
    //TIM_Cmd(TIM4, ENABLE);												/* ����ʱ�� */

	
}


/**************************************************************************************************************
�������� : ADC��ʼ��
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
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//�����ַ����
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;//�ڴ��ַ����
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

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //����ADC��Ƶ����6 72M/6=12,ADC���ʱ�䲻�ܳ���14M
	

	ADC_DeInit(ADC1);  //��λADC1 

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;	//ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;	//ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;	//ģ��ת�������ڵ���ת��ģʽ
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;	//ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;	//ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel = 1;	//˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);	//����ADC_InitStruct��ָ���Ĳ�����ʼ������ADCx�ļĴ���   

  
	ADC_Cmd(ADC1, ENABLE);	//ʹ��ָ����ADC1
	
	ADC_ResetCalibration(ADC1);	//ʹ�ܸ�λУ׼  
	 
	while(ADC_GetResetCalibrationStatus(ADC1));	//�ȴ���λУ׼����
	
	ADC_StartCalibration(ADC1);	 //����ADУ׼
 
	while(ADC_GetCalibrationStatus(ADC1));	 //�ȴ�У׼����
 	
	

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


	SystemInit();				//ϵͳ��ʼ��
   
	RCC_Configuration();	//ϵͳRCCʱ�ӳ�ʼ��
	
	GPIOInit();				//IO��ʼ��
	
	
	NVIC_Configuration();	//�ж����ȼ�����
	
	delay_init(72);			//�ӳٳ�ʼ����ʹ��systick���ӳ�
	
	delay_ms(500);			//�ϵ��ӳ�500ms������Ƭ���ȶ�
	
	Cal_PDU_CANID();		//CAN ID ����
	CAN_Configuration();	//CAN���ã�����125K
	CAN_Filter_Config();    //CAN���������ã�ֻ�����������������ݿ��Դ��ͽ���
	
	TIM_Configuration();	//��ʱ����ʼ��
	
	//ADC_DMA_Config();			//DMA��ʼ��
	ADC1_Config();			//ADC��ʼ��
	
	//IWDG_INIT();				//���Ź���ʼ��
	
	
	
	
   
	
	while (1)
	{
		if(SendHeartbeatFlag)   //��������
		{
			Send_CAN_HeartbeatFrame();
			SendHeartbeatFlag=0;
			IWDG_ReloadCounter();
		}
		if(CalAverageFlag)		//����ƽ��ֵ����
		{
			Cal_Curr_average();
			CalAverageFlag=0;
		}
		RUNLED_FLASH();	//���е���˸
		
		Get_Flash_CurrentValue();
		
		
	}
}

/******************* (C) COPYRIGHT 2015 Lopu *****END OF FILE****/
