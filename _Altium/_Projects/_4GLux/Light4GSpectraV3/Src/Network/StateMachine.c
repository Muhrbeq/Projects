/*
 * StateMachine.c
 *
 *  Created on: 18 jan. 2019
 *      Author: Rasmus.Muhrbeck
 */

#include "Global.h"
#include "StateMachine.h"
#include "sara.h"
#include "mqtt.h"
#include "Settings.h"
#include "firmware.h"
#include "utils.h"
#include "Battery.h"
#include "HDC1080.h"
#include "Tmp112.h"

#include "Error.h"
#include "MultiSensor.h"
#include "VEML7700.h"
#include "TimeFunc.h"
#include "LowLevel.h"

#include "SDCard.h"



RadioStates radioState = RADIO_STARTUP;
SensorStates sensorState = SENSOR_IDLE;

/* Returns state of Sensor */
SensorStates GetSensorState(void)
{
	return sensorState;
}

/* Sets the state of Sensor */
SensorStates SetSensorState(SensorStates sState)
{
	return sensorState = sState;
}

/* Reset Sensor state to idle */
void SensorResetState(void)
{
	/* Reset stage of Sensor */
	SetSensorState(RADIO_IDLE);
}



/* Returns state of Radio */
RadioStates GetRadioState(void)
{
	return radioState;
}

/* Sets state of Radio */
RadioStates SetRadioState(RadioStates rState)
{
	return radioState = rState;
}

/* Sometimes the Sara module gets an error while turning off and gets stuck in while-loop, this is the startTimer for breaking */
static void StartTurnOffCounter(void)
{
	GetTime(GetTimeStruct(turnOffStartVal));
}

/* Checks if Sara module has tried to turn off for more than X sec */
static uint8_t CompareTurnOffTimers(void)
{
	/* Gets what time it is now */
	GetTime(GetTimeStruct(turnOffEndVal));

	/* Compares to startTimer and checks if the diff is more than X sec */
	if (abs(
			RTC_GetElapsedSeconds(
					GetTimeStruct(turnOffStartVal),
					GetTimeStruct(turnOffEndVal))) > POWEROFF_TIMEOUT)
	{
		return true;
	}
	return false;
}

/* If the radio has tried turning off for more than X sec, reset and turn off */
static uint8_t ResetRadio(void)
{
	/* Reboot the Sara module */
	if (StartUpSara())
	{
		/* Turn off Echo and stuff */
		if (SaraInitAtCommands())
		{
			/* Shuts down securely */
			if (SaraPowerDown())
			{
				return true;
			}
		}
	}
	return false;
}

/* Reset Radio state to idle */
void RadioResetState(Error *er, Data *data)
{
	/* Resets Watchdog, safety measure due to the radio can take long time in certain scenarios */
	ResetIWDG();

	/* Want to have todays data fail */
	CopyTempToInfo();

	/* Start counter that evaluates how long it takes to turn off */
	StartTurnOffCounter();

	/* Raise error radio flag and start timer */
	ErrorTimerHourly(GetErrorTimerStruct(startTimerEnum), GetErrorFlagsStruct());

	/* Shut down Sara module */
	while (!SaraPowerDown())
	{
		/* Compare the turn off counter with the start value */
		if (CompareTurnOffTimers())
		{
			/* Hard reset of the radio */
			if (ResetRadio())
			{
				break;
			}
		}
	}

	radioFlags *rE = GetErrorFlagsStruct();

	Info *info = GetInfoStruct();

	/* Resets potential parameters */
	rE->alreadyConnected = 0;
	rE->trySubscribeAgain = 0;

	/* Enable SD card so that one can write to SD */
	EnableSD_Card();

	/* If any errors have occured */
	if (er->errorCounter > 0)
	{
		/* Try to write to SD card */
		if (SDwriteErrorLog(er) == HAL_OK)
		{
			/* If successful, reset counter */
			er->errorCounter = 0;
		}
		/* Else add to error log */
		else
		{
			/* Add to error log */
			ErrorAddLog(LOG_FUNC_MAIN, LOG_SUB_WRITE_ERROR,
			LOG_TYPE_WRITE,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
		}
	}

	/* If there is any Info in the InfoLog (which there will be in case of failure) */
	if (info->infoCounter > 0)
	{
		/* Write to SD card */
		if (SD_WriteInfoLog(info) == HAL_OK)
		{
			/* Reset all InfoStructs */
			ResetInfoStruct(GetInfoStruct());
			ResetInfoStruct(GetTempInfoStruct());
		}
	}

	/* Turn off SD card */
	DisableSD_Card();

	/* Reset stage of Sara */
	SetRadioState(RADIO_IDLE);
}

/* First run of the Radio is finished */
void RadioFirstRunFinished(void)
{
	SaraConfig *sC = GetSaraConfigurationStruct();

	radioFlags *rE = GetErrorFlagsStruct();

	/* Set alarm for wakeup */
	RTC_SetAlarm(sC->updateFrequency);

	/* Turn off radio */
	SetRadioState(RADIO_POWEROFF);

	/* Initialize sensors */
	SetSensorState(SENSOR_AWAKE);

	/* Debug message */
	debugPrint(&hlpuart1, "f");

	/* First run finished */
	rE->firstRun = 0;
}

static void RadioIdleState(Data *data)
{
	Time endTimeToSend;

	printf("IDLESTATE\r\n");

	/* Check if it is time to send */
	TimeToSend(GetTimeStruct(SaraLastConnectedVal), &endTimeToSend, data);

	/* Add to InfoLog */
	InfoLogAdd(
			ERRORLEVEL_INFO,
			LOG_FUNC_STATE,
			LOG_SUB_NONE,
			LOG_TYPE_NONE,
			LOG_STATUS_PASSED,
			0,
			0,
			0,
			GetTempInfoStruct());

}

static void RadioAwakeState(Data *data)
{
	/* Initialize everything to make the Radio work (that might not be woken up by the System Sleep function) */
	MX_USART2_UART_Init();
	MX_LPTIM1_Init();

	/* Enable SD card so that write processes are possible */
	EnableSD_Card();

	/* Write to SD card */
	SDWriteSensorData(data);

	/* Turn off to save power consumption */
	DisableSD_Card();

	/* Proceed to next state of the Radio */
	SetRadioState(RADIO_STARTUP);

	/* Add to InfoLog */
	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE,
	LOG_SUB_AWAKE,
	LOG_TYPE_NONE,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());


}

