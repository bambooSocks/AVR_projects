/*
 * GccApplication1.c
 *
 * Created: 3/7/2017 11:25:11 AM
 * Author : Matej Majtan
 */ 

#define F_CPU 16000000
#define DB_S 10   //Bounce sensitivity 

#include <avr/io.h>

void showNumber(int num);
void flickerNumber(int num);

int main(void)
{
    // setting display pins
    DDRD |= (1 << PORTD7); // setting D7 output
    PORTD |= (1 << PORTD7); // setting D7 high
    DDRB |= 0b00111111; // setting B0-B5 output
    PORTB |= 0b00111111; // setting B0-B5 high
    // setting input pins
    PORTD |= (1 << PORTD5) | (1 << PORTD6); // setting D5 and D6 to use pull up
    DDRD &= ~(1 << DDD5) | ~(1 << DDD6); // setting D5 and D6 to inputs

    uint16_t num = 0;
    showNumber(num);
    int Pressed1 = 0;
    int Pressed2 = 0;
    int chck1 = 0;
    int chck2 = 0;
    int Released1 = 0;
    int Released2 = 0;

    while (1) 
    {
    //DEBOUNCING
        if ((PIND & (1 << PIND5)) == 0){ //first button clicked
            Pressed1++;
            if (Pressed1 > DB_S){
                if (chck1 == 0){
                    if (num == 9){
                        num=0;
                        }else{
                        num++;
                    }
                    chck1 = 1;
                }
                Pressed1 = 0;
            }
        } else {
            Released1++;
            if (Released1 > DB_S){
                if (chck1 == 1){
                    chck1 = 0;
                }
                Released1 = 0;
            }
        }

        if ((PIND & (1 << PIND6)) == 0){ //second button clicked
            Pressed2++;           
            if (Pressed2 > DB_S){
                 if (chck2 == 0){
                    if (num == 0){
                        num=9;
                        }else{
                        num--;
                    }
                    chck2 = 1;
                }
                Pressed2 = 0;
            }
         } else {
             Released2++;
             if (Released2 > DB_S){
                 if (chck2 == 1){
                     chck2 = 0;
                 }
                 Released2 = 0;
             }
         }

        flickerNumber(num);
        //showNumber(num);
    }
}

void showNumber(int num){
    switch (num){
        case 0: 
        PORTB = 0x20; PORTD &= ~(1<<PORTD7); break;
        case 1: 
        PORTB = 0x3C; PORTD |= (1<<PORTD7); break;
        case 2: 
        PORTB = 0x12; PORTD &= ~(1<<PORTD7); break;
        case 3: 
        PORTB = 0x18; PORTD &= ~(1<<PORTD7); break;
        case 4: 
        PORTB = 0x0C; PORTD |= (1<<PORTD7); break;
        case 5: 
        PORTB = 0x09; PORTD &= ~(1<<PORTD7); break;
        case 6: 
        PORTB = 0x01; PORTD &= ~(1<<PORTD7); break;
        case 7: 
        PORTB = 0x3C; PORTD &= ~(1<<PORTD7); break;
        case 8: 
        PORTB = 0x00; PORTD &= ~(1<<PORTD7); break;
        case 9: 
        PORTB = 0x08; PORTD &= ~(1<<PORTD7); break;
        default: //E as ERROR :D
        PORTB = 0x03; PORTD &= ~(1<<PORTD7);
    }
}

void flickerNumber(int num){
    switch (num){
        case 0: // a0 b0 c0 d0 e0 f0 g1
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        //PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 1: // a1 b0 c0 d1 e1 f1 g1
        //PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        //PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        //PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        //PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        //PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 2: // a0 b0 c1 d0 e0 f1 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        //PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        //PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF 
        break;

        case 3: // a0 b0 c0 d0 e1 f1 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        //PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        //PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 4: // a1 b0 c0 d1 e1 f0 g0
        //PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        //PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        //PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 5: // a0 b1 c0 d0 e1 f0 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        //PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        //PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 6: // a0 b1 c0 d0 e0 f0 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        //PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 7: // a0 b0 c0 d1 e1 f1 g1
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        //PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        //PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        //PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        //PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF 
        break;

        case 8: // a0 b0 c0 d0 e0 f0 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        case 9: // a0 b0 c0 d0 e1 f0 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        //PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
        break;

        default: //E as ERROR :D  a0 b1 c1 d0 e0 f0 g0
        PORTD &= ~(1<<PORTD7); //LED a ON
        PORTD |= (1<<PORTD7); //LED a OFF
        //PORTB &= ~(1<<PORTB0); //LED b ON
        PORTB |= (1<<PORTB0); //LED b OFF
        //PORTB &= ~(1<<PORTB1); //LED c ON
        PORTB |= (1<<PORTB1); //LED c OFF
        PORTB &= ~(1<<PORTB2); //LED d ON
        PORTB |= (1<<PORTB2); //LED d OFF
        PORTB &= ~(1<<PORTB3); //LED e ON
        PORTB |= (1<<PORTB3); //LED e OFF
        PORTB &= ~(1<<PORTB4); //LED f ON
        PORTB |= (1<<PORTB4); //LED f OFF
        PORTB &= ~(1<<PORTB5); //LED g ON
        PORTB |= (1<<PORTB5); //LED g OFF
    }
}



