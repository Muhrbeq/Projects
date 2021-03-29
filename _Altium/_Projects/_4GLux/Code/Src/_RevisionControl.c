/*
 * _RevisionControl.c
 *
 *  Created on: 25 mars 2021
 *      Author: Rasmus.Muhrbeck
 */


#include "_RevisionControl.h"
#include "adc.h"

RevisionControl_State RevState = REVISION_ERROR;
uint8_t revisionControl_Counter = 0;

/* Set state of revision control */
void RevisionControl_SetState(RevisionControl_State rState)
{
	RevState = rState;
}

/* Get state of revision control */
RevisionControl_State RevisionControl_GetState(void)
{
	return RevState;
}

/* Set GPIO high to enable revision reader */
static HAL_StatusTypeDef RevisionControl_EnableRead(void)
{
	HAL_GPIO_WritePin(Revision_Enable_GPIO_Port, Revision_Enable_Pin, GPIO_PIN_SET);

	uint16_t counter = 0;
	while(counter < 1000)
	{
		if(HAL_GPIO_ReadPin(Revision_Enable_GPIO_Port, Revision_Enable_Pin) == GPIO_PIN_SET)
		{
			return HAL_OK;
		}
		counter++;
	}
	return HAL_ERROR;
}

/* Set GPIO low to disable revision reader */
static HAL_StatusTypeDef RevisionControl_DisableRead(void)
{
	HAL_GPIO_WritePin(Revision_Enable_GPIO_Port, Revision_Enable_Pin, GPIO_PIN_RESET);

	uint16_t counter = 0;
	while(counter < 1000)
	{
		if(HAL_GPIO_ReadPin(Revision_Enable_GPIO_Port, Revision_Enable_Pin) == GPIO_PIN_RESET)
		{
			return HAL_OK;
		}
		counter++;
	}
	return HAL_ERROR;
}

static HAL_StatusTypeDef RevisionControl_CalibrationStart(void)
{
	if(HAL_ADCEx_Calibration_Start(&hadc1, ADC_SINGLE_ENDED) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

static HAL_StatusTypeDef RevisionControl_Start(void)
{
	if(HAL_ADC_Start(&hadc1) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

static HAL_StatusTypeDef RevisionControl_PollConversion(void)
{
	if(HAL_ADC_PollForConversion(&hadc1, 10) == HAL_OK)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

static HAL_StatusTypeDef RevisionControl_GetADCState(void)
{
	if ((HAL_ADC_GetState(&hadc1) & HAL_ADC_STATE_REG_EOC)
				== HAL_ADC_STATE_REG_EOC)
	{
		return HAL_OK;
	}
	return HAL_ERROR;
}

static uint16_t RevisionControl_GetADCValue(void)
{
	return HAL_ADC_GetValue(&hadc1);
}

static RevisionControl_Revision RevisionControl_CheckRevision(void)
{
	__IO uint16_t adcValue = RevisionControl_GetADCValue();

	if(adcValue > 4000)//adcValue > 800 && adcValue < 1200)
	{
		return REVISION_V1;
	}
	else
	{
		return REVISION_ERROR;
	}
}

void RevisionControl_StateMachine(void)
{
	RevisionControl_State rState = RevisionControl_GetState();

	if(revisionControl_Counter > 100)
	{
		/* Error occur */
		TRACE("[Revision Control] - FAILED\r\n");
		RevisionControl_SetState(REVISIONSTATE_ERROR);
		revisionBoard = REVISION_ERROR;
	}

	switch(rState)
	{
		case REVISIONSTATE_ENABLE:
			if(RevisionControl_EnableRead() == HAL_OK)
			{
				RevisionControl_SetState(REVISIONSTATE_CALIBRATIONSTART);
				TRACE_DEBUG("RevisionControl EnableRead passed\r\n");
			}
			else
			{
				revisionControl_Counter++;
			}
			break;
		case REVISIONSTATE_CALIBRATIONSTART:
			if(RevisionControl_CalibrationStart() == HAL_OK)
			{
				RevisionControl_SetState(REVISIONSTATE_START);
				TRACE_DEBUG("RevisionControl Calibration start passed\r\n");
			}
			else
			{
				revisionControl_Counter++;
			}
			break;
		case REVISIONSTATE_START:
			if(RevisionControl_Start() == HAL_OK)
			{
				RevisionControl_SetState(REVISIONSTATE_POLLCONVERSION);
				TRACE_DEBUG("RevisionControl start passed\r\n");
			}
			else
			{
				revisionControl_Counter++;
			}
			break;
		case REVISIONSTATE_POLLCONVERSION:
			if(RevisionControl_PollConversion() == HAL_OK)
			{
				RevisionControl_SetState(REVISIONSTATE_GETADCSTATE);
				TRACE_DEBUG("RevisionControl Poll conversion passed\r\n");
			}
			else
			{
				revisionControl_Counter++;
			}
			break;
		case REVISIONSTATE_GETADCSTATE:
			if(RevisionControl_GetADCState() == HAL_OK)
			{
				RevisionControl_SetState(REVISIONSTATE_CHECKREVISION);
				TRACE_DEBUG("RevisionControl Get ADC state passed\r\n");
			}
			else
			{
				revisionControl_Counter++;
			}
			break;
		case REVISIONSTATE_CHECKREVISION:
			revisionBoard = RevisionControl_CheckRevision();
			RevisionControl_SetState(REVISIONSTATE_DISABLE);
			TRACE_DEBUG("RevisionControl Check Revision passed\r\n");
			break;
		case REVISIONSTATE_DISABLE:
			if(RevisionControl_DisableRead() == HAL_OK)
			{
				RevisionControl_SetState(REVISIONSTATE_DONE);
				TRACE_DEBUG("RevisionControl Disable read passed\r\n");
				TRACE("[Revision Control] - PASSED\r\n");
			}
			else
			{
				revisionControl_Counter++;
			}
			break;
		default:
			TRACE_DEBUG("RevisionControl ERROR, disabling state machine\r\n");
			break;
			/* DeInit ADC and bind GPIOs to analog inputs */

	}

}