static void RadioStartupState(Data *data, Error *er, radioFlags *rE)
{

	/* If the startup is successful, go to next stage */
	if (StartUpSara())
	{
		/* Startup successful */
		SetRadioState(RADIO_INIT);

		debugPrint(&hlpuart1, "."); //TODO

		/* Reset counter */
		rE->startUpError = 0;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_INIT,
		LOG_TYPE_START,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());
	}
	else
	{
		/* Startup not successful */
		rE->startUpError++;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_INIT,
		LOG_TYPE_START,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* If startup failed X times, go to sleep */
		if (rE->startUpError == MAX_FAILED_ATTEMPTS)
		{
			/* Add to InfoLog that an error have occured */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_INIT,
			LOG_TYPE_START,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_INIT,
			LOG_TYPE_START,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			if (rE->firstRun == 0)
			{
				/* Turn off Radio, Start error timer, Reset radio stage */
				RadioResetState(er, data);
			}

			else
			{
				/* Init time of SaraLastConnected as current MCU time */
				InitTimeStruct(GetTimeStruct(SaraLastConnectedVal));

				/* First run is now finished */
				RadioFirstRunFinished();
			}
			


			/* Reset error counter */
			rE->startUpError = 0;

			debugPrint(&hlpuart1, "2"); //TODO
		}
	}
}

static void RadioInitState(Data *data, Error *er, radioFlags *rE)
{
	/* Check if init commands successful */
	if (SaraInitAtCommands())
	{
		debugPrint(&hlpuart1, "."); //TODO

		/* Init commands successful, continue to next stage */
		SetRadioState(RADIO_NETWORK);

		/* Reset counter */
		rE->AtInit = 0;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_INIT,
		LOG_TYPE_INIT,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		/* Start timer for network check */
		GetTime(GetTimeStruct(networkStartVal));
	}
	else
	{
		/* Init commands not successful */
		rE->AtInit++;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_INIT,
		LOG_TYPE_INIT,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* If init commands failed X times, go to sleep */
		if (rE->AtInit == MAX_FAILED_ATTEMPTS)
		{
			/* Add to InfoLog that an error has occur */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_INIT,
			LOG_TYPE_INIT,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			if (rE->firstRun == 0)
			{
				/* Turn off Radio, Start error timer, Reset radio stage */
				RadioResetState(er, data);
			}

			else
			{
				/* Init time of SaraLastConnected as current MCU time */
				InitTimeStruct(GetTimeStruct(SaraLastConnectedVal));

				/* First run is now finished */
				RadioFirstRunFinished();
			}

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_INIT,
			LOG_TYPE_INIT,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			/* Reset error counter */
			rE->AtInit = 0;

			debugPrint(&hlpuart1, "2"); //TODO
		}
	}
}

