/*
 * NeoPixel.c
 *
 *  Created on: 30 mars 2020
 *      Author: Rasmus.Muhrbeck
 */

#include "NeoPixel.h"
#include "Uart_Handling.h"
#include <string.h>
#include <math.h>

static uint8_t leds_colors[LED_CFG_BYTES_PER_LED * LED_CFG_LEDS_CNT];

static uint32_t tmp_led_data[2 * LED_CFG_RAW_BYTES_PER_LED];

static uint8_t          is_reset_pulse;     /*!< Status if we are sending reset pulse or led data */
static volatile uint8_t is_updating;        /*!< Is updating in progress? */
static uint32_t         current_led;        /*!< Current LED number we are sending */
static uint8_t     led_start_reset_pulse(uint8_t num);

extern uint8_t Vbat_Flag;

NeoRainbow RainbowWarm;
NeoRainbow RainbowCold;
NeoRainbow Rainbow;
NeoColors neoColorTracker = NEO_RAINBOWWARM;
NeoRGB neo;
NeoIntensity neoIntensityTracker = NEO_LVL_5;

void SetNeoState(NeoColors nC)
{
	neoColorTracker = nC;
}

NeoColors GetNeoState()
{
	return neoColorTracker;
}

void SetNeoIntensityState(NeoIntensity nI)
{
	neoIntensityTracker = nI;
}

NeoColors GetNeoIntensityState()
{
	return neoIntensityTracker;
}


static void NeoStateFromBLE()
{
	uint8_t set[200] = {0};

	GetStringBLE(set);

	if(strcmp((char*)set, "red") == 0)
	{
		SetNeoState(NEO_RED);
	}
	else if(strcmp((char*)set, "green") == 0)
	{
		SetNeoState(NEO_GREEN);
	}
	else if(strcmp((char*)set, "blue") == 0)
	{
		SetNeoState(NEO_BLUE);
	}
	else if(strcmp((char*)set, "white") == 0)
	{
		SetNeoState(NEO_WHITE);
	}
	else if(strcmp((char*)set, "cyan") == 0)
	{
		SetNeoState(NEO_CYAN);
	}
	else if(strcmp((char*)set, "yellow") == 0)
	{
		SetNeoState(NEO_YELLOW);
	}
	else if(strcmp((char*)set, "orange") == 0)
	{
		SetNeoState(NEO_ORANGE);
	}
	else if(strcmp((char*)set, "purple") == 0)
	{
		SetNeoState(NEO_PURPLE);
	}
	else if(strcmp((char*)set, "rainbowcold") == 0)
	{
		SetNeoState(NEO_RAINBOWCOLD);
	}
	else if(strcmp((char*)set, "rainbowwarm") == 0)
	{
		SetNeoState(NEO_RAINBOWWARM);
	}
	else if(strcmp((char*)set, "rainbow") == 0)
	{
		SetNeoState(NEO_RAINBOW);
	}
	else if(strcmp((char*)set, "bat") == 0)
	{
		Vbat_Flag = 1;
		SetNeoState(NEO_BATTERY);
	}
	else if(strcmp((char*)set, "l1") == 0)
	{
		SetNeoIntensityState(NEO_LVL_1);
		if(GetNeoState() == NEO_RAINBOWCOLD || GetNeoState() == NEO_RAINBOWWARM)
			InitRainbows();
	}
	else if(strcmp((char*)set, "l2") == 0)
	{
		SetNeoIntensityState(NEO_LVL_2);
		if(GetNeoState() == NEO_RAINBOWCOLD || GetNeoState() == NEO_RAINBOWWARM)
			InitRainbows();
	}
	else if(strcmp((char*)set, "l3") == 0)
	{
		SetNeoIntensityState(NEO_LVL_3);
		if(GetNeoState() == NEO_RAINBOWCOLD || GetNeoState() == NEO_RAINBOWWARM)
			InitRainbows();
	}
	else if(strcmp((char*)set, "l4") == 0)
	{
		SetNeoIntensityState(NEO_LVL_4);
		if(GetNeoState() == NEO_RAINBOWCOLD || GetNeoState() == NEO_RAINBOWWARM)
			InitRainbows();
	}
	else if(strcmp((char*)set, "l5") == 0)
	{
		SetNeoIntensityState(NEO_LVL_5);
		if(GetNeoState() == NEO_RAINBOWCOLD || GetNeoState() == NEO_RAINBOWWARM)
			InitRainbows();
	}
}

