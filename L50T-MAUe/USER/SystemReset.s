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
;*  MSR FAULTMASK,R0      //清除FAULMASK禁止一切中断产生
;*  LDR R0,=0xE000ED0C
;*	LDR R1,=0x05FA0004
;*	STR R1,[R0]           //系统软件复位
;*	
;*	deadloop
;*	  B deadloop          //死循环使程序运行不到下面的代码
;*}
;*

    AREA ASMTEST,CODE,READONLY;
    THUMB ;
    PRESERVE8 ;
    export SystemReset ;
SystemReset
    MOV R0,#1  
    MSR FAULTMASK,R0      ;//清除FAULMASK禁止一切中断产生,faultmask是特殊功能寄存器,不需要定义直接用和R0一样
    LDR R0,=0xE000ED0C
	LDR R1,=0x05FA0004
	STR R1,[R0]           ;//系统软件复位
	
deadloop
	B deadloop          ;//死循环使程序运行不到下面的代码
      
	  END
