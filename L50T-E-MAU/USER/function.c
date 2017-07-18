#include "global.h"

///Define

#define ChannelCt    4           	//通道数（4）
#define SignalCt   3           		//灯色数（3）
#define OFF         0          		//点亮信号灯
#define ON        1				//熄灭信号灯

#define VoltageThreshold     8     //电压判定门限

#define SignalR    SignalCon[nchannel-1][0]	//每通道红灯
#define SignalY    SignalCon[nchannel-1][1]	//每通道黄灯
#define SignalG    SignalCon[nchannel-1][2]	//每通道绿灯

uint8_t DutyCount=0,PaluseCount=0,RunLedCount=0,HeartsendCnt=0;				//各计数标志
uint8_t Paluseflag=0;									//负脉冲标志
uint8_t SignalCon[ChannelCt][SignalCt]; //灯驱状态存储器，4相位，每相位3色
uint8_t nchannel;  //当前通道号
uint8_t ADC_ConvertCount=0;           //AD转换次数，1ms自增1
uint8_t TxBuff[11];

volatile uint8_t PDU_CAN_ID,GetHeartbaet,InitializedFlag,CMDgetFlag,SendHeartbeatFlag,CalAverageFlag,SystemStatusFlag;// PDU的CAN地址，心跳获取标志，初始化标志，控制命令获取标志，心跳发送标志，平均值计算标志，系统状态标志。
volatile int ADC_ConvertedValue[Channel_Num];    //AD转换值存储数组
volatile uint8_t Rx_Message[8];					//CAN数据接收缓冲区
volatile uint16_t FuncNumber;					//功能码
volatile uint8_t Sendlatestflag=0;
volatile uint8_t Allowcheckflag = 0;
volatile uint8_t C_Flashget1,C_Flashget2,C_Flashget3,C_Flashget4;
volatile uint8_t HeartlostCnt = 0;
volatile uint8_t PSstatus;						//外灯供电状态

uint8_t SenddataCnt;
uint8_t Statustemp,Allowcurrgetflag;



static uint8_t Receive_Buf[10];              	//数据接收处理缓冲区
uint8_t Convert_num;					 	//AD转换次数
uint8_t ChannelVoltage[12];				//12个通道的电压状态
int Current1[100],Current2[100],Current3[100],Current4[100];//通道电流存储数组
int ADCValue1,ADCValue2,ADCValue3,ADCValue4;//闪灯电流存储
int Currenttemp1[500],Currenttemp2[500],Currenttemp3[500],Currenttemp4[500];//闪灯电流存储数组
uint16_t Average_Current1[5],Average_Current2[5],Average_Current3[5],Average_Current4[5];//通道均方根电流存储数组
uint16_t Average_Current1temp,Average_Current2temp,Average_Current3temp,Average_Current4temp;//通道电流均方根临时变量
uint16_t CurrentValue1,CurrentValue2,CurrentValue3,CurrentValue4;//每200ms上传的平均电流值
uint16_t s_CurrentValue1,s_CurrentValue2,s_CurrentValue3,s_CurrentValue4;//每S上传的平均电流值
uint8_t s_CurrentCnt1,s_CurrentCnt2,s_CurrentCnt3,s_CurrentCnt4;//有效值记录次数
static uint8_t HeartbeatCnt=0,CMDgetCnt=0,HeartgetCnt=12;    //心跳计数，控制命令计数
static uint8_t CPUofflineFlag =1,HeartOffline = 0,SignalSetFlag=0;	//CPU离线标志，心跳离线标志，外灯刷新完成标志
uint8_t Flashflag[4][3];  //闪灯标志数组。

uint16_t FlashCnt1=0,FlashCnt2=0,FlashCnt3=0,FlashCnt4=0;//闪灯模式下计数，根据先亮或后亮取不同时间段的电流值
uint16_t Flash_current1=0,Flash_current2=0,Flash_current3=0,Flash_current4=0;
uint16_t Flash_current1temp,Flash_current2temp,Flash_current3temp,Flash_current4temp;
uint8_t  Flash_current1Cnt,Flash_current2Cnt,Flash_current3Cnt,Flash_current4Cnt;
uint8_t  SendFlashcurr1=0,SendFlashcurr2=0,SendFlashcurr3=0,SendFlashcurr4=0;


CHANNEL1 CH_1;   //结构体声明
CHANNEL2 CH_2;
CHANNEL3 CH_3;
CHANNEL4 CH_4;




//for test
TEST test;
uint8_t receivecmdCnt=1;
uint8_t CMDtime[20];
uint8_t cmdtimecnt;

//计算PDU-CANID
void Cal_PDU_CANID(void)
{
	uint8_t ADDR0,ADDR1,ADDR2,ADDR3;
	ADDR0 = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_3);
	ADDR1 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_2);
	ADDR2 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
	ADDR3 = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_1);
	
	PDU_CAN_ID  = ADDR0;
	PDU_CAN_ID |= ADDR1<<1;
	PDU_CAN_ID |= ADDR2<<2;
	PDU_CAN_ID |= ADDR3<<3;
	
	if(PDU_CAN_ID>8)
		PDU_CAN_ID = 8;
	
	PDU_CAN_ID += 7;
}

//发送CAN数据帧--PDU发送--单帧
uint8_t Send_CAN_DataFrame_PDU_S(uint16_t FuncNumber,uint8_t *ptr,uint8_t len) //ptr: 数据指针. len: 数据长度 
{
	uint16_t i=0,FuncH,FuncL;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	
	SENDID = (0x01<<7)|PDU_CAN_ID;
	FuncH = (FuncNumber&0xFF00)>>8;
	FuncL = (uint8_t)(FuncNumber&0x00FF);
	
	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;//CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;
    
	if((len<=5)&&(len>0))
	{
		TxMessage.DLC = len+3;
		
		TxMessage.Data[0] = 0x00;   //开始帧,单帧传输
		TxMessage.Data[1] = FuncL;	
		TxMessage.Data[2] = FuncH;
		for(i=0; i<len; i++)
		{
			TxMessage.Data[i+3] = *ptr++;
		}	
	}
	else
	{
		TxMessage.DLC = 0;
	}
	
	mailbox_num = CAN_Transmit(CAN1, &TxMessage);
	
	while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<1000)) //发送状态,等到发送完成  
	{
		i++;
	}
	if(i==1000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}