void NeoStateMachine()
{
	NeoStateFromBLE();

	NeoColors nC = GetNeoState();

	if(nC == NEO_GREEN)
	{
		SetNeoColor(0, 0xFF, 0);
	}
	else if(nC == NEO_CYAN)
	{
		SetNeoColor(0, 0xFF, 0xFF);
	}
	else if(nC == NEO_BLUE)
	{
		SetNeoColor(0, 0, 0xFF);
	}
	else if(nC == NEO_PURPLE)
	{
		SetNeoColor(0xFF, 0, 0xFF);
	}
	else if(nC == NEO_RED)
	{
		SetNeoColor(0xFF, 0, 0);
	}
	else if(nC == NEO_ORANGE)
	{
		SetNeoColor(0xFF, 0x2F, 0);
	}
	else if(nC == NEO_YELLOW)
	{
		SetNeoColor(0xFF, 0xFF, 0);
	}
	else if(nC == NEO_WHITE)
	{
		SetNeoColor(0xFF, 0xFF, 0xFF);
	}

	UpdateNeoColor();
}

void SetNeoColor(uint8_t Red, uint8_t Green, uint8_t Blue)
{
	neo.Red = Red;
	neo.Green = Green;
	neo.Blue = Blue;
}

void UpdateIntensity(NeoIntensity nI)
{
	if(nI == NEO_LVL_1)
	{
		SetNeoIntensityState(NEO_LVL_2);
	}
	else if(nI == NEO_LVL_2)
	{
		SetNeoIntensityState(NEO_LVL_3);
	}
	else if(nI == NEO_LVL_3)
	{
		SetNeoIntensityState(NEO_LVL_4);
	}
	else if(nI == NEO_LVL_4)
	{
		SetNeoIntensityState(NEO_LVL_5);
	}
	else if(nI == NEO_LVL_5)
	{
		SetNeoIntensityState(NEO_LVL_1);
	}
	UpdateNeoColor();

}

void UpdateNeoColor(void)
{
	NeoIntensity neoI = GetNeoIntensityState();

	uint8_t red = (uint8_t)floor(neo.Red / neoI);
	uint8_t green = (uint8_t)floor(neo.Green / neoI);
	uint8_t blue = (uint8_t)floor(neo.Blue / neoI);

	for(int i = 0; i < LED_CFG_LEDS_CNT; i++)
	{
		led_set_color(i, red, green, blue);
	}
	led_update(1);
}

void NeoBatteryLevelLight(uint8_t batteryLevel)
{
	if(batteryLevel > 80)
		SetNeoColor(0, 0, 0xFF);
	else if (batteryLevel > 60)
		SetNeoColor(0, 0xFF, 0xFF);
	else if (batteryLevel > 40)
		SetNeoColor(0, 0xFF, 0);
	else if (batteryLevel > 20)
		SetNeoColor(0xFF, 0xFF, 0);
	else
		SetNeoColor(0xFF, 0, 0);

	UpdateNeoColor();
}

