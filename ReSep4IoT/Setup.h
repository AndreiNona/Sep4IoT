#pragma once
#include <ATMEGA_FreeRTOS.h>
#include <event_groups.h>
#include "lora_driver.h" 
#include "message_buffer.h"
#include "SensorDataPackageHandler.h"


#define MEASURE_HUMIDITY_TEMPERATURE_BIT (1<<0)
#define READY_HUMIDITY_TEMPERATURE_BIT (1<<1)
#define MEASURE_LIGHT_BIT (1<<2) 
#define READY_LIGHT_BIT (1<<3) 
#define ALL_READY_BITS (READY_HUMIDITY_TEMPERATURE_BIT | READY_LIGHT_BIT) 
#define ALL_MEASURE_BITS (MEASURE_HUMIDITY_TEMPERATURE_BIT | MEASURE_LIGHT_BIT) 

extern EventGroupHandle_t measureEventGroup;
extern EventGroupHandle_t dataReadyEventGroup;
extern MessageBufferHandle_t upLinkMessageBuffer; 


void initializeEventGroup();
void createUpLinkMessageBuffer();