#include "global.h"


volatile uint8_t Rx_Message[8];
volatile uint16_t FuncNumber;
extern _Bool Led_Status[16];
volatile uint8_t CPUOFFLINEFLAG=0,GetHeartCnt=12;


//static uint8_t ChannelMode[8];

/*static uint8_t HeartbeatCnt=0,CMDgetCnt=0;*/
//static uint8_t HeartOffline = 1,SignalSetFlag=0,StatechangFlag=0;//SignalSetFlag IOU��ģ����Ա�־
//static uint8_t Flashflag[4][3];


//CHANNEL CH[8];



/**************************************************************************************************************
�������� : CAN��ʼ��
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM = DISABLE; /*��ֹʱ�䴥��ͨѶģʽ*/
	CAN_InitStructure.CAN_ABOM = ENABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
	CAN_InitStructure.CAN_AWUM = DISABLE; /*0-�����ͨ����0���ѣ�1-��⵽����ʱ���Զ�����*/
	CAN_InitStructure.CAN_NART = DISABLE; /*0-һֱ�ظ�����ֱ���ɹ���1-���۳ɹ��Է�ֻ����һ��*///�Զ��ش�
	CAN_InitStructure.CAN_RFLM = DISABLE; /*0-���ʱFIFOδ�������±��ĸǵ��ɱ��ģ�1-������������±���ֱ�Ӷ�ʧ*/
	CAN_InitStructure.CAN_TXFP = DISABLE; /*0-���ķ������ȼ��ɱ�־��������1-���ķ������ȼ��������Ⱥ�˳�����*/
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;/*ģʽ-����ģʽ-����ģʽ*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/*-----------------------------------------------
	//-----------------------------------------------
	//BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
	//-----------------------------------------------
	//   ����   |  SJW  |  BS1  |  BS2  | prescaler |
	//-----------------------------------------------
	//  500kbps |  1tq  | 12tq  |  5tq  |     4     |
	//-----------------------------------------------
	//  250kbps |  1tq  | 12tq  |  3tq  |     9     |
	//----------------------------------------------|
	//  100kbps |  1tq  | 12tq  |  5tq  |     20    |
	//----------------------------------------------
	//--------------------------------------------------------*//*

	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//��ͬ��ʱ����
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;//CAN_BS1_5tq;//ʱ��1���
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;//ʱ��2���
	CAN_InitStructure.CAN_Prescaler = 4;//��Ƶֵ(ʱ�䵥Ԫ����)
	CAN_Init(CAN1, &CAN_InitStructure);

*//*-----------------------------------------------
// APB1=7200/2=3600
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
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);	//CAN1�ж�ʹ��
}

/************************************************
             CAN���ݷ���
***********************************************/

//����CAN����֡--��֡
uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len) //ptr: ����ָ��. len: ���ݳ���
{
	uint16_t i=0,FuncH,FuncL;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	SENDID = (0x01<<7) | MAU_CAN_ID; //��ID.25����Ϊ1��1Ϊ���͵�ַ��0Ϊ���յ�ַ

	switch (MAUFunFlag)
	{
		case 21:
			FuncNumber = 21;//MLMC���0x15
			break;
		case 22:
			FuncNumber = 22;//MMAC���0x16
			break;
		case 23:
			FuncNumber = 23;//MFSC���0x17
			break;
		case 24:
			FuncNumber = 24;//MRTC���0x18
			break;
		case 25:
			FuncNumber = 25;//MKYC���0x19
			break;
		case 26:
			FuncNumber = 26;//MKYC���0x1A
			break;
		case 27:
			FuncNumber = 341;//MAUS���0x0155
			break;
		default:
			break;
	}

	FuncH = (FuncNumber&0xFF00)>>8;
	FuncL = (uint8_t)(FuncNumber&0x00FF);

	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;//CAN_ID_EXT;
	TxMessage.RTR = CAN_RTR_DATA;

	if ((len<=8)&&(len>0))
	{
		TxMessage.DLC = len+3;

		TxMessage.Data[0] = 0x00;   //��ʼ֡,��֡����
		TxMessage.Data[1] = FuncL;
		TxMessage.Data[2] = FuncH;

		for (i=0; i<len; i++)
		{
			TxMessage.Data[i+3] = *ptr++;
		}
	}
	else
	{
		TxMessage.DLC = 0;
	}

	mailbox_num = CAN_Transmit(CAN1, &TxMessage);

	while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //����״̬,�ȵ��������
	{
		i++;
	}
	if (i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//��ʱ�رշ���
		can_tx_fail = 1;
	}
	return can_tx_fail;
}


