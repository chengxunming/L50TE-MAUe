#include "global.h"


#define ChannelCt    8            	//ͨ������8��

uint8_t RunLedCount=0,HeartsendCnt=0;				//��������־
uint8_t TestCnt,Convert_num,ring_time,keylockflss;
uint8_t Keyfree_state;
uint8_t Ring_num,KeyNumb;
uint32_t TIMENumber;
uint8_t sw[4];
uint8_t CHn,MLMCqudou,MLMCswitchflag,switchflag,Switchqudou;
uint8_t PWASSWORD_N,Freequdou,passwordvalue,Keyqudou,Keyboardvalue,KeyValue,Keystate,KeyPush=0;;
uint8_t x,y,oldstatus;
uint8_t RUNledstatus,Offlinecount=0,keybeep=0;
_Bool Led_Status[16] = {0};
_Bool forcetest=0;
extern LED LEDStatus;


/**************************************************************************************************************
�������� : ��ʱ����ʼ��
***************************************************************************************************************/
void TIM_Configuration(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;

	//-----------------------------------------------------------------TIM2	  100ms    �������ƶ���ʱ��(����+ִ��)
	//�����жϷ��������ʱ��Ϊ TIM2 = ((1+TIM_Prescaler)/72M)*(1+TIM_Period)=100ms
	TIM_DeInit(TIM2);
	TIM_TimeBaseStructure.TIM_Period=(200-1);		 						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */					//��ʱ��ʽ��(TIM_Period + 1) * (TIM_Prescaler + 1) / TIMx Clock(72M)
	TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36000 = 2k*/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);							    /* �������жϱ�־ */
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM2, ENABLE);												/* ����ʱ�� */

	//-----------------------------------------------------------------TIM3		100us     ������ר��
	TIM_DeInit(TIM3);
	TIM_TimeBaseStructure.TIM_Period=(200-1);		 						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler= (36 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36 = 2M*/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ */
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	TIM_ClearFlag(TIM3, TIM_FLAG_Update);							    /* �������жϱ�־ */
	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM3, ENABLE);												/* ����ʱ�� */

	//-----------------------------------------------------------------TIM4		10ms    ���ݲɼ���ʱ��
	//�����жϷ��������ʱ��ΪTIM4 = ((1+TIM_Prescaler)/72M)*(1+TIM_Period)=10ms
	TIM_DeInit(TIM4);
	TIM_TimeBaseStructure.TIM_Period=(20-1);		 						/* �Զ���װ�ؼĴ������ڵ�ֵ(����ֵ) */
	/* �ۼ� TIM_Period��Ƶ�ʺ����һ�����»����ж� */
	TIM_TimeBaseStructure.TIM_Prescaler= (36000 - 1);				    	/* ʱ��Ԥ��Ƶ�� 72M/36000 = 2k,1/72MHz=13.89ns*/
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 				/* ������Ƶ ʱ��ָ�ֵ*/
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up;			/* ���ϼ���ģʽ */
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);           /* ��ʼ����ʱ��TIM4 */
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);							    /* �������жϱ�־ */
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);           /* ���ж� ����ж� */
	TIM_Cmd(TIM4, ENABLE);												/* ����ʱ�� */


}


