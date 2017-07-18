SET PATH=D:\Keil_v5\ARM\ARMCC\Bin;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;C:\Program Files (x86)\Subversion\bin;C:\Program Files\TortoiseSVN\bin;C:\Program Files\Intel\WiFi\bin\;C:\Program Files\Common Files\Intel\WirelessCommon\;%USERPROFILE%\AppData\Local\Microsoft\WindowsApps;
SET CPU_TYPE=STM32F103R8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=L50T-PDU
SET CPU_CLOCK=0x00B71B00
"D:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via ".\output\startup_stm32f10x_md._ia"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\main.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_it.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\function.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\delay.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\misc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_gpio.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_iwdg.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_pwr.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_rcc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_adc.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_can.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_dma.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_tim.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_flash.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\stm32f10x_exti.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\core_cm3.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"D:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via ".\Output\L50T-PDU.lnp"
"D:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" ".\Output\L50T-PDU.axf" --i32combined --output ".\Output\L50T-PDU.hex"
