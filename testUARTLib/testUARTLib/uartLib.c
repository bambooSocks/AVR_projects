#include "uartLib.h"

void initSevenSeg(int a, int b, int c, int d, int e, int f, int g, int type_){
    pinArray[0]= a;
    pinArray[1]= b;
    pinArray[2]= c;
    pinArray[3]= d;
    pinArray[4]= e;
    pinArray[5]= f;
    pinArray[6]= g;
    type=type_;
    for (int i = 0; i< 7; i++){
        int LED = pinArray[i];    
        if (LED >= 0 && LED <=7){ //port D
            DDRD |= (1 << LED);
        }
        if (LED >= 8 && LED <=13){ //port B
            DDRB |= (1 << LED-8);
        }
        if (LED >= 14 && LED <=19){ //port C
            DDRC |= (1 << LED-14);
        }
    }
}

void writeNum(int num){
    turnAllOff();
    switch (num){
        case 0:
            turnOn(0); turnOn(1); turnOn(2); turnOn(3); turnOn(4); turnOn(5); break;
        case 1:
            turnOn(1); turnOn(2); break;
        case 2:
            turnOn(0); turnOn(1); turnOn(3); turnOn(4); turnOn(6); break;
        case 3:
            turnOn(0); turnOn(1); turnOn(2); turnOn(3); turnOn(6); break;
        case 4:
            turnOn(1); turnOn(2); turnOn(5); turnOn(6); break;
        case 5:
            turnOn(0); turnOn(2); turnOn(3); turnOn(5); turnOn(6); break;
        case 6:
            turnOn(0); turnOn(2); turnOn(3); turnOn(4); turnOn(5); turnOn(6); break;
        case 7:
            turnOn(0); turnOn(1); turnOn(2); break;
        case 8:
            turnOn(0); turnOn(1); turnOn(2); turnOn(3); turnOn(4); turnOn(5); turnOn(6); break;
        case 9:
            turnOn(0); turnOn(1); turnOn(2); turnOn(3); turnOn(5); turnOn(6); break;
        case 10:
            turnOn(0); turnOn(1); turnOn(2); turnOn(4); turnOn(5); turnOn(6); break;
        case 11:
            turnOn(2); turnOn(3); turnOn(4); turnOn(5); turnOn(6); break;
        case 12:
            turnOn(0); turnOn(3); turnOn(4); turnOn(5); break;
        case 13:
            turnOn(1); turnOn(2); turnOn(3); turnOn(4); turnOn(6); break;
        case 14:
            turnOn(0); turnOn(3); turnOn(4); turnOn(5); turnOn(6); break;
        case 15:
            turnOn(0); turnOn(4); turnOn(5); turnOn(6); break;
    }
}

void turnOn(int LED){
    if (type){ // common anode
        setLow(pinArray[LED]);
    } else { // common cathode
        setHigh(pinArray[LED]);
    }
}

void turnAllOff(){
    if (type){ // common anode
        for (int i = 0; i < 7;i++){
            setHigh(pinArray[i]);
        }
    } else { // common cathode
        for (int i = 0; i < 7;i++){
            setLow(pinArray[i]);
        }
    }
}

void setHigh(int LED){
    if (LED >= 0 && LED <=7){ //port D
        PORTD |= (1 << LED);
    }
    if (LED >= 8 && LED <=13){ //port B
        PORTB |= (1 << LED-8);
    }
    if (LED >= 14 && LED <=19){ //port C
        PORTC |= (1 << LED-14);
    }
}

void setLow(int LED){
    if (LED >= 0 && LED <=7){ //port D
        PORTD &= ~(1 << LED);
    }
    if (LED >= 8 && LED <=13){ //port B
        PORTB &= ~(1 << LED-8);
    }
    if (LED >= 14 && LED <=19){ //port C
        PORTC &= ~(1 << LED-14);
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

void delayF(uint32_t ms){
    uint32_t i = ms *471;
    while(--i>0);
}


void initUART(uint16_t baud, uint8_t stopBits){
    UBRR0 = ((((F_CPU / 16) + (baud / 2)) / baud) - 1); // setting baud rate to entered value
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enabling Transmit and Receive
    if (stopBits == 1){
        UCSR0C &= ~(1 << USBS0); // setting stop bits to 1
    } else if(stopBits == 2){
        UCSR0C |= (1 << USBS0); // setting stop bits to 2
    }
}

void charTX(unsigned char c){
    //while ((UCSR0A & (1 << UDRE0)) == 0);
    UDR0 = c; // send new char to buffer
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}

char charRX(){
    while ((UCSR0A & (1<<RXC0))== 0); // wait for data do get to buffer
    return UDR0; // return data from buffer
}

void strTX(const char *str){
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