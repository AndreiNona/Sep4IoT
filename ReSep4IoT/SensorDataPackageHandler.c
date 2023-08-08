#include "SensorDataPackageHandler.h"

uint16_t light;
uint16_t humidity;
int16_t temperature;


void setLight(uint16_t _light)
{
	light =_light;
}

void setHumidity(uint16_t _humidity) 
{
	humidity=_humidity;
}

void setTemperature(int16_t _temperature) 
{
	temperature=_temperature;
}

lora_driver_payload_t getLoRaPayload(unit8_t port) //Get payload through pointer, same as uplink handler but pointers
{
	lora_driver_payload_t* payload; 

	payload = pvPortMalloc(sizeof(lora_driver_payload_t)); 

	//Check to stop trying to populate null pointer
	if(payload != NULL) 
    {
		//Populate
		uplinkPayload.len = 6; //The length of the payload. For future development length can be made variable
		uplinkPayload.port = port;

		uplinkPayload.bytes[0] = light >> 8;		//Most significant byte with the light value
		uplinkPayload.bytes[1] = light & 0xFF;		//Least significant byte of the light value
		uplinkPayload.bytes[2] = humidity >> 8;		//Most significant byte with the humidity value
		uplinkPayload.bytes[3] = humidity & 0xFF;	//Least significant byte of the humidity value
		uplinkPayload.bytes[4] = temperature >> 8;	//Most significant byte with the temperature value
		uplinkPayload.bytes[5] = temperature & 0xFF;//Least significant byte of the temperature value
			
	}
	return *payload; //Possibility of pointing to a NULL payload
}