#pragma once

#include <stddef.h>
#include <stdio.h>

#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>
#include <status_leds.h>
#include "SensorDataPackageHandler.h"
#include "message_buffer.h"
#include "Setup.h"


#define LORA_APP_EUI "2BBE8F09765BBF4B"; //AppEUI
#define LORA_APP_KEY "5F83717BC67B4646E3F00DC5EC3417DC"; //AppKey

void uplink_payload(UBaseType_t TaskPriority);
void setup(void);
void task_handler( void *pvParameters );
