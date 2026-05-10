#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

const uint8_t seg_table[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x6F
};

volatile uint8_t  count = 0;
volatile uint8_t  paused = 0;


#define DEBOUNCE_TICKS 1       

volatile uint16_t timer_ticks    = 0;   
volatile uint16_t last_press_tick = 0;  

void uart_init(uint32_t baud) {
    uint16_t ubrr = F_CPU / 16 / baud - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)ubrr;
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_print(const char* str) {
    while (*str) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = *str++;
    }
}

void update_display(uint8_t num) {
    uint8_t pattern = seg_table[num];
    PORTB = (PORTB & 0xC0) | (pattern & 0x3F);
    if (pattern & 0x40) PORTD |=  (1 << PD7);
    else                PORTD &= ~(1 << PD7);
}

void init_hardware() {
    DDRB |=  0x3F;
    DDRD |=  (1 << DDD7);
    DDRD &= ~(1 << DDD2);
    PORTD |= (1 << PORTD2);  

    TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10);
    OCR1A   = 7811;
    TIMSK1 |= (1 << OCIE1A);

    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);

    sei();
}
ISR(TIMER1_COMPA_vect) {
    timer_ticks++;        

    if (!paused) {
        count = (count >= 9) ? 0 : count + 1;
        update_display(count);

        char buf[16];
        sprintf(buf, "Count: %d\r\n", count);
        uart_print(buf);
    }
}

ISR(INT0_vect) {
    uint16_t elapsed = (uint16_t)(timer_ticks - last_press_tick);
    if (elapsed < DEBOUNCE_TICKS) return; 
    last_press_tick = timer_ticks;         

    paused = !paused;
    uart_print(paused ? "Paused\r\n" : "Resumed\r\n");
}


int main(void) {
    uart_init(9600);
    init_hardware();
    update_display(0);
    while (1) { 

    }
}