void InitRainbows(void)
{
	RainbowWarm.nrsRed = NEO_RISING;
	RainbowWarm.nrsBlue = NEO_STOP;
	RainbowWarm.nrsGreen = NEO_NEXTRISE;
	RainbowWarm.nrRed = 255;
	RainbowWarm.nrBlue = 0;
	RainbowWarm.nrGreen = 0;
	RainbowCold.nrsRed = NEO_STOP;
	RainbowCold.nrsBlue = NEO_RISING;
	RainbowCold.nrsGreen = NEO_NEXTRISE;
	RainbowCold.nrRed = 255;
	RainbowCold.nrBlue = 0;
	RainbowCold.nrGreen = 0;
	Rainbow.nrsRed = NEO_RISING;
	Rainbow.nrsBlue = NEO_STOP;
	Rainbow.nrsGreen = NEO_NEXTRISE;
	Rainbow.nrRed = 255;
	Rainbow.nrBlue = 0;
	Rainbow.nrGreen = 0;
}

void TestRainbow(uint8_t r, uint8_t g, uint8_t b);

void UpdateRainbowWarm(void)
{
	uint8_t maxValueRed = (uint8_t)floor(0xF0/GetNeoIntensityState())/2;
	uint8_t maxValueGreen = (uint8_t)floor(0xF0/GetNeoIntensityState())/4;
	uint8_t maxValueBlue = (uint8_t)floor(0xF0/GetNeoIntensityState())/4;
	//uint8_t maxValueBlue = (uint8_t)floor(0xFF/GetNeoIntensityState())/2;

	if(RainbowWarm.nrsGreen == NEO_NEXTRISE)
	{
//		SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);

		RainbowWarm.nrGreen++;
		if(RainbowWarm.nrGreen >= maxValueGreen)
		{
			RainbowWarm.nrsGreen = NEO_NEXTFALL;
		}
	}
	else if(RainbowWarm.nrsGreen == NEO_NEXTFALL)
	{
//		SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
		RainbowWarm.nrGreen--;
		if(RainbowWarm.nrGreen < 5)
		{
			RainbowWarm.nrsGreen = NEO_STOP;
			RainbowWarm.nrsBlue = NEO_NEXTRISE;
		}
	}
	else if(RainbowWarm.nrsBlue == NEO_NEXTRISE)
	{
//		SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
		RainbowWarm.nrBlue++;
		if(RainbowWarm.nrBlue >= maxValueBlue)
		{
			RainbowWarm.nrsBlue = NEO_NEXTFALL;
		}
	}
	else if(RainbowWarm.nrsBlue == NEO_NEXTFALL)
	{
		//SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
		RainbowWarm.nrBlue--;
		if(RainbowWarm.nrBlue < 5)
		{
			RainbowWarm.nrsBlue = NEO_STOP;
			RainbowWarm.nrsGreen = NEO_NEXTRISE;
		}
	}
	TestRainbow(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
	//UpdateNeoColor();

}

void TestRainbow(uint8_t r, uint8_t g, uint8_t b)
{
	for(int i = 0; i < LED_CFG_LEDS_CNT; i++)
	{
		led_set_color(i, r, g, b);
	}
	led_update(1);
}

void UpdateRainbowCold(void)
{
	uint8_t maxValueRed = (uint8_t)floor(0xF0/GetNeoIntensityState())/4;
	uint8_t maxValueGreen = (uint8_t)floor(0xF0/GetNeoIntensityState())/4;
	uint8_t maxValueBlue = (uint8_t)floor(0xF0/GetNeoIntensityState())/2;
	//uint8_t maxValueBlue = (uint8_t)floor(0xFF/GetNeoIntensityState())/2;

	if(RainbowCold.nrsGreen == NEO_NEXTRISE)
	{
//		SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);

		RainbowCold.nrGreen++;
		if(RainbowCold.nrGreen >= maxValueGreen)
		{
			RainbowCold.nrsGreen = NEO_NEXTFALL;
		}
	}
	else if(RainbowCold.nrsGreen == NEO_NEXTFALL)
	{
//		SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
		RainbowCold.nrGreen--;
		if(RainbowCold.nrGreen < 5)
		{
			RainbowCold.nrsGreen = NEO_STOP;
			RainbowCold.nrsRed = NEO_NEXTRISE;
		}
	}
	else if(RainbowCold.nrsRed == NEO_NEXTRISE)
	{
//		SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
		RainbowCold.nrRed++;
		if(RainbowCold.nrRed >= maxValueRed)
		{
			RainbowCold.nrsRed = NEO_NEXTFALL;
		}
	}
	else if(RainbowCold.nrsRed == NEO_NEXTFALL)
	{
		//SetNeoColor(maxValueRed, RainbowWarm.nrGreen, RainbowWarm.nrBlue);
		RainbowCold.nrRed--;
		if(RainbowCold.nrRed < 5)
		{
			RainbowCold.nrsRed = NEO_STOP;
			RainbowCold.nrsGreen = NEO_NEXTRISE;
		}
	}
	TestRainbow(RainbowCold.nrRed, RainbowCold.nrGreen, maxValueBlue);
	//UpdateNeoColor();
}

void UpdateRainbow(void)
{

}


void TurnAllOff(void)
{
	for(int i = 0; i < LED_CFG_LEDS_CNT; i++)
	{
		led_set_color(i, 0x0, 0x0, 0x0);
	}
	led_update(1);
}


void PowerUpSword(void)
{
//	for(int j = 0; j < 127; j++)
//	{
//		for(int i = 0; i < LED_CFG_LEDS_CNT; i++)
//		{
//			led_set_color(i, 0x0, j, j);
//		}
//		led_update(1);
//
//		HAL_Delay(50);
//	}

	for(int i = 0; i < LED_CFG_LEDS_CNT; i++)
	{
		for(int j = 0; j < 127; j= j+2)
		{
			led_set_color(i, 0x0, j, j);
			led_update(1);
			HAL_Delay(1);
		}
	}

}

void PowerDownSword(void)
{
	for(int i = LED_CFG_LEDS_CNT - 1; i >= 0; i--)
	{
		for(int j = 127; j >= 0; j= j-2)
		{
			led_set_color(i, 0x0, j, j);
			led_update(1);
			HAL_Delay(1);
		}
	}

	  led_set_color_all(0x00, 0x00, 0x00);
	  led_update(1);

}

void
led_init(void) {
    LL_TIM_InitTypeDef TIM_InitStruct;
    LL_TIM_OC_InitTypeDef TIM_OC_InitStruct;
    LL_GPIO_InitTypeDef GPIO_InitStruct;

    /* Peripheral clock enable */
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM2);
    LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_DMA1);

    /* TIM2 DMA Init */

    /* TIM2_CH2 Init */
    LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_5, LL_DMA_REQUEST_4);
    LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
    LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_LOW);
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_CIRCULAR);
    LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT);
    LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT);
    LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_WORD);
    LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MDATAALIGN_WORD);