//����CAN����֡--��֡���ݣ�10����Ϣ�ֽڣ���2֡
uint8_t Send_CAN_DataFrame_MAU(uint8_t *ptr) //ptr: ����ָ��
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0,FuncH,FuncL;
	uint32_t SENDID;
//	uint16_t n,m;
	CanTxMsg TxMessage;
	if(!CPUOFFLINEFLAG)
	{
		SENDID = (0x01<<7) | MAU_CAN_ID;//��ID.25����1
		switch (MAUFunFlag)
		{
		case 21:
			FuncNumber = 21;//MLMC���0x15
			break;
		case 22:
			FuncNumber = 22;//MMAC���0x16
			break;
		case 23:
			FuncNumber = 23;//MFSC���0x17
			break;
		case 24:
			FuncNumber = 24;//MRTC���0x18
			break;
		case 25:
			FuncNumber = 25;//MKYC���0x19
			break;
		case 26:
			FuncNumber = 26;//MAVS���0x1A
			break;
		case 27:
			FuncNumber = 341;//MAUS���0x0155
			break;

		default:
			break;
		}
	//	FuncNumber = 501+(MAU_CAN_ID-97);//���ù��ܱ���501��????

		FuncH = (FuncNumber&0xFF00)>>8;
		FuncL = (uint8_t)(FuncNumber&0x00FF);

		TxMessage.StdId = SENDID;
		TxMessage.IDE = CAN_ID_STD;//CAN_ID_EXT;
		TxMessage.RTR = CAN_RTR_DATA;

		//frame 1
		TxMessage.DLC = 8;
		TxMessage.Data[0] = 0x01;   //��ʼ֡
		TxMessage.Data[1] = FuncL;
		TxMessage.Data[2] = FuncH;
		TxMessage.Data[3] = *ptr++;
		TxMessage.Data[4] = *ptr++;
		TxMessage.Data[5] = *ptr++;
		TxMessage.Data[6] = *ptr++;
		TxMessage.Data[7] = *ptr++;

		mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		while (mailbox_num==0x04)
		{
			mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		}

		while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //����״̬,�ȵ��������
		{
			i++;
		}
		if (i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//��ʱ�رշ���
			can_tx_fail = 1;
		}

		//frame 2
		TxMessage.DLC = 8;
		TxMessage.Data[0] = 0x03;//����֡
		TxMessage.Data[1] = FuncL;
		TxMessage.Data[2] = FuncH;
		TxMessage.Data[3] = *ptr++;
		TxMessage.Data[4] = *ptr++;
		TxMessage.Data[5] = *ptr++;
		TxMessage.Data[6] = *ptr++;
		TxMessage.Data[7] = *ptr++;

		mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		while (mailbox_num==0x04)
		{
			mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		}

		while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //����״̬,�ȵ��������
		{
			i++;
		}
		if (i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//��ʱ�رշ���
			can_tx_fail = 1;
		}
	}

	
	return can_tx_fail;
}


//����CAN������
uint8_t Send_CAN_HeartbeatFrame(void)
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;
	/*
		uint8_t a,GetChannelMode_num=0;
		for(a=0;a<8;a++){
		GetChannelMode_num |= ChannelMode[a]<<a;//?????????????????????????????
		}
	*/
	SENDID = (0x01<<10)|(0x01<<7) | MAU_CAN_ID;//��ID.28��1��ID.25��1,ID28����֡�������ã�1Ϊ����֡��0Ϊ��Ϣ֡

	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;   //��׼ID
	TxMessage.RTR = CAN_RTR_DATA;
	/*
		TxMessage.Data[0]=0x00;

		TxMessage.Data[1]=MAU_CAN_ID;
		TxMessage.Data[2]=GetChannelMode_num;
	*/
	TxMessage.DLC = 0;

	mailbox_num = CAN_Transmit(CAN1, &TxMessage);


	while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000))   //����״̬,�ȵ��������
	{
		i++;
	}
	if (i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);   //��ʱ�رշ���
		can_tx_fail = 1;
	}
	return can_tx_fail;
}