static void RadioNetworkState(Data *data, Error *er, radioFlags *rE)
{
	/* Check if Sara found internet */
	if (SaraCheckCommand("AT+CEREG?\r\n", "+CEREG: 0,1OK", LOW_TIMEOUT))
	{
		debugPrint(&hlpuart1, "."); //TODO

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_NETWORK,
		LOG_TYPE_CONNECT,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		/* Sara found internet, next stage */
		SetRadioState(RADIO_RSSI);

		/* If it is the first run, go directly to the clock */
//		if (rE->firstRun == 1)
//		{
//			SetRadioState(RADIO_CLOCK);
//		}
	}
	else
	{
		/* Assign a timer */
		GetTime(GetTimeStruct(networkEndVal));

		/* Check if timer has been on for more than X seconds */
		if (abs(
				RTC_GetElapsedSeconds(
						GetTimeStruct(networkStartVal),
						GetTimeStruct(networkEndVal))) > MAXIMUM_WAIT_FOR_NETWORK)
		{
			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_NETWORK,
			LOG_TYPE_CONNECT,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_NETWORK,
			LOG_TYPE_SET,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			debugPrint(&hlpuart1, "2"); //TODO

			if (rE->firstRun == 0)
			{
				/* Turn off Radio, Start error timer, Reset radio stage */
				RadioResetState(er, data);
			}

			else
			{
				/* Init time of SaraLastConnected as current MCU time */
				InitTimeStruct(GetTimeStruct(SaraLastConnectedVal));

				/* First run is now finished */
				RadioFirstRunFinished();
			}
		}
	}
}

static void RadioRssiState(Data *data, Error *er, radioFlags *rE)
{
	/* Check signal strength if it is OK to proceed */
	if (SaraGetSignalStrength())
	{
		/* Double check so that we have good connection */
		if (SaraGetRSSI())
		{
			/* Add to InfoLog */
			InfoLogAdd(ERRORLEVEL_INFO,
			LOG_FUNC_STATE,
			LOG_SUB_RSSI,
			LOG_TYPE_SEEK,
			LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

			debugPrint(&hlpuart1, "."); //TODO

			/* Go to next stage */
			SetRadioState(RADIO_RSSI);

			/* Reset counter */
			rE->testCounterGetSignal = 0;
		}
	}

	/* Signal strength was too low */
	else
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_RSSI,
		LOG_TYPE_SEEK,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* Signal strength too low to proceed */
		rE->testCounterGetSignal++;

		/* Wait a small amount of time before trying again */
		HAL_Delay(500);

		/* If good connection can't be obtained, go to sleep */
		if (rE->testCounterGetSignal == MAX_FAILED_ATTEMPTS_GET_SIGNAL)
		{
			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_RSSI,
			LOG_TYPE_SEEK,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
					LOG_FUNC_STATE,
					LOG_SUB_RSSI,
					LOG_TYPE_SET,
					LOG_STATUS_FAIL,
					0,
					0,
					0,
					GetErrorStruct());

			/* Reset error counter */
			rE->testCounterGetSignal = 0;

			debugPrint(&hlpuart1, "2"); //TODO

			/* Turn off Radio, Start error timer, Reset radio stage */
			RadioResetState(er, data);
		}
	}
}

static void RadioClockState(Data *data, Error *er, radioFlags *rE)
{
	/* timeBuffer to store clock */
	char timeBuffer[64] = { 0 };

	/* Get network clock */
	SARA_getRealTimeClock(timeBuffer);

	//printf("time: %s\r\n", timeBuffer);

	/* If everything went good comparing and obtaining clocks */
	if (SaraCompareRealTimeClock(
			timeBuffer,
			GetTimeStruct(SaraLastConnectedVal),
			GetTimeStruct(mcuClockTimeVal)))
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_CLOCK,
		LOG_TYPE_SET,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		/* Move to next stage */
		SetRadioState(RADIO_SETSERVER);

		printf("."); //

		/* Reset Counter */
		rE->testCounterTime = 0;

		/* Clock gathered so first run is now done */
		if (rE->firstRun == 1)  //TODO THIS IS FIRST TIMER
		{
			RadioFirstRunFinished();
		}
	}

	/* Something went wrong obtaining or comparing clocks */
	else
	{
		/* Small delay */
		HAL_Delay(200);

		/* Comparison failed */
		rE->testCounterTime++;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_CLOCK,
		LOG_TYPE_SET,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* If comparison/obtaining of the clocks fails X times, go to sleep */
		if (rE->testCounterTime == MAX_FAILED_ATTEMPTS)
		{
			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_CLOCK,
			LOG_TYPE_SET,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			if (rE->firstRun == 0)
			{
				/* Turn off Radio, Start error timer, Reset radio stage */
				RadioResetState(er, data);
			}

			else
			{
				/* Init time of SaraLastConnected as current MCU time */
				InitTimeStruct(GetTimeStruct(SaraLastConnectedVal));

				/* First run is now finished */
				RadioFirstRunFinished();
			}

			/* Add to error log */
			ErrorAddLog(
					LOG_FUNC_STATE,
					LOG_SUB_CLOCK,
					LOG_TYPE_COMPARE,
					LOG_STATUS_FAIL,
					0,
					0,
					0,
					GetErrorStruct());

			debugPrint(&hlpuart1, "2"); //TODO

			/* Reset counter */
			rE->testCounterTime = 0;
		}
	}
}

