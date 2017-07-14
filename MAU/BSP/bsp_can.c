#include "bsp_can.h"
#include "global.h"
#include "bsp.h"

static Frame_DefType receive_Frame;
volatile uint8_t Rx_Message[8];
volatile uint16_t FuncNumber;
volatile uint8_t MAUFunFlag;

static void CAN_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	
	CAN1_GPIO_CLK_ENABLE();//ʹ��CAN1_GPIOʱ��	                   											

    GPIO_InitStructure.GPIO_Pin = CAN1_TX_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//��������
    GPIO_Init(CAN1_GPIO, &GPIO_InitStructure);			//��ʼ��IO
   
    GPIO_InitStructure.GPIO_Pin = CAN1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;//��������
    GPIO_Init(CAN1_GPIO, &GPIO_InitStructure);		 //��ʼ��IO	
}
/**************************************************************************************************************
�������� : CAN��ʼ��
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure; 

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);
	
	CAN_GPIO_Init();
	CAN1_CLK_ENABLE();//ʹ��CAN1ʱ��
	
	CAN_InitStructure.CAN_TTCM = DISABLE; /*��ֹʱ�䴥��ͨѶģʽ*/
	CAN_InitStructure.CAN_ABOM = ENABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
//	CAN_InitStructure.CAN_ABOM = DISABLE; /*�Զ��˳�����״̬��ʽ��0-�������ֶ����ߣ�1-�Զ��˳�����״̬*/
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
//	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
	
	/* Enable the CAN Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	CAN_Filter_Config();
}
/*
 * ��������CAN_Filter_Config
 * ����  ��CAN�Ĺ����� ����
 * ����  ����
 * ���  : ��
 * ����  ���ڲ�����
 */
void CAN_Filter_Config(void)
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
	
	//CAN��������ʼ��
	CAN_FilterInitStructure.CAN_FilterNumber=1;						//��������0
	CAN_FilterInitStructure.CAN_FilterMode=CAN_FilterMode_IdMask;	//�����ڱ�ʶ�б�ģʽ
	CAN_FilterInitStructure.CAN_FilterScale=CAN_FilterScale_32bit;	//������λ��Ϊ����32λ��
	// ʹ�ܱ��ı�ʾ�����������ձ�ʾ�������ݽ��бȶԹ��ˣ���չID�������µľ����������ǵĻ��������FIFO0��

	CAN_FilterInitStructure.CAN_FilterIdHigh= ((uint32_t)(MAU_CAN_ID<<21)&0xffff0000)>>16;				//Ҫ���˵�ID��λ
	CAN_FilterInitStructure.CAN_FilterIdLow= (CAN_ID_STD|CAN_RTR_DATA)&0xffff; //Ҫ���˵�ID��λ
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh=0x6FFF;//0x7FFF;			//��������16λ������֮֡�����ƥ��
	CAN_FilterInitStructure.CAN_FilterMaskIdLow=0xFFFF;//0xFFFF;			//��������16λÿλ����ƥ��
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment=CAN_Filter_FIFO0 ;				//��������������FIFO0
	CAN_FilterInitStructure.CAN_FilterActivation=ENABLE;			//ʹ�ܹ�����
	CAN_FilterInit(&CAN_FilterInitStructure);
	//CANͨ���ж�ʹ��
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

/*
 * ��������	Send_CAN_HeartbeatFrame
 * ����  ��	����CAN������
 * ����  ��	��
 * ���  : 	can_tx_fail  CAN����״̬ 0���ɹ�     1��ʧ��
 */
uint8_t Send_CAN_HeartbeatFrame(void)
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	SENDID = (0x01<<10)|(0x01<<7) | MAU_CAN_ID;//��ID.28��1��ID.25��1,ID28����֡�������ã�1Ϊ����֡��0Ϊ��Ϣ֡

	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;   //��׼ID
	TxMessage.RTR = CAN_RTR_DATA;

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

/*
 * ��������	Send_CAN_DataFrame_Single
 * ����  ��	����CAN����֡--��֡
 * ����  ��	ptr: ����ָ��
 * 			len: ���ݳ���
 * ���  :	can_tx_fail  CAN����״̬ 0���ɹ�     1��ʧ��
 */
uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len)
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





//����CAN����֡��������ʶ��
/*
 * ��������	Send_CAN_DataFrame_Single
 * ����  ��	����CAN����֡--��֡
 * ����  ��	canRx: CAN������Ϣ����ָ��
 * 			num	 : �������ݳ���
 * ���  :	can_tx_fail  CAN����״̬ 0���ɹ�     1��ʧ��
 */
//����CAN����֡��������ʶ��
void Receive_CAN_DataFrame(CanRxMsg* canRx,uint8_t num)
{
	uint8_t i;
	uint16_t ID_temp;
//	for (i=0; i<num; i++)
//	{
//		Receive_Buf[i] = canRx->Data[i];
//	}
//	ID_temp = Receive_Buf[2]<<8 | Receive_Buf[1];
//	switch (ID_temp)
//	{
//	case 11:
//		PASC();//�������뼰ʹ������,����0x0B(11)

//		break;
//	case 321:
//		WARN();//�������״̬,����0x141(321)

//		break;
//	case 322:
//		FATA();//���ع���״̬,����0x142(322)

//		break;
//	case 323:
//		LMPS();//���ͨ���������״̬,����0x143(323)

//		break;
//	case 324:
//		CSOR();//����Դ״̬,����0x144(324)

//		break;
//	case 325:
//		FLSS();//����״̬,����0x145(325)

//		break;

//	case 326:
//		REMT();//�źŻ�ң��״̬,����0x146(326)

//		break;
//	case 327:
//		KNMS();//�ֶ�������״̬,����0x147(327)

//		break;
//	case 328:
//		KSPS();//�ֶ�������״̬,����0x148(328)

//		break;
//	case 329:
//		KARS();//�ֶ�������״̬,����0x149(329)

//		break;
//	case 330:
//		KLAS();//�ֶ�������״̬,����0x14A(330)

//		break;
//	case 331:
//		KLBS();//�ֶ�������״̬,����0x14B(331)

//		break;
//	case 332:
//		KLCS();//�ֶ�������״̬,����0x14C(332)

//		break;
//	case 333:
//		KLDS();//�ֶ�������״̬,����0x14D(333)

//		break;
//	case 334:
//		KLES();//�ֶ�������״̬,����0x14E(334)

//		break;
//	case 335:
//		KLSS();//�ֶ�������״̬,����0x14F(335)

//		break;

////�������Ӳ��Ա�������
//	case 336:
//		Test();//����ָʾ��ȫ��,����0x150(336)
//		break;
//	default:
//		memset(Receive_Buf,0x00,10);
//		break;
//	}
}

/*
 * ��������	IRQHandler_CAN1_RX0_Callback
 * ����  ��	CAN�����жϻص�����
 * ����  ��	��
 * ���  :	��
 */
void IRQHandler_CAN1_RX0_Callback(void)
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
//			if(CPUOFFLINEFLAG)
//			{
//				MAVS();
//			}
//			CPUOFFLINEFLAG = 0;
//			RUNledstatus = !RUNledstatus;
//			GetHeartCnt = 12;
		}

	}
}
