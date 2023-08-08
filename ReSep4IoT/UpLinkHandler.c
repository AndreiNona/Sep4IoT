
#include <lora_driver.h>
#include <stdint.h>
#include <ATMEGA_FreeRTOS.h>
#include "UpLinkHandler.h"

#define LORA_APP_EUI "2BBE8F09765BBF4B"; //AppEUI
#define LORA_APP_KEY "5F83717BC67B4646E3F00DC5EC3417DC"; //AppKey


lora_driver_payload_t uplink_payload;

uint16_t light; //Light value
uint16_t humidity; //Humidity value
int16_t temperature; //Temperature value


static void reset_lora_module(void)
{
        
        lora_driver_resetRn2483(1); //Start reset
        vTaskDelay(2000); 

        lora_driver_resetRn2483(0); //Stop reset
        vTaskDelay(150); 

        lora_driver_flushBuffers(); //Flush buffer

}

static void _lora_setup(void)
{
	static char devEui[17]= "0004A30B00251192"; //EUI Dev#2

	uint8_t attempts = 10;
	lora_driver_returnCode_t rc;

	status_leds_slowBlink(led_ST2);
	//Reset module
	printf("FactoryReset");
    if  (lora_driver_rn2483FactoryReset() != LORA_OK) 
    {
		printf("Failed");
        return;
    }

	//Configure module
	printf("Configure");
    if (lora_driver_configureToEu868() != LORA_OK) 
    {
		printf("Failed");
        return;
    }

	printf("Get HWEUI");
    if (lora_driver_getRn2483AppEui(appEui) != LORA_OK)
    {
		printf("Failed");
        return;
    }
	//Set the application EUI
	printf("Set DevEUI");
    if (lora_driver_setOtaaIdentity(LORA_APP_EUI, LORA_APP_KEY, devEui) != LORA_OK) 
    {
		printf("Failed");
        return;
    }

	printf("Save mac");
    if (lora_driver_saveMac() != LORA_OK) 
    {
		printf("Failed");
        return;
    } 

	printf("Set Adaptive Data Rate");
    if (lora_driver_setAdaptiveDataRate(LORA_ON) != LORA_OK) 
    {
		printf("Failed");
        return;
    }

	do {
		rc = lora_driver_join(LORA_OTAA);
		printf("Join, attempts left:%d >%s<\n", attempts, lora_driver_mapReturnCodeToText(rc));

		if (rc != LORA_ACCEPTED)
		{
			//Something went wrong...
			status_leds_longPuls(led_ST1); 
			// 10 seconds
			vTaskDelay(pdMS_TO_TICKS(10000));
		}
		else
		{
			break;
		}
	} while (--attempts);

	if (rc == LORA_ACCEPTED)
	{
		status_leds_ledOn(led_ST2);
	}
	else
	{
		status_leds_ledOff(led_ST2);
		//Something went wrong...
		status_leds_fastBlink(led_ST1);

		//Stop here to allow for human interaction
		for (;;)
		{
			taskYIELD();
		}
	}

	lora_driver_payload_t get_lora_package(unit8_t port) {

		//Define payload
		lora_driver_payload_t payload;

		//Populate
		uplinkPayload.len = 6; //The length of the payload. For future development length can be made variable
		uplinkPayload.port = port;

		uplinkPayload.bytes[0] = light >> 8;		//Most significant byte with the light value
		uplinkPayload.bytes[1] = light & 0xFF;		//Least significant byte of the light value
		uplinkPayload.bytes[2] = humidity >> 8;		//Most significant byte with the humidity value
		uplinkPayload.bytes[3] = humidity & 0xFF;	//Least significant byte of the humidity value
		uplinkPayload.bytes[4] = temperature >> 8;	//Most significant byte with the temperature value
		uplinkPayload.bytes[5] = temperature & 0xFF;//Least significant byte of the temperature value


		return payload;
	}


    if ((rc = lora_driver_sendUploadMessage(false, &_payload)) == LORA_MAC_TX_OK) //send the uplink message
    {
        return;
    }
    else if (rc == LORA_MAC_TX_OK)
    {
        return;
    }
}


void UplinkHandlerTask(void *pvParameters)
{
    reset_lora_module();
    lorawan_driver_initialize(ser_USART1, NULL); 

    lorawan_driver_join(OTAA); 

    for (;;)
    {
        lorawan_driver_send(1, "body", 11, 1, 0); // Send message

		//10seconds
        vTaskDelay(pdMS_TO_TICKS(10000));
    }
    vTaskDelete(NULL);
}



void lora_handler_task( void *pvParameters )
{
	//Hardware reset
	lora_driver_resetRn2483(1);
	vTaskDelay(2);
	lora_driver_resetRn2483(0);
	

	vTaskDelay(150);

	lora_driver_flushBuffers(); //Flush buffer

	_lora_setup();

	TickType_t xLastWakeTime;
	const TickType_t xFrequency = pdMS_TO_TICKS(900000); //Upload message every 15 minutes (900000 ms)
	xLastWakeTime = xTaskGetTickCount();
	
	for(;;)
	{
		xTaskDelayUntil( &xLastWakeTime, xFrequency );

		xMessageBufferReceive(upLinkMessageBuffer,&uplink_payload,sizeof(uplink_payload),portMAX_DELAY);
		status_leds_shortPuls(led_ST4);  
		printf("Upload >%s<\n", lora_driver_mapReturnCodeToText(lora_driver_sendUploadMessage(false, &uplink_payload)));
	}
} 

void lora_handler_uplink_payload(UBaseType_t TaskPriority)
{
	xTaskCreate(
	 lora_handler_task
	,  "lora_handler_task"  
	,  configMINIMAL_STACK_SIZE+200  
	,  NULL 
	,  TaskPriority 
	,  NULL );
}

void UplinkHandler_createTask()
{
	xTaskCreate(
		UplinkHandler,
		,"UplinkHandler"
		,100
		,NULL
		,1
		,NULL);
}
