
#ifndef OMNILIB_H_
#define OMNILIB_H_

#include <avr/io.h>
#include <avr/interrupt.h>

#ifndef F_CPU
//#warning "F_CPU not defined for <uartLib.h>"
#define F_CPU 16000000UL
#endif

#define D0 0
#define D1 1
#define D2 2
#define D3 3
#define D4 4
#define D5 5
#define D6 6
#define D7 7

#define B0 8
#define B1 9
#define B2 10
#define B3 11
#define B4 12
#define B5 13

#define C0 14
#define C1 15 
#define C2 16
#define C3 17
#define C4 18
#define C5 19

#define ANODE 1
#define CATHODE 0

void initUART(uint16_t baud, uint8_t stopBits);
void charTX(unsigned char c);
char charRX();
void strTX(const char *str);
void numTX(uint16_t num);
void init_timer();
void delay(uint16_t ms);
void delayF(uint32_t ms);
uint16_t power(int x, int n);

void initSevenSeg(int a, int b, int c, int d, int e, int f, int g, int type_);
void writeNum(int num);
void turnOn(int LED);
void turnAllOff();
void setHigh(int LED);
void setLow(int LED);

int type;
int delayCount;
int pinArray[7];

#endif