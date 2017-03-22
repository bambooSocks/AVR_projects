/*
 * USART_A_Z_100ms.c
 *
 * Created: 3/16/2017 9:12:28 AM
 * Author : Matej Majtan
 */ 

 #define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void charTX(char c);

int main(void)
{
    UBRR0 = 103; // setting baud rate to 9600
    UCSR0B |= (1 << TXEN0); // enabling Transmit
    UCSR0C |= (1 << USBS0); // setting stop bits to 2
    while (1) 
    {
        char c;
        for (c = 'A'; c <= 'Z'; c++){
            charTX(c);// wait until the data is sent
            //_delay_ms(100);
        }
        charTX(10);
        charTX(13);
        //_delay_ms(100);
    }
}

void charTX(char c){
    UDR0 = c; // send new line to buffer
    //while ((UCSR0A & (1 << UDRE0)) == 0);
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}



