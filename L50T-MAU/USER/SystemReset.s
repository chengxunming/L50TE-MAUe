;*
;*
;*/* Private define ------------------------------------------------------------*/
;*/********************************************************************************
;*Function Name : SystemReset
;*Description : Configures the port pin connected to the push button.GPIO_D_4
;*Input : None
;*Output : None
;*Return : None
;********************************************************************************/
;*_asm void SystemReset(void)
;*{
;*  MOV R0,#1  //
;*  MSR FAULTMASK,R0      //���FAULMASK��ֹһ���жϲ���
;*  LDR R0,=0xE000ED0C
;*	LDR R1,=0x05FA0004
;*	STR R1,[R0]           //ϵͳ�����λ
;*	
;*	deadloop
;*	  B deadloop          //��ѭ��ʹ�������в�������Ĵ���
;*}
;*

    AREA ASMTEST,CODE,READONLY;
    THUMB ;
    PRESERVE8 ;
    export SystemReset ;
SystemReset
    MOV R0,#1  
    MSR FAULTMASK,R0      ;//���FAULMASK��ֹһ���жϲ���,faultmask�����⹦�ܼĴ���,����Ҫ����ֱ���ú�R0һ��
    LDR R0,=0xE000ED0C
	LDR R1,=0x05FA0004
	STR R1,[R0]           ;//ϵͳ�����λ
	
deadloop
	B deadloop          ;//��ѭ��ʹ�������в�������Ĵ���
      
	  END
