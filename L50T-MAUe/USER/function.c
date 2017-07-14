#include "global.h"

#define ON         1          		  //点亮指示灯
#define OFF        0			        	//熄灭指示灯

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
             CPU获取信息解析
***********************************************/
//介入密码及使能命令,命令0x0B(11)
void PASC(void)
{
	uint8_t i;
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];
	i=(Signal & 0X0F);

	if ((password & 0X10) == 0x10)
	{//健锁已使能
		off_passwordFlag=1;
		if ((Signal | 0X0F) == 0x1F )
		{
			if (i != (password & 0x0F))
			{//密码变更
				passwordchangeFlag=1;//密码改变,标志置1
				password = (i | 0X10);//更新密码
//			  flashstor(password);//密码存入Flash
			}
			else 	passwordchangeFlag=0;//密码不变,标志置0
		}
		else if ((Signal | 0X0F) == 0x0F )
		{//除能MAU介入密码
			passwordFlag = 0;//MAU介入密码标志置0
			KEYLOCKOFF;
			LEDStatus.Lock = 0;
			password=(Signal & 0X0F);
			passwordchangeFlag=1;//密码改变,标志置1
			MAUS();
		}
	}
	else if ((password & 0X10) == 0x10)
	{
		if ((Signal | 0X0F) == 0x1F )
		{		//使能MAU介入密码
			passwordFlag=1;
			KEYLOCKON;
			LEDStatus.Lock = 1;
			MAUS();
			password = (i | 0X10);//使能介入密码标志存入Flash
			passwordchangeFlag=1;//密码改变,标志置1
//			flashstor(password);//密码存入Flash
		}
		else if ((Signal | 0X0F) == 0x0F )
		{
			//除能MAU介入密码
			passwordFlag = 0;//MAU介入密码标志置0
		}
	}
	else
		memset(Receive_Buf,0x00,10);
}

//警告故障状态命令,命令0x141(321)
void WARN(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{//信号机存在警告故障
		WarningErrFlag = 1;  //故障标志设1
//		ErrLEDFLSS();
	}
	else if (Signal == 0x55){//信号机不存在警告故障
		WarningErrFlag = 0; //故障标志设0
		ERRLEDOFF;       //故障灯关闭
		LEDStatus.Fault = 0;
	}
	else
		memset(Receive_Buf,0x00,10);
}

//严重故障状态,命令0x142(322)
void FATA(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA)
	{//信号机存在严重故障
		SeriousErrFlag = 1;  //故障标志设1
		ERRLEDON;       //故障灯打开
		LEDStatus.Fault=1;
	}
	else if (Signal == 0x55){//信号机不存在严重故障
		SeriousErrFlag = 0; //故障标志设0
		ERRLEDOFF;       //故障灯关闭
		LEDStatus.Fault = 0;
	}
	else
		memset(Receive_Buf,0x00,10);
}

//外灯通道输出开关状态,命令0x143(323)
void LMPS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA){//信号机通道输出为打开状态
//   ChannelSetFlag=1;//信号机通道输出标志设1
		TUNONLEDON;//通道打开指示灯点亮
		LEDStatus.Open = 1;
	}
	else if (Signal == 0x55){//信号机通道输出为关闭状态
//		ChannelSetFlag=0;//信号机通道输出标志设0
		TUNONLEDOFF;//通道打开指示灯关灭
		LEDStatus.Open = 0;
	}
	else
		memset(Receive_Buf,0x00,10);
}