void LEDStatusFresh(void)
{
	if(Led_Status[0])
	{
		LEDSTEPON;
	}
	else
		LEDSTEPOFF;
	
	if(Led_Status[1])
	{
		LEDONEON;
	}
	else
		LEDONEOFF;
	
	if(Led_Status[2])
	{
		LEDTWOON;
	}
	else
		LEDTWOOFF;
	
	if(Led_Status[3])
	{
		LEDTHREEON;
	}
	else
		LEDTHREEOFF;
	
	if(Led_Status[4])
	{
		LEDALLREDON;
	}
	else
		LEDALLREDOFF;
	
	if(Led_Status[5])
	{
		LEDFOURON;
	}
	else
		LEDFOUROFF;
	
	if(Led_Status[6])
	{
		LEDFIVEON;
	}
	else
		LEDFIVEOFF;
	
	if(Led_Status[7])
	{
		LEDSIXON;
	}
	else
		LEDSIXOFF;
	
	if(Led_Status[8])
	{
		LEDKEYAON;
	}
	else
		LEDKEYAOFF;
	
	if(Led_Status[9])
	{
		LEDSEVENON;
	}
	else
		LEDSEVENOFF;
	
	if(Led_Status[10])
	{
		LEDEIGHTON;
	}
	else
		LEDEIGHTOFF;
	
	if(Led_Status[11])
	{
		LEDTESTON;
	}
	else
		LEDTESTOFF;
	
	if(Led_Status[12])
	{
		LEDKEYBON;
	}
	else
		LEDKEYBOFF;
	
	if(Led_Status[13])
	{
		LEDKEYCON;
	}
	else
		LEDKEYCOFF;
	
	if(Led_Status[14])
	{
		LEDKEYDON;
	}
	else
		LEDKEYDOFF;
	
	if(Led_Status[15])
	{
		LEDKEYEON;
	}
	else
		LEDKEYEOFF;	
}

void GetLedStatus(void)
{
	Led_Status[0] = LEDStatus.Step;
	Led_Status[1] = LEDStatus.One;
	Led_Status[2] = LEDStatus.Two;
	Led_Status[3] = LEDStatus.Three;
	Led_Status[4] = LEDStatus.Allred;
	Led_Status[5] = LEDStatus.Four;
	Led_Status[6] = LEDStatus.Five;
	Led_Status[7] = LEDStatus.Six;
	Led_Status[8] = LEDStatus.Key_A;
	Led_Status[9] = LEDStatus.Seven;
	Led_Status[10] = LEDStatus.Eight;
	Led_Status[11] = LEDStatus.Test;
	Led_Status[12] = LEDStatus.Key_B;
	Led_Status[13] = LEDStatus.Key_C;
	Led_Status[14] = LEDStatus.Key_D;
	Led_Status[15] = LEDStatus.Key_E;	
}

void StatusLedFresh(void)
{
	if(LEDStatus.Fault) ERRLEDON; else ERRLEDOFF;
	if(LEDStatus.Open) TUNONLEDON; else TUNONLEDOFF;
	if(LEDStatus.Lock) KEYLOCKON; else KEYLOCKOFF;
	
	if(LEDStatus.Test) TESTON; else TESTOFF;
	if(LEDStatus.Center) CENTREON; else CENTREOFF;
	if(LEDStatus.Manual) MANON; else MANOFF;
	if(LEDStatus.Timebase) TIMEBASEON; else TIMEBASEOFF;
	
	if(LEDStatus.A_Flash) AUTOFLSSON; else AUTOFLSSOFF;
	if(LEDStatus.M_Flash) MANFLSSON; else MANFLSSOFF;
	if(LEDStatus.F_Flash) ERRFLSSON; else ERRFLSSOFF;
	if(LEDStatus.H_Flash) ANTCPFLSSON; else ANTCPFLSSOFF;
}

void NormalFresh(void)
{
	StatusLedFresh();
	GetLedStatus();
	LEDStatusFresh();
}