//发送CAN数据帧--PDU多帧数据，11个信息字节，共3帧
uint8_t Send_CAN_DataFrame_PDU(uint8_t *ptr) //ptr: 数据指针
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0,FuncH,FuncL;
	uint32_t SENDID;
	uint16_t n,m;
	CanTxMsg TxMessage;
	
	if(!CPUofflineFlag)
	{
		SENDID = (0x01<<7) | PDU_CAN_ID;
	
		FuncNumber = 361+PDU_CAN_ID-8;
		FuncH = (FuncNumber&0xFF00)>>8;
		FuncL = (uint8_t)(FuncNumber&0x00FF);
		
		TxMessage.StdId = SENDID;
		TxMessage.IDE = CAN_ID_STD;//CAN_ID_EXT;
		TxMessage.RTR = CAN_RTR_DATA;
		
		//frame 1  
		TxMessage.DLC = 8;
		TxMessage.Data[0] = 0x01;   //开始帧
		TxMessage.Data[1] = FuncL;	
		TxMessage.Data[2] = FuncH;	
		TxMessage.Data[3] = *ptr++;
		TxMessage.Data[4] = *ptr++;
		TxMessage.Data[5] = *ptr++;
		TxMessage.Data[6] = *ptr++;
		TxMessage.Data[7] = *ptr++;

		mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		while(mailbox_num==0x04)
		{
			mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		}
		
		while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成  
		{
			i++;
		}
		if(i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
			can_tx_fail = 1;
		}
		
		//frame 2
		TxMessage.DLC = 8;
		TxMessage.Data[0] = 0x02;
		TxMessage.Data[1] = FuncL;
		TxMessage.Data[2] = FuncH;
		TxMessage.Data[3] = *ptr++;
		TxMessage.Data[4] = *ptr++;
		TxMessage.Data[5] = *ptr++;
		TxMessage.Data[6] = *ptr++;
		TxMessage.Data[7] = *ptr++;

		mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		while(mailbox_num==0x04)
		{
			mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		}
		
		while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成  
		{
			i++;
		}
		if(i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
			can_tx_fail = 1;
		}
		
		//frame 3
		TxMessage.DLC = 4;
		TxMessage.Data[0] = 0x03;
		TxMessage.Data[1] = FuncL;
		TxMessage.Data[2] = FuncH;
		TxMessage.Data[3] = *ptr++;


		mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		while(mailbox_num==0x04)
		{
			mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		}
		
		while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成  
		{
			i++;
		}
		if(i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
			can_tx_fail = 1;
		}
		
		return can_tx_fail;
	}
	
	

	
	
}




//发送CAN心跳桢
uint8_t Send_CAN_HeartbeatFrame(void)
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;
	
	SENDID = (0x01<<10)|(0x01<<7) | PDU_CAN_ID;
	
	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;//标准ID
	TxMessage.RTR = CAN_RTR_DATA;
    
	//TxMessage.Data[0]=0x00;
	
	TxMessage.DLC = 0;
	
	mailbox_num = CAN_Transmit(CAN1, &TxMessage);
	
	//GetHeartbaet = 0;
	
	while((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成  
	{
		i++;
	}
	if(i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}


//******************************************************************************************************************
//返回ADC转换的值
//******************************************************************************************************************
uint16_t ReadADCAverageValue(uint16_t Channel)
{
	uint8_t i;
	uint32_t sum = 0;
	for(i=0; i<Sample_Num; i++)
	{
		sum+=ADC_ConvertedValue[Channel];
	}
	return (sum/Sample_Num);
}




//外灯闪烁
uint8_t SFlash(uint8_t channel)
{
	
	switch(channel)
	{
		case 1:
			if(CH_1.F_duty>>5)    //占空比
			{
				if(CH_1.F_mode>>4)  //闪灯模式
				{
					if(DutyCount<5)
						return OFF;
					else if((DutyCount>=5)&&(DutyCount<10))
						return ON;
					else if((DutyCount>=10)&&(DutyCount<15))
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<5)
						return ON;
					else if((DutyCount>=5)&&(DutyCount<10))
						return OFF;
					else if((DutyCount>=10)&&(DutyCount<15))
						return ON;
					else
						return OFF;
				}
			}
			else
			{
				if(CH_1.F_mode>>4)
				{
					if(DutyCount<10)
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<10)
						return ON;
					else
						return OFF;
				}
			}
			break;
		case 2:
			if(CH_2.F_duty>>5)
			{
				if(CH_2.F_mode>>4)
				{
					if(DutyCount<5)
						return OFF;
					else if((DutyCount>=5)&&(DutyCount<10))
						return ON;
					else if((DutyCount>=10)&&(DutyCount<15))
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<5)
						return ON;
					else if((DutyCount>=5)&&(DutyCount<10))
						return OFF;
					else if((DutyCount>=10)&&(DutyCount<15))
						return ON;
					else
						return OFF;
				}
			}
			else
			{
				if(CH_2.F_mode>>4)
				{
					if(DutyCount<10)
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<10)
						return ON;
					else
						return OFF;
				}
			}
			break;
		case 3:
			if(CH_3.F_duty>>5)
			{
				if(CH_3.F_mode>>4)
				{
					if(DutyCount<5)
						return OFF;
					else if((DutyCount>=5)&&(DutyCount<10))
						return ON;
					else if((DutyCount>=10)&&(DutyCount<15))
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<6)
						return ON;
					else if((DutyCount>=5)&&(DutyCount<10))
						return OFF;
					else if((DutyCount>=10)&&(DutyCount<15))
						return ON;
					else
						return OFF;
				}
			}
			else
			{
				if(CH_3.F_mode>>4)
				{
					if(DutyCount<10)
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<10)
						return ON;
					else
						return OFF;
				}
			}
			break;
		case 4:
			if(CH_4.F_duty>>5)
			{
				if(CH_4.F_mode>>4)
				{
					if(DutyCount<5)
						return OFF;
					else if((DutyCount>=5)&&(DutyCount<10))
						return ON;
					else if((DutyCount>=10)&&(DutyCount<15))
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<5)
						return ON;
					else if((DutyCount>=5)&&(DutyCount<10))
						return OFF;
					else if((DutyCount>=10)&&(DutyCount<15))
						return ON;
					else
						return OFF;
				}
			}
			else
			{
				if(CH_4.F_mode>>4)
				{
					if(DutyCount<10)
						return OFF;
					else
						return ON;
				}
				else
				{
					if(DutyCount<10)
						return ON;
					else
						return OFF;
				}
			}
			break;
		default:
			break;
			
	}
}

//外灯负脉冲
uint8_t SPulse(void)
{
	if(PaluseCount<5)
		return OFF;
	else
		return ON;
}


//接收CAN数据帧，不作功能识别
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num)
{
	uint8_t i;
	uint16_t ID_temp;
	for(i=0; i<num; i++)
	{
		Receive_Buf[i]= canRx->Data[i];
	}
	ID_temp =Receive_Buf[2]<<8 | Receive_Buf[1];
	if(ID_temp != (uint16_t)(23+PDU_CAN_ID))    //功能编码不符合，清除
	{
		memset(Receive_Buf,0x00,10);
	}
	else
	{
		DutyCount= 0;
		PaluseCount = 0;
	}
}

