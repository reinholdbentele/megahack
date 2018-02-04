#include <stdbool.h>
#include <stdint.h>
#include <avr/io.h>
#include "game.h"
#include "megahack.h"

static uint8_t active_column = 0;
static uint16_t active_paddle = 0x00ff;
static bool direction = true;
static uint8_t speed_counter = 0;
static uint8_t current_speed = 5;

static void move_paddle(void)
{
    if (direction)
    {
        if (active_paddle & 0x8000)
        {
            direction = !direction;
        }
        else
        {
            active_paddle <<= 1;
        }
    }
    else
    {
        if (active_paddle & 0x0001)
        {
            direction = !direction;
        }
        else
        {
            active_paddle >>= 1;
        }
    }
}

/* Periodic callback, 30 Hz */
void game_tick(void)
{
    speed_counter++;
    if (speed_counter < current_speed)
    {
        return;
    }
    else
    {
        PORTD ^= _BV(LED_YELLOW_PIN);
        move_paddle();
        display_set_column(active_column, active_paddle);
        update_display();
        speed_counter = 0;
    }
}

/* Called when a debounced button is detected as pressed */
void game_button_pressed(void)
{
}
