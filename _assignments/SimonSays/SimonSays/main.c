/*
 * SimonSays.c
 *
 * Created: 3/9/2017 9:38:06 AM
 * Author : Matej Majtan & Kristian Nørgaard Jensen
 * 
 * B0 : red LED 
 * B1 : green LED
 * B2 : yellow LED
 * B3 : blue LED
 *
 * D4 : red BUTTON
 * D5 : green BUTTON
 * D6 : yellow BUTTON
 * D7 : blue BUTTON
 *
 * RED : 0
 * GREEN : 1
 * YELLOW : 2
 * BLUE : 3
 *
 */ 
 
#define DB_S 30 // debounce sensitivity (when lower then more precise but not so reliable)
#define ARRSIZE 15 // random sequence array size 

#include <avr/io.h>
#include <stdlib.h> // used for srand() and rand()
#include <avr/interrupt.h> // used for delay timer interrupt

void win(); // function that inform the played about winning by blinking LEDs
void reset(); // function that blinks all LEDs at once to inform player about losing
void showLED(int led); // function that depending on number of LED turns on and off LED
void initReg(); // function that initializes all registers (output, input, ADC, timer for delay)
void genRandSeq(uint16_t *num); // function that generates random sequence and stores it in provided array
void debounceBtn(uint16_t btn); // function that debounces the buttons
void delay(uint16_t ms); // self-made delay function using timer

uint16_t Pressed[4] = {0}; // variable used for debouncing clicking the button
uint16_t Released[4] = {0}; // variable used for debouncing releasing the button
uint16_t chck[4] = {0}; // checks if the button is clicked or released (changes depending on current state of button)
uint16_t clckd[4] = {0}; // checks if the button was clicked (sets to 1 only when the button was clicked)
uint16_t num[ARRSIZE] = {0}; // random number array
uint16_t level = 0; // level variable - x is incremented each time you get to next level
uint16_t runBlink = 1; // flag to run the blinking part to show the sequence
uint16_t count = 0; // variable that indicates position in array that is being checked
uint16_t btnClRs[4] = {0}; // buttons clicked and released flag for each button
uint16_t delayCount = 0; // counter used for self-made delay function
uint16_t isInpLow[4] = {0}; // determines whether the input of exact button is low or not