//    LL_DMA_DisableFifoMode(DMA1, LL_DMA_STREAM_6);

    /* Added by user */
    LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)&TIM2->CCR1);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
    LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_5);

    /* TIM2 interrupt Init */
    NVIC_SetPriority(TIM2_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(TIM2_IRQn);

    TIM_InitStruct.Prescaler = 0; // 0
    TIM_InitStruct.CounterMode = LL_TIM_COUNTERMODE_UP;
    TIM_InitStruct.Autoreload = 47; // 47
    TIM_InitStruct.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM2, &TIM_InitStruct);
    LL_TIM_OC_EnablePreload(TIM2, LL_TIM_CHANNEL_CH1);

    TIM_OC_InitStruct.OCMode = LL_TIM_OCMODE_PWM1;
    TIM_OC_InitStruct.OCState = LL_TIM_OCSTATE_DISABLE;
    TIM_OC_InitStruct.OCNState = LL_TIM_OCSTATE_DISABLE;
    TIM_OC_InitStruct.CompareValue = 0;
    TIM_OC_InitStruct.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    LL_TIM_OC_Init(TIM2, LL_TIM_CHANNEL_CH1, &TIM_OC_InitStruct);
    LL_TIM_OC_DisableFast(TIM2, LL_TIM_CHANNEL_CH1);
    LL_TIM_SetTriggerOutput(TIM2, LL_TIM_TRGO_RESET);
    LL_TIM_DisableMasterSlaveMode(TIM2);

    /**
     * TIM2 GPIO Configuration
     * PB3     ------> TIM2_CH2
     */
    GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
    GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Alternate = LL_GPIO_AF_1;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    LL_TIM_OC_SetCompareCH1(TIM2, LL_TIM_GetAutoReload(TIM2) * 2 / 3 - 1); /* Set channel 1 compare register */
    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);  /* Enable output on channel */
    LL_TIM_EnableDMAReq_CC1(TIM2);              /* Enable DMA requests on channel 1 */

    /* DMA interrupt init */
    /* DMA1_Stream6_IRQn interrupt configuration */
    NVIC_SetPriority(DMA1_Channel5_IRQn, NVIC_EncodePriority(NVIC_GetPriorityGrouping(), 0, 0));
    NVIC_EnableIRQ(DMA1_Channel5_IRQn);
}