/**************************************************************************************************************

 ------------------------------TIM2 MAU������ʱ��-------T=100ms------------------------------------------------

***************************************************************************************************************/
void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{//TIM2 interrupt has occured
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//Clear interrupt pending bit

		
		/*******************************************************************************
				                          ����״̬�ж�
		********************************************************************************/

		if (HeartsendCnt<200)
		{//����֡ѭ������200������
			
			if (HeartsendCnt == 10)
			{//�ȴ�10�η���1�Σ�������1000ms
				HeartsendCnt=0;  //���ʹ�������
				SendHeartbeatFlag = 1;//����������־��1
			}
			HeartsendCnt++;
			
		}
		else HeartsendCnt=0;

		/*******************************************************************************
				                          MAU����&ͨ��״̬
		********************************************************************************/

		if(CPUOFFLINEFLAG)
		{
			passwordFlag=1;
			off_passwordFlag=0;
			LEDALL(OFF);
			memset(&LEDStatus,0x00,sizeof(LEDStatus));
			GetLedStatus();	
			ringflag = 0;
			oldstatus =0;
      
		}
		
		else//���MAU���ߣ���������
		{   

			
			uint8_t X;

			if (!oldstatus)
			{
				MLMC();
				MMAC();
				MFSC();
				MRTC();
				MAVS();
				ReadKeyboard();
				X=Read_KeyValue();
				if (!X)
				{
					MKYC(X-1);
				}
				passwordFlag = 0;
				TIMENumber = 0;
        
			}
			oldstatus = 1;
      
			
			


		/*******************************************************************************
				                        ��������/�ر�
		********************************************************************************/
			
			
		

		/*******************************************************************************
				                        ���ϵ���˸��/�ر�
		********************************************************************************/
//    ErrLEDCnt++;
			
			

		/*******************************************************************************
				                        ָʾ�Ʋ���
		********************************************************************************/
			if ((TestFlag)||(forcetest))
			{
				
				//if ((TestCnt<30)||(forcetest))
				//{
				LEDALL(ON);
				if(TestCnt<30)
				{
					TestCnt++;
				}
					
				//}
				
				if ((TestCnt == 30)&&(forcetest==0))
				{
					TestFlag=0;
					LEDALL(OFF);
					TestCnt=0;
					NormalFresh();
				}
			}
			else
			{
				TestCnt=0;
			}
		}
    if(GetHeartCnt==0)
      CPUOFFLINEFLAG =1;
    GetHeartCnt--;
		if(ringflag)
		{
			ring_time++;
			if (ring_time<3)
				TIM_Cmd(TIM3, ENABLE);												/* ����ʱ�� */
			else if (ring_time<30) //time=ring_time*100ms(3s��һ��)
				TIM_Cmd(TIM3, DISABLE);
			else ring_time=0;
		}
		else 
		{
			TIM_Cmd(TIM3, DISABLE);												/* �ر�ʱ�� */
			ring_time=0;
		}
		if(keybeep>1)
		{
			TIM_Cmd(TIM3, ENABLE);
			if(keybeep==4)
			{
				TIM_Cmd(TIM3, DISABLE);
				keybeep=0;
			}
			keybeep++;
		}
			
	}
}

/***********************************************************************************************

------------------------Timer3 IRQ--------MAU������ʹ�ܶ�ʱ��----T=100us------------------------

***********************************************************************************************/

void TIM3_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//TIM2 interrupt has occured
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//Clear interrupt pending bit
		//�������򿪣�Ƶ��2kHz����T=100us*5=500us
		//�������򿪣�Ƶ��3.3kHz����T=100us*3=300us
		//�������򿪣�Ƶ��4kHz����T=50us*5=250us
		/***************************************************
		            			_   _
		                 | | | |
		         ________| |_| |_______
		****************************************************/
		Ring_num++;
		if (Ring_num<3)
      BEEPOFF;
			//BEEPON;
		else if (Ring_num<6)
			BEEPOFF;
		else
			Ring_num=0;
	}
}