//灯色数据设置
void SignalSet(uint8_t channel)
{
	uint8_t Signal = 0x00;
	switch (channel)
	{
		case 1:
			Signal 	= Receive_Buf[3]&0x0F;
			CH_1.F_mode  = Receive_Buf[3]&0x10;
			CH_1.F_duty  = Receive_Buf[3]&0x20;
			break;
		case 2:
			Signal  = Receive_Buf[4]&0x0F;
			CH_2.F_mode  = Receive_Buf[4]&0x10;
			CH_2.F_duty  = Receive_Buf[4]&0x20;
			break;
		case 3:
			Signal	= Receive_Buf[5]&0x0F;
			CH_3.F_mode  = Receive_Buf[5]&0x10;
			CH_3.F_duty  = Receive_Buf[5]&0x20;
			break;
		case 4:
			Signal 	= Receive_Buf[6]&0x0F;
			CH_4.F_mode  = Receive_Buf[6]&0x10;
			CH_4.F_duty  = Receive_Buf[6]&0x20;
			break;
		default:
			break;
	}	
	switch(Signal)
	{
		case 0x00:			//灭灯
			SignalR=OFF;
			SignalY=OFF;
			SignalG=OFF;
		break;
		case 0x01:			//红灯
			SignalR=ON;
			SignalY=OFF;
			SignalG=OFF;
		break;
		case 0x02:			//黄灯
			SignalR=OFF;
			SignalY=ON;
			SignalG=OFF;
		break;
		case 0x03:			//绿灯
			SignalR=OFF;
			SignalY=OFF;
			SignalG=ON;
		break;
		case 0x04:			//红闪灯
			SignalR=SFlash(channel);
			Flashflag[channel-1][0] = 1;
			SignalY=OFF;
			SignalG=OFF;
		break;
		case 0x05:			//黄闪灯
			SignalR=OFF;
			SignalY=SFlash(channel);
			Flashflag[channel-1][1] = 1;
			SignalG=OFF;
		break;
		case 0x06:			//绿闪灯
			SignalR=OFF;
			SignalY=OFF;
			SignalG=SFlash(channel);
			Flashflag[channel-1][2] = 1;
		break;
		case 0x07:			//红黄灯
			SignalR=ON;
			SignalY=ON;
			SignalG=OFF;	
		break;
		case 0x08:			//红闪+黄闪
			SignalR=SFlash(channel);
			Flashflag[channel-1][0] = 1;
			SignalY=SFlash(channel);
			Flashflag[channel-1][1] = 1;
			SignalG=OFF;
		break;
		case 0x09:	//红灯+脉冲
			SignalR=SPulse();
			Flashflag[channel-1][0] = 200;
			SignalY=OFF;
			SignalG=OFF;
		break;
		case 0x0a:	//绿灯+脉冲
			SignalR=OFF;
			SignalY=OFF;
			SignalG=SPulse();
			Flashflag[channel-1][2] = 200;
		break;
		case 0x0b:	   //保留

		break;
		case 0x0c:	

		break;
		case 0x0d:		

		break;
		case 0x0e:		

		break;
		case 0x0f:		

		break;
		default:
		break;
	}
}

/*******************************************************************
* 名    称： ChannelSet
* 功    能： 设置四个通道所有信号灯的状态
* 入口参数： 无
* 出口参数： 无
*******************************************************************/
void ChannelSet(void)
{
	for(nchannel=1;nchannel<=4;nchannel++)
		SignalSet(nchannel);
	if(SignalCon[0][0])
	{
		CH1RON;
	}
	else
	{
		CH1ROFF;
	}
	if(SignalCon[0][1])
	{
		CH1YON;
	}
	else
	{
		CH1YOFF;
	}
	if(SignalCon[0][2])
	{
		CH1GON;
	}
	else
	{
		CH1GOFF;
	}
	if(SignalCon[1][0])
	{
		CH2RON;
	}
	else
	{
		CH2ROFF;
	}
	if(SignalCon[1][1])
	{
		CH2YON;
	}
	else
	{
		CH2YOFF;
	}
	if(SignalCon[1][2])
	{
		CH2GON;
	}
	else
	{
		CH2GOFF;
	}
	if(SignalCon[2][0])
	{
		CH3RON;
	}
	else
	{
		CH3ROFF;
	}
	if(SignalCon[2][1])
	{
		CH3YON;
	}
	else
	{
		CH3YOFF;
	}
	if(SignalCon[2][2])
	{
		CH3GON;
	}
	else
	{
		CH3GOFF;
	}
	if(SignalCon[3][0])
	{
		CH4RON;
	}
	else
	{
		CH4ROFF;
	}
	if(SignalCon[3][1])
	{
		CH4YON;
	}
	else
	{
		CH4YOFF;
	}
	if(SignalCon[3][2])
	{
		CH4GON;
	}
	else
	{
		CH4GOFF;
	}
	SignalSetFlag =1;
	

}

/*******************************************************************
* 名    称： ChannelAlloff
* 功    能： 关闭所有外灯
* 入口参数： 无
* 出口参数： 无
*******************************************************************/
void ChannelAlloff(void)
{
	CH1ROFF;CH1YOFF;CH1GOFF;
	CH2ROFF;CH2YOFF;CH2GOFF;
	CH3ROFF;CH3YOFF;CH3GOFF;
	CH4ROFF;CH4YOFF;CH4GOFF;
}


/*******************************************************************
* 名    称： GetChannelVoltage
* 功    能： 获取外灯电压状态
* 入口参数： 无
* 出口参数： 无
*******************************************************************/
void GetChannelVoltageStatus(void)
{
	if(Allowcheckflag)
	{
		ChannelVoltage[0] |= !CH1RCHECK;
		ChannelVoltage[1] |= !CH1YCHECK;
		ChannelVoltage[2] |= !CH1GCHECK;
		ChannelVoltage[3] |= !CH2RCHECK;
		ChannelVoltage[4] |= !CH2YCHECK;
		ChannelVoltage[5] |= !CH2GCHECK;
		ChannelVoltage[6] |= !CH3RCHECK;
		ChannelVoltage[7] |= !CH3YCHECK;
		ChannelVoltage[8] |= !CH3GCHECK;
		ChannelVoltage[9] |= !CH4RCHECK;
		ChannelVoltage[10] |= !CH4YCHECK;
		ChannelVoltage[11] |= !CH4GCHECK;
	}
	PSstatus |= !PSSCHECK;
}


/*******************************************************************
* 名    称： sqrt_16
* 功    能： 平方根计算
* 入口参数： 无
* 出口参数： 无
*******************************************************************/