int main(void){

    initReg(); // Initialize the registers for input, output, ADC and delay timer (timer0)
    genRandSeq(num); // generating the starting sequence

    while (1) 
    {
        // check for win if the level
        if (level == ARRSIZE){
            win();
        }

        // blink the sequence
        if (runBlink == 1){ // if statement that runs every time when the sequence is right or runBlink flag is set
            for (uint16_t i = 0; i < level+1; i++){ // loops through array until level
                showLED(num[i]); // shows current LED
            }
            runBlink = 0;
        }

        //reading input from buttons
        isInpLow[0] = !(PIND & (1 << PIND4)); // setting the flag high when red button was pressed (input is LOW)
        isInpLow[1] = !(PIND & (1 << PIND5)); // setting the flag high when green button was pressed (input is LOW)
        isInpLow[2] = !(PIND & (1 << PIND6)); // setting the flag high when yellow button was pressed (input is LOW)
        isInpLow[3] = !(PIND & (1 << PIND7)); // setting the flag high when blue button was pressed (input is LOW)

        for (int i=0; i<4;i++){
            debounceBtn(i); // running the debounce for each button
        }

        //setting btnClRs variables to determine which buttons have been clicked and released
        btnClRs[0] = (clckd[0] == 1) && (chck[0] == 0); // red button clicked and released
        btnClRs[1] = (clckd[1] == 1) && (chck[1] == 0); // green button clicked and released
        btnClRs[2] = (clckd[2] == 1) && (chck[2] == 0); // yellow button clicked and released
        btnClRs[3] = (clckd[3] == 1) && (chck[3] == 0); // blue button clicked and released

        // if any button was clicked
        if (btnClRs[0] || btnClRs[1] || btnClRs[2] || btnClRs[3]){ // if any button is clicked
            //if right button was clicked
            if (clckd[num[count]] == 1){
                count++; // moving to next element in array
                if (count > level){ // player successfully entered the sequence
                    runBlink = 1; // sets rubBlink flag to 1 so the program can blink current sequence
                    count = 0; // sets count to the beginning of array
                    level++; // moving to another level
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
    switch(led){ // deciding which pin to set high
    case 0: PORTB |= (1 << PORTB0); break;
    case 1: PORTB |= (1 << PORTB1); break;
    case 2: PORTB |= (1 << PORTB2); break;
    case 3: PORTB |= (1 << PORTB3); break;
    }
    delay(500);
    PORTB &= 0xF0; // setting all LEDs LOW
    delay(200); // needed so multiple same following values can be distinguished
}

void reset(){
    // blinks all LED to indicate loss
    PORTB = 0x0F;
    delay(500);
    PORTB = 0x00;
    delay(500);
    // resets flags
    runBlink = 1;
    count = 0;
    level = 0;
    for (int i = 0; i < 4; i++){
        clckd[i] = 0;
    }
    ///generate new sequence
    genRandSeq(num);
}

void win(){
    // blinks all LED to indicate win
    for (int i=0; i<3; i++){
        delay(300);
        PORTB = 0x08;
        delay(300);
        PORTB = 0x04;
        delay(300);
        PORTB = 0x02;
        delay(300);
        PORTB = 0x01;
        delay(300);
        PORTB = 0x02;
        delay(300);
        PORTB = 0x04;
    }
    delay(300);
    PORTB = 0x08;
    delay(300);
    PORTB = 0x00;
    // resets flags
    runBlink = 1;
    count = 0;
    level = 0;
    for (int i = 0; i < 4; i++){
        clckd[i] = 0;
    }
    // generate new sequence
    genRandSeq(num);
    // wait a bit ... just looks better :D
    delay(1000);
}

void initReg() {
    PORTD |= 0xF0; // setting up pull ups
    DDRB |= 0x0F; // setting B0-B3 to outputs
    DDRD &= 0x0F; // setting D4-D7 to inputs
    PORTB &= 0xF0; // set LEDs to LOW
    // ADC setup
    ADMUX = (1<<REFS0); // setting reference voltage to Vcc
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // enabling ADC with prescaler of 128
    // delay timer setup
    TCCR0A |= (1 << WGM01); // CTC mode
    OCR0A = 250; // setting the compare value to 250 ticks = 1 ms
    TIMSK0 |= (1 << OCIE0A); // setting the output compare interrupt
    sei(); // setting external interrupt
    TCCR0B |= (1 << CS00) | (1 << CS01); // starts up the timer with prescaler of 64
}

void genRandSeq(uint16_t *num) { //Generate random numbers
    ADCSRA |= (1<<ADSC); // reads ADC value
    while(ADCSRA & (1<<ADSC)); // wait for conversion to finish

    srand(ADC);	//Use the ADC value as seed for the random numbers
    for (int i = 0; i < ARRSIZE; i++) { //Loop through the array and add new numbers
        num[i] = rand() % 4; //Create random numbers between 0 - 3
    }
}

void debounceBtn(uint16_t btn){
    //DEBOUNCE THE BUTTON
    if (isInpLow[btn]){ // pull up of D4 connected to GND
        Pressed[btn]++; // incrementing the value until it is bigger than sensitivity constant
        if (Pressed[btn] > DB_S){ // wait until button is clicked for a longer time = the button was really clicked (debouncing)
            if (chck[btn] == 0){ //check if the button was pressed just once
                clckd[btn] = 1; // setting flag that button was clicked
                chck[btn] = 1; // changing the value of the flag to indicate that the button is clicked
            }                // to avoid running the same code multiple times
            Pressed[btn] = 0; // clearing debouncing variable
        }
    } else {
        Released[btn]++; // incrementing the value until it is bigger than sensitivity constant
        if (Released[btn] > DB_S){ // wait until button is released for a longer time = the button was really released (debouncing)
            if (chck[btn] == 1){ //check if the button was released just once
                chck[btn] = 0; // clearing flag that button was released and can be clicked again 
            }
            Released[btn] = 0; // clearing debouncing variable
        }
    }
}

ISR(TIMER0_COMPA_vect){
    delayCount++; // increasing value of delayCount every 1 ms
}

void delay(uint16_t ms){
    delayCount = 0; // starts up counting of ms
    while (delayCount<ms); // waits until the number of ms from clearing is lower then entered value
}