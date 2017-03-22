/*
 * UART_button_press_count.c
 *
 * Created: 3/18/2017 1:59:31 AM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>

#define DB_S 10 //debounce sensitivity (when lower then more precise but not so reliable)

void initUART();
void charTX(char c);
char charRX();
void toString(char c[], uint16_t num);

int main(void)
{
    initUART();
    uint16_t count = 0;
    uint16_t prsd = 0;
    uint16_t rlsd = 0;
    uint16_t chck = 0;
    char msg[28] = "The button has been pressed ";
    char emsg[6] = " times";
    char num[8] = "";

    PORTD |= (1 << PORTD4); //pull up

    while (1) 
    {
        if ((PIND & (1 << PIND4)) == 0){
            prsd++;
            if  (prsd > DB_S){
                if (chck == 0){ //check if the button was prsd just once
                    count++;
                    for (int i = 0; i<28;i++){
                        charTX(msg[i]);
                    }
                    toString(num,count);
                    for (int i = 0; i<8; i++){
                        charTX(num[i]);
                    }
                    for (int i = 0; i<6; i++){
                        charTX(emsg[i]);
                    }
                    charTX('\r');
                    charTX('\n');
                    chck = 1;
                }
                prsd = 0;
            }
            } else {
            rlsd++;
            if (rlsd > DB_S){
                if (chck == 1){ //check if the button was rlsd just once
                    //do whatever when rlsd
                    chck = 0;
                }
                rlsd = 0;
            }
        }
    }
}

void initUART(){
    UBRR0 = 103; // setting baud rate to 9600
    UCSR0B |= (1 << TXEN0) | (1 << RXEN0); // enabling Transmit and Receive
    UCSR0C |= (1 << USBS0); // setting stop bits to 2
}

void charTX(char c){
    UDR0 = c; // send new line to buffer
    //while ((UCSR0A & (1 << UDRE0)) == 0);
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}

char charRX(){
    while ((UCSR0A & (1<<RXC0))== 0); // wait for data do get to buffer
    return UDR0; // return data from buffer
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