static void RadioSetServerState(Data *data, Error *er, radioFlags *rE, SaraConfig *sC)
{
	/* Set server as webPage (hosted on Rapi) */
	if (MqttSetHttp(sC->webPage))
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_SET,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		debugPrint(&hlpuart1, "."); //TODO

		/* Server set successful, move to next stage */
		SetRadioState(RADIO_CONNECT);

		/* Reset Counter */
		rE->setServerError = 0;
	}
	
	/* Set server not successful */
	else
	{
		/* Not successful counter */
		rE->setServerError++;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_SET,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* If it has failed more than X times, go to sleep */
		if (rE->setServerError == MAX_FAILED_ATTEMPTS)
		{
			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_SET,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_SET,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			debugPrint(&hlpuart1, "2"); //TODO

			/* Reset counter */
			rE->setServerError = 0;

			/* Turn off Radio, Start error timer, Reset radio stage */
			RadioResetState(er, data);
		}
	}
}

static void RadioConnectState(Data *data, Error *er, radioFlags *rE)
{
	/* Try to connect */
	if (MqttConnect())
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_CONNECT,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		debugPrint(&hlpuart1, "."); //TODO

		/* Clear UART buffer after send */
		uint8_t rxBuff[256] = { 0 };
		SaraWaitForResponse(rxBuff, 256, 200);

		/* Connect successful */
		SetRadioState(RADIO_PUSHDATA);



		/* Reset counter */
		rE->testCounterConnectServer = 0;

		if (rE->trySubscribeAgain == 1)
		{
			SetRadioState(RADIO_SUBSCRIBE);

			rE->trySubscribeAgain = 0;
		}
	
	}
	
	/* If not successful */
	else
	{
		/* Not successful counter */
		rE->testCounterConnectServer++;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_CONNECT,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* If connect to server failed X times, go to sleep */
		if (rE->testCounterConnectServer == MAX_FAILED_ATTEMPTS)
		{
			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_CONNECT,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());
			
			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_CONNECT,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			debugPrint(&hlpuart1, "2"); //TODO

			/* Reset error counter */
			rE->testCounterConnectServer = 0;

			/* Turn off Radio, Start error timer, Reset radio stage */
			RadioResetState(er, data);
		}
	}
}

static void RadioPushdataState(Data *data, Error *er, radioFlags *rE)
{
	/* Turn on SD card to enable write and read */
	EnableSD_Card();

	/* Init SD card */
	if (SD_Init() == HAL_OK)
	{
		/* Check if there is a file on the SD card to send */
		if (FindFile())
		{
			/* Find all files on the SD card that should be sent and send them */
			FindAndSendFile();

			/* Turn off SD card for power consumption */
			DisableSD_Card();

			/* Add to InfoLog */
			InfoLogAdd(ERRORLEVEL_INFO,
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_WRITE,
			LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

			debugPrint(&hlpuart1, "###"); //TODO
			/* Check if we have something in flash */

			/* Radio finished with pushing */
			//			SetRadioState(RADIO_POWEROFF);
			SetRadioState(RADIO_SUBSCRIBE);

			/* Reset Counter */
			rE->publishError = 0;

			if (rE->alreadyConnected == 1)
			{
				SetRadioState(RADIO_SERVERDO);
			}
		}
	}



	/* Publish not successful */
	else
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_WRITE,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());
		
		/* Not successful counter */
		rE->publishError++;

		/* If publish failed X times, go to sleep */
		if (rE->publishError == MAX_FAILED_ATTEMPTS)
		{
			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_WRITE,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());
			
			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_WRITE,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			debugPrint(&hlpuart1, "2"); //TODO

			/* Reset Counter */
			rE->publishError = 0;

			/* Turn off Radio, Start error timer, Reset radio stage */
			RadioResetState(er, data);
		}
	}
}

