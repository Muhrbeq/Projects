/*
 * NeoPixel.h
 *
 *  Created on: 30 mars 2020
 *      Author: Rasmus.Muhrbeck
 */

#ifndef NEOPIXEL_H_
#define NEOPIXEL_H_

#include "main.h"

#define LED_CFG_LEDS_CNT                10       /*!< Number of leds in a strip row */
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
	NEO_RAINBOWCOLD,
	NEO_RAINBOWWARM,
	NEO_RAINBOW,
	NEO_BATTERY,
	NEO_OFF,
} NeoColors;

typedef enum NeoRainbowState
{
	NEO_RISING,
	NEO_NEXTRISE,
	NEO_NEXTFALL,
	NEO_FALLING,
	NEO_STOP,
}NeoRainbowState;

typedef struct NeoRainbow
{
	uint8_t nrRed;
	NeoRainbowState nrsRed;
	uint8_t nrGreen;
	NeoRainbowState nrsGreen;
	uint8_t nrBlue;
	NeoRainbowState nrsBlue;
}NeoRainbow;

typedef enum NeoIntensity
{
	NEO_LVL_1 = 16,
	NEO_LVL_2 = 8,
	NEO_LVL_3 = 4,
	NEO_LVL_4 = 2,
	NEO_LVL_5 = 1,
}NeoIntensity;

typedef struct NeoRGB
{
	uint8_t Red;
	uint8_t Green;
	uint8_t Blue;
} NeoRGB;

void SetNeoColor(uint8_t Red, uint8_t Green, uint8_t Blue);
void UpdateNeoColor(void);

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

void NeoStateMachine();
NeoColors GetNeoState();
void SetNeoState(NeoColors nC);
void SetNeoIntensityState(NeoIntensity nI);
NeoColors GetNeoIntensityState();
void UpdateIntensity(NeoIntensity nI);
void NeoBatteryLevelLight(uint8_t batteryLevel);
void UpdateRainbowWarm(void);
void UpdateRainbowCold(void);
void InitRainbows(void);

#endif /* NEOPIXEL_H_ */