/********************************************************************
             CAN�����������������
********************************************************************/

//����CAN����֡��������ʶ��
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num)
{
	uint8_t i;
	uint16_t ID_temp;
	for (i=0; i<num; i++)
	{
		Receive_Buf[i] = canRx->Data[i];
	}
	ID_temp = Receive_Buf[2]<<8 | Receive_Buf[1];
	switch (ID_temp)
	{
	case 11:
		PASC();//�������뼰ʹ������,����0x0B(11)

		break;
	case 321:
		WARN();//�������״̬,����0x141(321)

		break;
	case 322:
		FATA();//���ع���״̬,����0x142(322)

		break;
	case 323:
		LMPS();//���ͨ���������״̬,����0x143(323)

		break;
	case 324:
		CSOR();//����Դ״̬,����0x144(324)

		break;
	case 325:
		FLSS();//����״̬,����0x145(325)

		break;

	case 326:
		REMT();//�źŻ�ң��״̬,����0x146(326)

		break;
	case 327:
		KNMS();//�ֶ�������״̬,����0x147(327)

		break;
	case 328:
		KSPS();//�ֶ�������״̬,����0x148(328)

		break;
	case 329:
		KARS();//�ֶ�������״̬,����0x149(329)

		break;
	case 330:
		KLAS();//�ֶ�������״̬,����0x14A(330)

		break;
	case 331:
		KLBS();//�ֶ�������״̬,����0x14B(331)

		break;
	case 332:
		KLCS();//�ֶ�������״̬,����0x14C(332)

		break;
	case 333:
		KLDS();//�ֶ�������״̬,����0x14D(333)

		break;
	case 334:
		KLES();//�ֶ�������״̬,����0x14E(334)

		break;
	case 335:
		KLSS();//�ֶ�������״̬,����0x14F(335)

		break;

//�������Ӳ��Ա�������
	case 336:
		Test();//����ָʾ��ȫ��,����0x150(336)
		break;
	default:
		memset(Receive_Buf,0x00,10);
		break;
	}
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

	//��ʼ������
	RxMessage.StdId = 0x00;  //��׼��ʶ����ȡֵ 0~0x7FF��
	RxMessage.ExtId = 0x00;  //��չ��ʶ����ȡֵ 0~0x3FFFF��
	RxMessage.IDE = 0x00;    //��ʶ��������
	RxMessage.DLC = 0x00;    //��Ϣ��֡���ȣ�ȡֵ 0~0x8��
	RxMessage.FMI = 0x00;    //Ҫͨ���Ĺ�������������Щ��Ϣ�洢�������У�ȡֵΪ0~0xFF��
	for (i=0; i<8; i++)
	{
		RxMessage.Data[i] = 0x00;//���������ݣ�ȡֵΪ0~0xFF��
		Rx_Message[i] = 0;
	}
	//���������ϵ������ͷ�FIFO
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//����
	//������յ�����
	for (i=0; i<RxMessage.DLC; i++)
	{
		Rx_Message[i] = RxMessage.Data[i];
	}

	if (RxMessage.IDE==CAN_ID_STD)
	{
		if (RxMessage.DLC>0)
		{
			Receive_CAN_DataFrame(&RxMessage,RxMessage.DLC);//����CAN����
		}
		if (RxMessage.DLC==0)
		{
			
			if(CPUOFFLINEFLAG)
			{
				MAVS();
			}
			CPUOFFLINEFLAG = 0;
			RUNledstatus = !RUNledstatus;
			GetHeartCnt = 12;
		}

	}
}
