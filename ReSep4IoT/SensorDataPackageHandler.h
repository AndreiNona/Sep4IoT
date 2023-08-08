#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <ATMEGA_FreeRTOS.h>

#include <lora_driver.h>


void setLight(uint16_t light);

void setHumidity(uint16_t humidity);

void setTemperature(uint16_t temperature);

lora_driver_payload_t getPayload(uint16_t port);