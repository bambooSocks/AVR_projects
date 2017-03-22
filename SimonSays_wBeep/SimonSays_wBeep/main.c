/*
 * SimonSays.c
 *
 * Created: 3/9/2017 9:38:06 AM
 * Author : Matej Majtan & Kristian Nørgaard Jensen
 * 
 * D6 : buzzer
 *
 * B0 : red LED 
 * B1 : green LED
 * B2 : yellow LED
 * B3 : blue LED
 *
 * D4 : red BUTTON
 * D5 : green BUTTON
 * D3 : yellow BUTTON
 * D7 : blue BUTTON
 *
 * RED : 0
 * GREEN : 1
 * YELLOW : 2
 * BLUE : 3
 *
 */ 
 
#define F_CPU 16000000UL
#define DB_S 30 //debounce sensitivity (when lower then more precise but not so reliable)
#define ARRSIZE 15

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

void win();
void reset();
void showLED(int led);
void init_registers();
void generate_Rand_Num(uint16_t *num);
void debounceButtons();
void tone(uint16_t hz, uint16_t delay);

uint16_t Pressed[4] = {0};
uint16_t Released[4] = {0};
uint16_t chck[4] = {0}; // checks if the button is clicked or released (changes depending on current state of button)
uint16_t clckd[4] = {0}; // checks if the button was clicked (sets to 1 only when the button was clicked)
uint16_t num[ARRSIZE] = {0}; // random number array
uint16_t x = 0;
uint16_t runBlink = 1;
uint16_t count = 0;
uint16_t flag = 1;
uint16_t btnClRs[4] = {0}; // buttons clicked and released

int main(void){
    init_registers();	//Initialize the registers.
    //Both for timers, and input and output pins
    generate_Rand_Num(num);

    while (1) 
    {
        debounceButtons();

        //read buttons
        btnClRs[0] = (clckd[0] == 1) && (chck[0] == 0); // red button clicked and released
        btnClRs[1] = (clckd[1] == 1) && (chck[1] == 0); // green button clicked and released
        btnClRs[2] = (clckd[2] == 1) && (chck[2] == 0); // yellow button clicked and released
        btnClRs[3] = (clckd[3] == 1) && (chck[3] == 0); // blue button clicked and released

        // check for win
        if (x == ARRSIZE+1){
            win();
        }

        ////blink////
        if (runBlink == 1){
            for (uint16_t i = 0; i < x+1; i++){
                showLED(num[i]); // shows current LED
            }
            runBlink = 0;
        }

        //if multiple buttons were clicked then resets (anti cheat :D)
        if ((btnClRs[0] && btnClRs[1]) || (btnClRs[0] && btnClRs[2]) || (btnClRs[0] && btnClRs[3]) || (btnClRs[1] && btnClRs[2]) || (btnClRs[1] && btnClRs[3]) || (btnClRs[2] && btnClRs[3])){
            reset();
        }
        /////////////////////needs check doesn't work ^^^^^^^^ this one :D //// or does it?? sometimes :D

        // if any button was clicked
        if (btnClRs[0] || btnClRs[1] || btnClRs[2] || btnClRs[3]){ // if any button is clicked
            //if right button was clicked
            if (clckd[num[count]] == 1){
                count++;
                if (count > x){ // player successfully entered the sequence
                    runBlink = 1; // sets rubBlink flag to 1 so the program can blink current sequence
                    count = 0;
                    x++;
                    for (int i = 0; i < 4; i++){
                        clckd[i] = 0;
                    }
                }
            } else { // else any other button was clicked and is released then reset
                reset();
            }
            //clear the button flags
            for (int i = 0; i < 4; i++){
                clckd[i] = 0;
            }
        }
    } //while (1) end
} // main end


/////////////////////CUSTOM FUNCTIONS///////////////////////////

void showLED(int led){ // blinks set LED
    switch(led){
    case 0: PORTB |= (1 << PORTB0); break;
    case 1: PORTB |= (1 << PORTB1); break;
    case 2: PORTB |= (1 << PORTB2); break;
    case 3: PORTB |= (1 << PORTB3); break;
    }
    tone(440, 300);
    _delay_ms(500);
    PORTB &= 0xF0; // setting all LEDs LOW
    _delay_ms(200); // needed so multiple same following values can be distinguished
}

void reset(){
    // blinks all LED to indicate loss
    PORTB = 0x0F;
    _delay_ms(500);
    PORTB = 0x00;
    _delay_ms(500);
    // resets flags
    runBlink = 1;
    count = 0;
    x = 0;
    for (int i = 0; i < 4; i++){
        clckd[i] = 0;
    }
    ///generate new sequence
    generate_Rand_Num(num);
}

