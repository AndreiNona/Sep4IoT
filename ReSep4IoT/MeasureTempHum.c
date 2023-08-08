//Note: Due to a change in hardware late in the development process the driver present in the project might be missing methodes called in this class,
//	work is not needed on the task but the driver that implements this methods needs to be replaced or redesigned 

#include "MeasureTempHum.h"

int16_t temp = 0;
uint16_t hum = 0;


uint16_t getHumidity()
{
	return hum;
}


int16_t getTemperature()
{
	return temp;
}


void create(UBaseType_t priority)
{
	initializeTempAndHumDriver();
	createTempAndHumTask(priority);
	
}


void initializeTempAndHumDriver()
{
	

	if (HCSR501_OK == hcsr501_initialise() )
	{
		printf("Temperature and Humidity driver started \n");
	}
	
	else {
		printf("Temperature and Humidity can't initialize \n");
	}
}

//Wakeup call
void measureTempAndHum()
{
	if (HCSR501_OK != hcsr501_wakeup() )
	{
		printf("Wakeup failed\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(50)); // Delay for sensor response
	
	if ( HCSR501_OK != hcsr501_measure() )
	{
		printf("Measure failed\n");
	}
	
	vTaskDelay(pdMS_TO_TICKS(20)); // Delay to measure
}


void TempAndHumTask(void* pvpParameter)
{
	
	while(1)
	{
		EventBits_t eventBits = xEventGroupWaitBits(measureEventGroup,MEASURE_HUMIDITY_TEMPERATURE_BIT,pdTRUE,pdTRUE,portMAX_DELAY);

		if(eventBits & (MEASURE_HUMIDITY_TEMPERATURE_BIT))
		{
			measureTempAndHum();

			//Store recorded values locally 
			temp = hcsr501_getTemperature();
			hum = hcsr501_getHumidity();
			//Settingready bit to 1 in order to mark the sensor as available
			xEventGroupSetBits(dataReadyEventGroup,READY_HUMIDITY_TEMPERATURE_BIT);
		}
	}
}


void createTempAndHumTask(UBaseType_t Taskpriority)
{
	initializeTempAndHumDriver(); // initialize the sensor
	xTaskCreate( 
	TempAndHumTask // Task function
	,  "TempAndHumTask" // Task name
	,  configMINIMAL_STACK_SIZE // Stack size
	,  NULL // Parameter
	,  tskIDLE_PRIORITY + Taskpriority // Priority
	,  NULL ); // Task handle
}