unsigned int sqrt_16(unsigned long M)
{
  unsigned int N, i;
  unsigned long tmp, ttp;
  if (M == 0)
  return 0;
  N = 0;
  tmp = (M >> 30);
  M <<= 2;
  if (tmp > 1)
  {
      N ++;
      tmp -= N;
  }
  for (i=15; i>0; i--)
  {
      N <<= 1;
      tmp <<= 2;
      tmp += (M >> 30);
      ttp = N;
      ttp = (ttp<<1)+1;
      M <<=2;
      if(tmp>=ttp)
      {
              tmp-=ttp;
              N++;
      }
  }
  return N;
}
/*******************************************************************
* 名    称： square_sum_average
* 功    能： 有效值计算
* 入口参数： 无
* 出口参数： 无
*******************************************************************/
int square_sum_average(int *p, int n)
{
        long int square_sum_average=0;
        long int square_sum=0;
        int i;
        for(i=0;i<n;i++)
        {
            square_sum+=(*(p+i))*(*(p+i));
        }
        square_sum_average=square_sum/n;
        square_sum_average=sqrt_16(square_sum_average);
        return square_sum_average;
}


/*******************************************************************
* 名    称： LightStatusCheck
* 功    能： 外灯状态判断
* 入口参数： 无
* 出口参数： 无
*******************************************************************/
void LightStatusCheck_Status(void)
{
	//XX_A    
	//0x00:外灯点亮，应控点亮
	//0x01:外灯熄灭，应控熄灭
	//0x02:外灯点亮，应控熄灭
	//0x03:外灯熄灭，应控点亮
	
	//XX_B    
	//0x00:正常
	//0x01:外灯断路
	//0x02:外灯短路
	//0x03:保险丝熔断/可控硅故障
	
	//CH1 R 
	if(((ChannelVoltage[0]) == 1)&&(SignalCon[0][0] == 1)) //应控点亮，外灯点亮
	{
		CH_1.RStatusA = 0x00;   
		//CH_1.RStatusB = 0x00;//正常
	}
	else if(((ChannelVoltage[0]) == 0)&&(SignalCon[0][0] == 0))//应控关闭，外灯关闭
	{
		CH_1.RStatusA = 0x01;
		CH_1.RStatusB = 0x00;//正常
	}
	else if(((ChannelVoltage[0]) == 1)&&(SignalCon[0][0] == 0))//外灯点亮，应控关闭
	{
		CH_1.RStatusA = 0x02;
		CH_1.RStatusB = 0x02;//外灯短路
	}
	else if(((ChannelVoltage[0]) == 0)&&(SignalCon[0][0] == 1))//外灯关闭，应控点亮
	{
		CH_1.RStatusA = 0x03;
		CH_1.RStatusB = 0x03;//保险丝熔断
	}
	if(((ChannelVoltage[0]) == 1)&&(SignalCon[0][0] == 1)&&(CurrentValue1>=VoltageThreshold))//(Average_Current1[Convert_num]>=VoltageThreshold))//外灯点亮，应控点亮，且通道有电流
	{
		CH_1.RStatusB = 0x00;//正常
	}
	else if(((ChannelVoltage[0]) == 1)&&(SignalCon[0][0] == 1)&&(CurrentValue1<VoltageThreshold))//&&(Flashflag[0][0]!=1))//外灯点亮，应控点亮，且通道无电流
	{
		CH_1.RStatusB = 0x01;//外灯断路
		CH_1.RStatusA = 0x03;	
	}
	
	//CH1 Y 
	if(((ChannelVoltage[1]) == 1)&&(SignalCon[0][1] == 1))
	{
		CH_1.YStatusA = 0x00;
	}
	else if(((ChannelVoltage[1]) == 0)&&(SignalCon[0][1] == 0))
	{
		CH_1.YStatusA = 0x01;
		CH_1.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[1]) == 1)&&(SignalCon[0][1] == 0))
	{
		CH_1.YStatusA = 0x02;
		CH_1.YStatusB = 0x02;
	}
	else if(((ChannelVoltage[1]) == 0)&&(SignalCon[0][1] == 1))
	{
		CH_1.YStatusA = 0x03;
		CH_1.YStatusB = 0x03;
	}
	if(((ChannelVoltage[1]) == 1)&&(SignalCon[0][1] == 1)&&(CurrentValue1>=VoltageThreshold))
	{
		CH_1.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[1]) == 1)&&(SignalCon[0][1] == 1)&&(CurrentValue1<VoltageThreshold))//&&(Flashflag[0][1]!=1))
	{
		CH_1.YStatusB = 0x01;
		CH_1.YStatusA = 0x03;
	}
	
	//CH1 G 
	if(((ChannelVoltage[2]) == 1)&&(SignalCon[0][2] == 1))
	{
		CH_1.GStatusA = 0x00;
	}
	else if(((ChannelVoltage[2]) == 0)&&(SignalCon[0][2] == 0))
	{
		CH_1.GStatusA = 0x01;
		CH_1.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[2]) == 1)&&(SignalCon[0][2] == 0))
	{
		CH_1.GStatusA = 0x02;
		CH_1.GStatusB = 0x02;
	}
	else if(((ChannelVoltage[2]) == 0)&&(SignalCon[0][2] == 1))
	{
		CH_1.GStatusA = 0x03;
		CH_1.GStatusB = 0x03;
	}
	if(((ChannelVoltage[2]) == 1)&&(SignalCon[0][2] == 1)&&(CurrentValue1>=VoltageThreshold))
	{
		CH_1.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[2]) == 1)&&(SignalCon[0][2] == 1)&&(CurrentValue1<VoltageThreshold))//&&(Flashflag[0][2]!=1))
	{
		CH_1.GStatusB = 0x01;
		CH_1.GStatusA = 0x03;
	}

	//CH2 R 
	if(((ChannelVoltage[3]) == 1)&&(SignalCon[1][0] == 1))
	{
		CH_2.RStatusA = 0x00;
	}
	else if(((ChannelVoltage[3]) == 0)&&(SignalCon[1][0] == 0))
	{
		CH_2.RStatusA = 0x01;
		CH_2.RStatusB = 0x00;
	}
	else if(((ChannelVoltage[3]) == 1)&&(SignalCon[1][0] == 0))
	{
		CH_2.RStatusA = 0x02;
		CH_2.RStatusB = 0x02;
	}
	else if(((ChannelVoltage[3]) == 0)&&(SignalCon[1][0] == 1))
	{
		CH_2.RStatusA = 0x03;
		CH_2.RStatusB = 0x03;
	}
	if(((ChannelVoltage[3]) == 1)&&(SignalCon[1][0] == 1)&&(CurrentValue2>=VoltageThreshold))//(CurrentValue2>=VoltageThreshold))
	{
		CH_2.RStatusB = 0x00;
	}
	else if(((ChannelVoltage[3]) == 1)&&(SignalCon[1][0] == 1)&&(CurrentValue2<VoltageThreshold))//&&(Flashflag[1][0]!=1))
	{
		CH_2.RStatusB = 0x01;
		CH_2.RStatusA = 0x03;
	}
	
	
	//CH2 Y 
	if(((ChannelVoltage[4]) == 1)&&(SignalCon[1][1] == 1))
	{
		CH_2.YStatusA = 0x00;
	}
	else if(((ChannelVoltage[4]) == 0)&&(SignalCon[1][1] == 0))
	{
		CH_2.YStatusA = 0x01;
		CH_2.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[4]) == 1)&&(SignalCon[1][1] == 0))
	{
		CH_2.YStatusA = 0x02;
		CH_2.YStatusB = 0x02;
	}
	else if(((ChannelVoltage[4]) == 0)&&(SignalCon[1][1] == 1))
	{
		CH_2.YStatusA = 0x03;
		CH_2.YStatusB = 0x03;
	}
	if(((ChannelVoltage[4]) == 1)&&(SignalCon[1][1] == 1)&&(CurrentValue2>=VoltageThreshold))
	{
		CH_2.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[4]) == 1)&&(SignalCon[1][1] == 1)&&(CurrentValue2<VoltageThreshold))//&&(Flashflag[1][1]!=1))
	{
		CH_2.YStatusB = 0x01;
		CH_2.YStatusA = 0x03;
	}
	
	//CH2 G 
	if(((ChannelVoltage[5]) == 1)&&(SignalCon[1][2] == 1))
	{
		CH_2.GStatusA = 0x00;
	}
	else if(((ChannelVoltage[5]) == 0)&&(SignalCon[1][2] == 0))
	{
		CH_2.GStatusA = 0x01;
		CH_2.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[5]) == 1)&&(SignalCon[1][2] == 0))
	{
		CH_2.GStatusA = 0x02;
		CH_2.GStatusB = 0x02;
	}
	else if(((ChannelVoltage[5]) == 0)&&(SignalCon[1][2] == 1))
	{
		CH_2.GStatusA = 0x03;
		CH_2.GStatusB = 0x03;
	}
	if(((ChannelVoltage[5]) == 1)&&(SignalCon[1][2] == 1)&&(CurrentValue2>=VoltageThreshold))
	{
		CH_2.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[5]) == 1)&&(SignalCon[1][2] == 1)&&(CurrentValue2<VoltageThreshold))//&&(Flashflag[1][2]!=1))
	{
		CH_2.GStatusB = 0x01;
		CH_2.GStatusA = 0x03;
	}
	
	//CH3 R 
	if(((ChannelVoltage[6]) == 1)&&(SignalCon[2][0] == 1))
	{
		CH_3.RStatusA = 0x00;
	}
	else if(((ChannelVoltage[6]) == 0)&&(SignalCon[2][0] == 0))
	{
		CH_3.RStatusA = 0x01;
		CH_3.RStatusB = 0x00;
	}
	else if(((ChannelVoltage[6]) == 1)&&(SignalCon[2][0] == 0))
	{
		CH_3.RStatusA = 0x02;
		CH_3.RStatusB = 0x02;
	}
	else if(((ChannelVoltage[6]) == 0)&&(SignalCon[2][0] == 1))
	{
		CH_3.RStatusA = 0x03;
		CH_3.RStatusB = 0x03;
	}
	if(((ChannelVoltage[6]) == 1)&&(SignalCon[2][0] == 1)&&(CurrentValue3>=VoltageThreshold))
	{
		CH_3.RStatusB = 0x00;
	}
	else if(((ChannelVoltage[6]) == 1)&&(SignalCon[2][0] == 1)&&(CurrentValue3<VoltageThreshold))//&&(Flashflag[2][0]!=1))
	{
		CH_3.RStatusB = 0x01;
		CH_3.RStatusA = 0x03;
	}
	
	//CH3 Y 
	if(((ChannelVoltage[7]) == 1)&&(SignalCon[2][1] == 1))
	{
		CH_3.YStatusA = 0x00;
	}
	else if(((ChannelVoltage[7]) == 0)&&(SignalCon[2][1] == 0))
	{
		CH_3.YStatusA  = 0x01;
		CH_3.YStatusB  = 0x00;
	}
	else if(((ChannelVoltage[7]) == 1)&&(SignalCon[2][1] == 0))
	{
		CH_3.YStatusA  = 0x02;
		CH_3.YStatusB  = 0x02;
	}
	else if(((ChannelVoltage[7]) == 0)&&(SignalCon[2][1] == 1))
	{
		CH_3.YStatusA = 0x03;
		CH_3.YStatusB = 0x03;
	}
	if(((ChannelVoltage[7]) == 1)&&(SignalCon[2][1] == 1)&&(CurrentValue3>=VoltageThreshold))
	{
		CH_3.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[7]) == 1)&&(SignalCon[2][1] == 1)&&(CurrentValue3<VoltageThreshold))//&&(Flashflag[2][1]!=1))
	{
		CH_3.YStatusB = 0x01;
		CH_3.YStatusA = 0x03;
	}
	
	//CH3 G 
	if(((ChannelVoltage[8]) == 1)&&(SignalCon[2][2] == 1))
	{
		CH_3.GStatusA = 0x00;
	}
	else if(((ChannelVoltage[8]) == 0)&&(SignalCon[2][2] == 0))
	{
		CH_3.GStatusA = 0x01;
		CH_3.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[8]) == 1)&&(SignalCon[2][2] == 0))
	{
		CH_3.GStatusA = 0x02;
		CH_3.GStatusB = 0x02;
	}
	else if(((ChannelVoltage[8]) == 0)&&(SignalCon[2][2] == 1))
	{
		CH_3.GStatusA = 0x03;
		CH_3.GStatusB = 0x03;
	}
	if(((ChannelVoltage[8]) == 1)&&(SignalCon[2][2] == 1)&&(CurrentValue3>=VoltageThreshold))
	{
		CH_3.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[8]) == 1)&&(SignalCon[2][2] == 1)&&(CurrentValue3<VoltageThreshold))//&&(Flashflag[2][2]!=1))
	{
		CH_3.GStatusB = 0x01;
		CH_3.GStatusA = 0x03;
	}
	
	
	//CH4 R 
	if(((ChannelVoltage[9]) == 1)&&(SignalCon[3][0] == 1))
	{
		CH_4.RStatusA = 0x00;
	}
	else if(((ChannelVoltage[9]) == 0)&&(SignalCon[3][0] == 0))
	{
		CH_4.RStatusA = 0x01;
		CH_4.RStatusB = 0x00;
	}
	else if(((ChannelVoltage[9]) == 1)&&(SignalCon[3][0] == 0))
	{
		CH_4.RStatusA = 0x02;
		CH_4.RStatusB = 0x02;
	}
	else if(((ChannelVoltage[9]) == 0)&&(SignalCon[3][0] == 1))
	{
		CH_4.RStatusA = 0x03;
		CH_4.RStatusB = 0x03;
	}
	if(((ChannelVoltage[9]) == 1)&&(SignalCon[3][0] == 1)&&(CurrentValue4>=VoltageThreshold))
	{
		CH_4.RStatusB = 0x00;
	}
	else if(((ChannelVoltage[9]) == 1)&&(SignalCon[3][0] == 1)&&(CurrentValue4<VoltageThreshold))//&&(Flashflag[3][0]!=1))
	{
		CH_4.RStatusB = 0x01;
		CH_4.RStatusA = 0x03;
	}
	
	//CH4 Y 
	if(((ChannelVoltage[10]) == 1)&&(SignalCon[3][1] == 1))
	{
		CH_4.YStatusA = 0x00;
	}
	else if(((ChannelVoltage[10]) == 0)&&(SignalCon[3][1] == 0))
	{
		CH_4.YStatusA = 0x01;
		CH_4.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[10]) == 1)&&(SignalCon[3][1] == 0))
	{
		CH_4.YStatusA = 0x02;
		CH_4.YStatusB = 0x02;
	}
	else if(((ChannelVoltage[10]) == 0)&&(SignalCon[3][1] == 1))
	{
		CH_4.YStatusA = 0x03;
		CH_4.YStatusB = 0x03;
	}
	if(((ChannelVoltage[10]) == 1)&&(SignalCon[3][1] == 1)&&(CurrentValue4>=VoltageThreshold))
	{
		CH_4.YStatusB = 0x00;
	}
	else if(((ChannelVoltage[10]) == 1)&&(SignalCon[3][1] == 1)&&(CurrentValue4<VoltageThreshold))//&&(Flashflag[3][1]!=1))
	{
		CH_4.YStatusB = 0x01;
		CH_4.YStatusA = 0x03;
	}
	
	//CH4 G 
	if(((ChannelVoltage[11]) == 1)&&(SignalCon[3][2] == 1))
	{
		CH_4.GStatusA = 0x00;
	}
	else if(((ChannelVoltage[11]) == 0)&&(SignalCon[3][2] == 0))
	{
		CH_4.GStatusA = 0x01;
		CH_4.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[11]) == 1)&&(SignalCon[3][2] == 0))
	{
		CH_4.GStatusA = 0x02;
		CH_4.GStatusB = 0x02;
	}
	else if(((ChannelVoltage[11]) == 0)&&(SignalCon[3][2] == 1))
	{
		CH_4.GStatusA = 0x03;
		CH_4.GStatusB = 0x03;
	}
	if(((ChannelVoltage[10]) == 1)&&(SignalCon[3][2] == 1)&&(CurrentValue4>=VoltageThreshold))
	{
		CH_4.GStatusB = 0x00;
	}
	else if(((ChannelVoltage[10]) == 1)&&(SignalCon[3][2] == 1)&&(CurrentValue4<VoltageThreshold))//&&(Flashflag[3][2]!=1))
	{
		CH_4.GStatusB = 0x01;
		CH_4.GStatusA = 0x03;
	}
	
	
}



