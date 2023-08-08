#include "Light.h"


uint16_t light = 0;

void create(UBaseType_t priority)
{
	initializeLightDriver();
	createLightTask(priority);

}

void initializeLightDriver(){
	if (TSL2591_OK == tsl2591_initialise())
	{
		printf("Driver started \n");
	}

	else {
		printf("Driver couldn't initialize\n");
	}

	uint16_t getLight(){
		return light;
	}

	void lightTask(void* pvpParameter) 
	{
		while(1){
			EventBits_t eventBits = xEventGroupWaitBits(measureEventGroup,MEASURE_LIGHT_BIT,pdTRUE,pdTRUE,portMAX_DELAY); // wait for Application to set MEASURE_BIT
		
			if(eventBits & (MEASURE_LIGHT_BIT)) //Check if the light sensor is needed
				{
					measurelight(); //Action
			}
		
			xEventGroupSetBits(dataReadyEventGroup,READY_LIGHT_BIT); //set READY_LIGHT_BIT to Event Group
		
			vTaskDelay(pdMS_TO_TICKS(20)); //Delay to allow for inconsistencies 
		}
	
	}

	void measureLight(){

		if (TSL2591_OK != tsl2591_enable())
		{
			printf("Sensor failed to start\n");
		}

		vTaskDelay(pdMS_TO_TICKS(50)); // Delay for sensor response

		if (TSL2591_OK != tsl2591_getCombinedDataRaw())
		{
			printf("Measure failed\n");
		}

		vTaskDelay(pdMS_TO_TICKS(20)); // Delay to measure

		if (TSL2591_OK != tsl2591_disable())
		{
			printf("Sensor failed to stop\n");
		}
		vTaskDelay(pdMS_TO_TICKS(20)); // Delay for action
	}


void createLightTask(UBaseType_t Taskpriority){
	initializeLightDriver(); 
	xTaskCreate(
	LightTask,  
	"LightTask",	
	configMINIMAL_STACK_SIZE,  
	NULL, 
	tskIDLE_PRIORITY + Taskpriority,  
	NULL 
	);
}