//AVR
#include <stdio.h>
#include <avr/io.h>

//Arduino
#include <ATMEGA_FreeRTOS.h>
#include <task.h>
#include <semphr.h>

//FreeRTOS
#include <stdio_driver.h>
#include <serial.h>
#include <event_groups.h>

 //LoRaWAN related
#include <lora_driver.h>
#include <status_leds.h>

//Application
#include "Main.h"
#include "MeasureTempHum.h"
#include "Light.h"
#include "Setup.h"
#include "UpLinkHandler.h"
#include "Configuration.h"

void initialise(void) // Initialise 
{
 initializeEventGroup();
 createUpLinkMessageBuffer();
 createDownLinkMessageBuffer();
 

}

void create_tasks(void) //Tasks
{
 createTempAndHumTask(1);
 createLightTask(1);
 createAppTask(2);
 lora_handler_uplink_payload(4);
 
}

void initSystem(void)
{
 
 DDRA |= _BV(DDA0) | _BV(DDA7); //PA0  and PA7 are set as output

 initializeUsedData();

 stdio_initialise(ser_USART0); // Allows stdio on port 0 and Arduino board

 status_leds_initialise(5); // Priority 5 for internal task
 create_tasks();



}

int main(void)
{
 initSystem(); // Initialise system. MUST be the first thing
 printf("Porgram Started Successfully\n");
 vTaskStartScheduler(); // Initialise FreeRTOS. Execution should never return from this point.
 for (;;)
 {
 }
}