/******************************************************************************************************
------------------------Timer4 IRQ-----����&��������״̬�ɼ�ר��---T=10ms---------------------------
*******************************************************************************************************/
void TIM4_IRQHandler(void){

	if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//TIM2 interrupt has occured
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//Clear interrupt pending bit

		if (TIMENumber==0)
		{
			passwordvalue= *(uint32_t*)FLASHPASSWORD;
			

			if ((passwordvalue & 0x10) == 0x10 )
			{//�ж��������״̬
				if (!passwordFlag)
				{
					passwordFlag=1;
					TIMENumber=1;//��������
					password=passwordvalue;
					KEYLOCKON;
					LEDStatus.Lock = 1;
					MAUS();
				}
			}
			else
			{
				TIMENumber=1;//��������
				if(TestFlag==0)
				{
					KEYLOCKOFF;
				}
				LEDStatus.Lock = 0;
				passwordFlag=0;
			}
		}

		if (TIMENumber<90001)
		{ //���������ȴ�ʱ��=10ms*90000=900s=15min
			//��ƿ����ж�
			sw[0]=SIGNAL1;
			if (sw[0] != SwitchMode[0])
			{
				MLMCswitchflag=1;
			}
			if (MLMCqudou)
			{
				if (sw[0] == x)
				{
					SwitchMode[0]=SIGNAL1;
					MLMCswitchflag=0;
					MLMCqudou=0;
					MLMC();
				}
				else
				{
					MLMCswitchflag=0;
					MLMCqudou=0;
				}
			}
			else if (MLMCswitchflag)
			{
				x = sw[0];
				MLMCqudou=1;
			}

			//�����и���
			if (passwordchangeFlag)
			{
				PWASSWORD_N=0;
			}

			if (passwordFlag)
			{//�ж�����
				//�ж���������
				//ɨ�����
				
				ReadKeyboard();
				if ((KEYx & 0xff)!=0x0f){//�а�������
					if (!Keystate){
						keylockflss=1;
						Keyboardvalue=Read_KeyValue();
						if ((Keyboardvalue & 0xff)!=0x00)
						{
							if (PWASSWORD_N ==0 )
							{
								passwordvalue= *(uint32_t*)FLASHPASSWORD;
							}
							if (Keyqudou)
							{
								Keystate=1;//�а������£�����״̬��0
								Keyqudou=0;
								PWASSWORD_N++;
								if (!Keyfree_state)
									Keyled_state=1;//����ָʾ��״̬��1
								
								KeyValue=(Keyboardvalue-1);//����ֵ��Ч,��¼����ֵ
							
								GetLedStatus();
								Led_Status[KeyValue] = !Led_Status[KeyValue];
								keybeep =2;
							}
							else 
								Keyqudou=1;
							
							
							
						}
					}
				}


				//�ж���������
				else if (Keystate)
				{
					if (Freequdou)
					{
						Keystate=0;//�а������𣬰���״̬��0
						KeyValue=(Keyboardvalue-1);
						Led_Status[KeyValue] = !Led_Status[KeyValue];
						keybeep=0;

						if (Keyled_state)
							Keyfree_state=1;
						else
							Keyfree_state=0;

						Freequdou=0;
						if ((( passwordvalue >>(4-PWASSWORD_N) ) & 0x01) != KeyValue )
						{//�жϼ��������Ƿ���洢������ͬ
							PWASSWORD_N=0;
						}

						if (PWASSWORD_N==4)
						{
							passwordFlag=0;
							TIMENumber=1;//��ʼ����
							keylockflss=0;
							KEYLOCKOFF;
							MAUS();
							PWASSWORD_N=0;
						}
					}
					else Freequdou=1;
				}
        KEYLOCKON;
			}

			else
			{//���û������
				if(TestFlag==0)
				{
					KEYLOCKOFF;
				}
				//��������ɨ��
				sw[1]=SIGNAL2;
				sw[2]=SIGNAL3;
				sw[3]=SIGNAL4;
				if (CHn<4)
				{
					if (sw[CHn] != SwitchMode[CHn])
					{
						TIMENumber=1;
						switchflag=1;
					}
					else
					{
						CHn++;
					}
					if (Switchqudou)
					{
						if (sw[CHn] == y)
						{
							SwitchMode[CHn]=sw[CHn];
							switchflag=0;
							Switchqudou=0;
							switch (CHn)
							{
								case 1:
									MMAC();//MAU�ֿ�/�Զ��л�����
									break;
								case 2:
									MFSC();//MAU���ƿ�������
									break;
								case 3:
									MRTC();//MAUң�ع���ʹ������
									break;
								default:
									break;
							}
							CHn++;
						}
						else
						{
							switchflag=0;
							Switchqudou=0;
						}
					}
					else if (switchflag)
					{
						y = sw[CHn];
						Switchqudou=1;
					}
				}
				else CHn=1;

				//ɨ�谴��
				ReadKeyboard();
				if ((KEYx & 0xff)!=0x0f)
				{   //�а�������
					TIMENumber=1;
					if (!Keystate)
					{          //�ж�����״̬
						Keyboardvalue=Read_KeyValue();
						if ((Keyboardvalue & 0xff)!=0x00)
						{
							if (Keyqudou)
							{
								Keystate=1;//�а������£�����״̬��1
								Keyqudou=0;
								if (!Keyfree_state)
									Keyled_state=1;
								KeyValue=(Keyboardvalue-1);//����ֵ��Ч,��¼����ֵ
								
								GetLedStatus();
								Led_Status[KeyValue] = !Led_Status[KeyValue];
								MKYC(KeyValue);	//���ͼ�ֵ
								if(KeyValue==11)
								{
									forcetest=1;
								}
								keybeep=2;
							}
							else 
							{
								Keyqudou=1;
							}	
						}
					}
				}
				//�ж���������
				else if (Keystate)
				{
					if (Freequdou)
					{
						Keystate=0;//�а������𣬰���״̬��0
						KeyValue=(Keyboardvalue-1);//����ֵ��Ч,��¼����ֵ
						
						if(KeyValue==11)
						{
							forcetest=0;
						}
						
						Freequdou=0;

						if (Keyled_state)
							Keyfree_state=1;
						else
							Keyfree_state=0;
						
						Led_Status[KeyValue] = !Led_Status[KeyValue];
						
						MKYC(Keyrelease);	//���ͼ�ֵ
						
						GetLedStatus();
					}
					else 
						Freequdou=1;
				}
			}
			TIMENumber++;

			//����ָʾ����˸
			if (keylockflss && (TIMENumber !=6000)){

				if ((TIMENumber%20) == 0)
				{//100ms��˸T = 20*10ms=200ms
					KEYLOCKON;
				}
				else 	
					KEYLOCKOFF;
			}
			else 	
				keylockflss=0;
		}
		else if (((password | 0X0F) == 0x1F ) && off_passwordFlag)
		{//�ж��������״̬
			if (!passwordFlag)
			{
				passwordFlag=1;
				TIMENumber=1;//��������
				KEYLOCKON;
				MAUS();
			}
			else TIMENumber=1;
		}
		else 
		{
			TIMENumber=1;//��������
			KEYLOCKOFF;
			passwordFlag=0;
		}
		if((TestFlag==0)&&(forcetest==0))
		{
			LEDStatusFresh();
			StatusLedFresh();
		}
		
		
//		KeyLEDFLSS();
		/*******************************************************************************
				                        ��������˸
		********************************************************************************/
		// if (Keyled_state)
		// {

			// if (!Keyfree_state)
				// KeyNumb=(Keyboardvalue-1);

			// Keylednum++;
			// KeyLEDFLSS(KeyNumb);
		// }
		
	}
}


void RUNLED(void)
{
	if(RUNledstatus)
		RUNLEDOFF;
	else
		RUNLEDON;
	if(passwordFlag)
		KEYLOCKON;
	else
		KEYLOCKOFF;
	if(SeriousErrFlag)
	{
		ERRLEDON;
	}
	else	
	{
		if(WarningErrFlag)
		{
			if (RUNledstatus)
			{
				ERRLEDON;       //���ϵƴ�
			}
			else
			{
				ERRLEDOFF;       //���ϵƹر�
			}
		}
		else
			ERRLEDOFF;
		
	}
}

