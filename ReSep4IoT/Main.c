#include "Main.h"


void TaskLoop(void *pvParameters) 
{
	for (;;)
	{
		// Set event bits to start measuring
		xEventGroupSetBits(measureEventGroup, MEASURE_ALL_BITS); 
		
		// Wait for measurement tasks to be done
		EventBits_t eventBits = xEventGroupWaitBits(dataReadyEventGroup,READY_ALL_BITS,pdTRUE,pdTRUE,portMAX_DELAY); 
		
		//If everything finished mesurements need to be set to the recorded data
		if(eventBits & (READY_ALL_BITS))
		{

			//Get measurements from sensor classes
			setTemperature(getTemperature());
			setHumidity(getHumidity());
			setLight(getLight()); 

			lora_driver_payload_t _uplink_payload = getLoRaPayload(2);
			xMessageBufferSend(upLinkMessageBuffer, &_uplink_payload, sizeof(_uplink_payload), portMAX_DELAY);

			//Wait for 15 minutes									
			vTaskDelay(pdMS_TO_TICKS(900000));
		}
		
		//Application with other 50 milliseconds until the next measurement starts.
		vTaskDelay(pdMS_TO_TICKS(50));
	}
}



void createAppTask(UBaseType_t priority)
{
	xTaskCreate(
	ApplicationTask	
	,  "AppTask" 
	,  configMINIMAL_STACK_SIZE	
	,  NULL 
	,  tskIDLE_PRIORITY + priority 
	,  NULL );
}