//ADC取值
uint16_t Get_Adc(void)   
{
  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_10, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	ADCValue1 = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_11, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	ADCValue2 = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_14, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	ADCValue3 = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
	
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ADC_Channel_15, 1, ADC_SampleTime_239Cycles5 );	//ADC1,ADC通道,采样时间为239.5周期	  			    
  
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);		//使能指定的ADC1的软件转换启动功能	
	 
	while(!ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ));//等待转换结束

	ADCValue4 = ADC_GetConversionValue(ADC1);	//返回最近一次ADC1规则组的转换结果
}




void Cal_Curr_average(void)  //计算电流平均值，330（放大100倍）*500（电流比）/249（取样电阻）/4096分辨率
{
	Average_Current1temp = ((uint16_t)(square_sum_average(Current1,100))*332*2/4096);
	Average_Current2temp = ((uint16_t)(square_sum_average(Current2,100))*332*2/4096);
	Average_Current3temp = ((uint16_t)(square_sum_average(Current3,100))*332*2/4096);
	Average_Current4temp = ((uint16_t)(square_sum_average(Current4,100))*332*2/4096);
}



void RUNLED_FLASH(void)   //sysflag;系统状态LED闪烁
{
	switch(CPUofflineFlag)
	{
		case 0:
			if(RunLedCount)							//运行指示灯0.5Hz闪烁
				RUNLEDON;
			else
				RUNLEDOFF;
		break;
		case 1:
			RUNLEDON;
		break;
		default:
		break;
	}
}


