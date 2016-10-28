/*
 * gps_vktel.h
 *
 *  Created on: 27/10/2016
 *      Author: Mariano
 */

#ifndef GPS_VKTEL_H_
#define GPS_VKTEL_H_

#include "hard.h"
#include "stm32f0xx.h"

//--- Definicion de pines de hardware, los que no se conecten en la placa utilizar 1 (o lo que corresponda)
//--- se relacionanan con los nombres de hard.h

//--- GPS_PIN - salida al modulo ---//
#define GPS_PIN			EN_GPS
#define GPS_PIN_DISA	EN_GPS_OFF
#define GPS_PIN_ENA		EN_GPS_ON

//--- GPS_PPS - entrada desde el modulo ---//
#define GPS_PPS			PPS




typedef enum  {

	GPS_INIT = 0,
	GPS_INIT2,
	GPS_INIT3,
	GPS_INIT4

} GPSState;

//--- Tiemouts del GPS ---//
#define TT_GPS_MINI		5

#define One_Output_Ten_Secs_String_Cmd	{0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0x10, 0x27, 0x01, 0x00, 0x01, 0x00, 0x4D, 0xDD, 0xB5, 0x62, 0x06, 0x08, 0x00, 0x00, 0x0E, 0x30 }
#define One_Output_One_Secs_String_Cmd	{0xB5, 0x62, 0x06, 0x08, 0x06, 0x00, 0xE8, 0x03, 0x01, 0x00, 0x01, 0x00, 0x01, 0x39}

unsigned char GPSStart(void);
void GPSTimeoutCounters (void);

#endif /* GPS_VKTEL_H_ */
