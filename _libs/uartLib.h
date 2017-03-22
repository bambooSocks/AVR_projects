#ifndef UARTLIB_H_
#define UARTLIB_H_

#include <avr/io.h>

#define NONE 0
#define ODD 1
#define EVEN 2

#ifndef F_CPU
#warning "F_CPU not defined for <uartLib.h>"
#define F_CPU 16000000UL
#endif

void initUART(uint16_t baud, uint8_t stopBits, uint8_t parity);
void charTX(char c);
char charRX();
void strTX(const char *str);
void numTX(uint16_t num);

#endif