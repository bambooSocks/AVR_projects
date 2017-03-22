#include "uartLib.h"

void initUART(uint16_t baud, uint8_t stopBits, uint8_t parity){ // example initUART(9600, 1, NONE);
    UBRR0 = ((((F_CPU / 16) + (baud / 2)) / baud) - 1); // setting baud rate to entered value
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enabling Transmit and Receive
    switch (parity){
        case 0: //none parity
            UCSR0C &= ~(1 << UPM00) &  ~(1 << UPM01); //NONE
        case 1: //odd parity
            UCSR0C |= (1 << UPM00) &  (1 << UPM01); //ODD
        case 2: //even parity
            UCSR0C &= ~(1 << UPM00); //EVEN
            UCSR0C |=  (1 << UPM01);
    }
    if (stopBits == 1){
        UCSR0C &= ~(1 << USBS0); // setting stop bits to 1
    } else if(stopBits == 2){
        UCSR0C |= (1 << USBS0); // setting stop bits to 2
    }
}

void charTX(char c){
    while ((UCSR0A & (1 << UDRE0)) == 0); // waits until the buffer is empty
    UDR0 = c; // send new char to buffer
}

char charRX(){
    while ((UCSR0A & (1<<RXC0))== 0); // wait for data do get to buffer
    return UDR0; // return data from buffer
}

void strTX (const char *str){
	while (*str){
		charTX(*str);
		str++;
	}
}

void numTX(uint16_t num){
    int n = num, count = 0;
    while (n != 0){ // getting the digit count
        count++;
        n /= 10;
    }
    int rem;
    for (int i = 0; i < count; i++){
        rem = num % 10;
        num = num / 10;
        charTX(rem+'0');
    }
}