void Get_Flashstatus(void)
{
	CH_1.F_mode  = Receive_Buf[3]&0x10;
	CH_1.F_duty  = Receive_Buf[3]&0x20;

	CH_2.F_mode  = Receive_Buf[4]&0x10;
	CH_2.F_duty  = Receive_Buf[4]&0x20;

	CH_3.F_mode  = Receive_Buf[5]&0x10;
	CH_3.F_duty  = Receive_Buf[5]&0x20;

	CH_4.F_mode  = Receive_Buf[6]&0x10;
	CH_4.F_duty  = Receive_Buf[6]&0x20;
	
	
}

/******************************************************************************/
/*                 STM32F10x Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f10x_xx.s).                                            */
/******************************************************************************/

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	uint8_t i;
	CanRxMsg RxMessage;
 	

	RxMessage.StdId = 0x00;  //标准标识符（取值 0~0x7FF）
	RxMessage.ExtId = 0x00;  //扩展标识符（取值 0~0x3FFFF）
	RxMessage.IDE = 0x00;    //标识符的类型
	RxMessage.DLC = 0x00;    //消息的帧长度（取值 0~0x8）
	RxMessage.FMI = 0x00;    //要通过的过滤器索引，这些消息存储于邮箱中（取值为0~0xFF）
	for(i=0; i<8; i++)
	{
		RxMessage.Data[i] = 0x00;//待传输数据（取值为0~0xFF）
		Rx_Message[i] = 0;
	}
	
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//接收
	
	for(i=0; i<RxMessage.DLC; i++)
	{
		Rx_Message[i] = RxMessage.Data[i];
	}
	
	if(RxMessage.IDE==CAN_ID_STD)
	{
		if(RxMessage.DLC>0)
		{
			Receive_CAN_DataFrame(&RxMessage,RxMessage.DLC);//接收CAN数据
			Get_Flashstatus();
			if(!InitializedFlag)
			{
				InitializedFlag = 1;

				TIM_Cmd(TIM4, ENABLE);//启动ADC定时器

			}
			
			if(!CMDgetFlag)
			{
				TIM2->CNT = 99;
				DutyCount=0;
				Convert_num=0;
				HeartsendCnt = 0;
				TIM3->CNT = 199;
			}
			if(SenddataCnt==4)//接收到控制命令的时候，如没有发送5次数据，补齐5次
			{
					
				Send_CAN_DataFrame_PDU(TxBuff);
        
				ADC_ConvertCount=0;
				
				memset(TxBuff,0x00,11);
				
				
			}
			CMDgetFlag = 1;
			TIM3->CNT = 199;					//每次接收到命令同步定时器
			TIM2->CNT = 99;						//每次接收到命令同步定时器
			DutyCount=0;
			CMDgetCnt = 12;

			SenddataCnt=0;
			Convert_num = 0;
			s_CurrentValue1 = 0;
			s_CurrentValue2 = 0;
			s_CurrentValue3 = 0;
			s_CurrentValue4 = 0;
			
			FlashCnt1 = 0;
			FlashCnt2 = 0;
			FlashCnt3 = 0;
			FlashCnt4 = 0;
			
			memset(Flashflag,0x00,12);
			memset(TxBuff,0x00,11);
			
			
		}
		if(RxMessage.DLC==0)
		{
			GetHeartbaet = 1;
			HeartgetCnt = 12;
			CPUofflineFlag = 0;
			RunLedCount = !RunLedCount;
		}
		
	}
}