static void RadioSubscribeState(Data *data, Error *er, radioFlags *rE, SaraConfig *sC)
{
	/* See if the module can connect to the network */
	if (MqttSubscribe(sC->connectSubTopic, 0))
	{
		/* Clear UART buffer after send */
		uint8_t rxBuff[256] = { 0 };
		SaraWaitForResponse(rxBuff, 256, 2);

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_SUBSCRIBE,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		rE->alreadyConnected = 1;

		/* Subscribe successful */
		SetRadioState(RADIO_SERVERDO);

		/* Reset counter */
		rE->subscribeError = 0;

		rE->subscribeErrorCounter = 0;
	}

	/* Sometimes the subscribe failes because it is already subscribed */
	else if (MqttUnSubscribe(sC->connectSubTopic))
	{
		SetRadioState(RADIO_SUBSCRIBE);
	}

	/* Not successful subscribe */
	else
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_SUBSCRIBE,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());
		
		/* Add to error log */
		ErrorAddLog(
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_SUBSCRIBE,
		LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

		/* Not successful counter */
		rE->subscribeError++;

		/* If subscribed failed X times, go to sleep */
		if (rE->subscribeError == MAX_FAILED_ATTEMPTS)
		{
			rE->subscribeError = 0;

			rE->subscribeErrorCounter++;

			MqttDisconnect();

			SetRadioState(RADIO_SETSERVER);

			rE->trySubscribeAgain = 1;

			if (rE->subscribeErrorCounter == MAX_FAILED_ATTEMPTS)
			{
				/* Add to InfoLog that error occurred*/
				InfoLogAdd(ERRORLEVEL_HIGH,
				LOG_FUNC_STATE,
				LOG_SUB_SERVER,
				LOG_TYPE_SUBSCRIBE,
				LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

				rE->subscribeErrorCounter = 0;

				/* Turn off Radio, Start error timer, Reset radio stage */
				RadioResetState(er, data);
			}
		}
	}
}

