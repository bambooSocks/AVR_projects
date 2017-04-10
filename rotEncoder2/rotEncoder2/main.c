/*
 * rotEncoder2.c
 *
 * Created: 3/26/2017 11:34:48 PM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "omniLib.h"

int main(void)
{
    DDRB |= (1 << DDB5);
    DDRC &= ~(1 << DDC0) & ~(1 << DDC1);
    PCICR |= (1 << PCIE1);
    PCMSK1 |= (1 << PCINT8);
    initUART(9600,1);
    sei();
    while (1) 
    {
        
    }
}

ISR(PCINT1_vect)
{
    if((PINC & (1 << PINC0)) == 0){
        if (PINC & (1 << PINC1)){ //CW    
            charTX('-');
        } else { //CCW
            charTX('+');
        }
    }
}