/**
 * \brief           Set R,G,B color for specific LED
 * \param[in]       index: LED index in array, starting from `0`
 * \param[in]       r,g,b: Red, Green, Blue values
 * \return          `1` on success, `0` otherwise
 */
uint8_t
led_set_color(size_t index, uint8_t r, uint8_t g, uint8_t b

) {
    if (index < LED_CFG_LEDS_CNT) {
        leds_colors[index * LED_CFG_BYTES_PER_LED + 0] = r;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 1] = g;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 2] = b;

        return 1;
    }
    return 0;
}

uint8_t
led_set_color_all(uint8_t r, uint8_t g, uint8_t b

) {
    size_t index;
    for (index = 0; index < LED_CFG_LEDS_CNT; index++) {
        leds_colors[index * LED_CFG_BYTES_PER_LED + 0] = r;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 1] = g;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 2] = b;

    }
    return 1;
}

uint8_t

led_set_color_rgb(uint32_t index, uint32_t rgbw) {
/* !LED_CFG_USE_RGBW */
    if (index < LED_CFG_LEDS_CNT) {
        leds_colors[index * LED_CFG_BYTES_PER_LED + 0] = (rgbw >> 24) & 0xFF;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 1] = (rgbw >> 16) & 0xFF;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 2] = (rgbw >> 8) & 0xFF;

        return 1;
    }
    return 0;
}

uint8_t
led_set_color_all_rgb(uint32_t rgbw) {
/* !LED_CFG_USE_RGBW */
    size_t index;
    for (index = 0; index < LED_CFG_LEDS_CNT; index++) {
        leds_colors[index * LED_CFG_BYTES_PER_LED + 0] = (rgbw >> 24) & 0xFF;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 1] = (rgbw >> 16) & 0xFF;
        leds_colors[index * LED_CFG_BYTES_PER_LED + 2] = (rgbw >> 8) & 0xFF;
    }
    return 1;
}

/**
 * \brief           Check if update procedure is finished
 * \return          `1` if not updating, `0` if updating process is in progress
 */
uint8_t
led_is_update_finished(void) {
    return !is_updating;                        /* Return updating flag status */
}

/**
 * \brief           Start LEDs update procedure
 * \param[in]       block: Set to `1` to block for update process until finished
 * \return          `1` if update started, `0` otherwise
 */
uint8_t
led_update(uint8_t block) {
    if (is_updating) {                          /* Check if update in progress already */
        return 0;
    }
    is_updating = 1;                            /* We are now updating */

    led_start_reset_pulse(1);                   /* Start reset pulse */
    if (block) {
        while (!led_is_update_finished());      /* Wait to finish */
    }
    return 1;
}

