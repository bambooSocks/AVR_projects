/*
 * start_seconds_UART.c
 *
 * Created: 3/16/2017 10:44:05 AM
 * Author : Matej Majtan
 */ 

#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>

void uartInit();
void uartTransmitChar(char data);
void toString(char c[], uint16_t num);

int main(void)
{
    uartInit();
    char str[5] = {0};
    uint16_t sec = 0;
    char msg[33] = "Seconds since the program start: ";
    uint16_t arrL = sizeof(msg)/sizeof(msg[0]);
    while (1) 
    {
        _delay_ms(1000);
        sec++;
        for (int i = 0; i < arrL; i++){
            uartTransmitChar(msg[i]);
        }
        toString(str,sec);
        for (int i = 0; i < 5; i++){
            uartTransmitChar(str[i]);
        }
            uartTransmitChar(10);
            uartTransmitChar(13);
    }
}

void uartInit(){
    UBRR0 = 103; // setting baud rate to 9600
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enabling transmit
    UCSR0C |= (1 << USBS0); // setting 2 stop bits
}

void uartTransmitChar(char data){
    UDR0 = data; // send new line to buffer
    //while ((UCSR0A & (1 << UDRE0)) == 0);
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}

void toString(char c[], uint16_t num){
    int i, rem, len = 0, n;
    
    n = num;
    while (n != 0)
    {
        len++;
        n /= 10;
    }
    for (i = 0; i < len; i++)
    {
        rem = num % 10;
        num = num / 10;
        c[len - (i + 1)] = rem + '0';
    }
    c[len] = '\0';
}