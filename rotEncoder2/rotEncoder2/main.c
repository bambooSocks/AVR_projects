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
    DDRD &= ~(1 << DDD2) & ~(1 << DDD3);
    PORTD |= (1 << PORTD2) | (1 << PORTD3); 
    EICRA |= (1 << ISC11) /*| (1 << ISC10)*/ | (1 << ISC01);
    EIMSK |= (1 << INT1) | (1 << INT0);
    sei();
    initUART(9600,1);
    while (1) 
    {
        
    }
}

ISR(INT0_vect )
{
    if((PIND & PIND3) != 0)
    {
        charTX('+');
    }else{
        charTX('-');
    }
}

ISR(INT1_vect)
{
    if((PIND & PIND2) != 0)
    {
        charTX('+');
    }else{
        charTX('-');
    }
}