/**
 * \brief           Prepares data from memory for PWM output for timer
 * \note            Memory is in format R,G,B, while PWM must be configured in G,R,B[,W]
 * \param[in]       ledx: LED index to set the color
 * \param[out]      ptr: Output array with at least LED_CFG_RAW_BYTES_PER_LED-words of memory
 */
static uint8_t
led_fill_led_pwm_data(size_t ledx, uint32_t* ptr) {
    size_t i;

    if (ledx < LED_CFG_LEDS_CNT) {
        for (i = 0; i < 8; i++) {
            ptr[i] =        (leds_colors[LED_CFG_BYTES_PER_LED * ledx + 1] & (1 << (7 - i))) ? (2 * TIM2->ARR / 3) : (TIM2->ARR / 3);
            ptr[8 + i] =    (leds_colors[LED_CFG_BYTES_PER_LED * ledx + 0] & (1 << (7 - i))) ? (2 * TIM2->ARR / 3) : (TIM2->ARR / 3);
            ptr[16 + i] =   (leds_colors[LED_CFG_BYTES_PER_LED * ledx + 2] & (1 << (7 - i))) ? (2 * TIM2->ARR / 3) : (TIM2->ARR / 3);
        }
        return 1;
    }
    return 0;
}

/**
 * \brief           Update sequence function, called on each DMA transfer complete or half-transfer complete events
 * \param[in]       tc: Transfer complete flag. Set to `1` on TC event, or `0` on HT event
 *
 * \note            TC = Transfer-Complete event, called at the end of block
 * \note            HT = Half-Transfer-Complete event, called in the middle of elements transfered by DMA
 *                  If block is 48 elements (our case),
 *                      HT is called when first LED_CFG_RAW_BYTES_PER_LED elements are transfered,
 *                      TC is called when second LED_CFG_RAW_BYTES_PER_LED elements are transfered.
 *
 * \note            This function must be called from DMA interrupt
 */
static void
led_update_sequence(uint8_t tc) {
    tc = !!tc;                                  /* Convert to 1 or 0 value only */

    /* Check for reset pulse at the end of PWM stream */
    if (is_reset_pulse == 2) {                  /* Check for reset pulse at the end */
        LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1); /* Disable channel */
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);    /* Disable DMA stream */

        is_updating = 0;                        /* We are not updating anymore */
        return;
    }

    /* Check for reset pulse on beginning of PWM stream */
    if (is_reset_pulse == 1) {                  /* Check if we finished with reset pulse */
        /*
         * When reset pulse is active, we have to wait full DMA response,
         * before we can start modifying array which is shared with DMA and PWM
         */
        if (!tc) {                              /* We must wait for transfer complete */
            return;                             /* Return and wait to finish */
        }

        /* Disable timer output and disable DMA stream */
        LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1); /* Disable channel */
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);

        is_reset_pulse = 0;                     /* Not in reset pulse anymore */
        current_led = 0;                        /* Reset current led */
    } else {
        /*
         * When we are not in reset mode,
         * go to next led and process data for it
         */
        current_led++;                          /* Go to next LED */
    }

    /*
     * This part is used to prepare data for "next" led,
     * for which update will start once current transfer stops in circular mode
     */
    if (current_led < LED_CFG_LEDS_CNT) {
        /*
         * If we are preparing data for first time (current_led == 0)
         * or if there was no TC event (it was HT):
         *
         *  - Prepare first part of array, because either there is no transfer
         *      or second part (from HT to TC) is now in process for PWM transfer
         *
         * In other case (TC = 1)
         */
        if (current_led == 0 || !tc) {
            led_fill_led_pwm_data(current_led, &tmp_led_data[0]);
        } else {
            led_fill_led_pwm_data(current_led, &tmp_led_data[LED_CFG_RAW_BYTES_PER_LED]);
        }

        /*
         * If we are preparing first led (current_led = 0), then:
         *
         *  - We setup first part of array for first led,
         *  - We have to prepare second part for second led to have one led prepared in advance
         *  - Set DMA to circular mode and start the transfer + PWM output
         */
        if (current_led == 0) {
            current_led++;                      /* Go to next LED */
            led_fill_led_pwm_data(current_led, &tmp_led_data[LED_CFG_RAW_BYTES_PER_LED]);   /* Prepare second LED too */

            /* Set DMA to circular mode and set length to 48 elements for 2 leds */
            LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_CIRCULAR);
            LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)tmp_led_data);
            LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, 2 * LED_CFG_RAW_BYTES_PER_LED);

            /* Clear DMA flags */
            LL_DMA_ClearFlag_TC5(DMA1);
            LL_DMA_ClearFlag_HT5(DMA1);
            LL_DMA_EnableIT_HT(DMA1, LL_DMA_CHANNEL_5);
