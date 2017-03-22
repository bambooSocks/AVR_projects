/*
 * ArduinoTest.c
 *
 * Created: 3/7/2017 9:18:49 AM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>

void delay(unsigned long ms);

int main(void)
{
    DDRB |= (1<< DDB5); // setting the B5 to be output
    PORTB &= ~(1 << PORTB4); //
    //PORTB |= (1 << PORTB4); // setting pull up resistor on B4
    DDRB &= ~(1<< DDB4); // setting B4 to input

    PORTB &= ~(1<<PORTB5); // setting the LED to be off

    while (1) 
    {
        if ((PINB & (1 << PINB4)) == 0){ // is input low
            PORTB |= (1 << PORTB5); // set LED on
        } else {
            PORTB &= ~(1 << PORTB5); // set LED off
        }
    }
}

void delay(unsigned long ms){
    
    for (unsigned long i=0;i <(ms*471);i++);
}
