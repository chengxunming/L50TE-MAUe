#include "global.h"

#define ON         1          		  //����ָʾ��
#define OFF        0			        	//Ϩ��ָʾ��

#define Warning		1
#define Serious		2
#define	Normal 		0

uint8_t Receive_Buf[10];
uint8_t SwitchMode[4];

uint8_t TxBuff[10];
volatile uint8_t TestFlag,SendHeartbeatFlag=1,ringflag=0;
volatile uint8_t GetHeartbaet,WarningErrFlag,SeriousErrFlag,ChannelSetFlag;//
volatile uint8_t MAUFunFlag;
uint8_t passwordFlag,password,passwordchangeFlag,off_passwordFlag,KEYx=0,Keylednum,Key_LED[16];;
//uint8_t * Address = (uint8_t * ) FLASHPASSWORD ;

uint8_t passwordAddress[1],flashvalue,Keyled_state;
//uint8_t SignalSetFlag=0,StatechangFlag=0;
uint8_t keylin[8];

KEYLEDSTATUS KEYLED[16];

LED LEDStatus;

extern _Bool Led_Status[16];

/************************************************
             CPU��ȡ��Ϣ����
***********************************************/
//�������뼰ʹ������,����0x0B(11)
void PASC(void)
{
	uint8_t i;
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];
	i=(Signal & 0X0F);

	if ((password & 0X10) == 0x10)
	{//������ʹ��
		off_passwordFlag=1;
		if ((Signal | 0X0F) == 0x1F )
		{
			if (i != (password & 0x0F))
			{//������
				passwordchangeFlag=1;//����ı�,��־��1
				password = (i | 0X10);//��������
//			  flashstor(password);//�������Flash
			}
			else 	passwordchangeFlag=0;//���벻��,��־��0
		}
		else if ((Signal | 0X0F) == 0x0F )
		{//����MAU��������
			passwordFlag = 0;//MAU���������־��0
			KEYLOCKOFF;
			LEDStatus.Lock = 0;
			password=(Signal & 0X0F);
			passwordchangeFlag=1;//����ı�,��־��1
			MAUS();
		}
	}
	else if ((password & 0X10) == 0x10)
	{
		if ((Signal | 0X0F) == 0x1F )
		{		//ʹ��MAU��������
			passwordFlag=1;
			KEYLOCKON;
			LEDStatus.Lock = 1;
			MAUS();
			password = (i | 0X10);//ʹ�ܽ��������־����Flash
			passwordchangeFlag=1;//����ı�,��־��1
//			flashstor(password);//�������Flash
		}
		else if ((Signal | 0X0F) == 0x0F )
		{
			//����MAU��������
			passwordFlag = 0;//MAU���������־��0
		}
	}
	else
		memset(Receive_Buf,0x00,10);
}

//�������״̬����,����0x141(321)
void WARN(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{//�źŻ����ھ������
		WarningErrFlag = 1;  //���ϱ�־��1
//		ErrLEDFLSS();
	}
	else if (Signal == 0x55){//�źŻ������ھ������
		WarningErrFlag = 0; //���ϱ�־��0
		ERRLEDOFF;       //���ϵƹر�
		LEDStatus.Fault = 0;
	}
	else
		memset(Receive_Buf,0x00,10);
}

//���ع���״̬,����0x142(322)
void FATA(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{//�źŻ��������ع���
		SeriousErrFlag = 1;  //���ϱ�־��1
		ERRLEDON;       //���ϵƴ�
		LEDStatus.Fault=1;
	}
	else if (Signal == 0x55){//�źŻ����������ع���
		SeriousErrFlag = 0; //���ϱ�־��0
		ERRLEDOFF;       //���ϵƹر�
		LEDStatus.Fault = 0;
	}
	else
		memset(Receive_Buf,0x00,10);
}

//���ͨ���������״̬,����0x143(323)
void LMPS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA){//�źŻ�ͨ�����Ϊ��״̬
//   ChannelSetFlag=1;//�źŻ�ͨ�������־��1
		TUNONLEDON;//ͨ����ָʾ�Ƶ���
		LEDStatus.Open = 1;
	}
	else if (Signal == 0x55){//�źŻ�ͨ�����Ϊ�ر�״̬
//		ChannelSetFlag=0;//�źŻ�ͨ�������־��0
		TUNONLEDOFF;//ͨ����ָʾ�ƹ���
		LEDStatus.Open = 0;
	}
	else
		memset(Receive_Buf,0x00,10);
}

