#include <avr/io.h>
#include <util/delay.h>


void USART_Init(void) {
    UBRR0H = 0;
    UBRR0L = 103; // 9600 baud @ 16MHz

    UCSR0B = (1 << TXEN0); // enable transmitter
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 8-bit data
}

// Send number as decimal via Serial
void USART_PrintDecimal(uint8_t val) {

    if (val == 0) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = '0';
        return;
    }

    char buffer[3];
    int i = 0;

    while (val > 0) {
        buffer[i++] = (val % 10) + '0';
        val /= 10;
    }

    // print reverse
    for (int j = i - 1; j >= 0; j--) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = buffer[j];
    }

    // newline
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = '\n';
}

// SPI Slave init
void SPI_SlaveInit(void) {
    // MISO (PB4) as OUTPUT
    DDRB |= (1 << DDB4);

    // Enable SPI (slave mode by default)
    SPCR = (1 << SPE);
}

// Receive SPI byte
uint8_t SPI_SlaveReceive(void) {
    while (!(SPSR & (1 << SPIF)));
    return SPDR;
}

int main(void) {

    USART_Init();
    SPI_SlaveInit();

    while (1) {
        uint8_t received = SPI_SlaveReceive();
        USART_PrintDecimal(received);
    }
}
