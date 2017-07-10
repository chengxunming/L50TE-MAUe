#include "global.h"


volatile uint8_t Rx_Message[8];
volatile uint16_t FuncNumber;
extern _Bool Led_Status[16];
volatile uint8_t CPUOFFLINEFLAG=0,GetHeartCnt=12;


//static uint8_t ChannelMode[8];

/*static uint8_t HeartbeatCnt=0,CMDgetCnt=0;*/
//static uint8_t HeartOffline = 1,SignalSetFlag=0,StatechangFlag=0;//SignalSetFlag IOU板模拟调试标志
//static uint8_t Flashflag[4][3];


//CHANNEL CH[8];



/**************************************************************************************************************
函数描述 : CAN初始化
***************************************************************************************************************/
void CAN_Configuration(void)
{
	CAN_InitTypeDef  CAN_InitStructure;

	CAN_DeInit(CAN1);/* CAN register init */
	CAN_StructInit(&CAN_InitStructure);

	CAN_InitStructure.CAN_TTCM = DISABLE; /*禁止时间触发通讯模式*/
	CAN_InitStructure.CAN_ABOM = ENABLE; /*自动退出离线状态方式，0-有条件手动离线，1-自动退出离线状态*/
	CAN_InitStructure.CAN_AWUM = DISABLE; /*0-由软件通过清0唤醒，1-检测到报文时，自动唤醒*/
	CAN_InitStructure.CAN_NART = DISABLE; /*0-一直重复发送直到成功，1-不论成功以否只发送一次*///自动重传
	CAN_InitStructure.CAN_RFLM = DISABLE; /*0-溢出时FIFO未锁定，新报文盖掉旧报文，1-锁定，溢出后新报文直接丢失*/
	CAN_InitStructure.CAN_TXFP = DISABLE; /*0-报文发送优先级由标志符决定，1-报文发送优先级由请求先后顺序决定*/
//	CAN_InitStructure.CAN_Mode=CAN_Mode_LoopBack;/*模式-测试模式-正常模式*/
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;

	/*-----------------------------------------------
	//-----------------------------------------------
	//BaudRate = APB1 / ((BS1 + BS2 + 1) * Prescaler)
	//-----------------------------------------------
	//   速率   |  SJW  |  BS1  |  BS2  | prescaler |
	//-----------------------------------------------
	//  500kbps |  1tq  | 12tq  |  5tq  |     4     |
	//-----------------------------------------------
	//  250kbps |  1tq  | 12tq  |  3tq  |     9     |
	//----------------------------------------------|
	//  100kbps |  1tq  | 12tq  |  5tq  |     20    |
	//----------------------------------------------
	//--------------------------------------------------------*//*

	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;//重同步时间宽度
	CAN_InitStructure.CAN_BS1 = CAN_BS1_12tq;//CAN_BS1_5tq;//时间1宽度
	CAN_InitStructure.CAN_BS2 = CAN_BS2_5tq;//时间2宽度
	CAN_InitStructure.CAN_Prescaler = 4;//分频值(时间单元长度)
	CAN_Init(CAN1, &CAN_InitStructure);

*//*-----------------------------------------------
// APB1=7200/2=3600
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
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);	//CAN1中断使能
}

/************************************************
             CAN数据发送
***********************************************/

//发送CAN数据帧--单帧
uint8_t Send_CAN_DataFrame_Single(uint8_t *ptr, uint8_t len) //ptr: 数据指针. len: 数据长度
{
	uint16_t i=0,FuncH,FuncL;
	uint8_t mailbox_num, can_tx_fail=0;
	uint32_t SENDID;
	CanTxMsg TxMessage;

	SENDID = (0x01<<7) | MAU_CAN_ID; //将ID.25设置为1，1为发送地址，0为接收地址

	switch (MAUFunFlag)
	{
		case 21:
			FuncNumber = 21;//MLMC编号0x15
			break;
		case 22:
			FuncNumber = 22;//MMAC编号0x16
			break;
		case 23:
			FuncNumber = 23;//MFSC编号0x17
			break;
		case 24:
			FuncNumber = 24;//MRTC编号0x18
			break;
		case 25:
			FuncNumber = 25;//MKYC编号0x19
			break;
		case 26:
			FuncNumber = 26;//MKYC编号0x1A
			break;
		case 27:
			FuncNumber = 341;//MAUS编号0x0155
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

		TxMessage.Data[0] = 0x00;   //开始帧,单帧传输
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

	while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成
	{
		i++;
	}
	if (i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}


//发送CAN数据帧--多帧数据，10个信息字节，共2帧
uint8_t Send_CAN_DataFrame_MAU(uint8_t *ptr) //ptr: 数据指针
{
	uint16_t i=0;
	uint8_t mailbox_num, can_tx_fail=0,FuncH,FuncL;
	uint32_t SENDID;
//	uint16_t n,m;
	CanTxMsg TxMessage;
	if(!CPUOFFLINEFLAG)
	{
		SENDID = (0x01<<7) | MAU_CAN_ID;//将ID.25设置1
		switch (MAUFunFlag)
		{
		case 21:
			FuncNumber = 21;//MLMC编号0x15
			break;
		case 22:
			FuncNumber = 22;//MMAC编号0x16
			break;
		case 23:
			FuncNumber = 23;//MFSC编号0x17
			break;
		case 24:
			FuncNumber = 24;//MRTC编号0x18
			break;
		case 25:
			FuncNumber = 25;//MKYC编号0x19
			break;
		case 26:
			FuncNumber = 26;//MAVS编号0x1A
			break;
		case 27:
			FuncNumber = 341;//MAUS编号0x0155
			break;

		default:
			break;
		}
	//	FuncNumber = 501+(MAU_CAN_ID-97);//设置功能编码501号????

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
		while (mailbox_num==0x04)
		{
			mailbox_num = CAN_Transmit(CAN1, &TxMessage);
		}

		while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成
		{
			i++;
		}
		if (i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
			can_tx_fail = 1;
		}

		//frame 2
		TxMessage.DLC = 8;
		TxMessage.Data[0] = 0x03;//结束帧
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

		while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000)) //发送状态,等到发送完成
		{
			i++;
		}
		if (i==2000)
		{
			CAN_CancelTransmit(CAN1, mailbox_num);//超时关闭发送
			can_tx_fail = 1;
		}
	}

	
	return can_tx_fail;
}


//发送CAN心跳桢
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
	SENDID = (0x01<<10)|(0x01<<7) | MAU_CAN_ID;//将ID.28置1和ID.25置1,ID28数据帧类型设置，1为心跳帧，0为信息帧

	TxMessage.StdId = SENDID;
	TxMessage.IDE = CAN_ID_STD;   //标准ID
	TxMessage.RTR = CAN_RTR_DATA;
	/*
		TxMessage.Data[0]=0x00;

		TxMessage.Data[1]=MAU_CAN_ID;
		TxMessage.Data[2]=GetChannelMode_num;
	*/
	TxMessage.DLC = 0;

	mailbox_num = CAN_Transmit(CAN1, &TxMessage);


	while ((CAN_TransmitStatus(CAN1, mailbox_num) != CANTXOK)&&(i<2000))   //发送状态,等到发送完成
	{
		i++;
	}
	if (i==2000)
	{
		CAN_CancelTransmit(CAN1, mailbox_num);   //超时关闭发送
		can_tx_fail = 1;
	}
	return can_tx_fail;
}



