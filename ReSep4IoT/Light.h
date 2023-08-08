#pragma once

#include <stdint.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <task.h>
#include <semphr.h>
#include <stdbool.h>
#include <serial.h>
#include <tsl2591>  
#include "Setup.h"


void create(UBaseType_t Taskpriority);
void initializeLightDriver();
uint16_t getLight();
void lightTask(void* pvpParameter);
void measureLight();
void createLightTask(UBaseType_t Taskpriority);