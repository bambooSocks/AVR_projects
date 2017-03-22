/*
 * btnReading.c
 *
 * Created: 3/17/2017 11:39:52 PM
 * Author : Matej Majtan
 */ 

#define DB_S 10 //debounce sensitivity (when lower then more precise but not so reliable)

#include <avr/io.h>

uint16_t prsd[3] = {0}; // variable used for debouncing clicking the button
uint16_t rlsd[3] = {0}; // variable used for debouncing releasing the button
uint16_t chck[3] = {0}; // checks if the button is clicked or rlsd (changes depending on current state of button)
uint16_t isInpLow[3] = {0}; // determines whether the input of exact button is low or not

void charTX(char c);
void initUART();
void debounceBtn(uint16_t btn);
char numToChar(uint16_t num);

int main(void)
{
    initUART();
    PORTD |= (1 << PORTD4) | (1 << PORTD5) | (1 << PORTD6); // setting up pull ups
    while (1) 
    {   

        isInpLow[0] = !(PIND & (1 << PIND4)); // setting the flag high when red button was pressed (input is LOW)
        isInpLow[1] = !(PIND & (1 << PIND5)); // setting the flag high when green button was pressed (input is LOW)
        isInpLow[2] = !(PIND & (1 << PIND6)); // setting the flag high when yellow button was pressed (input is LOW)

        for (int i = 0; i<3; i++){
            debounceBtn(i);
        }

    }
}

void charTX(char c){
    UDR0 = c; // send new line to buffer
    //while ((UCSR0A & (1 << UDRE0)) == 0);
    while ((UCSR0A & (1 << TXC0)) == 0);
    UCSR0A |= (1 << TXC0);
}

void initUART(){
    UBRR0 = 103; // setting baud rate to 9600
    UCSR0B |= (1 << TXEN0); // enabling Transmit
    UCSR0C |= (1 << USBS0); // setting stop bits to 2
}

void debounceBtn(uint16_t btn){
    //DEBOUNCE THE BUTTON
    if (isInpLow[btn]){ // pull up of D4 connected to GND
        prsd[btn]++; // incrementing the value until it is bigger than sensitivity constant
        if (prsd[btn] > DB_S){ // wait until button is clicked for a longer time = the button was really clicked (debouncing)
            if (chck[btn] == 0){ //check if the button was prsd just once
                    charTX('B');
                    charTX(numToChar(btn+1));
                    charTX('\r');
                    charTX('\n');
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

char numToChar(uint16_t num){
    switch (num){
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    default: return '0';
    }
}