//����Դ״̬,����0x144(324)
void CSOR(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];
	switch (Signal)
	{
	case 0x01:
		//����������
		ringflag=0;
		TESTON;
		
		//����ָʾ�ƹر�
		CENTREOFF;
		
		MANOFF;
		
		TIMEBASEOFF;
		LEDStatus.Test = 1;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		break;
	case 0x02:
		//���Ŀ��ƣ�SystemControl
		CENTREON;//���Ŀ���ָʾ�ƴ�

		//����ָʾ�ƹر�
		ringflag=0;
		MANOFF;
		TESTOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 1;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x03:
		//���Ĵ�����SystemStandby
		//��ʱ���ź�ָʾ����ʾ

		//����ָʾ�ƹر�
		ringflag=0;
		CENTREOFF;
		MANOFF;
		TESTOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x04:
		//���ݿ��ƣ�BackupMode
		//��ʱ���ź�ָʾ����ʾ

		//����ָʾ�ƹر�
		ringflag=0;
		CENTREOFF;
		MANOFF;
		TESTOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x05:
		//�ֶ����ƣ�Manual
		MANON;//�ֶ�����ָʾ�ƴ�
		ringflag=1;

		//����ָʾ�ƹر�
		CENTREOFF;
		TESTOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 1;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x06:
		//ʱ�����ƣ�Timebase
		TIMEBASEON;//ʱ������ָʾ�ƴ�

		//����ָʾ�ƹر�
		ringflag=0;
		CENTREOFF;
		MANOFF;
		TESTOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 1;
		
		break;
	case 0x07:
		//������Interconnect
		//��ʱ���ź�ָʾ����ʾ

		//����ָʾ�ƹر�
		ringflag=0;
		TESTOFF;//����ң��ָʾ�ƴ�
		CENTREOFF;
		MANOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x08:
		//�������ݣ�InterconnectBackup
		//��ʱ���ź�ָʾ����ʾ

		//����ָʾ�ƹر�
		ringflag=0;
		TESTOFF;//����ң��ָʾ�ƴ�
		CENTREOFF;
		MANOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		
		break;

	default:
		memset(Receive_Buf,0x00,10);
		break;
	}
}

//����״̬,����0x145(325)
void FLSS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];
	switch (Signal){
	case 0x01:
		//����ԭ������
		//��ʱ���ź�ָʾ����ʾ

		//�ر���������
		AUTOFLSSOFF;
		MANFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x02:
		//������״̬
		//�ر���������

		AUTOFLSSOFF;
		MANFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x03:
		//�Զ�����
		AUTOFLSSON;//�Զ���ָʾ�ƴ�

		//����ָʾ�ƹر�
		MANFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 1;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x04:
		//�����ֿ�����
		MANFLSSON;//�ֿ���ָʾ�ƴ�

		//����ָʾ�ƹر�
		AUTOFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 1;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x05:
		//���ϼ�������
//    ERRFLSSON;//������ָʾ�ƴ�

		//����ָʾ�ƹر�
		ERRFLSSOFF;//������ָʾ�ƴ�
		AUTOFLSSOFF;
		MANFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x06:
		//MFU����
		ERRFLSSON;//������ָʾ�ƴ�

//����ָʾ�ƹر�
		MANFLSSOFF;
		AUTOFLSSOFF;
//	  ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 1;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x07:
		//��������
		//��ʱ���ź�ָʾ����ʾ

//����ָʾ�ƹر�
		MANFLSSOFF;
		AUTOFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x08:
		//��������
		ANTCPFLSSON;//������ָʾ�ƴ�

		//����ָʾ�ƹر�
		AUTOFLSSOFF;
		MANFLSSOFF;
		ERRFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 1;
		
		break;

	default:
		memset(Receive_Buf,0x00,10);
		break;
	}
}

