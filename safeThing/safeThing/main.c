/*
 * safeThing.c
 *
 * Created: 3/29/2017 1:06:29 PM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>


int main(void)
{
    DDRB |= (1 << DDB5);
    PORTB |= (1 << PORTB5);
    while (1) 
    {
    }
}