//控制源状态,命令0x144(324)
void CSOR(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];
	switch (Signal)
	{
	case 0x01:
		//其它，测试
		ringflag=0;
		TESTON;
		
		//其它指示灯关闭
		CENTREOFF;
		
		MANOFF;
		
		TIMEBASEOFF;
		LEDStatus.Test = 1;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		break;
	case 0x02:
		//中心控制，SystemControl
		CENTREON;//中心控制指示灯打开

		//其它指示灯关闭
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
		//中心待命，SystemStandby
		//暂时无信号指示灯显示

		//其它指示灯关闭
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
		//备份控制，BackupMode
		//暂时无信号指示灯显示

		//其它指示灯关闭
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
		//手动控制，Manual
		MANON;//手动控制指示灯打开
		ringflag=1;

		//其它指示灯关闭
		CENTREOFF;
		TESTOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 1;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x06:
		//时基控制，Timebase
		TIMEBASEON;//时基控制指示灯打开

		//其它指示灯关闭
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
		//互联，Interconnect
		//暂时无信号指示灯显示

		//其它指示灯关闭
		ringflag=0;
		TESTOFF;//无线遥控指示灯打开
		CENTREOFF;
		MANOFF;
		TIMEBASEOFF;
		
		LEDStatus.Test = 0;
		LEDStatus.Center = 0;
		LEDStatus.Manual = 0;
		LEDStatus.Timebase = 0;
		
		break;
	case 0x08:
		//互联备份，InterconnectBackup
		//暂时无信号指示灯显示

		//其它指示灯关闭
		ringflag=0;
		TESTOFF;//无线遥控指示灯打开
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

//闪灯状态,命令0x145(325)
void FLSS(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];
	switch (Signal){
	case 0x01:
		//其他原因闪灯
		//暂时无信号指示灯显示

		//关闭所有闪灯
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
		//非闪灯状态
		//关闭所有闪灯

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
		//自动闪灯
		AUTOFLSSON;//自动闪指示灯打开

		//其它指示灯关闭
		MANFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 1;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x04:
		//本地手控闪灯
		MANFLSSON;//手控闪指示灯打开

		//其它指示灯关闭
		AUTOFLSSOFF;
		ERRFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 1;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x05:
		//故障监视闪灯
//    ERRFLSSON;//故障闪指示灯打开

		//其它指示灯关闭
		ERRFLSSOFF;//故障闪指示灯打开
		AUTOFLSSOFF;
		MANFLSSOFF;
		ANTCPFLSSOFF;
		
		LEDStatus.A_Flash = 0;
		LEDStatus.M_Flash = 0;
		LEDStatus.F_Flash = 0;
		LEDStatus.H_Flash = 0;
		
		break;
	case 0x06:
		//MFU闪灯
		ERRFLSSON;//故障闪指示灯打开

//其它指示灯关闭
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
		//启动闪灯
		//暂时无信号指示灯显示

//其它指示灯关闭
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
		//抢先闪灯
		ANTCPFLSSON;//抢先闪指示灯打开

		//其它指示灯关闭
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

//信号机遥控状态0x146(326)
void REMT(void){
	uint8_t Signal = 0x00;
	Signal = Receive_Buf[3];

	if (Signal == 0xAA){//信号机遥控工作为激活状态
		//REMOTEON;       //遥控指示灯打开
	}
	else if (Signal == 0x55){//信号机遥控工作为关闭状态
		//REMOTEOFF;       //遥控指示灯关闭
	}
	else
		memset(Receive_Buf,0x00,10);
}
//信号机数字键指示状态0x147(327)
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
//信号机步进键指示状态0x148(328)
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
//信号机全红指示状态0x149(329)
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
//信号机A键指示状态0x14A(330)
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
//信号机B键指示状态0x14B(331)
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
//信号机C键指示状态0x14C(332)
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
//信号机D键指示状态0x14D(333)
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
//信号机E键指示状态0x14E(334)
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
//信号机灯测试键指示状态0x14F(335)
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

//键盘指示灯全控,命令0x150(336)
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
//状态指示灯开关
void LEDALL(uint8_t Mark)
{
	//指示灯全部打开
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

		//指示灯全部关闭
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

            MAU波动开关&按键状态读取

***********************************************/
//拨动开关状态读取
/*******************************************************************
* 名    称： GetSwitchStatus
* 功    能： 获取拨动开关状态信息
* 入口参数： 无
* 出口参数： 无
*******************************************************************/

void GetSwitchMode(void)
{
	SwitchMode[0] = SIGNAL1;//外灯开关，1为打开，0为关闭
	SwitchMode[1] = SIGNAL2;//手控开关
	SwitchMode[2] = SIGNAL3;//闪灯开关
	SwitchMode[3] = SIGNAL4;//遥控开关

}

//MAU控制信号机开关灯命令
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

//MAU手控/自动切换命令
void MMAC(void)
{
	if (SIGNAL2)
	{
		TxBuff[0] = 0xAA;	//手控功能使能
		//LEDStatus.Manual = 1;
	}
	else
	{
		TxBuff[0] = 0x55;//自动功能使能
		//LEDStatus.Manual = 0;
	}
		

	MAUFunFlag=22;
//  Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAU闪灯控制命令
void MFSC(void)
{
	if (SIGNAL3)
	{
		TxBuff[0] = 0xAA;	//闪灯功能使能
	}
	else
		TxBuff[0] = 0x55;//

	MAUFunFlag=23;
//  Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAU遥控功能使能命令
void MRTC(void)
{
	if (SIGNAL4)
	{
		TxBuff[0] = 0xAA;	//遥控功能使能
	}
	else
		TxBuff[0] = 0x55;//

	MAUFunFlag=24;
//	Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}

//MAU软硬件版本信息
void MAVS(void)
{
	TxBuff[0] = (Versions & 0xff);	//印制板版本
	TxBuff[0] = ((Versions>>8) & 0xff);	//固件版本
	MAUFunFlag=26;
//	Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,2);
}

//MAU按键控制命令
void MKYC(uint8_t Key)
{
//	Key = Read_KeyValue();
	switch (Key)
	{
	case 0:
		TxBuff[0] = 0x00;//步进
		break;
	case 1:
		TxBuff[0] = 0x01;//数字1
		break;
	case 2:
		TxBuff[0] = 0x02;//数字2
		break;
	case 3:
		TxBuff[0] = 0x03;//数字3
		break;
	case 4:
		TxBuff[0] = 0x09;//全红
		break;
	case 5:
		TxBuff[0] = 0x04;//数字4
		break;
	case 6:
		TxBuff[0] = 0x05;//数字5
		break;
	case 7:
		TxBuff[0] = 0x06;//数字6
		break;
	case 8:
		TxBuff[0] = 0x0A;// A
		break;
	case 9:
		TxBuff[0] = 0x07;//数字7
		break;
	case 10:
		TxBuff[0] = 0x08;//数字8
		break;
	case 11:
		TxBuff[0] = 0x0F;//灯测试
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
		TxBuff[0] = 0x10;//按键释放
		break;

	default:
		memset(Receive_Buf,0x00,10);
		break;
	}

	MAUFunFlag=25;

	Send_CAN_DataFrame_Single(TxBuff,1);
}



//MAU密码介入状态命令
void MAUS(void)
{
	if (passwordFlag)
	{
		TxBuff[0] = 0x01;	//MAU处于有密码保护状态
	}
	else
		TxBuff[0] = 0x00;//MAU处于无密码保护状态

	MAUFunFlag=27;
//	Send_CAN_DataFrame_MAU(TxBuff);
	Send_CAN_DataFrame_Single(TxBuff,1);
}


void flashstor(uint8_t value)
{
	FLASH_Unlock();//解锁

	FLASH_ErasePage(FLASHPASSWORD);	//删
//	FLASH_ProgramOptionByteData(FLASHPASSWORD, value);//写
	FLASH_ProgramHalfWord(FLASHPASSWORD,value);
//	Delay_ms(10);

	FLASH_Lock();//锁
//	ReadFlashNBtye(FLASHPASSWORD,Address,1);
//	value = *passwordAddress;//读密码
	flashvalue = *(uint32_t*)FLASHPASSWORD;
//	Delay_ms(10);
}
/*************************************************************
读内部flash指定地址的N个字节;
ReadAddress:指定地址ReadBuf:数据存储指针ReadNum:读取字节数
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

//蜂鸣器打开，每3s发出一声提示音，频率18KHz周期0.056ms(T=1/f)
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
/*------------------------------矩阵键盘扫描--------------------------------------------
*  功能:  扫描矩阵键盘，并返回键值
*  参数:
*  输入:  无
*  返回:  有键按下返回该键值
*         无键按下时则返回0
--------------------------------------------------------------------------------------*/
/*
*                                 //   |COL4 | COL3 | COL2 | COL1 | ROW4 | ROW3 | ROW2 | ROW1 |
*                                 //-------------------------------------------------------------
*                                 // 无|  0  |  0   |   0  |  0   |  1   |   1  |   1  |   1  |
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

	keylin[0] = GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_15);//ROW1数据采集，默认置高输入
	keylin[1] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_6);
	keylin[2] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_7);
	keylin[3] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_8);
	keylin[4] = GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_9);//COL1数据采集,默认拉低输出
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
		GPIO_SetBits(GPIOC,GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8);//ROW1数据采集，默认置高输入

		GPIO_ResetBits(GPIOC,GPIO_Pin_9);
		GPIO_ResetBits(GPIOA,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);

//			return KeyValue;
	}
	return KeyValue;
}