//�źŻ�ң��״̬0x146(326)
void REMT(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA){//�źŻ�ң�ع���Ϊ����״̬
		//REMOTEON;       //ң��ָʾ�ƴ�
	}
	else if (Signal == 0x55){//�źŻ�ң�ع���Ϊ�ر�״̬
		//REMOTEOFF;       //ң��ָʾ�ƹر�
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ����ּ�ָʾ״̬0x147(327)
void KNMS(void)
{
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if ((Signal&0x01)==0x01)
	{
		LEDONEON;
		LEDStatus.One = 1;
		
	}
	else
	{
		LEDONEOFF;
		LEDStatus.One = 0;
	}

	if ((Signal&0x02)==0x02)
	{
		
		LEDTWOON;
		LEDStatus.Two = 1;

	}
	else
	{
		LEDTWOOFF;
		LEDStatus.Two = 0;
	}

	if ((Signal&0x04)==0x04)
	{
		LEDTHREEON;
		LEDStatus.Three = 1;
	}
	else{
		LEDTHREEOFF;
		LEDStatus.Three = 0;
	}

	if ((Signal&0x08)==0x08)
	{
		LEDFOURON;
		LEDStatus.Four = 1;
	}
	else
	{
		LEDFOUROFF;
		LEDStatus.Four = 0;
	}

	if ((Signal&0x10)==0x10)
	{
		LEDFIVEON;
		LEDStatus.Five = 1;
	}
	else
	{
		LEDFIVEOFF;
		LEDStatus.Five = 0;
	}

	if ((Signal&0x20)==0x20)
	{
		LEDSIXON;
		LEDStatus.Six = 1;
	}
	else
	{
		LEDSIXOFF;
		LEDStatus.Six = 0;
	}
	if ((Signal&0x40)==0x40)
	{
		LEDSEVENON;
		LEDStatus.Seven = 1;
	}
	else
	{
		LEDSEVENOFF;
		LEDStatus.Seven = 0;
	}

	if ((Signal&0x80)==0x80)
	{
		LEDEIGHTON;
		LEDStatus.Eight = 1;
	}
	else
	{
		LEDEIGHTOFF;
		LEDStatus.Eight = 0;
	}
	GetLedStatus();

	memset(Receive_Buf,0x00,10);
}
//�źŻ�������ָʾ״̬0x148(328)
void KSPS(void)
{
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDSTEPON;
		LEDStatus.Step = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDSTEPOFF;
		LEDStatus.Step = 0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ�ȫ��ָʾ״̬0x149(329)
void KARS(void)
{
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDALLREDON;
		LEDStatus.Allred = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDALLREDOFF;
		LEDStatus.Allred = 0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ�A��ָʾ״̬0x14A(330)
void KLAS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDKEYAON;
		LEDStatus.Key_A = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDKEYAOFF;
		LEDStatus.Key_A = 0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ�B��ָʾ״̬0x14B(331)
void KLBS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDKEYBON;
		LEDStatus.Key_B = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDKEYBOFF;
		LEDStatus.Key_B = 0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ�C��ָʾ״̬0x14C(332)
void KLCS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDKEYCON;
		LEDStatus.Key_C = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDKEYCOFF;
		LEDStatus.Key_C = 0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ�D��ָʾ״̬0x14D(333)
void KLDS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDKEYDON;
		LEDStatus.Key_D = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDKEYDOFF;
		LEDStatus.Key_D = 1;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ�E��ָʾ״̬0x14E(334)
void KLES(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDKEYEON;
		LEDStatus.Key_E = 1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDKEYEOFF;
		LEDStatus.Key_E = 0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}
//�źŻ��Ʋ��Լ�ָʾ״̬0x14F(335)
void KLSS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		LEDTESTON;
		LEDStatus.Test = 1;
		TestFlag=1;
		GetLedStatus();
	}
	else if (Signal == 0x55)
	{
		LEDTESTOFF;
		LEDStatus.Test = 0;
		TestFlag=0;
		GetLedStatus();
	}
	else
		memset(Receive_Buf,0x00,10);
}

//����ָʾ��ȫ��,����0x150(336)
void Test(void)
{
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{
		TestFlag=1;
	}
	else if (Signal == 0x55)
	{
		TestFlag=0;
	}
	else
		memset(Receive_Buf,0x00,10);
}
//״ָ̬ʾ�ƿ���
void LEDALL(uint8_t Mark)
{
	//ָʾ��ȫ����
	if (Mark)
	{
		LEDSTEPON;
		LEDONEON;
		LEDTWOON;
		LEDTHREEON;
		LEDFOURON;
		LEDFIVEON;
		LEDSIXON;
		LEDSEVENON;
		LEDEIGHTON;
		LEDKEYAON;
		LEDKEYBON;
		LEDKEYCON;
		LEDKEYDON;
		LEDKEYEON;
		LEDALLREDON;
		LEDTESTON;


		ERRLEDON;
		TUNONLEDON;
		KEYLOCKON;
		CENTREON;
		MANON;
		TESTON;
		TIMEBASEON;
		AUTOFLSSON;
		MANFLSSON;
		ERRFLSSON;
		ANTCPFLSSON;

		//ringflag=1;
		//SeriousErrFlag=1;
		//passwordFlag=1;

		//ָʾ��ȫ���ر�
	}
	else
	{
		LEDSTEPOFF;
		LEDONEOFF;
		LEDTWOOFF;
		LEDTHREEOFF;
		LEDFOUROFF;
		LEDFIVEOFF;
		LEDSIXOFF;
		LEDSEVENOFF;
		LEDEIGHTOFF;
		LEDKEYAOFF;
		LEDKEYBOFF;
		LEDKEYCOFF;
		LEDKEYDOFF;
		LEDKEYEOFF;
		LEDALLREDOFF;
		LEDTESTOFF;
		
		ERRLEDOFF;
		TUNONLEDOFF;
		KEYLOCKOFF;
		CENTREOFF;
		MANOFF;
		TESTOFF;
		TIMEBASEOFF;
		AUTOFLSSOFF;
		MANFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;

		ringflag=0;
		SeriousErrFlag=0;
		WarningErrFlag=0;
//		passwordFlag=0;
		
	}
}

/************************************************

            MAU��������&����״̬��ȡ

***********************************************/
//��������״̬��ȡ
/*******************************************************************
* ��    �ƣ� GetSwitchStatus
* ��    �ܣ� ��ȡ��������״̬��Ϣ
* ��ڲ����� ��
* ���ڲ����� ��
*******************************************************************/

void GetSwitchMode(void)
{
	SwitchMode[0] = SIGNAL1;//��ƿ��أ�1Ϊ�򿪣�0Ϊ�ر�
	SwitchMode[1] = SIGNAL2;//�ֿؿ���
	SwitchMode[2] = SIGNAL3;//���ƿ���
	SwitchMode[3] = SIGNAL4;//ң�ؿ���

}

//MAU�����źŻ����ص�����
void MLMC(void)
{
	if (SIGNAL1)
	{
		TxBuff[0] = 0xAA;
		//LEDStatus.Open = 1;
	}
	else
	{
		TxBuff[0] = 0x55;
		//LEDStatus.Open = 0;
	}
		

	MAUFunFlag=21;

	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAU�ֿ�/�Զ��л�����
void MMAC(void)
{
	if (SIGNAL2)
	{
		TxBuff[0] = 0xAA;	//�ֿع���ʹ��
		//LEDStatus.Manual = 1;
	}
	else
	{
		TxBuff[0] = 0x55;//�Զ�����ʹ��
		//LEDStatus.Manual = 0;
	}
		

	MAUFunFlag=22;
//  Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAU���ƿ�������
void MFSC(void)
{
	if (SIGNAL3)
	{
		TxBuff[0] = 0xAA;	//���ƹ���ʹ��
	}
	else
		TxBuff[0] = 0x55;//

	MAUFunFlag=23;
//  Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAUң�ع���ʹ������
void MRTC(void)
{
	if (SIGNAL4)
	{
		TxBuff[0] = 0xAA;	//ң�ع���ʹ��
	}
	else
		TxBuff[0] = 0x55;//

	MAUFunFlag=24;
//	Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAU��Ӳ���汾��Ϣ
void MAVS(void)
{
	TxBuff[0] = (Versions & 0xff);	//ӡ�ư�汾
	TxBuff[0] = ((Versions>>8) & 0xff);	//�̼��汾
	MAUFunFlag=26;
//	Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,2);
}

//MAU������������
void MKYC(uint8_t Key)
{
//	Key = Read_KeyValue();
	switch (Key)
	{
	case 0:
		TxBuff[0] = 0x00;//����
		break;
	case 1:
		TxBuff[0] = 0x01;//����1
		break;
	case 2:
		TxBuff[0] = 0x02;//����2
		break;
	case 3:
		TxBuff[0] = 0x03;//����3
		break;
	case 4:
		TxBuff[0] = 0x09;//ȫ��
		break;
	case 5:
		TxBuff[0] = 0x04;//����4
		break;
	case 6:
		TxBuff[0] = 0x05;//����5
		break;
	case 7:
		TxBuff[0] = 0x06;//����6
		break;
	case 8:
		TxBuff[0] = 0x0A;// A
		break;
	case 9:
		TxBuff[0] = 0x07;//����7
		break;
	case 10:
		TxBuff[0] = 0x08;//����8
		break;
	case 11:
		TxBuff[0] = 0x0F;//�Ʋ���
		TestFlag = 1;
		break;
	case 12:
		TxBuff[0] = 0x0B;// B
		break;
	case 13:
		TxBuff[0] = 0x0C;// C
		break;
	case 14:
		TxBuff[0] = 0x0D;// D
		break;
	case 15:
		TxBuff[0] = 0x0E;// E
		break;
	case 16:
		TxBuff[0] = 0x10;//�����ͷ�
		break;

	default:
		memset(Receive_Buf,0x00,10);
		break;
	}

	MAUFunFlag=25;

	Send_CAN_DataFrame_Single(TxBuff,1);
}



//MAU�������״̬����
void MAUS(void)
{
	if (passwordFlag)
	{
		TxBuff[0] = 0x01;	//MAU���������뱣��״̬
	}
	else
		TxBuff[0] = 0x00;//MAU���������뱣��״̬

	MAUFunFlag=27;
//	Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}


void flashstor(uint8_t value)
{
	FLASH_Unlock();//����

	FLASH_ErasePage(FLASHPASSWORD);	//ɾ
//	FLASH_ProgramOptionByteData(FLASHPASSWORD, value);//д
	FLASH_ProgramHalfWord(FLASHPASSWORD,value);
//	Delay_ms(10);

	FLASH_Lock();//��
//	ReadFlashNBtye(FLASHPASSWORD,Address,1);
//	value = *passwordAddress;//������
	flashvalue = *(uint32_t*)FLASHPASSWORD;
//	Delay_ms(10);
}
/*************************************************************
���ڲ�flashָ����ַ��N���ֽ�;
ReadAddress:ָ����ַReadBuf:���ݴ洢ָ��ReadNum:��ȡ�ֽ���
************************************************************/

void ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int8_t ReadNum){
	int DataNum = 0;

//	ReadAddress = (uint32_t)FLASHPASSWORD + ReadAddress;
	while (DataNum < ReadNum){
		*(ReadBuf + DataNum) = *(__IO uint8_t*) ReadAddress++;
		DataNum++;
	}
// return DataNum;
}

//�������򿪣�ÿ3s����һ����ʾ����Ƶ��18KHz����0.056ms(T=1/f)
/***************************************************
            			_   _
                 | | | |
         ________| |_| |_______
****************************************************/
void ring(void)
{
	uint32_t Ring_num=0;
	for (Ring_num=0;Ring_num<400;Ring_num++)
	{
		if (Ring_num<200)
			BEEPON;
		else
			BEEPOFF;
	}
}
/*------------------------------�������ɨ��--------------------------------------------
*  ����:  ɨ�������̣������ؼ�ֵ
*  ����:
*  ����:  ��
*  ����:  �м����·��ظü�ֵ
*         �޼�����ʱ�򷵻�0
--------------------------------------------------------------------------------------*/
/*
*                                 //   |COL4 | COL3 | COL2 | COL1 | ROW4 | ROW3 | ROW2 | ROW1 |
*                                 //-------------------------------------------------------------
*                                 // ��|  0  |  0   |   0  |  0   |  1   |   1  |   1  |   1  |
*                                 //-------------------------------------------------------------
*                                 // S0|  1  |  1   |   1  |  0   |  1   |   1  |   1  |   0  |
*                                 //-------------------------------------------------------------
*                                 // S1|  1  |  1   |   0  |  1   |  1   |   1  |   1  |   0  |
*                                 //-------------------------------------------------------------
*                                 // S2|  1  |  0   |   1  |  1   |  1   |   1  |   1  |   0  |
*                                 //-------------------------------------------------------------
*                                 // S3|  0  |  1   |   1  |  1   |  1   |   1  |   1  |   0  |
*                                 //-------------------------------------------------------------
*                                 // S4|  1  |  1   |   1  |  0   |  1   |   1  |   0  |   1  |
*-------------------------------- //-------------------------------------------------------------
* |  S0 |  S1 |  S2 |  S3  |      // S5|  1  |  1   |   0  |  1   |  1   |   1  |   0  |   1  |
*-------------------------------- //-------------------------------------------------------------
* |  S4 |  S5 |  S6 |  S7  |      // S6|  1  |  0   |   1  |  1   |  1   |   1  |   0  |   1  |
*-------------------------------- //-------------------------------------------------------------
* |  S8 |  S9 | S10 | S11  |      // S7|  0  |  1   |   1  |  1   |  1   |   1  |   0  |   1  |
*-------------------------------- //-------------------------------------------------------------
* | S12 | S13 | S14 | S15  |      // S8|  1  |  1   |   1  |  0   |  1   |   0  |   1  |   1  |
*-------------------------------- //-------------------------------------------------------------
*                                 // S9|  1  |  1   |   0  |  1   |  1   |   0  |   1  |   1  |
*                                 //------------------------------------------------------------- 
*                                // S10|  1  |  0   |   1  |  1   |  1   |   0  |   1  |   1  |
*                                //-------------------------------------------------------------
*                                // S11|  0  |  1   |   1  |  1   |  1   |   0  |   1  |   1  |
*                                //-------------------------------------------------------------
*                                // S12|  1  |  1   |   1  |  0   |  0   |   1  |   1  |   1  |
*                                //-------------------------------------------------------------
*                                // S13|  1  |  1   |   0  |  1   |  0   |   1  |   1  |   1  |
*                                //-------------------------------------------------------------
*                                // S14|  1  |  0   |   1  |  1   |  0   |   1  |   1  |   1  |
*                                //-------------------------------------------------------------
*                                // S15|  0  |  1   |   1  |  1   |  0   |   1  |   1  |   1  |
*                                //-------------------------------------------------------------*/
void ReadKeyboard(void){
	KEYx=0;

	keylin[0] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);//ROW1���ݲɼ���Ĭ���ø�����
	keylin[1] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
	keylin[2] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);
	keylin[3] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
	keylin[4] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);//COL1���ݲɼ�,Ĭ���������
	keylin[5] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_8);
	keylin[6] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_9);
	keylin[7] = GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_10);


	KEYx  = keylin[0];
	KEYx |= keylin[1]<<1;
	KEYx |= keylin[2]<<2;
	KEYx |= keylin[3]<<3;
	KEYx |= keylin[4]<<4;
	KEYx |= keylin[5]<<5;
	KEYx |= keylin[6]<<6;
	KEYx |= keylin[7]<<7;
}

