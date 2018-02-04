#ifndef __MEGAHACK_H__
#define __MEGAHACK_H__

#include <stdint.h>
#include <stdbool.h>

#define CS_PIN PB2
#define LED_GREEN_PIN PB0
#define LED_YELLOW_PIN PD7
#define LED_RED_PIN PD6
#define SCK_PIN PB5
#define MOSI_PIN PB3
#define SWITCH_PIN PB1

void clear_display(void);
void display_set_pixel(uint8_t x, uint8_t y, bool on);
void display_set_column(uint8_t x, uint16_t column);
void update_display(void);

#endif
