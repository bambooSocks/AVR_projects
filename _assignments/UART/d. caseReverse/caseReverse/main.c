/*
 * caseReverse.c
 *
 * Created: 3/18/2017 12:42:28 AM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>

unsigned char charRX();
void charTX(char c);
void initUART();

int main(void)
{
    initUART();
    char a;
    while (1) 
    {
        a = charRX();
        if ('a'<=a && a<='z'){ // a is lowercase
            a-=32;
        } else if ('A'<=a && a<='Z'){ // a is uppercase
            a+=32;
        }
       charTX(a);
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
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enabling Transmit
    UCSR0C |= (1 << USBS0); // setting stop bits to 2
}