uint8_t Read_KeyValue(void){
	uint8_t KeyValue=0;
	
	ReadKeyboard();

	if ((KEYx & 0xff)!= 0x0f)
	{
		GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		ReadKeyboard();
		switch (KEYx & 0xff)
		{
			case 0xEE:
				KeyValue=16;
				break;
			case 0xED:
				KeyValue=15;
				break;
			case 0xEB:
				KeyValue=14;
				break;
			case 0xE7:
				KeyValue=13;
				break;
		}
		GPIO_SetBits(GPIOA,GPIO_Pin_9|GPIO_Pin_10);
		GPIO_SetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8);
		ReadKeyboard();
		switch (KEYx & 0xff){
		case 0xDE:
			KeyValue=12;
			break;
		case 0xDD:
			KeyValue=11;
			break;
		case 0xDB:
			KeyValue=10;
			break;
		case 0xD7:
			KeyValue=9;
			break;
		}
		GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_10);
		GPIO_SetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_9);
		ReadKeyboard();
		switch (KEYx & 0xff){
		case 0xBE:
			KeyValue=8;
			break;
		case 0xBD:
			KeyValue=7;
			break;
		case 0xBB:
			KeyValue=6;
			break;
		case 0xB7:
			KeyValue=5;
			break;
		}
		GPIO_SetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9);
		GPIO_SetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_10);
		ReadKeyboard();
		switch (KEYx & 0xff)
		{
			case 0x7E:
				KeyValue=4;
				break;
			case 0x7D:
				KeyValue=3;
				break;
			case 0x7B:
				KeyValue=2;
				break;
			case 0x77:
				KeyValue=1;
				break;
//		    default:
//					KeyValue=16;
//		      break;
		}

//			return KeyValue;
		GPIO_SetBits(GPIOB,GPIO_Pin_15);
		GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);//ROW1���ݲɼ���Ĭ���ø�����

		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);

//			return KeyValue;
	}
	return KeyValue;
}

