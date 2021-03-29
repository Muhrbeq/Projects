/*
 * NeoPixel.h
 *
 *  Created on: 30 mars 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include "main.h"

#define LED_CFG_LEDS_CNT                33       /*!< Number of leds in a strip row */
#define LED_CFG_BYTES_PER_LED           3
#define LED_CFG_RAW_BYTES_PER_LED       (LED_CFG_BYTES_PER_LED * 8)

void        led_init(void);
uint8_t     led_update(uint8_t block);

uint8_t     led_set_color(size_t index, uint8_t r, uint8_t g, uint8_t b);
uint8_t     led_set_color_all(uint8_t r, uint8_t g, uint8_t b);
uint8_t		led_set_color_rgb(uint32_t index, uint32_t rgbw);
uint8_t     led_set_color_all_rgb(uint32_t rgb);
uint8_t     led_is_update_finished(void);

typedef enum NeoColors
{
	NEO_GREEN,
	NEO_CYAN,
	NEO_BLUE,
	NEO_PURPLE,
	NEO_RED,
	NEO_ORANGE,
	NEO_YELLOW,
	NEO_WHITE,
	NEO_RAINBOW,
	NEO_OFF,
} NeoColors;

void TurnAllRed(void);
void TurnAllBlue(void);
void TurnAllGreen(void);
void TurnAllCyan(void);
void TurnAllPurple(void);
void TurnAllOrange(void);
void TurnAllYellow(void);
void TurnAllWhite(void);
void TurnAllOff(void);
void PowerUpSword(void);
void PowerDownSword(void);
void RainBow(uint32_t i);

void NeoStateMachine(NeoColors nC);
NeoColors GetNeoState();

#endif /* NEOPIXEL_H_ */
