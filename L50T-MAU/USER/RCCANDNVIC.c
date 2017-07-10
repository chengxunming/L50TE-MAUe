/* Includes ------------------------------------------------------------------*/
//#include "stm32f10x.h"
//#include <stdio.h>
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
	if (HSEStartUpStatus==SUCCESS)
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
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

		//ϵͳʱ��SYSCLK����PLL���
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		//�л�ʱ�Ӻ�ȴ�ϵͳʱ���ȶ�
		while (RCC_GetSYSCLKSource()!=0x08);
	}

	//����GPIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC | RCC_APB2Periph_GPIOD, ENABLE);
	//����AFIO
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	//����ADC
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
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