//            LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
            LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);
            LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);  /* Enable channel */
        }

    /*
     * When we reached all leds, we have to wait to transmit data for all leds before we can disable DMA and PWM:
     *
     *  - If TC event is enabled and we have EVEN number of LEDS (2, 4, 6, ...)
     *  - If HT event is enabled and we have ODD number of LEDS (1, 3, 5, ...)
     */
    } else if ((!tc && (LED_CFG_LEDS_CNT & 0x01)) || (tc && !(LED_CFG_LEDS_CNT & 0x01))) {
        LL_TIM_CC_DisableChannel(TIM2, LL_TIM_CHANNEL_CH1); /* Disable channel */
        LL_DMA_DisableChannel(DMA1, LL_DMA_CHANNEL_5);

        /* It is time to send final reset pulse, 50us at least */
        led_start_reset_pulse(2);                /* Start reset pulse at the end */
    }
}

/**
 * \brief           Start reset pulse sequence
 * \param[in]       num: Number indicating pulse is for beginning (1) or end (2) of PWM data stream
 */
static uint8_t led_start_reset_pulse(uint8_t num)
{
    is_reset_pulse = num;                       /* Set reset pulse flag */

    memset(tmp_led_data, 0, sizeof(tmp_led_data));   /* Set all bytes to 0 to achieve 50us pulse */

    if (num == 1) {
        tmp_led_data[0] = TIM2->ARR / 2;
    }

    /* Set DMA to normal mode, set memory to beginning of data and length to 40 elements */
    /* 800kHz PWM x 40 samples = ~50us pulse low */
    LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_NORMAL);  /* Go to non-circular mode */
    LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)tmp_led_data);
    LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, 40);

    /* Reset DMA configuration and enable stream */
    LL_DMA_ClearFlag_TC5(DMA1);
    LL_DMA_ClearFlag_HT5(DMA1);
    LL_DMA_DisableIT_HT(DMA1, LL_DMA_CHANNEL_5);
    LL_DMA_EnableIT_TC(DMA1, LL_DMA_CHANNEL_5);
    LL_DMA_EnableChannel(DMA1, LL_DMA_CHANNEL_5);

    LL_TIM_CC_EnableChannel(TIM2, LL_TIM_CHANNEL_CH1);  /* Enable channel for timer */
    LL_TIM_EnableCounter(TIM2);                 /* Start timer counter */

    return 1;
}

void
DMA1_Channel5_IRQHandler(void) {
    if (LL_DMA_IsActiveFlag_HT5(DMA1)) {        /* Check for HT event */
        LL_DMA_ClearFlag_HT5(DMA1);
        led_update_sequence(0);                 /* Call update sequence as HT event */
    } else if (LL_DMA_IsActiveFlag_TC5(DMA1)) { /* Check for TC event */
        LL_DMA_ClearFlag_TC5(DMA1);
        led_update_sequence(1);                 /* Call update sequence as TC event */
    }
}
