/*
 * rotEncoder3.c
 *
 * Created: 3/27/2017 12:36:54 AM
 * Author : Matej Majtan
 */ 

#include <avr/io.h>
#include "omniLib.h"

uint8_t readEncoder(void);

int main(void)
{
    uint8_t oldState = readEncoder(), state = 0, number = 0;
    initUART(9600, 1);
    charTX(power(3,2)+'0');
    numTX(number);

    //PORTD |= (1 << PORTD2) | (1 << PORTD3);
    while (1) 
    {
        state = readEncoder();
        if (oldState != state){
            if(oldState == 1 && state == 3){ //counterclockwise
                if (number != 255){
                    number++;
                    numTX(number);
                    charTX('\r');
                    charTX('\n');
                }
            } else if(oldState == 0 && state == 2){ //clockwise
                if (number != 0){
                    number--;
                    numTX(number);
                    charTX('\r');
                    charTX('\n');S
                }
            }
        }
        oldState = state;       
    }
}

uint8_t readEncoder(void){
    uint8_t res = 0;
    if(PINC & (1 << PINC0)){   // res |= PINC & (1 << PINC0) ? (1 << 1) : (0 << 1);
        res |= (1 << 1);
    }
    if(PINC & (1 << PINC1)){
        res |= (1 << 0);
    }
    return res;
}