static void RadioServerdoState(Data *data, Error *er, radioFlags *rE, SaraConfig *sC)
{
	/* See if the server wants the module to do something */
	if (ServerDo(sC->connectPubTopic))
	{
		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_READ,
		LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

		/* Server wants module to do something */
		SetRadioState(RADIO_TODO);

		/* Reset counter */
		rE->serverDo = 0;
	}
	/* Server did not want module to so something (check X times) */
	else
	{
		/* Counter */
		rE->serverDo++;

		/* Add to InfoLog */
		InfoLogAdd(ERRORLEVEL_INFO,
		LOG_FUNC_STATE,
		LOG_SUB_SERVER,
		LOG_TYPE_READ,
		LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

		/* If counter reaches X, go to sleep */
		if (rE->serverDo >= MAX_FAILED_ATTEMPTS_SERVER)
		{
			rE->serverDo = 0;

			rE->alreadyConnected = 0;

			MqttUnSubscribe(sC->connectSubTopic);

			SetRadioState(RADIO_PUSHDATA);

			rE->triedToSendCount++;

			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_READ,
			LOG_STATUS_FAIL, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_READ,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			if (rE->triedToSendCount >= MAX_FAILED_ATTEMPTS)
			{
				rE->triedToSendCount = 0;

				/* Turn off Radio, Start error timer, Reset radio stage */
				RadioResetState(er, data);
			}
		}
	}
}

static void RadioTodoState(Data *data, Error *er, radioFlags *rE, SaraConfig *sC)
{
	/* Turn on SD card to enable read/write */
	EnableSD_Card();

	/* If the server want the module to do something */
	if (sC->serverToDo == 3)
	{
		/* Init SD */
		if (SD_Init() == HAL_OK)
		{
			/* Check if there are any files on SD card */
			if (FindFile())
			{
				/* Archive it */
				ArchiveFiles();
			}

			HAL_Delay(100);

			/* Check if there are any files on SD card */
			if (!FindFile())
			{
				printf("All Data sent\r\n");

				/* Add of InfoLog */
				InfoLogAdd(ERRORLEVEL_INFO,
				LOG_FUNC_STATE,
				LOG_SUB_SERVER,
				LOG_TYPE_READ,
				LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

				rE->everythingOK = 1;

				rE->radioPublishCounter = 0;

				SetRadioState(RADIO_DISCONNECT);

				/* If any errors have occured */
				if (er->errorCounter > 0)
				{
					/* Try to write to SD card */
					if (SDwriteErrorLog(er) == HAL_OK)
					{
						/* If successful, reset counter */
						er->errorCounter = 0;
					}
					/* Else add to error log */
					else
					{
						/* Add to error log */
						ErrorAddLog(LOG_FUNC_STATE, LOG_SUB_WRITE_ERROR,
						LOG_TYPE_WRITE,
						LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());
					}
				}
				/* Check if there are anything in the InfoLog */
				if (GetInfoStruct()->infoCounter > 0)
				{
					/* Write to SD card */
					if (SD_WriteInfoLog(GetInfoStruct()) == HAL_OK)
					{
						ResetInfoStruct(GetInfoStruct());
					}
				}
				/* Disable SD card to save power */
				DisableSD_Card();
			}
			else
			{
				/* More data to send available */
				SetRadioState(RADIO_PUSHDATA);
				rE->radioPublishCounter = 0;
			}
		}
	}
	else //if (sC->serverToDo == 6)
	{
		printf("Not all data sent\r\n");
		SetRadioState(RADIO_PUSHDATA);

		rE->radioPublishCounter++;

		if (rE->radioPublishCounter >= MAX_FAILED_ATTEMPTS)
		{
			rE->radioPublishCounter = 0;

			/* Add to InfoLog that error occurred */
			InfoLogAdd(ERRORLEVEL_HIGH,
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_READ,
			LOG_STATUS_TIMEOUT, 0, 0, 0, GetTempInfoStruct());

			/* Add to error log */
			ErrorAddLog(
			LOG_FUNC_STATE,
			LOG_SUB_SERVER,
			LOG_TYPE_READ,
			LOG_STATUS_FAIL, 0, 0, 0, GetErrorStruct());

			/* Turn off Radio, Start error timer, Reset radio stage */
			RadioResetState(er, data);
		}
	}
}

/* Disconnect from server (not important if the DC is made) */
static void RadioDisconnectState(radioFlags *rE)
{
	if (MqttDisconnect())
	{
		SetRadioState(RADIO_POWEROFF);
		printf("Disconnected from server\r\n");

		/* Reset variables */
		rE->alreadyConnected = 0;
		rE->errorDayCounter = 0;
		rE->errorHourCounter = 0;
		rE->everythingOK = 0;
	}
	else
	{
		SetRadioState(RADIO_POWEROFF);
		printf("Just turn off\r\n");

		/* Reset variables */
		rE->alreadyConnected = 0;
		rE->errorDayCounter = 0;
		rE->errorHourCounter = 0;
		rE->everythingOK = 0;
	}
}

/* Turn off the Radio */
static void RadioPoweroffState(radioFlags *rE)
{

	debugPrint(&hlpuart1, "RPON");

	/* Reset variables */
	rE->alreadyConnected = 0;
	
	rE->trySubscribeAgain = 0;
	
	/* Start timer to evaluate  */
	StartTurnOffCounter();
	
	/* Shut down Sara module */
	while (!SaraPowerDown())
	{
		/* See how long it has tried to turn off */
		if (CompareTurnOffTimers())
		{
			/* Hard reset */
			if (ResetRadio())
			{
				break;
			}
		}
	}
	debugPrint(&hlpuart1, "RPOFF");
	//		SetRadioState(RADIO_IDLE);


	SetRadioState(RADIO_CHECKPOWER);
	
	/* Start a new counter for next stage */
	StartTurnOffCounter();

}

static void RadioCheckpowerState(void)
{
	/* Check the status of V_INT */
	GPIO_PinState checkPowerOff = HAL_GPIO_ReadPin(VINT_Sense_GPIO_Port, VINT_Sense_Pin);
	if (checkPowerOff == GPIO_PIN_SET)
	{
		printf("Still on\r\n");

		/* If timer reaches limit, something is wrong, retry shutdown */
		if (CompareTurnOffTimers())
		{
			SetRadioState(RADIO_POWEROFF);
		}
	}
	/* Everything is good */
	if (checkPowerOff == GPIO_PIN_RESET)
	{
		printf("Radio Off\r\n");
		SetRadioState(RADIO_IDLE);
	}
}

/* RadioStateMachine, now contains some debugPrints that should be removed in the end //TODO */
void RadioStateMachine(Data *data, Error *er)
{
	/* Get current radio state */
	RadioStates rS = GetRadioState();

	/* Idle stage, do nothing */
	if (rS == RADIO_IDLE)
	{
		RadioIdleState(data);
	}

	if (rS == RADIO_AWAKE)
	{
		RadioAwakeState(data);
	}

	/* Startup stage, starting Sara module up */
	else if (rS == RADIO_STARTUP)
	{
		RadioStartupState(data, er, GetErrorFlagsStruct());
	}

	/* Init commands, like Disable Echo etc.. */
	else if (rS == RADIO_INIT)
	{
		RadioInitState(data, er, GetErrorFlagsStruct());
	}

	/* Wait for network. If over X minutes go to sleep */
	else if (rS == RADIO_NETWORK)
	{
		RadioNetworkState(data, er, GetErrorFlagsStruct());
	}

	/* Get signal strength of network */
	else if (rS == RADIO_RSSI)
	{
		RadioRssiState(data, er, GetErrorFlagsStruct());
	}

	/* Get network clock and compare to local clock */
	else if (rS == RADIO_CLOCK)
	{
		RadioClockState(data, er, GetErrorFlagsStruct());
	}

	/* Configure the server that Sara should connect to */
	else if (rS == RADIO_SETSERVER)
	{
		RadioSetServerState(data, er, GetErrorFlagsStruct(), GetSaraConfigurationStruct());
	}

	/* Try to connect to set server */
	else if (rS == RADIO_CONNECT)
	{
		RadioConnectState(data, er, GetErrorFlagsStruct());
	}

	/* Push data to server */
	else if (rS == RADIO_PUSHDATA)
	{
		RadioPushdataState(data, er, GetErrorFlagsStruct());
	}

	/* States after this is supposed to be for version 2 where we have a smart server */

	/* Subscribe to update */
	else if (rS == RADIO_SUBSCRIBE)
	{
		RadioSubscribeState(data, er, GetErrorFlagsStruct(), GetSaraConfigurationStruct());
	}

	/* Check if server wants module to do something, download settings */
	else if (rS == RADIO_SERVERDO)
	{
		RadioServerdoState(data, er, GetErrorFlagsStruct(), GetSaraConfigurationStruct());
	}

	/* Server want to do */ //TODO This is not finished
	else if (rS == RADIO_TODO)
	{
		RadioTodoState(data, er, GetErrorFlagsStruct(), GetSaraConfigurationStruct());
	}

	/* Disconnects from server */
	else if (rS == RADIO_DISCONNECT)
	{
		RadioDisconnectState(GetErrorFlagsStruct());
	}

	/* Power off Radio */
	else if (rS == RADIO_POWEROFF)
	{
		RadioPoweroffState(GetErrorFlagsStruct());
	}

	/* Check state of V_INT Pin */
	else if (rS == RADIO_CHECKPOWER)
	{
		RadioCheckpowerState();
	}
}

static void SensorAwakeState(void)
{
	/* Init everything that is needed for sensor measure */
	MX_ADC1_Init();
	MX_LPTIM1_Init();
	MX_I2C1_Init();
	MX_I2C2_Init();
	SetSensorState(SENSOR_INIT);

	/* Add to InfoLog */
	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE, LOG_SUB_SENSOR, LOG_TYPE_AWAKE,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());


}

static void SensorInitState(Data *data)
{
	if (data->sensCounter == 0)
	{
		/* Enable Sensor Pins */
		DisEnableSensorPins(1);

		/* Init Battery sensor */
		Battery_init();
	}

	else
	{
		/* Enable Sensor Pins */
		HAL_GPIO_WritePin(Temperature_EN_GPIO_Port, Temperature_EN_Pin, GPIO_PIN_SET);

		HAL_GPIO_WritePin(Spectrum_EN_GPIO_Port, Spectrum_EN_Pin, GPIO_PIN_SET);

		HAL_Delay(10);
	}


	/* Init Temperature sensor */
	TMP112_Init();

	/* Init Humidity sensor */
	HDC1080_Init();

	/* Init Lux sensor */
	VEML7700_Initialize();

	debugPrint(&hlpuart1, "-");

	/* Delay so that nothing wrong happens with Init (Has been seen on sensors) */
	//HAL_Delay(1000);

	/* If it is the first value of the set, get the time and battery as well */
	if (data->sensCounter == 0)
	{
		SetSensorState(SENSOR_FIRST);
	}
	/* Else just go directly to gather the measurements */
	else
	{
		SetSensorState(SENSOR_SLEEP);
	}

	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE, LOG_SUB_SENSOR,
	LOG_TYPE_INIT,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());
}