//------------------------Timer2 IRQ----------------50ms定时---------------------------------
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)//TIM2 interrupt has occured
	{
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);//Clear interrupt pending bit
		
		
		//if((!HeartOffline)&&(!CPUofflineFlag))//心跳在线，控制命令在线
		if(!CPUofflineFlag)//控制命令在线
		{
		
			if(SystemStatusFlag == 0)
			{
				uint8_t vertemp[2];
				
				vertemp[0] = HardV;	//硬件版本号
				vertemp[1] = SoftV;	//软件版本号	
				
				Send_CAN_DataFrame_PDU_S(PDVS,vertemp,2);	
			}

			ChannelSet();								//PDU驱动该板上的四个通道，不断刷新信号灯显示
			SystemStatusFlag = 1;
	
      
			if((Convert_num==1)&&(SignalSetFlag))
			{
				Allowcheckflag = 1;
				Allowcurrgetflag = 1;
				CurrentValue1 = 0;
				CurrentValue2 = 0;
				CurrentValue3 = 0;
				CurrentValue4 = 0;
			}
			
			//外灯状态判断，更新缓冲区数据
			if((Convert_num==3)&&(!CPUofflineFlag))
			{
				
				if(SignalSetFlag)    //判断外灯是否处于闪灯状态
				{
					
					//200ms平均电流计算
					// if(Average_Current1temp>CurrentValue1)
					// {
						// s_CurrentValue1 = Average_Current1temp;
					// }
					// if(Average_Current2temp>CurrentValue2)
					// {
						// s_CurrentValue2 = Average_Current2temp;
					// }
					// if(Average_Current3temp>CurrentValue3)
					// {
						// s_CurrentValue3 = Average_Current3temp;
					// }
					// if(Average_Current4temp>CurrentValue4)
					// {
						// s_CurrentValue4 = Average_Current4temp;
					// }
          
          
					CurrentValue1 = (Average_Current1temp+CurrentValue1)/2;
					CurrentValue2 = (Average_Current2temp+CurrentValue2)/2;
					CurrentValue3 = (Average_Current3temp+CurrentValue3)/2;
					CurrentValue4 = (Average_Current4temp+CurrentValue4)/2;

					LightStatusCheck_Status();  //外灯状态检测
					
					memset(ChannelVoltage,0x00,12);
					
					memset(Flashflag,0x00,12);
					
					//外灯发送数据准备
					TxBuff[0] = (CH_1.YStatusB<<6)|(CH_1.YStatusA<<4)|(CH_1.RStatusB<<2)|(CH_1.RStatusA);
					TxBuff[1] = (CH_2.RStatusB<<6)|(CH_2.RStatusA<<4)|(CH_1.GStatusB<<2)|(CH_1.GStatusA);
					TxBuff[2] = (CH_2.GStatusB<<6)|(CH_2.GStatusA<<4)|(CH_2.YStatusB<<2)|(CH_2.YStatusA);
					TxBuff[3] = (CH_3.YStatusB<<6)|(CH_3.YStatusA<<4)|(CH_3.RStatusB<<2)|(CH_3.RStatusA);
					TxBuff[4] = (CH_4.RStatusB<<6)|(CH_4.RStatusA<<4)|(CH_3.GStatusB<<2)|(CH_3.GStatusA);
					TxBuff[5] = (CH_4.GStatusB<<6)|(CH_4.GStatusA<<4)|(CH_4.YStatusB<<2)|(CH_4.YStatusA);
					SignalSetFlag = 0;
					
					
					//AD采样误差消除
					if(s_CurrentValue1<MINCURRENT)
						s_CurrentValue1=0;
					else if(s_CurrentValue1>MAXCURRENT)
						s_CurrentValue1=255;
					
					if(s_CurrentValue2<MINCURRENT)
						s_CurrentValue2=0;
					else if(s_CurrentValue2>MAXCURRENT)
						s_CurrentValue2=255;
					
					if(s_CurrentValue3<MINCURRENT)
						s_CurrentValue3=0;
					else if(s_CurrentValue3>MAXCURRENT)
						s_CurrentValue3=255;
					
					if(s_CurrentValue4<MINCURRENT)
						s_CurrentValue4=0;
					else if(s_CurrentValue4>MAXCURRENT)
						s_CurrentValue4=255;
					
					if(s_CurrentValue1>TxBuff[6])
					{
						TxBuff[6] = (uint8_t)s_CurrentValue1;
					}
					
					if(s_CurrentValue2>TxBuff[7])
					{
						TxBuff[7] = (uint8_t)s_CurrentValue2;
					}
					if(s_CurrentValue3>TxBuff[8])
					{
						TxBuff[8] = (uint8_t)s_CurrentValue3;
					}
					if(s_CurrentValue4>TxBuff[9])
					{
						TxBuff[9] = (uint8_t)s_CurrentValue4;
					}
					
					
					
					if(PSstatus)
					{
						TxBuff[10] = 0xAA;
					}
					else  //如未检测到外灯供电，不做故障判断，上报电流均为0
					{
						memset(TxBuff,0x00,11);
						
						TxBuff[10] = 0x55;
					}
					
					
					
					
				}
					
				//200ms发送
				
				
				//Convert_num = 0;
				ADC_ConvertCount=0;
				
				SenddataCnt++;

				if(SenddataCnt ==5)
				{
					SenddataCnt = 0; 
				}
				Allowcheckflag = 0;
			}

			Convert_num++;    //采集次数+1
			if(Convert_num==4)
			{
				Send_CAN_DataFrame_PDU(TxBuff);
				Convert_num = 0;
				PSstatus = 0;  //供电状态读取复位
			}	
		}
		else 
		{
			Convert_num = 0;
		}
		
		if((PaluseCount>0)&&(PaluseCount<200)) //负脉冲计数
		{
			PaluseCount++;
		}

		if(CPUofflineFlag)										//无数据命令
		{
			ChannelAlloff();                  	//PDU离线关闭所有外灯驱动
			SystemStatusFlag = 0;
		}

		if(DutyCount==20)
		{
			DutyCount=0;	
    }
		
		DutyCount++;//占空比计数
		
	}
}
//------------------------Timer3 IRQ-------100ms定时器-心跳包专用-----------------------------------------
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)//TIM2 interrupt has occured
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);//Clear interrupt pending bit
			
		cmdtimecnt++;
		
		if(HeartsendCnt<200)
		{
			HeartsendCnt++;

			if(HeartsendCnt == 10)
			{
				HeartsendCnt=0;
				
				SendHeartbeatFlag = 1;
					
			}
		}		
		
		if(CMDgetFlag)
		{
			CMDgetCnt--;
			if(CMDgetCnt==0)
			{
				ChannelAlloff();
				CMDgetFlag = 0;
				Convert_num = 0;
				memset(Receive_Buf,0x00,10);
			}
		}
		if(HeartgetCnt==0)
		{
			CPUofflineFlag =1;
		}
		HeartgetCnt--;
		
		
		
	}
}
//------------------------Timer4 IRQ-----1ms定时器--AD转换专用------------------------------------------
void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)//TIM2 interrupt has occured
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);//Clear interrupt pending bit
		
		GetChannelVoltageStatus();
		
		Get_Adc();
		
		if((ADC_ConvertCount<101)&&(Allowcurrgetflag))
		{

			
			Current1[ADC_ConvertCount] = ADCValue1;
			Current2[ADC_ConvertCount] = ADCValue2;
			Current3[ADC_ConvertCount] = ADCValue3;
			Current4[ADC_ConvertCount] = ADCValue4;
			
			
			
			if(ADC_ConvertCount==100)
			{
				CalAverageFlag = 1;
				Allowcurrgetflag =0;	
			}
			ADC_ConvertCount++;
			
		}
		
		
		if((Flashflag[0][0]==1)|(Flashflag[0][1]==1)|(Flashflag[0][2]==1))
		{
			
			
			if((FlashCnt1>=250)&&(FlashCnt1<750))
			{
				Currenttemp1[FlashCnt1-250] = ADCValue1;
			}
			if(FlashCnt1 == 750)
			{
				C_Flashget1 = 1;
				SendFlashcurr1 = 1;
				
			}
			
			
			
		}
		if((Flashflag[1][0]==1)|(Flashflag[1][1]==1)|(Flashflag[1][2]==1))
		{
			
			
			if((FlashCnt2>=250)&&(FlashCnt2<750))
			{
				Currenttemp2[FlashCnt2-250] = ADCValue2;
			}
			if(FlashCnt2 == 750)
			{
				C_Flashget2 = 1;
				SendFlashcurr2 = 1;
			}
			
			
		}
		if((Flashflag[2][0]==1)|(Flashflag[2][1]==1)|(Flashflag[2][2]==1))
		{
			
			
			if((FlashCnt3>=250)&&(FlashCnt3<750))
			{
				Currenttemp3[FlashCnt3-250] = ADCValue3;
			}
			if(FlashCnt3 == 750)
			{
				C_Flashget3 = 1;
				SendFlashcurr3 = 1;
			}
			
		}
		if((Flashflag[3][0]==1)|(Flashflag[3][1]==1)|(Flashflag[3][2]==1))
		{
			
			
			if((FlashCnt4>=250)&&(FlashCnt4<750))
			{
				Currenttemp4[FlashCnt4-250] = ADCValue4;
			}
			if(FlashCnt4 == 750)
			{
				C_Flashget4 = 1;
				SendFlashcurr4 = 1;
			}
			
		}
		
		if(FlashCnt1<1000)
		{
			FlashCnt1++;
		}
		
		if(FlashCnt2<1000)
		{
			FlashCnt2++;
		}
		
		if(FlashCnt3<1000)
		{
			FlashCnt3++;
		}
		
		if(FlashCnt4<1000)
		{
			FlashCnt4++;
		}
		
	}

}

