#ifndef __function_H
#define __function_H

#include "global.h"

/////
//////

void PASC(void);
void WARN(void);
void FATA(void);
void LMPS(void);
void CSOR(void);
void FLSS(void);
void REMT(void);
void KNMS(void);
void KSPS(void);
void KARS(void);
void KLAS(void);
void KLBS(void);
void KLCS(void);
void KLDS(void);
void KLES(void);
void KLSS(void);

void Test(void);
void DRnC(void);
void LEDALL(uint8_t N);

void GetSwitchMode(void);
void MLMC(void);
void MMAC(void);
void MFSC(void);
void MRTC(void);
uint8_t Read_KeyValue(void);
void MKYC(uint8_t Key);
void REMT(void);
void MAVS(void);


void MAUS(void);

void ring(void);
//void KeyLEDFLSS(uint8_t Numb);

void LEDAlloff(void);
void LEDAllon(void);
void flashstor(uint8_t value);
void ReadFlashNBtye(uint32_t ReadAddress, uint8_t *ReadBuf, int8_t ReadNum);
void ReadKeyboard(void);

#endif