static void SensorFirstState(Data *data)
{
	/* Read timestamp of MCU */
	data->readData.timeStamp = RTC_Get_DateTime_Comp();

	/* Read battery level */
	data->readData.batteryLevel = Battery_GetLevel();

	HAL_GPIO_WritePin(VBAT_Sense_EN_GPIO_Port, VBAT_Sense_EN_Pin, GPIO_PIN_RESET);

	/* First measurement initialized, now measure */
	SetSensorState(SENSOR_SLEEP);

	debugPrint(&hlpuart1, "-");

	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE, LOG_SUB_SENSOR,
	LOG_TYPE_START,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());
}

static void SensorSleepState(void)
{
	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE, LOG_SUB_SENSOR,
	LOG_TYPE_SETPOWSAVE,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

	RTC_SetAlarmB(1);

	SD_DeInitTest();

	MX_I2C1_DeInit();

	MX_I2C2_DeInit();

	MX_LPUART1_UART_DeInit();

	MX_ADC_DeInit();

//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);

	GPIOSetAnalogSensor();

	/* Disable Wakeup Counter */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	HAL_PWREx_EnterSTOP2Mode(PWR_STOPENTRY_WFI);

	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	__HAL_RTC_CLEAR_FLAG(RTC_EXTI_LINE_WAKEUPTIMER_EVENT);

	/* Enable SysTick interrupts */

	/* Stop RTC timer to generate interrupt */
	HAL_RTCEx_DeactivateWakeUpTimer(&hrtc);

	SystemClock_Config();

	GPIOInitSensor();

//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);

	MX_LPUART1_UART_Init();

	MX_USART2_UART_Init();

	MX_ADC1_Init();

	MX_LPTIM1_Init();

	MX_FATFS_Init();

//	HAL_SD_MspInit(&hsd1);

	printf("Start Sensor\r\n");

	MX_I2C1_Init();

	MX_I2C2_Init();

	SetSensorState(SENSOR_MEASURE);

	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE, LOG_SUB_SENSOR,
	LOG_TYPE_SET,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());

}

