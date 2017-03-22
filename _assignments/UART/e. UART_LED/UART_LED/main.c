/*
 * UART_LED.c
 *
 * Created: 3/18/2017 1:38:40 AM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>

unsigned char charRX();
void charTX(char c);
void initUART();

int main(void)
{
    DDRB |= 0x07; // setting b0-3 as output
    PORTB &= 0xF8; // turning leds off
    initUART();
    while (1) 
    {
        char c = charRX();
        switch (c){
        case 'R':
        case 'r':
            PORTB &= 0xF8;
            PORTB |= 0x01;
            break;
        case 'G':
        case 'g':
            PORTB &= 0xF8;
            PORTB |= 0x02;
            break;
        case 'Y':
        case 'y':
            PORTB &= 0xF8;
            PORTB |= 0x04;
            break;
        case 'A':
        case 'a':
            PORTB &= 0xF8;
            PORTB |= 0x07;
            break;
        }
    }
}

unsigned char charRX(){
    while ((UCSR0A & (1<<RXC0))== 0); // wait for data do get to buffer
    return UDR0; // return data from buffer
}

void charTX(char c){
    UDR0 = c; // send new line to buffer
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}

void initUART(){
    UBRR0 = 103; // setting baud rate to 9600
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enabling Transmit and receive
    UCSR0C |= (1 << USBS0); // setting stop bits to 2
}