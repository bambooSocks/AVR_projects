/*
 * delay.c
 *
 * Created: 3/16/2017 1:02:13 PM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
int delayCount = 0;

void init_timer();
void delay(uint16_t ms);

int main(void)
{
    init_timer();
    DDRB = (1 << DDB0);
    while (1) 
    {
        PORTB ^= (1 << PORTB0);
        delay(1000);
    }
}



void init_timer(){
    TCCR0A |= (1 << WGM01); // CTC mode
    OCR0A = 250; // 250 ticks = 1 ms
    TIMSK0 |= (1 << OCIE0A); // setting the output compare interrupt
    sei(); // setting external interrupt
    TCCR0B |= (1 << CS00) | (1 << CS01); // starts up the timer with prescaler of 64
}

ISR(TIMER0_COMPA_vect){
    delayCount++;
}


void delay(uint16_t ms){
    delayCount = 0;
    while (delayCount<ms);
}