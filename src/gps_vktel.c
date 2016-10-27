/*
 * gps_vktel.c
 *
 *  Created on: 27/10/2016
 *      Author: Mariano
 */

#include "gps_vktel.h"
#include "main_menu.h"
#include "uart.h"

GPSState gps_state = GPS_INIT;

volatile unsigned short gps_timeout;

//Inicio del GPS
//contesta RESP_CONTNUE o RESP_OK
unsigned char GPSStart(void)
{
	unsigned char resp = RESP_CONTINUE;

	switch (gps_state)
	{
		case GPS_INIT:
			GPS_PIN_DISA;
			gps_timeout = 100;
			gps_state++;
			break;

		case GPS_INIT2:
			if (!gps_timeout)
			{
				GPS_PIN_ENA;
				gps_timeout = 100;
				gps_state++;
			}
			break;

		case GPS_INIT3:
			if (!gps_timeout)
			{
				unsigned char dummy = 0xFF;
				USARTSendUnsigned(&dummy, 1);
				gps_timeout = 2;
				gps_state++;
			}
			break;

		case GPS_INIT4:
			if (!gps_timeout)
			{
				resp = RESP_OK;
				gps_state = GPS_INIT;
			}
			break;

		default:
			gps_state = GPS_INIT;
			break;
	}
	return resp;
}

//Timers internos para el GPS
void GPSTimeoutCounters (void)
{
	if (gps_timeout)
		gps_timeout--;

}