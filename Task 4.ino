#include <avr/io.h>
#include <avr/interrupt.h>

volatile uint8_t countdown = 0;

ISR(INT0_vect) {
    PORTB |= (1 << 5);   
    countdown = 10;       
}

ISR(TIMER1_COMPA_vect) {
    if (countdown > 0) {
        countdown--;                   
        if (countdown == 0) {
            PORTB &= ~(1 << 5);        
        }
    }
}

int main(void) {

    DDRB |= (1 << 5);     
    PORTB &= ~(1 << 5);   

    DDRD  &= ~(1 << 2);   
    PORTD |=  (1 << 2);   
   
    EICRA |=  (1 << ISC01);  
    EICRA &= ~(1 << ISC00);  
    EIMSK |=  (1 << INT0);   

    TCCR1A = 0;                           
    TCCR1B = (1 << WGM12);             
    OCR1A  = 249;                         
    TIMSK1 = (1 << OCIE1A);              

    TCCR1B |= (1 << CS11) | (1 << CS10);


    sei();
    while (1) { }

    return 0;
}
