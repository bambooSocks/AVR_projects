/*
 * btnTXledRX.c
 *
 * Created: 3/20/2017 12:22:30 PM
 * Author : Matej Majtan
 *
 * buttons : D4-7
 * leds : B0-3
 *
 *
 */ 

#define DB_S 10 //debounce sensitivity (when lower then more precise but not so reliable)
#include <avr/io.h>

uint16_t prsd[4] = {0}; // variable used for debouncing clicking the button
uint16_t rlsd[4] = {0}; // variable used for debouncing releasing the button
uint16_t chck[4] = {0}; // checks if the button is clicked or rlsd (changes depending on current state of button)
uint16_t isInpLow[4] = {0}; // determines whether the input of exact button is low or not

void debounceBtn(uint16_t btn);
void initUART();
char charRX();
void charTX(char c);
void showLED(uint16_t led);

int main(void)
{
    initUART(); // initialization of registers
    PORTD |= 0xF0; // pull up
    DDRB |= 0x0F; // output

    while (1) 
    {
        isInpLow[0] = !(PIND & (1 << PIND4)); // setting the flag high when red button was prsd (input is LOW)
        isInpLow[1] = !(PIND & (1 << PIND5));
        isInpLow[2] = !(PIND & (1 << PIND6));
        isInpLow[3] = !(PIND & (1 << PIND7));
        
        for (int i=0; i<4; i++){ // debouncing all 4 buttons
            debounceBtn(i);
        }
        char c = charRX(); // reading char from uart
        showLED(c - '0'); // substracting by char of 0 to get the number
    }
}

void showLED(uint16_t led){ // turning on led by entered number
    PORTB &= 0xF0; 
    switch (led){
    case 0: PORTB |= 0x01;
    break;
    case 1: PORTB |= 0x02;
    break;
    case 2: PORTB |= 0x04;
    break;
    case 3: PORTB |= 0x08;
    break;
    }
    
}

void debounceBtn(uint16_t btn){
    //DEBOUNCE THE BUTTON
    if (isInpLow[btn]){ // pull up of D4 connected to GND
        prsd[btn]++; // incrementing the value until it is bigger than sensitivity constant
        if (prsd[btn] > DB_S){ // wait until button is clicked for a longer time = the button was really clicked (debouncing)
            if (chck[btn] == 0){ //check if the button was prsd just once
                    charTX('0' + btn);
                chck[btn] = 1; // changing the value of the flag to indicate that the button is clicked
            }                // to avoid running the same code multiple times
            prsd[btn] = 0; // clearing debouncing variable
        }
    } else {
        rlsd[btn]++; // incrementing the value until it is bigger than sensitivity constant
        if (rlsd[btn] > DB_S){ // wait until button is rlsd for a longer time = the button was really rlsd (debouncing)
            if (chck[btn] == 1){ //check if the button was rlsd just once

                chck[btn] = 0; // clearing flag that button was rlsd and can be clicked again
            }
            rlsd[btn] = 0; // clearing debouncing variable
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
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}

char charRX(){
    while ((UCSR0A & (1<<RXC0))== 0); // wait for data do get to buffer
    return UDR0; // return data from buffer
}