void Get_Flash_CurrentValue(void)
{
	if(C_Flashget1 == 1)
	{
		Flash_current1temp = ((uint16_t)(square_sum_average(Currenttemp1,500))*332*2/4096);
		if((Flash_current1temp >= Flash_current1)&&(Flash_current1temp>3))
		{
			Flash_current1 = Flash_current1temp;
		}
		else if(((Flash_current1-Flash_current1temp)>0)&&(Flash_current1temp>3))
		{
			Flash_current1 = Flash_current1temp;
		}
		Flash_current1Cnt++;
		if(Flash_current1Cnt==3)
		{
			Flash_current1 = Flash_current1temp;
			Flash_current1Cnt=0;
		}
		
		C_Flashget1 = 0;
	}
	
	if(C_Flashget2 == 1)
	{
		Flash_current2temp = ((uint16_t)(square_sum_average(Currenttemp2,500))*332*2/4096);
		
		if((Flash_current2temp >= Flash_current2)&&(Flash_current2temp>3))
		{
			Flash_current2 = Flash_current2temp;
		}
		else if(((Flash_current2-Flash_current2temp)>0)&&(Flash_current2temp>3))
		{
			Flash_current2 = Flash_current2temp;
		}
		Flash_current2Cnt++;
		if(Flash_current2Cnt==3)
		{
			Flash_current2 = Flash_current2temp;
			Flash_current2Cnt=0;
		}
		
		C_Flashget2 = 0;
	}
	
	if(C_Flashget3 == 1)
	{
		Flash_current3temp = ((uint16_t)(square_sum_average(Currenttemp3,500))*332*2/4096);
		
		if((Flash_current3temp >= Flash_current3)&&(Flash_current3temp>3))
		{
			Flash_current3 = Flash_current3temp;
		}
		else if(((Flash_current3-Flash_current3temp)>0)&&(Flash_current3temp>3))
		{
			Flash_current3 = Flash_current3temp;
		}
		Flash_current3Cnt++;
		if(Flash_current3Cnt==3)
		{
			Flash_current3 = Flash_current3temp;
			Flash_current3Cnt=0;
		}
		
		C_Flashget3 = 0;
	}
	
	if(C_Flashget4 == 1)
	{
		Flash_current4temp = ((uint16_t)(square_sum_average(Currenttemp4,500))*332*2/4096);
		
		if((Flash_current4temp >= Flash_current4)&&(Flash_current4temp>3))
		{
			Flash_current4 = Flash_current4temp;
		}
		else if(((Flash_current4-Flash_current4temp)>0)&&(Flash_current4temp>3))
		{
			Flash_current4 = Flash_current4temp;
		}
		Flash_current4Cnt++;
		if(Flash_current4Cnt==3)
		{
			Flash_current4 = Flash_current4temp;
			Flash_current4Cnt=0;
		}
		
		C_Flashget4 = 0;
	}
}





