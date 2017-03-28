/////////////////////////////////////////DEBOUNCE///////////////////////////////////////////

#define DB_S 10 //debounce sensitivity (when lower then more precise but not so reliable)

int prsd = 0;
int rlsd = 0;
int chck = 0;

if (/*button clicked condition*/){
         prsd++;           
            if  prsd > DB_S){
                 if (chck == 0){ //check if the button was prsd just once
                    //do whatever when prsd
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

////////////////////////////////////////UART///////////////////////////////////////////////

void initUART();
void charTX(char c);
char charRX();
void strTX(const char *str);
void numTX(uint16_t num);

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

void strTX (const char *str){
	while (*str) 
	{
		charTX(*str);
		str++; 
	}
}

void numTX(uint16_t num){
    char c[30] = "";
    int n = num, len = 0;
    while (n != 0){
        len++;
        n /= 10;
    }
    int rem;
    for (int i = 0; i < len; i++){
        rem = num % 10;
        num = num / 10;
        c[len - (i + 1)] = rem + '0';
    }
    c[len] = '\0';
    strTx(c);
}

///////////////////////////////////////DELAY//////////////////////////////////////////////

#include <avr/interrupt.h>

int delayCount = 0;

void init_timer();
void delay(uint16_t ms);

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

/////////////////////////////////////DEBOUNCING MORE BUTTONS//////////////////////////////////////////////

#define DB_S 10 //debounce sensitivity (when lower then more precise but not so reliable)

uint16_t prsd[n] = {0}; // variable used for debouncing clicking the button
uint16_t rlsd[n] = {0}; // variable used for debouncing releasing the button
uint16_t chck[n] = {0}; // checks if the button is clicked or rlsd (changes depending on current state of button)
uint16_t isInpLow[n] = {0}; // determines whether the input of exact button is low or not

isInpLow[n] = !(PINx & (1 << PINxx)); // setting the flag high when red button was prsd (input is LOW)

void debounceBtn(uint16_t btn){
    //DEBOUNCE THE BUTTON
    if (isInpLow[btn]){ // pull up of D4 connected to GND
        prsd[btn]++; // incrementing the value until it is bigger than sensitivity constant
        if (prsd[btn] > DB_S){ // wait until button is clicked for a longer time = the button was really clicked (debouncing)
            if (chck[btn] == 0){ //check if the button was prsd just once
                
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

///////////////////////////////////////////////number to string///////////////////////////////////

void toString(char c[], uint16_t num){
    int n = num, len = 0;
    while (n != 0){
        len++;
        n /= 10;
    }
    int rem;
    for (int i = 0; i < len; i++){
        rem = num % 10;
        num = num / 10;
        c[len - (i + 1)] = rem + '0';
    }
    c[len] = '\0';
}