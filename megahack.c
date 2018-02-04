#include <stdbool.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#define CMD_DECODE_MODE 9
#define CMD_INTENSITY 10
#define CMD_SCAN_LIMIT 11
#define CMD_SHUTDOWN 12
#define CMD_DISPLAY_TEST 15
#define CMD_DIGIT0 1
#define CS_PIN PB2
#define LED_GREEN_PIN PB0
#define LED_YELLOW_PIN PD7
#define LED_RED_PIN PD6
#define SCK_PIN PB5
#define MOSI_PIN PB3
#define SWITCH_PIN PB1
#define switch_pressed() ((PINB & _BV(SWITCH_PIN)) == 0)

static uint8_t display_data[8][8];

uint8_t spi_send_byte(uint8_t data)
{
    /* Push data to SPI unit */
    SPDR = data;

    /* Wait for SPI to set finished flag */
    while ((SPSR & _BV(SPIF)) == 0)
        ;

    /* Read from SPDR to clear SPIF from SPSR */
    return SPDR;
}

void spi_send(const uint8_t *data, uint8_t length)
{
    for (uint8_t i = 0; i < length; i++)
    {
        spi_send_byte(data[i]);
    }
}

void cmd_to_all_with_data(uint8_t cmd_code, const uint8_t *data)
{
    uint8_t tx_data[16];
    for (uint8_t i = 0; i < 8; i++)
    {
        tx_data[2 * i] = cmd_code;
        tx_data[2 * i + 1] = data[i];
    }
    PORTB &= ~_BV(CS_PIN); // chip select on

    spi_send(tx_data, 16);

    PORTB |= _BV(CS_PIN); // chip select off
}

void same_cmd_to_all(uint8_t cmd_code, uint8_t data)
{
    uint8_t horst[8];
    memset(horst, data, 8);
    cmd_to_all_with_data(cmd_code, horst);
}

void clear_display(void)
{
    memset(display_data, 0, sizeof(display_data));
}

void set_pixel(uint8_t x, uint8_t y)
{
    if (y >= 8)
    {
        x += 32;
        y -= 8;
    }

    uint8_t l = 7 - y;
    uint8_t m = x / 8;
    uint8_t n = 1 << (7 - (x % 8));

    display_data[l][m] |= n;
}

void update_display(void)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        cmd_to_all_with_data(CMD_DIGIT0 + i, display_data[i]);
    }
}

int main(void)
{
    DDRB |= _BV(LED_GREEN_PIN) | _BV(SCK_PIN) | _BV(PB3) | _BV(CS_PIN);
    DDRD |= _BV(LED_YELLOW_PIN) | _BV(LED_RED_PIN);

    PORTB |= _BV(CS_PIN);     // chip select off
    PORTB |= _BV(SWITCH_PIN); // set pull-up resistor

    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1);

    /* No 7-segment decoding active */
    same_cmd_to_all(CMD_DECODE_MODE, 0);

    /* Intensity level from 0..15 */
    same_cmd_to_all(CMD_INTENSITY, 10);

    /* Enable all eight rows */
    same_cmd_to_all(CMD_SCAN_LIMIT, 7);

    /* Normal operation */
    same_cmd_to_all(CMD_SHUTDOWN, 1);

    /* No display test. */
    same_cmd_to_all(CMD_DISPLAY_TEST, 0);

    while (true)
    {
        if (switch_pressed())
        {
            PORTB |= _BV(LED_GREEN_PIN);
        }
        else
        {
            PORTB &= ~_BV(LED_GREEN_PIN);
        }

        PORTD |= _BV(LED_YELLOW_PIN) | _BV(LED_RED_PIN);
        clear_display();
        set_pixel(0, 0);
        set_pixel(31, 0);
        update_display();
        _delay_ms(500);

        PORTD &= ~(_BV(LED_YELLOW_PIN) | _BV(LED_RED_PIN));
        clear_display();
        set_pixel(0, 15);
        set_pixel(31, 15);
        update_display();
        _delay_ms(500);
    }
}