/********************************************************************
             CAN接收数据针分析解析
********************************************************************/

//接收CAN数据帧，作功能识别
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
		PASC();//介入密码及使能命令,命令0x0B(11)

		break;
	case 321:
		WARN();//警告故障状态,命令0x141(321)

		break;
	case 322:
		FATA();//严重故障状态,命令0x142(322)

		break;
	case 323:
		LMPS();//外灯通道输出开关状态,命令0x143(323)

		break;
	case 324:
		CSOR();//控制源状态,命令0x144(324)

		break;
	case 325:
		FLSS();//闪灯状态,命令0x145(325)

		break;

	case 326:
		REMT();//信号机遥控状态,命令0x146(326)

		break;
	case 327:
		KNMS();//手动步进点状态,命令0x147(327)

		break;
	case 328:
		KSPS();//手动步进点状态,命令0x148(328)

		break;
	case 329:
		KARS();//手动步进点状态,命令0x149(329)

		break;
	case 330:
		KLAS();//手动步进点状态,命令0x14A(330)

		break;
	case 331:
		KLBS();//手动步进点状态,命令0x14B(331)

		break;
	case 332:
		KLCS();//手动步进点状态,命令0x14C(332)

		break;
	case 333:
		KLDS();//手动步进点状态,命令0x14D(333)

		break;
	case 334:
		KLES();//手动步进点状态,命令0x14E(334)

		break;
	case 335:
		KLSS();//手动步进点状态,命令0x14F(335)

		break;

//自行增加测试备用命令
	case 336:
		Test();//键盘指示灯全亮,命令0x150(336)
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

	//初始化接收
	RxMessage.StdId = 0x00;  //标准标识符（取值 0~0x7FF）
	RxMessage.ExtId = 0x00;  //扩展标识符（取值 0~0x3FFFF）
	RxMessage.IDE = 0x00;    //标识符的类型
	RxMessage.DLC = 0x00;    //消息的帧长度（取值 0~0x8）
	RxMessage.FMI = 0x00;    //要通过的过滤器索引，这些消息存储于邮箱中（取值为0~0xFF）
	for (i=0; i<8; i++)
	{
		RxMessage.Data[i] = 0x00;//待传输数据（取值为0~0xFF）
		Rx_Message[i] = 0;
	}
	//接收总线上的数据释放FIFO
	CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);//接收
	//处理接收的数据
	for (i=0; i<RxMessage.DLC; i++)
	{
		Rx_Message[i] = RxMessage.Data[i];
	}

	if (RxMessage.IDE==CAN_ID_STD)
	{
		if (RxMessage.DLC>0)
		{
			Receive_CAN_DataFrame(&RxMessage,RxMessage.DLC);//接收CAN数据
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