void win(){
    // blinks all LED to indicate win
    for (int i=0; i<3; i++){
        _delay_ms(300);
        PORTB = 0x08;
        _delay_ms(300);
        PORTB = 0x04;
        _delay_ms(300);
        PORTB = 0x02;
        _delay_ms(300);
        PORTB = 0x01;
        _delay_ms(300);
        PORTB = 0x02;
        _delay_ms(300);
        PORTB = 0x04;
    }
    _delay_ms(300);
    PORTB = 0x08;
    _delay_ms(300);
    PORTB = 0x00;
    // resets flags
    runBlink = 1;
    count = 0;
    x = 0;
    for (int i = 0; i < 4; i++){
        clckd[i] = 0;
    }
    ///generate new sequence
    generate_Rand_Num(num);
    _delay_ms(1000);
}

void init_registers() {
    PORTD |= 0xB8; // setting up pull ups
    DDRB |= 0x0F; // setting B0-B3 to outputs
    DDRD &= 0x47; // setting D3, D4, D5 and D7 to inputs
    DDRD |= (1 << DDD6); // setting D3 (buzzer) to output
    PORTB &= 0xF0; // set LEDs to LOW

    //Initialize the timer for random number generator
    TCCR2B |= (1 << CS20); //Initialize timer0 with a prescaler of 1
    TCNT2 = 0;	//Initialize the counter. The counter can go from 0 - 255 since timer0 is a 8 bit timer

    TCCR0A |= (1 << WGM01) | (1 << COM0A0); // setting timer to CTC and toggling flag at compare match (OC2B (D3 pin))
    TCCR0A |= (1 << CS02) | (1 << CS01);

}

void tone(uint16_t hz, uint16_t delay){
    //TCCR2A |= (1 << CS22) | (1 << CS21); // turning timer 2 on with prescaler 256 
    OCR0A = (F_CPU/(hz*2*256)); // setting output compare register 2B
    TCCR0A |= (1 << WGM01) | (1 << COM0A0);
    //_delay_ms(delay);
    //TCCR2A &= ~(1 << CS22) & ~(1 << CS21); // turning timer 2 off
}

void generate_Rand_Num(uint16_t *num) { //Generate random numbers
    srand(TCNT2);	//Use the timer value as seed for the random numbers
    //Set the seed value to 1 to get the same numbers each time
    //Will be goo for debugging
    for (int i = 0; i < ARRSIZE; i++) { //Loop through the array and add new numbers
        num[i] = rand() % 4; //Create random numbers between 0 - 3
    }
}

void debounceButtons(){
    //DEBOUNCE THE RED BUTTON
    if ((PIND & (1 << PIND4)) == 0){ // pull up connected to GND
        Pressed[0]++;
        if (Pressed[0] > DB_S){
            if (chck[0] == 0){ //check if the button was pressed just once
                clckd[0]=1;
                chck[0] = 1;
            }
            Pressed[0] = 0;
        }
    } else {
        Released[0]++;
        if (Released[0] > DB_S){
            if (chck[0] == 1){ //check if the button was released just once
                chck[0] = 0;
            }
            Released[0] = 0;
        }
    }

    //DEBOUNCE THE GREEN BUTTON
    if ((PIND & (1 << PIND5)) == 0){
        Pressed[1]++;
        if (Pressed[1] > DB_S){
            if (chck[1] == 0){ //check if the button was pressed just once
                clckd[1]=1;
                chck[1] = 1;
            }
            Pressed[1] = 0;
        }
    } else {
        Released[1]++;
        if (Released[1] > DB_S){
            if (chck[1] == 1){ //check if the button was released just once
                chck[1] = 0;
            }
            Released[1] = 0;
        }
    }

    //DEBOUNCE THE YELLOW BUTTON
    if ((PIND & (1 << PIND3)) == 0){
        Pressed[2]++;
        if (Pressed[2] > DB_S){
            if (chck[2] == 0){ //check if the button was pressed just once
                clckd[2] = 1;
                chck[2] = 1;
            }
            Pressed[2] = 0;
        }
    } else {
        Released[2]++;
        if (Released[2] > DB_S){
            if (chck[2] == 1){ //check if the button was released just once
                chck[2] = 0;
            }
            Released[2] = 0;
        }
    }

    //DEBOUNCE THE BLUE BUTTON
    if ((PIND & (1 << PIND7)) == 0){
        Pressed[3]++;
        if (Pressed[3] > DB_S){
            if (chck[3] == 0){ //check if the button was pressed just once
                clckd[3]=1;
                chck[3] = 1;
            }
            Pressed[3] = 0;
        }
   } else {
        Released[3]++;
        if (Released[3] > DB_S){
            if (chck[3] == 1){ //check if the button was released just once
                chck[3] = 0;
            }
            Released[3] = 0;
        }
    }
}
