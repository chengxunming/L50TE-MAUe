/******************** (C) COPYRIGHT 2012 WildFire Team ***************************
 * �ļ���  ��SysTick.c
 * ����    ��SysTick ϵͳ�δ�ʱ��10us�жϺ�����,�ж�ʱ����������ã�
 *           ���õ��� 1us 10us 1ms �жϡ�
 * ʵ��ƽ̨��Ұ��STM32������
 * Ӳ�����ӣ�-----------------
 *          |                 |
 *          |      ��         |
 *          |                 |
 *           -----------------
 * ��汾  ��ST3.5.0
 * ����    ��wildfire team
 * ��̳    ��http://www.amobbs.com/forum-1008-1.html
 * �Ա�    ��http://firestm32.taobao.com
**********************************************************************************/
#include "SysTick.h"

static __IO u32 TimingDelay;

/*
 * ��������SysTick_Init
 * ����  ������ϵͳ�δ�ʱ�� SysTick
 * ����  ����
 * ���  ����
 * ����  ���ⲿ����
 */
void SysTick_Init(void)
{
	/* SystemFrequency / 1000    1ms�ж�һ��
	 * SystemFrequency / 100000	 10us�ж�һ��
	 * SystemFrequency / 1000000 1us�ж�һ��
	 */
//	if (SysTick_Config(SystemFrequency / 100000))	// ST3.0.0��汾
	if (SysTick_Config(SystemCoreClock / 100000))	// ST3.5.0��汾
	{
		/* Capture error */
		while (1);
	}
	// �رյδ�ʱ��
	SysTick->CTRL &= ~ SysTick_CTRL_ENABLE_Msk;
}


/*
 * ��������Delay_us
 * ����  ��us��ʱ����,10usΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_us( 1 ) ��ʵ�ֵ���ʱΪ 1 * 10us = 10us
 *       ���ⲿ����
 */

void Delay_us(__IO u32 nTime)
{
	TimingDelay = nTime;

	// ʹ�ܵδ�ʱ��
	SysTick->CTRL |=  SysTick_CTRL_ENABLE_Msk;

	while (TimingDelay != 0);
}

/*
 * ��������Delay_ms
 * ����  ��ms��ʱ����,1msΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_ms( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1ms = 1ms
 *       ���ⲿ����
 */

void Delay_ms(__IO u32 nTime)
{
	u32 i=100*nTime;
	Delay_us(i);
}

/*
 * ��������Delay_s
 * ����  ��s��ʱ����,1sΪһ����λ
 * ����  ��- nTime
 * ���  ����
 * ����  ��Delay_s( 1 ) ��ʵ�ֵ���ʱΪ 1 * 1s = 1s
 *       ���ⲿ����
 */

void Delay_s(__IO u8 nTime)
{
	u32 i=1000*nTime;
	Delay_ms(i);
}

/*
 * ��������TimingDelay_Decrement
 * ����  ����ȡ���ĳ���
 * ����  ����
 * ���  ����
 * ����  ���� SysTick �жϺ��� SysTick_Handler()����
 */
void TimingDelay_Decrement(void)
{
	if (TimingDelay != 0x00)
	{
		TimingDelay--;
	}
}
/******************* (C) COPYRIGHT 2012 WildFire Team *****END OF FILE************/
