/*
 * testUARTLib.c
 *
 * Created: 3/22/2017 11:06:34 AM
 * Author : Matej Majtan
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include "uartLib.h"

int main(void){

    initSevenSeg(B5,B4,B3,B2,B1,B0,C0,ANODE);
    init_timer();
    while (1) 
    {
        writeNum(10);
        delay(1000);
    }
}

