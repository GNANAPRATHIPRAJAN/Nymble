#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>

#define F_CPU 16000000UL
#define BAUD 2400
#define MYUBRR F_CPU/16/BAUD-1

class UART {
public:
    static void init(unsigned int ubrr) {
        UBRR0H = static_cast<unsigned char>(ubrr >> 8);
        UBRR0L = static_cast<unsigned char>(ubrr);
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    }

    static void transmit(char data) {
        while (!(UCSR0A & (1 << UDRE0)));
        UDR0 = data;
    }

    static char receive() {
        while (!(UCSR0A & (1 << RXC0)));
        return UDR0;
    }
};

void storeInEEPROM(const char* data, int dataSize, int startingAddress) {
    for (int i = 0; i < dataSize; ++i) {
        eeprom_write_byte(reinterpret_cast<uint8_t*>(startingAddress + i), data[i]);
        _delay_ms(5);
    }
}

void transmitStoredData(int dataSize, int startingAddress) {
    for (int i = 0; i < dataSize; ++i) {
        UART::transmit(eeprom_read_byte(reinterpret_cast<uint8_t*>(startingAddress + i)));
    }
}

int main(void) {
    UART::init(MYUBRR);

    char receivedData;
    int dataSize = 0;
    int startingAddress = 0;

    while (1) {
        receivedData = UART::receive();
        if (receivedData == '\0') {
            break;
        }
        storeInEEPROM(&receivedData, 1, startingAddress);
        ++dataSize;
    }

    transmitStoredData(dataSize, startingAddress);

    return 0;
}
