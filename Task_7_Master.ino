#include <avr/io.h>
#include <util/delay.h>

void SPI_MasterInit(void) {
    // MOSI (PB3), SCK (PB5), SS (PB2) as OUTPUT
    DDRB |= (1 << DDB3) | (1 << DDB5) | (1 << DDB2);

    // SS HIGH (no slave selected initially)
    PORTB |= (1 << PORTB2);

    // Enable SPI, Master mode, clock = fck/16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);
}

void SPI_MasterTransmit(uint8_t data) {

    // SS LOW → select slave
    PORTB &= ~(1 << PORTB2);

    // Load data → starts transmission
    SPDR = data;

    // Wait until transmission complete
    while (!(SPSR & (1 << SPIF)));

    // SS HIGH → end communication
    PORTB |= (1 << PORTB2);

    _delay_ms(10);
}

int main(void) {

    SPI_MasterInit();

    uint8_t values[3] = {85, 170, 255};
    uint8_t i = 0;

    while (1) {
        SPI_MasterTransmit(values[i]);
        i = (i + 1) % 3;   // loop 0 → 1 → 2 → repeat
        _delay_ms(1000);
    }
}