static void SensorMeasureState(Data *data)
{
	/* Measure all sensors */
	MultiSensor_takeMeasurements(&data->readData.sensData[data->sensCounter]);

	/* Add one to counter */
	data->sensCounter++;

	/* If sensCounter starts getting out of arraySize */
	if (data->sensCounter >= SENSOR_STRUCT_ARRAY_SIZE)
	{
		EnableSD_Card();

		SDWriteSensorData(data);

		DisableSD_Card();
	}

	/* Go to sleep, measurement done */
	SetSensorState(SENSOR_IDLE);

	PrintSensorData(data);

	debugPrint(&hlpuart1, "-");

	InfoLogAdd(ERRORLEVEL_INFO,
	LOG_FUNC_STATE, LOG_SUB_SENSOR,
	LOG_TYPE_READ,
	LOG_STATUS_PASSED, 0, 0, 0, GetTempInfoStruct());
}

/* SensorStateMachine, goes through each step and gets one sensor value (NOT OPTIMIZED DUE TO HAL_DELAY) */
void SensorStateMachine(Data *data, Error *er)
{
	SensorStates sS = GetSensorState();

	/* Idle stage, do nothing */
	if (sS == SENSOR_IDLE)
	{

	}

	/* Sensor is now awake */
	else if (sS == SENSOR_AWAKE)
	{
		SensorAwakeState();
	}

	/* Init sensors */
	else if (sS == SENSOR_INIT)
	{
		SensorInitState(data);
	}

	/* First sensor value, check time and battery */
	else if (sS == SENSOR_FIRST)
	{
		SensorFirstState(data);
	}

	/* Sensor have a wake-up time, MCU sleep during this */
	else if (sS == SENSOR_SLEEP)
	{
		SensorSleepState();
	}

	/* Take measure of all sensors */
	else if (sS == SENSOR_MEASURE)
	{
		SensorMeasureState(data);
	}
}

/* Sleeps when Radio and Sensors are IDLE, Should also implement sleep when Radio is searching for network */
void SleepStateMachine()
{
	/* Only allowed to sleep when radio is searching for network, otherwise it is not worth sleeping */

	RadioStates rS = GetRadioState();

	SensorStates sS = GetSensorState();

	if (rS == RADIO_IDLE && sS == SENSOR_IDLE)
	{
		debugPrint(&hlpuart1, "!");

		/* DeInitialize SD */
		SD_DeInit();

		/* If some error has occurred during the iteration, add the whole iteration to Infostruct */
		if (GetTempInfoStruct()->highestError >= ERRORLEVEL_WARNING)
		{
			CopyTempToInfo();
		}

		/* Check if USB is plugged in, in that case save InfoStruct to SD */
		if ((HAL_GPIO_ReadPin(Charge_Detect_GPIO_Port, Charge_Detect_Pin) == GPIO_PIN_RESET)
				&& (HAL_GPIO_ReadPin(VBUS_Detect_GPIO_Port, VBUS_Detect_Pin) == GPIO_PIN_SET))
		{
			EnableSD_Card();

			/* Write to SD card */
			SD_WriteInfoLog(GetInfoStruct());

			/* Reset everything in the InfoStruct */
			ResetInfoStruct(GetInfoStruct());

			DisableSD_Card();
		}

		/* Reset the iteration Info struct */
		ResetInfoStruct(GetTempInfoStruct());

		/* Enter sleep mode */
		SystemSleepAndWakeUp();
	}

	else if (rS == RADIO_NETWORK && sS == SENSOR_IDLE)
	{
		/* WORK IN PROGRESS, not necessary for the first version */
		SD_DeInit();

		EnterStopRadioNetwork();
		//EnterStopModeForEverythingButRadio();
	}

}
